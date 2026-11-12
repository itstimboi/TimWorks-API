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

    // Creates a window.
    TWAPI_Window* window = TWAPI_CreateWindow(
        100, 100, 800, 600, "Input Showcase"
    );

    // Verifies if it's created.
    if (!window) {
        printf("Failed to create window.\n");
        return 1;
    }

    // Main game loop.
    while (!TWAPI_WindowShouldClose(window)) 
    {
        // Checks for events.
        TWAPI_PollEvents();

        // Checks if the W key is held down.
        if (TWAPI_IsKeyDown(window, TWAPI_KEY_W))
            printf("W is held\r");

        // Checks if the Space bar is pressed.
        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_SPACE))
            printf("Space pressed!\n");

        // Checks if the Space Bar is released.
        if (TWAPI_IsKeyReleased(window, TWAPI_KEY_SPACE))
            printf("Space released!\n");

        // Checks if the left mouse button is pressed.
        if (TWAPI_IsMousePressed(window, TWAPI_MOUSE_LEFT))
            printf("Left mouse button pressed!\n");

        // Checks if the left mouse button is held down.
        if (TWAPI_IsMouseDown(window, TWAPI_MOUSE_LEFT))
            printf("Mouse: %d, %d       \r",
                TWAPI_GetMouseX(window),
                TWAPI_GetMouseY(window));

        // Checks if the escape key is pressed.
        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_ESCAPE))
            break;

        // Calculates each frame.
        Sleep(16);
    }

    // Destroys the window we made.
    TWAPI_DestroyWindow(window);

    return 0;
}