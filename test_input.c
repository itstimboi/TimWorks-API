#include <stdio.h>
#include <windows.h>

#include <TWAPI/TW_window.h>

int main(void)
{
    TWAPI_Window* window = TWAPI_CreateWindow(
        100, 100, 800, 600, "TimAPI - Input Test"
    );

    if (!window) {
        printf("Failed to create window.\n");
        return 1;
    }

    printf("WASD to test keyboard input.\n");
    printf("Click inside the window to test the mouse.\n");
    printf("Press ESC to quit.\n");

    while (!TWAPI_WindowShouldClose(window)) {
        TWAPI_PollEvents();

        if (TWAPI_IsKeyDown(window, TWAPI_KEY_W))
            printf("W is held\r");

        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_SPACE))
            printf("Space pressed!\n");

        if (TWAPI_IsKeyReleased(window, TWAPI_KEY_SPACE))
            printf("Space released!\n");

        if (TWAPI_IsMousePressed(window, TWAPI_MOUSE_LEFT))
            printf("Left mouse button pressed!\n");

        if (TWAPI_IsMouseDown(window, TWAPI_MOUSE_LEFT))
            printf("Mouse: %d, %d       \r",
                TWAPI_GetMouseX(window),
                TWAPI_GetMouseY(window));

        if (TWAPI_IsKeyPressed(window, TWAPI_KEY_ESCAPE))
            break;

        Sleep(1);
    }

    TWAPI_DestroyWindow(window);
    printf("\nTimAPI input test ended.\n");

    return 0;
}