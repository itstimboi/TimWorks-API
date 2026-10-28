#ifndef TW_WINDOW_INTERNAL_H
#define TW_WINDOW_INTERNAL_H

#include "TWAPI/TW_window.h"

#define TWAPI_WINDOW_TITLE_MAX 255
#define TWAPI_INPUT_KEY_COUNT 512

struct TWAPI_Window {
    TWAPI_WindowBackend backend;

    int x;
    int y;
    int width;
    int height;

    char title[TWAPI_WINDOW_TITLE_MAX + 1];
    int shouldClose;

    int resizable;
    int fullscreen;
    int vSync;

    /* Saved windowed-mode geometry and styles. */
    int windowedX;
    int windowedY;
    int windowedOuterWidth;
    int windowedOuterHeight;

    long windowedStyle;
    long windowedExtendedStyle;

    void* nativeHandle;
    void* nativeContext;

    /* Persistent Win32 software-presentation buffer. */
    void* presentDC;
    void* presentBitmap;
    void* presentOldBitmap;
    int presentWidth;
    int presentHeight;

    unsigned char keysDown[TWAPI_INPUT_KEY_COUNT];
    unsigned char keysPressed[TWAPI_INPUT_KEY_COUNT];
    unsigned char keysReleased[TWAPI_INPUT_KEY_COUNT];

    int mouseX;
    int mouseY;

    unsigned char mouseDown[TWAPI_MOUSE_BUTTON_COUNT];
    unsigned char mousePressed[TWAPI_MOUSE_BUTTON_COUNT];
    unsigned char mouseReleased[TWAPI_MOUSE_BUTTON_COUNT];
};

#endif