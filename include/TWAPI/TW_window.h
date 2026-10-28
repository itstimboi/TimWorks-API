#ifndef TW_WINDOW_H
#define TW_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TWAPI_Window TWAPI_Window;

typedef enum TWAPI_WindowBackend {
    TWAPI_WINDOW_BACKEND_NONE = 0,
    TWAPI_WINDOW_BACKEND_WIN32,
    TWAPI_WINDOW_BACKEND_COCOA,
    TWAPI_WINDOW_BACKEND_X11,
    TWAPI_WINDOW_BACKEND_WAYLAND
} TWAPI_WindowBackend;

/* Portable key identifiers. Letters and digits use ASCII values. */
typedef enum TWAPI_Key {
    TWAPI_KEY_UNKNOWN = 0,

    TWAPI_KEY_A = 'A',
    TWAPI_KEY_B = 'B',
    TWAPI_KEY_C = 'C',
    TWAPI_KEY_D = 'D',
    TWAPI_KEY_E = 'E',
    TWAPI_KEY_F = 'F',
    TWAPI_KEY_G = 'G',
    TWAPI_KEY_H = 'H',
    TWAPI_KEY_I = 'I',
    TWAPI_KEY_J = 'J',
    TWAPI_KEY_K = 'K',
    TWAPI_KEY_L = 'L',
    TWAPI_KEY_M = 'M',
    TWAPI_KEY_N = 'N',
    TWAPI_KEY_O = 'O',
    TWAPI_KEY_P = 'P',
    TWAPI_KEY_Q = 'Q',
    TWAPI_KEY_R = 'R',
    TWAPI_KEY_S = 'S',
    TWAPI_KEY_T = 'T',
    TWAPI_KEY_U = 'U',
    TWAPI_KEY_V = 'V',
    TWAPI_KEY_W = 'W',
    TWAPI_KEY_X = 'X',
    TWAPI_KEY_Y = 'Y',
    TWAPI_KEY_Z = 'Z',

    TWAPI_KEY_0 = '0',
    TWAPI_KEY_1 = '1',
    TWAPI_KEY_2 = '2',
    TWAPI_KEY_3 = '3',
    TWAPI_KEY_4 = '4',
    TWAPI_KEY_5 = '5',
    TWAPI_KEY_6 = '6',
    TWAPI_KEY_7 = '7',
    TWAPI_KEY_8 = '8',
    TWAPI_KEY_9 = '9',

    TWAPI_KEY_SPACE = 256,
    TWAPI_KEY_ENTER,
    TWAPI_KEY_ESCAPE,
    TWAPI_KEY_TAB,
    TWAPI_KEY_BACKSPACE,
    TWAPI_KEY_LEFT,
    TWAPI_KEY_RIGHT,
    TWAPI_KEY_UP,
    TWAPI_KEY_DOWN,
    TWAPI_KEY_SHIFT,
    TWAPI_KEY_CONTROL,
    TWAPI_KEY_ALT,
    TWAPI_KEY_F1,
    TWAPI_KEY_F2,
    TWAPI_KEY_F3,
    TWAPI_KEY_F4,
    TWAPI_KEY_F5,
    TWAPI_KEY_F6,
    TWAPI_KEY_F7,
    TWAPI_KEY_F8,
    TWAPI_KEY_F9,
    TWAPI_KEY_F10,
    TWAPI_KEY_F11,
    TWAPI_KEY_F12
} TWAPI_Key;

typedef enum TWAPI_MouseButton {
    TWAPI_MOUSE_LEFT = 0,
    TWAPI_MOUSE_RIGHT,
    TWAPI_MOUSE_MIDDLE,
    TWAPI_MOUSE_BUTTON_COUNT
} TWAPI_MouseButton;


typedef enum TWAPI_OpenGLProfile {
    TWAPI_OPENGL_PROFILE_DEFAULT = 0,
    TWAPI_OPENGL_PROFILE_CORE,
    TWAPI_OPENGL_PROFILE_COMPATIBILITY
} TWAPI_OpenGLProfile;

typedef struct TWAPI_OpenGLContextConfig {
    int majorVersion;
    int minorVersion;
    TWAPI_OpenGLProfile profile;
    int debug;

    int samples; /* 0 == off, 2 = 2x, 4, = 4x, 8 = 8x */
} TWAPI_OpenGLContextConfig;

typedef void (*TWAPI_GLProc)(void);

TWAPI_Window* TWAPI_CreateWindow(
    int x, int y, int width, int height, const char* title);

void TWAPI_DestroyWindow(TWAPI_Window* window);
int TWAPI_WindowShouldClose(const TWAPI_Window* window);
void TWAPI_PollEvents(void);

/* Keyboard state */
int TWAPI_IsKeyDown(const TWAPI_Window* window, TWAPI_Key key);
int TWAPI_IsKeyPressed(const TWAPI_Window* window, TWAPI_Key key);
int TWAPI_IsKeyReleased(const TWAPI_Window* window, TWAPI_Key key);

/* Mouse state. Coordinates are relative to the client area. */
int TWAPI_GetMouseX(const TWAPI_Window* window);
int TWAPI_GetMouseY(const TWAPI_Window* window);
int TWAPI_IsMouseDown(
    const TWAPI_Window* window, TWAPI_MouseButton button);
int TWAPI_IsMousePressed(
    const TWAPI_Window* window, TWAPI_MouseButton button);
int TWAPI_IsMouseReleased(
    const TWAPI_Window* window, TWAPI_MouseButton button);

int TWAPI_CreateOpenGLContext(TWAPI_Window* window);
void TWAPI_DestroyOpenGLContext(TWAPI_Window* window);
int TWAPI_MakeOpenGLCurrent(TWAPI_Window* window);
int TWAPI_SwapOpenGLBuffers(TWAPI_Window* window);
TWAPI_GLProc TWAPI_GetOpenGLProcAddress(const char* name);

/* Create a context with a requested version and profile. */
int TWAPI_CreateOpenGLContextEx(
    TWAPI_Window* window,
    const TWAPI_OpenGLContextConfig* config
);

/* Backward-compatible entry point for a legacy OpenGL context. */
int TWAPI_CreateOpenGLContext(TWAPI_Window* window);

/* Get the current client-area dimensions in pixels. */
void TWAPI_GetWindowSize(
    const TWAPI_Window* window,
    int* width,
    int* height
);

typedef struct TWAPI_WindowConfig
{
    int x;
    int y;
    int width;
    int height;
    const char* title;
    int resizable;  /* Nonzero = resizable, zero = fixed-size */
    int fullscreen; /* 1 = fullscreen, 0 = windowed */
} TWAPI_WindowConfig;

TWAPI_Window* TWAPI_CreateWindowEx(
    const TWAPI_WindowConfig* config
);

int TWAPI_SetFullscreen(TWAPI_Window* window, int fullscreen);
int TWAPI_IsFullscreen(const TWAPI_Window* window);

int TWAPI_SetVSync(TWAPI_Window* window, int enabled);
int TWAPI_IsVSync(TWAPI_Window* window);

int TWAPI_RecreateOpenGLWindow(
    TWAPI_Window** window,
    const TWAPI_WindowConfig* windowConfig,
    const TWAPI_OpenGLContextConfig* glConfig,
    int vsync);

#ifdef __cplusplus
}
#endif

#endif
