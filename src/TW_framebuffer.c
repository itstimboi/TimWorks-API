#include <TWAPI/TW_framebuffer.h>

#include <stdlib.h>
#include <limits.h>

TWAPI_Framebuffer* TWAPI_CreateFramebuffer(int width, int height)
{
    TWAPI_Framebuffer* framebuffer;
    size_t pixelCount;

    if (width <= 0 || height <= 0)
        return NULL;

    if (width > INT_MAX / (int)sizeof(uint32_t))
        return NULL;

    if ((size_t)width > ((size_t)-1) / (size_t)height)
        return NULL;

    pixelCount = (size_t)width * (size_t)height;

    if (pixelCount > ((size_t)-1) / sizeof(uint32_t))
        return NULL;

    framebuffer = (TWAPI_Framebuffer*)calloc(
        1, sizeof(TWAPI_Framebuffer));

    if (!framebuffer)
        return NULL;

    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->pitch = width * (int)sizeof(uint32_t);

    framebuffer->pixels = (uint32_t*)calloc(
        pixelCount, sizeof(uint32_t));

    if (!framebuffer->pixels)
    {
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
    size_t count;
    size_t i;

    if (!framebuffer || !framebuffer->pixels)
        return;

    count = (size_t)framebuffer->width *
            (size_t)framebuffer->height;

    for (i = 0; i < count; ++i)
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
    int left;
    int top;
    int right;
    int bottom;
    int rowIndex;
    int columnIndex;
    double rightEdge;
    double bottomEdge;

    if (!framebuffer || !framebuffer->pixels ||
        width <= 0 || height <= 0)
        return;

    /*
       Use double for endpoint addition to avoid signed-int overflow.
       Every 32-bit integer is represented exactly by double.
    */
    rightEdge = (double)x + (double)width;
    bottomEdge = (double)y + (double)height;

    if (x < 0)
        left = 0;
    else
        left = x;

    if (y < 0)
        top = 0;
    else
        top = y;

    if (rightEdge > (double)framebuffer->width)
        right = framebuffer->width;
    else if (rightEdge < 0.0)
        return;
    else
        right = (int)rightEdge;

    if (bottomEdge > (double)framebuffer->height)
        bottom = framebuffer->height;
    else if (bottomEdge < 0.0)
        return;
    else
        bottom = (int)bottomEdge;

    if (left >= right || top >= bottom)
        return;

    for (rowIndex = top; rowIndex < bottom; ++rowIndex)
    {
        uint32_t* row;

        row = framebuffer->pixels +
              (size_t)rowIndex * (size_t)framebuffer->width;

        for (columnIndex = left; columnIndex < right; ++columnIndex)
            row[columnIndex] = color;
    }
}
