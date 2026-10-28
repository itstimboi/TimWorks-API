#ifndef TW_WIN32_H
#define TW_WIN32_H

#include "TW_window_internal.h"

#ifdef _WIN32

#ifdef __cplusplus
extern "C" {
#endif

int TWAPI_Win32_Init(void);

void* TWAPI_Win32_CreateWindow(
    TWAPI_Window* window
);

void TWAPI_Win32_DestroyWindow(
    TWAPI_Window* window
);

void TWAPI_Win32_PollEvents(void);

int TWAPI_Win32_WindowShouldClose(
    const TWAPI_Window* window
);

#include "TWAPI/TW_framebuffer.h"

int TWAPI_Win32_Present(
    const TWAPI_Window* window,
    const TWAPI_Framebuffer* framebuffer
);

int TWAPI_Win32_CreateOpenGLContext(TWAPI_Window* window);

int TWAPI_Win32_CreateOpenGLContextEx(
    TWAPI_Window* window,
    const TWAPI_OpenGLContextConfig* config);

void TWAPI_Win32_DestroyOpenGLContext(TWAPI_Window* window);

int TWAPI_Win32_SwapOpenGLBuffers(TWAPI_Window* window);

int TWAPI_Win32_MakeOpenGLCurrent(TWAPI_Window* window);

TWAPI_GLProc TWAPI_Win32_GetOpenGLProcAddress(const char* name);

int TWAPI_Win32_SetFullscreen(TWAPI_Window* window, int fullscreen);

int TWAPI_Win32_SetVSync(TWAPI_Window* window, int enabled);

#ifdef __cplusplus
}
#endif

#endif /* _WIN32 */
#endif /* TW_WIN32_H */