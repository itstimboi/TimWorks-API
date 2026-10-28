#ifndef TW_FRAMEBUFFER_H
#define TW_FRAMEBUFFER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TWAPI_Window TWAPI_Window;

typedef struct TWAPI_Framebuffer {
    int width;
    int height;
    int pitch;          /* Bytes per row. */
    uint32_t* pixels;   /* Pixels in 0x00RRGGBB format. */
} TWAPI_Framebuffer;

TWAPI_Framebuffer* TWAPI_CreateFramebuffer(int width, int height);
void TWAPI_DestroyFramebuffer(TWAPI_Framebuffer* framebuffer);

void TWAPI_Clear(
    TWAPI_Framebuffer* framebuffer,
    uint32_t color
);

void TWAPI_SetPixel(
    TWAPI_Framebuffer* framebuffer,
    int x,
    int y,
    uint32_t color
);

uint32_t TWAPI_GetPixel(
    const TWAPI_Framebuffer* framebuffer,
    int x,
    int y
);

void TWAPI_FillRect(
    TWAPI_Framebuffer* framebuffer,
    int x,
    int y,
    int width,
    int height,
    uint32_t color
);

/* Presents the framebuffer in a native window. */
int TWAPI_Present(
    const TWAPI_Window* window,
    const TWAPI_Framebuffer* framebuffer
);

typedef struct TWAPI_Window TWAPI_Window;

#ifdef __cplusplus
}
#endif

#endif
