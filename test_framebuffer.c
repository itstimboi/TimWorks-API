#include <stdio.h>
#include <windows.h>

#include <TWAPI/TWAPI.h>

int main(void)
{

    // Checks if TimWorks API couldn't initialize.
    if (!TWAPI_Init())
    {
        fprintf(stderr, "Failed to initialize the TimWorks API.\n");
        return -1;
    }

    // Creates the window.
    TWAPI_Window* window = TWAPI_CreateWindow(
        100, 100, 960, 600,
        "TimWorks API - Software Framebuffer"
    );

    // This checks if the window creation failed.
    if (!window) {
        printf("Failed to create window.\n");
        return 1;
    }

    // Creates the framebuffer
    TWAPI_Framebuffer* framebuffer =
        TWAPI_CreateFramebuffer(960, 600);

    // This checks if the framebuffer creation failed.
    if (!framebuffer) {
        printf("Failed to create framebuffer.\n");
        TWAPI_DestroyWindow(window);
        return 1;
    }

    // Main game loop.
    while (!TWAPI_WindowShouldClose(window)) 
    {
        // Handle Events.
        TWAPI_PollEvents();

        // This draws a color of the framebuffer in the     RRGGBB format.
        //                                              0x00101828
        TWAPI_Clear(framebuffer, 0x00101828);

        // Presents the framebuffer
        TWAPI_Present(window, framebuffer);

        // Since the window doesn't use OpenGL, we have to handle framing ourselves.
        Sleep(16);
        // But if it does, use "TWAPI_SwapOpenGLBuffers(window);".
    }

    // This destroys the framebuffer and window objects we made.
    TWAPI_DestroyFramebuffer(framebuffer);
    TWAPI_DestroyWindow(window);

    return 0;
}