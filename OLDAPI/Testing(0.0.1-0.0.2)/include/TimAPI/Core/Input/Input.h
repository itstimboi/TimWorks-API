#pragma once

#include "../Window/Window.h"
#include "../Math/Vectors.h"
#include "InputCodes.h"

namespace TAPI
{
    //struct Window;

    // Keyboard

    TAPI_API bool IsKeyDown(Window*, Key);
    TAPI_API bool IsKeyPressed(Window*, Key);
    TAPI_API bool IsKeyReleased(Window*, Key);

    // Mouse buttons

    TAPI_API bool IsMouseButtonDown(Window*, MouseButton);
    TAPI_API bool IsMouseButtonPressed(Window*, MouseButton);
    TAPI_API bool IsMouseButtonReleased(Window*, MouseButton);

    // Mouse

    TAPI_API Vector2 GetMousePosition(Window*);
    TAPI_API Vector2 GetMouseDelta(Window*);
    TAPI_API Vector2 GetMouseScroll(Window*);

    // Cursor

    TAPI_API void ShowCursor(Window*);
    TAPI_API void HideCursor(Window*);
    TAPI_API void LockCursor(Window*);
    TAPI_API void UnlockCursor(Window*);

    // Cursor position

    TAPI_API void SetMousePosition(Window*, Vector2);

    // Mouse wheel

    TAPI_API Vector2 GetMouseScroll(Window* window);

    // Text input

    //char GetCharPressed();
}