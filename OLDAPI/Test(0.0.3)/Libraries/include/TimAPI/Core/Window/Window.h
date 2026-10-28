#pragma once

#include <string>

#include "../Math/Vectors.h"
#include "../Graphics/Color.h"

#ifdef TIMAPI_EXPORTS
#define TAPI_API __declspec(dllexport)
#else
#define TAPI_API __declspec(dllimport)
#endif

struct GLFWwindow;

namespace TAPI
{
    struct Window
    {
        GLFWwindow* Handle = nullptr;

        int Width = 0;
        int Height = 0;

        std::string Title;

        bool fullscreen = false;

        int xPos = 0;
        int yPos = 0;

        int RestoreXPos = 0;
        int RestoreYPos = 0;

        int RestoreWidth = 800;
        int RestoreHeight = 600;

        bool vSync = true;
    };

    TAPI_API bool Init();
    TAPI_API void DeInit();
    TAPI_API std::string GetVersion();
    TAPI_API Window* InitWindow(
        int windowWidth,
        int windowHeight,
        const std::string& Title
    );

    TAPI_API void DestroyWindow(Window* window);


    // Window updates

    TAPI_API void PollEvents();

    TAPI_API void SwapBuffers(Window* window);

    TAPI_API bool WindowShouldClose(Window* window);

    TAPI_API void RequestClose(Window* window);

    TAPI_API void CancelClose(Window* window);


    // Window title

    TAPI_API void SetWindowTitle(
        Window* window,
        const std::string& title
    );

    TAPI_API std::string GetWindowTitle(
        Window* window
    );


    // Window size

    TAPI_API void SetWindowSize(
        Window* window,
        int width,
        int height
    );

    TAPI_API Vector2Int GetWindowSize(
        Window* window
    );

    TAPI_API void SetWindowSizeCallback(Window* window);

    // Window position

    TAPI_API void SetWindowPosition(
        Window* window,
        int x,
        int y
    );

    TAPI_API Vector2Int GetWindowPosition(
        Window* window
    );


    // Window state

    TAPI_API void MaximizeWindow(Window* window);

    TAPI_API void MinimizeWindow(Window* window);

    TAPI_API void RestoreWindow(Window* window);

    TAPI_API void ShowWindow(Window* window);

    TAPI_API void HideWindow(Window* window);

    TAPI_API void FocusWindow(Window* window);

    // Graphics

    TAPI_API bool InitGraphics(Window* window);
    TAPI_API void SetClearColor(Color color);
    TAPI_API void ClearBuffers();

    // Fullscreen

    TAPI_API void SetFullscreen(
        Window* window,
        bool fullscreen
    );

    TAPI_API bool IsFullscreen(
        Window* window
    );

    // V-Sync

    TAPI_API void SetVSync(Window* window, bool enabled);

    // Cursor

    TAPI_API void ShowCursor(Window* window);
    TAPI_API void HideCursor(Window* window);

    // Frame Checks

    // TAPI_API void BeginFrame(Window* window);
}