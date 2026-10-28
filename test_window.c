#include <stdio.h>

#include <TWAPI/TW_window.h>

#include <windows.h>

int main(void)
{
    TWAPI_Window* window = TWAPI_CreateWindow(
        100,
        100,
        800,
        600,
        "TimAPI - First Native Window"
    );

    if (!window)
    {
        fprintf(stderr, "Failed to create TimAPI window.\n");
        return 1;
    }

    puts("TimAPI window created.");
    puts("Close the window to exit.");

    while (!TWAPI_WindowShouldClose(window))
    {
        TWAPI_PollEvents();
        Sleep(1);
    }

    TWAPI_DestroyWindow(window);

    puts("Window closed.");
    return 0;
}
