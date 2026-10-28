#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "TW_win32.h"

#include <stdio.h>
#include <string.h>

static void TWAPI_Win32_DestroyPresentBuffer(
    TWAPI_Window* window);

static const char* TWAPI_WIN32_CLASS_NAME =
    "TimWorksAPI_WindowClass";

static ATOM TWAPI_Win32_WindowClassAtom = 0;

static TWAPI_Window* TWAPI_Win32_ActiveWindow = NULL;

static void TWAPI_Win32_KeyDown(
    TWAPI_Window* window, WPARAM key);

static void TWAPI_Win32_KeyUp(
    TWAPI_Window* window, WPARAM key);

static void TWAPI_Win32_MouseDown(
    TWAPI_Window* window, TWAPI_MouseButton button);

static void TWAPI_Win32_MouseUp(
    TWAPI_Window* window, TWAPI_MouseButton button);
    
#ifndef WGL_DRAW_TO_WINDOW_ARB
#define WGL_DRAW_TO_WINDOW_ARB     0x2001
#define WGL_SUPPORT_OPENGL_ARB     0x2010
#define WGL_DOUBLE_BUFFER_ARB      0x2011
#define WGL_PIXEL_TYPE_ARB         0x2013
#define WGL_COLOR_BITS_ARB         0x2014
#define WGL_ALPHA_BITS_ARB         0x201B
#define WGL_DEPTH_BITS_ARB         0x2022
#define WGL_STENCIL_BITS_ARB       0x2023
#define WGL_TYPE_RGBA_ARB          0x202B
#define WGL_SAMPLE_BUFFERS_ARB     0x2041
#define WGL_SAMPLES_ARB            0x2042
#endif

typedef BOOL (WINAPI *TWAPI_WGLChoosePixelFormatARB)(
    HDC,
    const int*,
    const FLOAT*,
    UINT,
    int*,
    UINT*
);

static PROC TWAPI_Win32_LoadWGLProc(const char* name)
{
    PROC proc = wglGetProcAddress(name);

    if (proc == NULL ||
        proc == (PROC)1 ||
        proc == (PROC)2 ||
        proc == (PROC)3 ||
        proc == (PROC)-1)
    {
        return NULL;
    }

    return proc;
}

static int TWAPI_Win32_ChoosePixelFormatMSAA(
    HDC targetDC,
    int requestedSamples,
    PIXELFORMATDESCRIPTOR* outputPFD)
{
    if (!targetDC || !outputPFD)
        return 0;

    /* If a pixel format is already set, Windows cannot replace it. */
    int existing = GetPixelFormat(targetDC);

    if (existing != 0)
    {
        DescribePixelFormat(
            targetDC, existing, sizeof(*outputPFD), outputPFD);
        return existing;
    }

    HINSTANCE instance = GetModuleHandleA(NULL);

    const char* className = "TWAPI_WGL_Bootstrap_Class";

    WNDCLASSA wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProcA;
    wc.hInstance = instance;
    wc.lpszClassName = className;

    if (!RegisterClassA(&wc) &&
        GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
    {
        return 0;
    }

    HWND dummy = CreateWindowExA(
        0, className, "",
        WS_POPUP,
        0, 0, 1, 1,
        NULL, NULL, instance, NULL);

    if (!dummy)
        return 0;

    HDC dummyDC = GetDC(dummy);

    if (!dummyDC)
    {
        DestroyWindow(dummy);
        return 0;
    }

    PIXELFORMATDESCRIPTOR dummyPFD;
    ZeroMemory(&dummyPFD, sizeof(dummyPFD));

    dummyPFD.nSize = sizeof(dummyPFD);
    dummyPFD.nVersion = 1;
    dummyPFD.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    dummyPFD.iPixelType = PFD_TYPE_RGBA;
    dummyPFD.cColorBits = 32;
    dummyPFD.cAlphaBits = 8;
    dummyPFD.cDepthBits = 24;
    dummyPFD.cStencilBits = 8;
    dummyPFD.iLayerType = PFD_MAIN_PLANE;

    int dummyFormat = ChoosePixelFormat(dummyDC, &dummyPFD);

    if (!dummyFormat ||
        !SetPixelFormat(dummyDC, dummyFormat, &dummyPFD))
    {
        ReleaseDC(dummy, dummyDC);
        DestroyWindow(dummy);
        return 0;
    }

    HGLRC dummyContext = wglCreateContext(dummyDC);

    if (!dummyContext ||
        !wglMakeCurrent(dummyDC, dummyContext))
    {
        if (dummyContext)
            wglDeleteContext(dummyContext);

        ReleaseDC(dummy, dummyDC);
        DestroyWindow(dummy);
        return 0;
    }

    PROC proc = TWAPI_Win32_LoadWGLProc(
        "wglChoosePixelFormatARB");

    TWAPI_WGLChoosePixelFormatARB chooseARB = NULL;

    if (proc)
        memcpy(&chooseARB, &proc, sizeof(chooseARB));

    int chosenFormat = 0;

    if (chooseARB)
    {
        int samplesToTry[] = {8, 4, 2, 0};
        int count = sizeof(samplesToTry) / sizeof(samplesToTry[0]);

        for (int i = 0; i < count; ++i)
        {
            int samples = samplesToTry[i];

            if (samples > requestedSamples)
                continue;

            int attributes[] = {
                WGL_DRAW_TO_WINDOW_ARB,  TRUE,
                WGL_SUPPORT_OPENGL_ARB,  TRUE,
                WGL_DOUBLE_BUFFER_ARB,   TRUE,
                WGL_PIXEL_TYPE_ARB,      WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB,      32,
                WGL_ALPHA_BITS_ARB,      8,
                WGL_DEPTH_BITS_ARB,      24,
                WGL_STENCIL_BITS_ARB,    8,
                WGL_SAMPLE_BUFFERS_ARB,  samples > 0 ? 1 : 0,
                WGL_SAMPLES_ARB,         samples,
                0, 0
            };

            UINT formatsFound = 0;

            if (chooseARB(
                    targetDC,
                    attributes,
                    NULL,
                    1,
                    &chosenFormat,
                    &formatsFound) &&
                formatsFound > 0)
            {
                fprintf(
                    stderr,
                    "[TimAPI GL] Selected pixel format with %d sample(s).\n",
                    samples);
                break;
            }

            chosenFormat = 0;
        }
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummy, dummyDC);
    DestroyWindow(dummy);

    if (chosenFormat)
    {
        if (!DescribePixelFormat(
                targetDC,
                chosenFormat,
                sizeof(*outputPFD),
                outputPFD))
        {
            return 0;
        }

        return chosenFormat;
    }

    /* Fallback to a conventional, non-multisampled format. */
    ZeroMemory(outputPFD, sizeof(*outputPFD));

    outputPFD->nSize = sizeof(*outputPFD);
    outputPFD->nVersion = 1;
    outputPFD->dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    outputPFD->iPixelType = PFD_TYPE_RGBA;
    outputPFD->cColorBits = 32;
    outputPFD->cAlphaBits = 8;
    outputPFD->cDepthBits = 24;
    outputPFD->cStencilBits = 8;
    outputPFD->iLayerType = PFD_MAIN_PLANE;

    return ChoosePixelFormat(targetDC, outputPFD);
}

/* ---------------------------------------------------------
   Native window procedure
   --------------------------------------------------------- */

static LRESULT CALLBACK TWAPI_Win32_WindowProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    TWAPI_Window* window = (TWAPI_Window*)
        GetWindowLongPtrA(hwnd, GWLP_USERDATA);

    if (message == WM_NCCREATE)
    {
        CREATESTRUCTA* createInfo =
            (CREATESTRUCTA*)lParam;

        window = (TWAPI_Window*)createInfo->lpCreateParams;

        if (window)
        {
            SetWindowLongPtrA(
                hwnd,
                GWLP_USERDATA,
                (LONG_PTR)window);

            window->nativeHandle = (void*)hwnd;
        }
    }

    switch (message)
    {
        case WM_CLOSE:
        {
            if (window)
                window->shouldClose = 1;

            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            if (window)
                window->shouldClose = 1;

            return 0;
        }

        case WM_NCDESTROY:
        {
            LRESULT result =
                DefWindowProcA(
                    hwnd,
                    message,
                    wParam,
                    lParam);

            if (window)
            {
                window->nativeHandle = NULL;
                window->shouldClose = 1;
            }

            SetWindowLongPtrA(
                hwnd,
                GWLP_USERDATA,
                0);

            return result;
        }

        case WM_SIZE:
        {
            if (window)
            {
                RECT clientRect;

                if (GetClientRect(hwnd, &clientRect))
                {
                    window->width =
                        clientRect.right - clientRect.left;

                    window->height =
                        clientRect.bottom - clientRect.top;
                }
            }

            return 0;
        }

        case WM_MOVE:
        {
            if (window)
            {
                RECT rect;

                if (GetWindowRect(hwnd, &rect))
                {
                    window->x = rect.left;
                    window->y = rect.top;
                }
            }

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            TWAPI_Win32_KeyDown(window, wParam);
            return 0;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            TWAPI_Win32_KeyUp(window, wParam);
            return 0;

        case WM_MOUSEMOVE:
            if (window) {
                window->mouseX = (int)(short)LOWORD(lParam);
                window->mouseY = (int)(short)HIWORD(lParam);
            }
            return 0;

        case WM_LBUTTONDOWN:
            SetCapture(hwnd);
            TWAPI_Win32_MouseDown(window, TWAPI_MOUSE_LEFT);
            return 0;

        case WM_LBUTTONUP:
            TWAPI_Win32_MouseUp(window, TWAPI_MOUSE_LEFT);
            if (GetCapture() == hwnd)
                ReleaseCapture();
            return 0;

        case WM_RBUTTONDOWN:
            SetCapture(hwnd);
            TWAPI_Win32_MouseDown(window, TWAPI_MOUSE_RIGHT);
            return 0;

        case WM_RBUTTONUP:
            TWAPI_Win32_MouseUp(window, TWAPI_MOUSE_RIGHT);
            if (GetCapture() == hwnd)
                ReleaseCapture();
            return 0;

        case WM_MBUTTONDOWN:
            SetCapture(hwnd);
            TWAPI_Win32_MouseDown(window, TWAPI_MOUSE_MIDDLE);
            return 0;

        case WM_MBUTTONUP:
            TWAPI_Win32_MouseUp(window, TWAPI_MOUSE_MIDDLE);
            if (GetCapture() == hwnd)
                ReleaseCapture();
            return 0;

        case WM_KILLFOCUS:
            if (window) {
                int i;

                for (i = 0; i < TWAPI_INPUT_KEY_COUNT; ++i) {
                    if (window->keysDown[i]) {
                        window->keysDown[i] = 0;
                        window->keysReleased[i] = 1;
                    }
                }

                for (i = 0; i < TWAPI_MOUSE_BUTTON_COUNT; ++i) {
                    if (window->mouseDown[i]) {
                        window->mouseDown[i] = 0;
                        window->mouseReleased[i] = 1;
                    }
                }
            }
            return 0;

        default:
            return DefWindowProcA(
                hwnd,
                message,
                wParam,
                lParam);
    }
}

/* ---------------------------------------------------------
   Initialize the Win32 backend
   --------------------------------------------------------- */

int TWAPI_Win32_Init(void)
{
    if (TWAPI_Win32_WindowClassAtom != 0)
        return 1;

    HINSTANCE instance = GetModuleHandleA(NULL);

    if (!instance)
        return 0;

    WNDCLASSEXA windowClass;
    memset(&windowClass, 0, sizeof(windowClass));

    windowClass.cbSize = sizeof(windowClass);
    windowClass.style =
        CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    windowClass.lpfnWndProc =
        TWAPI_Win32_WindowProc;

    windowClass.hInstance = instance;

    windowClass.hCursor =
        LoadCursorA(NULL, IDC_ARROW);

    windowClass.hbrBackground = NULL;

    windowClass.lpszClassName =
        TWAPI_WIN32_CLASS_NAME;

    ATOM atom = RegisterClassExA(&windowClass);

    if (!atom)
    {
        if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
            return 0;

        /*
           The class may already exist, for example if another
           copy of the library registered it in this process.
        */
        TWAPI_Win32_WindowClassAtom = 1;
        return 1;
    }

    TWAPI_Win32_WindowClassAtom = atom;
    return 1;
}

/* ---------------------------------------------------------
   Create a native Win32 window
   --------------------------------------------------------- */

void* TWAPI_Win32_CreateWindow(
    TWAPI_Window* window)
{
    if (!window)
        return NULL;

    if (!TWAPI_Win32_Init())
        return NULL;

    DWORD style;
    DWORD extendedStyle = 0;

    if (window->fullscreen)
    {
        style = WS_POPUP;

        window->x = 0;
        window->y = 0;

        window->width = GetSystemMetrics(SM_CXSCREEN);
        window->height = GetSystemMetrics(SM_CYSCREEN);
    }
    else
    {
        style = WS_OVERLAPPEDWINDOW;

        if (!window->resizable)
        {
            style &= ~WS_THICKFRAME;
            style &= ~WS_MAXIMIZEBOX;
        }
    }

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = window->width;
    rect.bottom = window->height;

    /*
       The public width and height represent the client area,
       not the entire outer window.
    */
    if (!AdjustWindowRectEx(
            &rect,
            style,
            FALSE,
            extendedStyle))
    {
        return NULL;
    }

    int outerWidth = rect.right - rect.left;
    int outerHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindowExA(
        extendedStyle,
        TWAPI_WIN32_CLASS_NAME,
        window->title,
        style,
        window->x,
        window->y,
        outerWidth,
        outerHeight,
        NULL,
        NULL,
        GetModuleHandleA(NULL),
        window);

    if (!hwnd)
        return NULL;

    window->nativeHandle = (void*)hwnd;
    TWAPI_Win32_ActiveWindow = window;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return (void*)hwnd;
}

/* ---------------------------------------------------------
   Destroy a native window
   --------------------------------------------------------- */

void TWAPI_Win32_DestroyWindow(TWAPI_Window* window)
{
    if (!window)
        return;

    TWAPI_Win32_DestroyPresentBuffer(window);

    HWND hwnd = (HWND)window->nativeHandle;

    if (hwnd && IsWindow(hwnd))
        DestroyWindow(hwnd);

    window->nativeHandle = NULL;
    window->shouldClose = 1;
}
/* ---------------------------------------------------------
   Process pending Windows messages
   --------------------------------------------------------- */

void TWAPI_Win32_PollEvents(void)
{
    TWAPI_Window* window = TWAPI_Win32_ActiveWindow;

    if (window) {
        memset(window->keysPressed, 0, sizeof(window->keysPressed));
        memset(window->keysReleased, 0, sizeof(window->keysReleased));
        memset(window->mousePressed, 0, sizeof(window->mousePressed));
        memset(window->mouseReleased, 0, sizeof(window->mouseReleased));
    }

    MSG message;

    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        if (message.message == WM_QUIT)
            continue;

        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

/* ---------------------------------------------------------
   Check whether a window should close
   --------------------------------------------------------- */

int TWAPI_Win32_WindowShouldClose(
    const TWAPI_Window* window)
{
    if (!window)
        return 1;

    if (window->shouldClose)
        return 1;

    if (!window->nativeHandle)
        return 1;

    return 0;
}

static int TWAPI_Win32_MapKey(WPARAM key)
{
    if (key >= 'A' && key <= 'Z')
        return (int)key;

    if (key >= '0' && key <= '9')
        return (int)key;

    switch (key) {
    case VK_SPACE:    return TWAPI_KEY_SPACE;
    case VK_RETURN:   return TWAPI_KEY_ENTER;
    case VK_ESCAPE:   return TWAPI_KEY_ESCAPE;
    case VK_TAB:      return TWAPI_KEY_TAB;
    case VK_BACK:     return TWAPI_KEY_BACKSPACE;
    case VK_LEFT:     return TWAPI_KEY_LEFT;
    case VK_RIGHT:    return TWAPI_KEY_RIGHT;
    case VK_UP:       return TWAPI_KEY_UP;
    case VK_DOWN:     return TWAPI_KEY_DOWN;
    case VK_SHIFT:    return TWAPI_KEY_SHIFT;
    case VK_CONTROL:  return TWAPI_KEY_CONTROL;
    case VK_MENU:     return TWAPI_KEY_ALT;

    case VK_F1:       return TWAPI_KEY_F1;
    case VK_F2:       return TWAPI_KEY_F2;
    case VK_F3:       return TWAPI_KEY_F3;
    case VK_F4:       return TWAPI_KEY_F4;
    case VK_F5:       return TWAPI_KEY_F5;
    case VK_F6:       return TWAPI_KEY_F6;
    case VK_F7:       return TWAPI_KEY_F7;
    case VK_F8:       return TWAPI_KEY_F8;
    case VK_F9:       return TWAPI_KEY_F9;
    case VK_F10:      return TWAPI_KEY_F10;
    case VK_F11:      return TWAPI_KEY_F11;
    case VK_F12:      return TWAPI_KEY_F12;

    default:
        return TWAPI_KEY_UNKNOWN;
    }
}

static void TWAPI_Win32_KeyDown(TWAPI_Window* window, WPARAM key)
{
    int code = TWAPI_Win32_MapKey(key);

    if (!window || code <= TWAPI_KEY_UNKNOWN ||
        code >= TWAPI_INPUT_KEY_COUNT)
        return;

    if (!window->keysDown[code])
        window->keysPressed[code] = 1;

    window->keysDown[code] = 1;
}

static void TWAPI_Win32_KeyUp(TWAPI_Window* window, WPARAM key)
{
    int code = TWAPI_Win32_MapKey(key);

    if (!window || code <= TWAPI_KEY_UNKNOWN ||
        code >= TWAPI_INPUT_KEY_COUNT)
        return;

    if (window->keysDown[code])
        window->keysReleased[code] = 1;

    window->keysDown[code] = 0;
}

static void TWAPI_Win32_MouseDown(
    TWAPI_Window* window, TWAPI_MouseButton button)
{
    if (!window || button < 0 || button >= TWAPI_MOUSE_BUTTON_COUNT)
        return;

    if (!window->mouseDown[button])
        window->mousePressed[button] = 1;

    window->mouseDown[button] = 1;
}

static void TWAPI_Win32_MouseUp(
    TWAPI_Window* window, TWAPI_MouseButton button)
{
    if (!window || button < 0 || button >= TWAPI_MOUSE_BUTTON_COUNT)
        return;

    if (window->mouseDown[button])
        window->mouseReleased[button] = 1;

    window->mouseDown[button] = 0;
}


static void TWAPI_Win32_DestroyPresentBuffer(
    TWAPI_Window* window)
{
    if (!window)
        return;

    HDC memoryDC = (HDC)window->presentDC;
    HBITMAP bitmap = (HBITMAP)window->presentBitmap;
    HBITMAP oldBitmap = (HBITMAP)window->presentOldBitmap;

    if (memoryDC) {
        if (oldBitmap)
            SelectObject(memoryDC, oldBitmap);

        if (bitmap)
            DeleteObject(bitmap);

        DeleteDC(memoryDC);
    }

    window->presentDC = NULL;
    window->presentBitmap = NULL;
    window->presentOldBitmap = NULL;
    window->presentWidth = 0;
    window->presentHeight = 0;
}

static int TWAPI_Win32_EnsurePresentBuffer(
    TWAPI_Window* window,
    HDC windowDC,
    int width,
    int height)
{
    if (window->presentDC &&
        window->presentBitmap &&
        window->presentWidth == width &&
        window->presentHeight == height)
        return 1;

    TWAPI_Win32_DestroyPresentBuffer(window);

    HDC memoryDC = CreateCompatibleDC(windowDC);
    if (!memoryDC)
        return 0;

    HBITMAP bitmap =
        CreateCompatibleBitmap(windowDC, width, height);

    if (!bitmap) {
        DeleteDC(memoryDC);
        return 0;
    }

    HGDIOBJ oldBitmap = SelectObject(memoryDC, bitmap);

    if (!oldBitmap || oldBitmap == HGDI_ERROR) {
        DeleteObject(bitmap);
        DeleteDC(memoryDC);
        return 0;
    }

    window->presentDC = memoryDC;
    window->presentBitmap = bitmap;
    window->presentOldBitmap = oldBitmap;
    window->presentWidth = width;
    window->presentHeight = height;

    return 1;
}

int TWAPI_Win32_Present(
    const TWAPI_Window* constWindow,
    const TWAPI_Framebuffer* framebuffer)
{
    if (!constWindow || !framebuffer ||
        !framebuffer->pixels ||
        framebuffer->width <= 0 ||
        framebuffer->height <= 0 ||
        framebuffer->pitch < framebuffer->width * 4)
        return 0;

    /* The cache is owned by the window, so this function updates it. */
    TWAPI_Window* window = (TWAPI_Window*)constWindow;

    HWND hwnd = (HWND)window->nativeHandle;

    if (!hwnd || !IsWindow(hwnd))
        return 0;

    RECT client;

    if (!GetClientRect(hwnd, &client))
        return 0;

    int clientWidth = client.right - client.left;
    int clientHeight = client.bottom - client.top;

    if (clientWidth <= 0 || clientHeight <= 0)
        return 1;

    HDC windowDC = GetDC(hwnd);

    if (!windowDC)
        return 0;

    if (!TWAPI_Win32_EnsurePresentBuffer(
            window, windowDC, clientWidth, clientHeight)) {
        ReleaseDC(hwnd, windowDC);
        return 0;
    }

    HDC memoryDC = (HDC)window->presentDC;

    /* Draw the entire frame off-screen first. */
    PatBlt(memoryDC, 0, 0,
           clientWidth, clientHeight, BLACKNESS);

    double sx = (double)clientWidth / framebuffer->width;
    double sy = (double)clientHeight / framebuffer->height;
    double scale = sx < sy ? sx : sy;

    int drawWidth = (int)(framebuffer->width * scale);
    int drawHeight = (int)(framebuffer->height * scale);

    if (drawWidth < 1) drawWidth = 1;
    if (drawHeight < 1) drawHeight = 1;

    int drawX = (clientWidth - drawWidth) / 2;
    int drawY = (clientHeight - drawHeight) / 2;

    BITMAPINFO info;
    ZeroMemory(&info, sizeof(info));

    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = framebuffer->width;
    info.bmiHeader.biHeight = -framebuffer->height;
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biBitCount = 32;
    info.bmiHeader.biCompression = BI_RGB;

    /*
       StretchDIBits assumes tightly packed rows here.
       If pitch differs from width * 4, copy rows into a
       tightly packed temporary buffer before presenting.
    */
    if (framebuffer->pitch != framebuffer->width * 4) {
        ReleaseDC(hwnd, windowDC);
        return 0;
    }

    int result = StretchDIBits(
        memoryDC,
        drawX, drawY, drawWidth, drawHeight,
        0, 0, framebuffer->width, framebuffer->height,
        framebuffer->pixels,
        &info,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    int success = result != (int)GDI_ERROR;

    if (success) {
        success = BitBlt(
            windowDC,
            0, 0, clientWidth, clientHeight,
            memoryDC,
            0, 0,
            SRCCOPY
        ) != 0;
    }

    ReleaseDC(hwnd, windowDC);
    return success;
}


#include <glad/gl10.h>

int TWAPI_Win32_CreateOpenGLContext(TWAPI_Window* window)
{
    if (!window || !window->nativeHandle)
        return 0;

    if (window->nativeContext)
        return 1;

    HWND hwnd = (HWND)window->nativeHandle;
    HDC dc = GetDC(hwnd);

    if (!dc)
        return 0;

    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int format = GetPixelFormat(dc);

    if (format == 0) {
        format = ChoosePixelFormat(dc, &pfd);

        if (!format || !SetPixelFormat(dc, format, &pfd)) {
            ReleaseDC(hwnd, dc);
            return 0;
        }
    }

    HGLRC context = wglCreateContext(dc);

    if (!context) {
        ReleaseDC(hwnd, dc);
        return 0;
    }

    window->nativeContext = (void*)context;

    int success = wglMakeCurrent(dc, context) != 0;

    if (!success) {
        wglDeleteContext(context);
        window->nativeContext = NULL;
    }

    ReleaseDC(hwnd, dc);
    return success;
}

void TWAPI_Win32_DestroyOpenGLContext(TWAPI_Window* window)
{
    if (!window || !window->nativeContext)
        return;

    HGLRC context = (HGLRC)window->nativeContext;

    if (wglGetCurrentContext() == context)
        wglMakeCurrent(NULL, NULL);

    wglDeleteContext(context);
    window->nativeContext = NULL;
}

int TWAPI_Win32_MakeOpenGLCurrent(TWAPI_Window* window)
{
    if (!window)
    {
        fprintf(stderr, "[TimAPI GL] Window is NULL.\n");
        return 0;
    }

    if (!window->nativeHandle)
    {
        fprintf(stderr, "[TimAPI GL] Native window handle is NULL.\n");
        return 0;
    }

    if (!window->nativeContext)
    {
        fprintf(stderr, "[TimAPI GL] OpenGL context is NULL.\n");
        return 0;
    }

    HWND hwnd = (HWND)window->nativeHandle;
    HGLRC context = (HGLRC)window->nativeContext;

    HDC dc = GetDC(hwnd);

    if (!dc)
    {
        fprintf(stderr,
                "[TimAPI GL] GetDC failed. Error=%lu\n",
                (unsigned long)GetLastError());
        return 0;
    }

    int pixelFormat = GetPixelFormat(dc);

    if (pixelFormat == 0)
    {
        fprintf(stderr,
                "[TimAPI GL] Window has no pixel format. Error=%lu\n",
                (unsigned long)GetLastError());

        ReleaseDC(hwnd, dc);
        return 0;
    }

    if (wglGetCurrentContext() == context)
    {
        ReleaseDC(hwnd, dc);
        return 1;
    }

    if (!wglMakeCurrent(dc, context))
    {
        DWORD error = GetLastError();

        fprintf(stderr,
                "[TimAPI GL] wglMakeCurrent failed. Error=%lu\n"
                "[TimAPI GL] HDC=%p HGLRC=%p PixelFormat=%d\n",
                (unsigned long)error,
                (void*)dc,
                (void*)context,
                pixelFormat);

        ReleaseDC(hwnd, dc);
        return 0;
    }

    ReleaseDC(hwnd, dc);

    fprintf(stderr, "[TimAPI GL] Context made current successfully.\n");
    return 1;
}

int TWAPI_Win32_SwapOpenGLBuffers(TWAPI_Window* window)
{
    if (!window || !window->nativeHandle ||
        !window->nativeContext)
        return 0;

    HWND hwnd = (HWND)window->nativeHandle;
    HDC dc = GetDC(hwnd);

    if (!dc)
        return 0;

    int success = SwapBuffers(dc) != 0;

    ReleaseDC(hwnd, dc);
    return success;
}

TWAPI_GLProc TWAPI_Win32_GetOpenGLProcAddress(const char* name)
{
    if (!name)
        return NULL;

    PROC proc = wglGetProcAddress(name);

    if (proc &&
        proc != (PROC)1 &&
        proc != (PROC)2 &&
        proc != (PROC)3 &&
        proc != (PROC)-1)
        return (TWAPI_GLProc)proc;

    HMODULE module = GetModuleHandleA("opengl32.dll");

    if (!module)
        module = LoadLibraryA("opengl32.dll");

    if (!module)
        return NULL;

    return (TWAPI_GLProc)GetProcAddress(module, name);
}

/* WGL context attributes, defined by WGL extensions. */
#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#endif

#ifndef WGL_CONTEXT_MINOR_VERSION_ARB
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#endif

#ifndef WGL_CONTEXT_FLAGS_ARB
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#endif

#ifndef WGL_CONTEXT_DEBUG_BIT_ARB
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#endif

#ifndef WGL_CONTEXT_PROFILE_MASK_ARB
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#endif

#ifndef WGL_CONTEXT_CORE_PROFILE_BIT_ARB
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

#ifndef WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#endif

typedef HGLRC (WINAPI *TWAPI_WGLCreateContextAttribsARB)(
    HDC hdc,
    HGLRC shareContext,
    const int* attribList
);

static PROC TWAPI_Win32_GetWGLProcAddress(const char* name)
{
    PROC proc = wglGetProcAddress(name);

    if (proc == NULL ||
        proc == (PROC)1 ||
        proc == (PROC)2 ||
        proc == (PROC)3 ||
        proc == (PROC)-1)
        return NULL;

    return proc;
}

int TWAPI_Win32_CreateOpenGLContextEx(
    TWAPI_Window* window,
    const TWAPI_OpenGLContextConfig* config)
{
    if (!window || !window->nativeHandle || !config)
        return 0;

    if (window->nativeContext)
        return 0;

    if (config->majorVersion < 1 ||
        config->minorVersion < 0 ||
        config->debug < 0 ||
        config->debug > 1)
        return 0;

    if (config->profile < TWAPI_OPENGL_PROFILE_DEFAULT ||
        config->profile > TWAPI_OPENGL_PROFILE_COMPATIBILITY)
        return 0;

    if (config->majorVersion < 3 &&
        config->profile != TWAPI_OPENGL_PROFILE_DEFAULT)
        return 0;

    if (config->majorVersion == 3 &&
        config->minorVersion < 2 &&
        config->profile != TWAPI_OPENGL_PROFILE_DEFAULT)
        return 0;

    HWND hwnd = (HWND)window->nativeHandle;
    HDC dc = GetDC(hwnd);

    if (!dc)
        return 0;

    if (config->samples != 0 &&
        config->samples != 2 &&
        config->samples != 4 &&
        config->samples != 8 &&
        config->samples != 16)
    {
        ReleaseDC(hwnd, dc);
        return 0;
    }

    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));

    int pixelFormat = TWAPI_Win32_ChoosePixelFormatMSAA(
        dc,
        config->samples,
        &pfd);

    if (!pixelFormat)
    {
        fprintf(stderr,
            "[TimAPI GL] ERROR: Could not choose a pixel format.\n");

        ReleaseDC(hwnd, dc);
        return 0;
    }

    if (GetPixelFormat(dc) == 0)
    {
        if (!SetPixelFormat(dc, pixelFormat, &pfd))
        {
            fprintf(stderr,
                "[TimAPI GL] ERROR: SetPixelFormat failed.\n");

            ReleaseDC(hwnd, dc);
            return 0;
        }
    }

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    pixelFormat = GetPixelFormat(dc);

    if (pixelFormat == 0) {
        pixelFormat = ChoosePixelFormat(dc, &pfd);

        if (!pixelFormat ||
            !SetPixelFormat(dc, pixelFormat, &pfd)) {
            ReleaseDC(hwnd, dc);
            return 0;
        }
    }

    if (config->majorVersion == 1 &&
        config->minorVersion <= 1 &&
        config->profile == TWAPI_OPENGL_PROFILE_DEFAULT &&
        config->debug == 0)
    {
        fprintf(stderr, "[TimAPI GL] Creating legacy context...\n");

        int format = GetPixelFormat(dc);
        fprintf(stderr, "[TimAPI GL] Pixel format: %d\n", format);

        if (format == 0)
        {
            fprintf(stderr,
                    "[TimAPI GL] ERROR: No pixel format is set.\n");
            ReleaseDC(hwnd, dc);
            return 0;
        }

        HGLRC context = wglCreateContext(dc);

        if (!context)
        {
            fprintf(stderr,
                    "[TimAPI GL] wglCreateContext failed. "
                    "GetLastError=%lu\n",
                    (unsigned long)GetLastError());

            ReleaseDC(hwnd, dc);
            return 0;
        }

        if (!wglMakeCurrent(dc, context))
        {
            fprintf(stderr,
                    "[TimAPI GL] wglMakeCurrent failed. "
                    "GetLastError=%lu\n",
                    (unsigned long)GetLastError());

            wglDeleteContext(context);
            ReleaseDC(hwnd, dc);
            return 0;
        }

        fprintf(stderr, "[TimAPI GL] Context created successfully.\n");

        window->nativeContext = (void*)context;

        ReleaseDC(hwnd, dc);
        return 1;
    }

    /*
       Create a temporary legacy context so we can load
       wglCreateContextAttribsARB.
    */
    HGLRC bootstrap = wglCreateContext(dc);

    if (!bootstrap) {
        ReleaseDC(hwnd, dc);
        return 0;
    }

    if (!wglMakeCurrent(dc, bootstrap)) {
        wglDeleteContext(bootstrap);
        ReleaseDC(hwnd, dc);
        return 0;
    }

    PROC proc = TWAPI_Win32_GetWGLProcAddress(
        "wglCreateContextAttribsARB");

    TWAPI_WGLCreateContextAttribsARB createAttribs = NULL;

    if (proc != NULL)
    {
        memcpy(&createAttribs, &proc, sizeof(createAttribs));
    }

    /*
       OpenGL 1.x requests use the bootstrap context directly.
       A modern version requires the WGL extension.
    */
    if (config->majorVersion == 1 &&
        config->minorVersion <= 1) {
        window->nativeContext = (void*)bootstrap;
        ReleaseDC(hwnd, dc);
        return 1;
    }

    if (!createAttribs) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(bootstrap);
        ReleaseDC(hwnd, dc);
        return 0;
    }

    int attributes[9];
    int i = 0;

    attributes[i++] = WGL_CONTEXT_MAJOR_VERSION_ARB;
    attributes[i++] = config->majorVersion;

    attributes[i++] = WGL_CONTEXT_MINOR_VERSION_ARB;
    attributes[i++] = config->minorVersion;

    if (config->debug) {
        attributes[i++] = WGL_CONTEXT_FLAGS_ARB;
        attributes[i++] = WGL_CONTEXT_DEBUG_BIT_ARB;
    }

    if (config->majorVersion > 3 ||
        (config->majorVersion == 3 &&
         config->minorVersion >= 2)) {
        if (config->profile != TWAPI_OPENGL_PROFILE_DEFAULT) {
            attributes[i++] = WGL_CONTEXT_PROFILE_MASK_ARB;
            attributes[i++] =
                config->profile == TWAPI_OPENGL_PROFILE_CORE
                ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB
                : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
        }
    }

    attributes[i++] = 0;

    HGLRC context = createAttribs(dc, NULL, attributes);

    if (!context) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(bootstrap);
        ReleaseDC(hwnd, dc);
        return 0;
    }

    if (!wglMakeCurrent(dc, context)) {
        wglDeleteContext(context);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(bootstrap);
        ReleaseDC(hwnd, dc);
        return 0;
    }

    /* The modern context is now active. Retire the bootstrap. */
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(bootstrap);

    if (!wglMakeCurrent(dc, context)) {
        wglDeleteContext(context);
        ReleaseDC(hwnd, dc);
        return 0;
    }

    window->nativeContext = (void*)context;

    ReleaseDC(hwnd, dc);
    return 1;
}

// int TWAPI_Win32_MakeOpenGLCurrent(TWAPI_Window* window)
// {
//     if (!window || !window->nativeHandle || !window->nativeContext)
//         return 0;

//     HWND hwnd = (HWND)window->nativeHandle;
//     HDC dc = GetDC(hwnd);

//     if (!dc)
//         return 0;

//     HGLRC context = (HGLRC)window->nativeContext;
//     int success = wglMakeCurrent(dc, context) != 0;

//     ReleaseDC(hwnd, dc);

//     return success;
// }

int TWAPI_Win32_SetFullscreen(
    TWAPI_Window* window,
    int fullscreen)
{
    if (!window || !window->nativeHandle)
        return 0;

    HWND hwnd = (HWND)window->nativeHandle;
    fullscreen = fullscreen != 0;

    if (window->fullscreen == fullscreen)
        return 1;

    if (fullscreen)
    {
        /* Save the original window position and dimensions. */
        RECT rect;

        if (!GetWindowRect(hwnd, &rect))
            return 0;

        window->windowedX = rect.left;
        window->windowedY = rect.top;
        window->windowedOuterWidth = rect.right - rect.left;
        window->windowedOuterHeight = rect.bottom - rect.top;

        window->windowedStyle =
            (long)GetWindowLongPtrA(hwnd, GWL_STYLE);

        window->windowedExtendedStyle =
            (long)GetWindowLongPtrA(hwnd, GWL_EXSTYLE);

        /* Remove borders and title bar. */
        SetWindowLongPtrA(
            hwnd,
            GWL_STYLE,
            WS_POPUP | WS_VISIBLE);

        /* Cover the primary monitor. */
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);

        if (!SetWindowPos(
                hwnd,
                HWND_TOP,
                0, 0,
                width, height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW))
        {
            return 0;
        }

        window->fullscreen = 1;
    }
    else
    {
        /* Restore the original window styles. */
        SetWindowLongPtrA(
            hwnd,
            GWL_STYLE,
            window->windowedStyle);

        SetWindowLongPtrA(
            hwnd,
            GWL_EXSTYLE,
            window->windowedExtendedStyle);

        /* Restore the original position and size. */
        if (!SetWindowPos(
                hwnd,
                NULL,
                window->windowedX,
                window->windowedY,
                window->windowedOuterWidth,
                window->windowedOuterHeight,
                SWP_FRAMECHANGED |
                SWP_NOZORDER |
                SWP_NOOWNERZORDER |
                SWP_SHOWWINDOW))
        {
            return 0;
        }

        window->fullscreen = 0;
    }

    return 1;
}

typedef BOOL (WINAPI *TWAPI_PFNWGLSWAPINTERVALEXT)(int interval);

int TWAPI_Win32_SetVSync(
    TWAPI_Window* window,
    int enabled)
{
    if (!window ||
        !window->nativeHandle ||
        !window->nativeContext)
    {
        return 0;
    }

    HWND hwnd = (HWND)window->nativeHandle;
    HDC dc = GetDC(hwnd);

    if (!dc)
        return 0;

    HGLRC context = (HGLRC)window->nativeContext;

    if (!wglMakeCurrent(dc, context))
    {
        ReleaseDC(hwnd, dc);
        return 0;
    }

    PROC proc = wglGetProcAddress("wglSwapIntervalEXT");

    TWAPI_PFNWGLSWAPINTERVALEXT swapInterval = NULL;

    if (proc != NULL &&
        proc != (PROC)1 &&
        proc != (PROC)2 &&
        proc != (PROC)3 &&
        proc != (PROC)-1)
    {
        memcpy(&swapInterval, &proc, sizeof(swapInterval));
    }

    if (!swapInterval ||
        swapInterval == (TWAPI_PFNWGLSWAPINTERVALEXT)1 ||
        swapInterval == (TWAPI_PFNWGLSWAPINTERVALEXT)2 ||
        swapInterval == (TWAPI_PFNWGLSWAPINTERVALEXT)3 ||
        swapInterval == (TWAPI_PFNWGLSWAPINTERVALEXT)-1)
    {
        ReleaseDC(hwnd, dc);
        return 0;
    }

    BOOL success = swapInterval(enabled ? 1 : 0);

    ReleaseDC(hwnd, dc);
    return success ? 1 : 0;
}