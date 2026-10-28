
#include <TW_window_internal.h>
#include <TW_platform_detect.h>
#include <TW_win32.h>

#include <stdlib.h>
#include <string.h>

// TWAPI_Window* TWAPI_CreateWindow(
//     int x,
//     int y,
//     int width,
//     int height,
//     const char* title)
// {
//     if (width <= 0 || height <= 0)
//         return NULL;

//     TWAPI_Window* window =
//         (TWAPI_Window*)calloc(1, sizeof(TWAPI_Window));

//     if (!window)
//         return NULL;

//     window->x = x;
//     window->y = y;
//     window->width = width;
//     window->height = height;
//     window->backend = TWAPI_WINDOW_BACKEND_NONE;

//     if (title)
//     {
//         strncpy(window->title, title, TWAPI_WINDOW_TITLE_MAX);
//         window->title[TWAPI_WINDOW_TITLE_MAX] = '\0';
//     }

// #if defined(TWAPI_PLATFORM_WIN32)

//     if (!TWAPI_Win32_Init())
//     {
//         free(window);
//         return NULL;
//     }

//     window->nativeHandle = TWAPI_Win32_CreateWindow(window);

//     if (!window->nativeHandle)
//     {
//         free(window);
//         return NULL;
//     }

//     window->backend = TWAPI_WINDOW_BACKEND_WIN32;

// #else

//     /* This target does not have a window backend yet. */
//     free(window);
//     return NULL;

// #endif

//     return window;
// }

TWAPI_Window* TWAPI_CreateWindow(
    int x,
    int y,
    int width,
    int height,
    const char* title)
{
    TWAPI_WindowConfig config = {0};

    config.x = x;
    config.y = y;
    config.width = width;
    config.height = height;
    config.title = title;

    /* Preserve the original behavior: resizable by default. */
    config.resizable = 1;

    return TWAPI_CreateWindowEx(&config);
}

void TWAPI_DestroyWindow(TWAPI_Window* window)
{
    if (!window)
        return;

    switch (window->backend)
    {
#if defined(TWAPI_PLATFORM_WIN32)
        case TWAPI_WINDOW_BACKEND_WIN32:
            TWAPI_Win32_DestroyOpenGLContext(window);
            TWAPI_Win32_DestroyWindow(window);
            break;
#endif

        default:
            break;
    }

    free(window);
}

int TWAPI_WindowShouldClose(const TWAPI_Window* window)
{
    if (!window)
        return 1;

#if defined(TWAPI_PLATFORM_WIN32)
    if (window->backend == TWAPI_WINDOW_BACKEND_WIN32)
        return TWAPI_Win32_WindowShouldClose(window);
#endif

    return window->shouldClose;
}

void TWAPI_PollEvents(void)
{
#if defined(TWAPI_PLATFORM_WIN32)
    TWAPI_Win32_PollEvents();
#endif
}

int TWAPI_IsKeyDown(const TWAPI_Window* window, TWAPI_Key key)
{
    if (!window || key < 0 || key >= TWAPI_INPUT_KEY_COUNT)
        return 0;

    return window->keysDown[key] != 0;
}

int TWAPI_IsKeyPressed(const TWAPI_Window* window, TWAPI_Key key)
{
    if (!window || key < 0 || key >= TWAPI_INPUT_KEY_COUNT)
        return 0;

    return window->keysPressed[key] != 0;
}

int TWAPI_IsKeyReleased(const TWAPI_Window* window, TWAPI_Key key)
{
    if (!window || key < 0 || key >= TWAPI_INPUT_KEY_COUNT)
        return 0;

    return window->keysReleased[key] != 0;
}

int TWAPI_GetMouseX(const TWAPI_Window* window)
{
    return window ? window->mouseX : 0;
}

int TWAPI_GetMouseY(const TWAPI_Window* window)
{
    return window ? window->mouseY : 0;
}

int TWAPI_IsMouseDown(
    const TWAPI_Window* window, TWAPI_MouseButton button)
{
    if (!window || button < 0 || button >= TWAPI_MOUSE_BUTTON_COUNT)
        return 0;

    return window->mouseDown[button] != 0;
}

int TWAPI_IsMousePressed(
    const TWAPI_Window* window, TWAPI_MouseButton button)
{
    if (!window || button < 0 || button >= TWAPI_MOUSE_BUTTON_COUNT)
        return 0;

    return window->mousePressed[button] != 0;
}

int TWAPI_IsMouseReleased(
    const TWAPI_Window* window, TWAPI_MouseButton button)
{
    if (!window || button < 0 || button >= TWAPI_MOUSE_BUTTON_COUNT)
        return 0;

    return window->mouseReleased[button] != 0;
}


#include "TWAPI/TW_framebuffer.h"

/* Keep this function with the other public window API functions. */
int TWAPI_Present(
    const TWAPI_Window* window,
    const TWAPI_Framebuffer* framebuffer)
{
    if (!window || !framebuffer || !framebuffer->pixels)
        return 0;

#if defined(TWAPI_PLATFORM_WIN32)
    if (window->backend == TWAPI_WINDOW_BACKEND_WIN32)
        return TWAPI_Win32_Present(window, framebuffer);
#endif

    return 0;
}

int TWAPI_SwapOpenGLBuffers(TWAPI_Window* window)
{
    if (!window)
        return 0;

#if defined(TWAPI_PLATFORM_WIN32)
    if (window->backend == TWAPI_WINDOW_BACKEND_WIN32)
        return TWAPI_Win32_SwapOpenGLBuffers(window);
#endif

    return 0;
}

void TWAPI_DestroyOpenGLContext(TWAPI_Window* window)
{
    if (!window)
        return;

#if defined(TWAPI_PLATFORM_WIN32)
    if (window->backend == TWAPI_WINDOW_BACKEND_WIN32)
        TWAPI_Win32_DestroyOpenGLContext(window);
#endif
}

int TWAPI_CreateOpenGLContext(TWAPI_Window* window)
{
    if (!window)
        return 0;

#if defined(TWAPI_PLATFORM_WIN32)
    if (window->backend == TWAPI_WINDOW_BACKEND_WIN32)
        return TWAPI_Win32_CreateOpenGLContext(window);
#endif

    return 0;
}

int TWAPI_MakeOpenGLCurrent(TWAPI_Window* window)
{
    if (!window || !window->nativeContext)
        return 0;

#if defined(TWAPI_PLATFORM_WIN32)
    if (window->backend == TWAPI_WINDOW_BACKEND_WIN32)
        return TWAPI_Win32_MakeOpenGLCurrent(window);
#endif

    return 0;
}

#include <stdio.h>

int TWAPI_CreateOpenGLContextEx(
    TWAPI_Window* window,
    const TWAPI_OpenGLContextConfig* config)
{
    fprintf(stderr, "[TimAPI] CreateOpenGLContextEx called.\n");

    if (!window || !config) {
        fprintf(stderr, "[TimAPI] ERROR: window or config is NULL.\n");
        return 0;
    }

#if defined(TWAPI_PLATFORM_WIN32)
    fprintf(stderr, "[TimAPI] Calling Win32 backend.\n");

    int result = TWAPI_Win32_CreateOpenGLContextEx(
        window, config);

    fprintf(stderr, "[TimAPI] Win32 backend returned %d.\n", result);
    return result;
#else
    fprintf(stderr, "[TimAPI] ERROR: Unsupported platform.\n");
    return 0;
#endif
}

TWAPI_GLProc TWAPI_GetOpenGLProcAddress(const char* name)
{
    if (!name || name[0] == '\0')
        return NULL;

#if defined(TWAPI_PLATFORM_WIN32)
    return TWAPI_Win32_GetOpenGLProcAddress(name);
#else
    return NULL;
#endif
}

void TWAPI_GetWindowSize(
    const TWAPI_Window* window,
    int* width,
    int* height)
{
    if (width)
        *width = window ? window->width : 0;

    if (height)
        *height = window ? window->height : 0;
}

TWAPI_Window* TWAPI_CreateWindowEx(
    const TWAPI_WindowConfig* config)
{
    if (!config)
        return NULL;

    if (config->width <= 0 || config->height <= 0)
        return NULL;

    TWAPI_Window* window = (TWAPI_Window*)calloc(
        1,
        sizeof(TWAPI_Window)
    );

    if (!window)
        return NULL;

    window->backend = TWAPI_WINDOW_BACKEND_WIN32;

    window->x = config->x;
    window->y = config->y;
    window->width = config->width;
    window->height = config->height;

    window->resizable = config->resizable != 0;
    window->fullscreen = config->fullscreen != 0;

    if (config->title)
    {
        strncpy(
            window->title,
            config->title,
            TWAPI_WINDOW_TITLE_MAX
        );

        window->title[TWAPI_WINDOW_TITLE_MAX] = '\0';
    }
    else
    {
        window->title[0] = '\0';
    }

    if (!TWAPI_Win32_CreateWindow(window))
    {
        free(window);
        return NULL;
    }

    return window;
}

int TWAPI_SetFullscreen(
    TWAPI_Window* window,
    int fullscreen)
{
    if (!window)
        return 0;

    #if defined(TWAPI_PLATFORM_WIN32)
        return TWAPI_Win32_SetFullscreen(window, fullscreen);
    #else
        return 0;
    #endif
}

int TWAPI_IsFullscreen(const TWAPI_Window* window)
{
    return window ? window->fullscreen : 0;
}

int TWAPI_SetVSync(TWAPI_Window* window, int enabled)
{
#if defined(TWAPI_PLATFORM_WIN32)
    return TWAPI_Win32_SetVSync(window, enabled);
#else
    (void)window;
    (void)enabled;
    return 0;
#endif
}

int TWAPI_IsVSync(TWAPI_Window* window)
{
    return window ? window->fullscreen : 0;
}

int TWAPI_RecreateOpenGLWindow(
    TWAPI_Window** window,
    const TWAPI_WindowConfig* windowConfig,
    const TWAPI_OpenGLContextConfig* glConfig,
    int vsync)
{
    if (!window || !*window || !windowConfig || !glConfig)
        return 0;

    TWAPI_Window* oldWindow = *window;

    /* Create the replacement without disturbing the old window. */
    TWAPI_Window* newWindow = TWAPI_CreateWindowEx(windowConfig);

    if (!newWindow)
        return 0;

    if (!TWAPI_CreateOpenGLContextEx(newWindow, glConfig))
    {
        TWAPI_DestroyWindow(newWindow);
        return 0;
    }

    if (vsync && !TWAPI_SetVSync(newWindow, 1))
    {
        fprintf(stderr,
            "[TimAPI] Warning: VSync could not be enabled.\n");
    }

    /*
       Replacement is ready. Destroy the old window and update
       the caller's pointer.
    */
    TWAPI_DestroyWindow(oldWindow);
    *window = newWindow;

    return 1;
}