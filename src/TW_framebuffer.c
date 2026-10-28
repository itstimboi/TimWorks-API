
#include <TWAPI/TW_framebuffer.h>

#include <stdlib.h>
#include <limits.h>

TWAPI_Framebuffer* TWAPI_CreateFramebuffer(int width, int height)
{
    if (width <= 0 || height <= 0)
        return NULL;

    if ((size_t)width > SIZE_MAX / sizeof(uint32_t) / (size_t)height)
        return NULL;

    TWAPI_Framebuffer* framebuffer =
        (TWAPI_Framebuffer*)calloc(1, sizeof(TWAPI_Framebuffer));

    if (!framebuffer)
        return NULL;

    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->pitch = width * (int)sizeof(uint32_t);

    framebuffer->pixels = (uint32_t*)calloc(
        (size_t)width * (size_t)height,
        sizeof(uint32_t)
    );

    if (!framebuffer->pixels) {
        free(framebuffer);
        return NULL;
    }

    return framebuffer;
}

void TWAPI_DestroyFramebuffer(TWAPI_Framebuffer* framebuffer)
{
    if (!framebuffer)
        return;

    free(framebuffer->pixels);
    free(framebuffer);
}

void TWAPI_Clear(
    TWAPI_Framebuffer* framebuffer,
    uint32_t color)
{
    if (!framebuffer || !framebuffer->pixels)
        return;

    size_t count =
        (size_t)framebuffer->width * (size_t)framebuffer->height;

    for (size_t i = 0; i < count; ++i)
        framebuffer->pixels[i] = color;
}

void TWAPI_SetPixel(
    TWAPI_Framebuffer* framebuffer,
    int x,
    int y,
    uint32_t color)
{
    if (!framebuffer || !framebuffer->pixels)
        return;

    if (x < 0 || y < 0 ||
        x >= framebuffer->width || y >= framebuffer->height)
        return;

    framebuffer->pixels[
        (size_t)y * (size_t)framebuffer->width + (size_t)x
    ] = color;
}

uint32_t TWAPI_GetPixel(
    const TWAPI_Framebuffer* framebuffer,
    int x,
    int y)
{
    if (!framebuffer || !framebuffer->pixels)
        return 0;

    if (x < 0 || y < 0 ||
        x >= framebuffer->width || y >= framebuffer->height)
        return 0;

    return framebuffer->pixels[
        (size_t)y * (size_t)framebuffer->width + (size_t)x
    ];
}

void TWAPI_FillRect(
    TWAPI_Framebuffer* framebuffer,
    int x,
    int y,
    int width,
    int height,
    uint32_t color)
{
    if (!framebuffer || !framebuffer->pixels ||
        width <= 0 || height <= 0)
        return;

    /* Clip the rectangle to the framebuffer. */
    int64_t left   = x;
    int64_t top    = y;
    int64_t right  = (int64_t)x + width;
    int64_t bottom = (int64_t)y + height;

    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (right > framebuffer->width) right = framebuffer->width;
    if (bottom > framebuffer->height) bottom = framebuffer->height;

    if (left >= right || top >= bottom)
        return;

    for (int64_t py = top; py < bottom; ++py) {
        uint32_t* row = framebuffer->pixels +
            (size_t)py * (size_t)framebuffer->width;

        for (int64_t px = left; px < right; ++px)
            row[px] = color;
    }
}