#pragma once

#include "../Math/Vectors.h"
#include "InputCodes.h"

namespace TAPI
{
    struct Window;

    // Keyboard

    bool IsKeyDown(Window*, Key);
    bool IsKeyPressed(Window*, Key);
    bool IsKeyReleased(Window*, Key);

    // Mouse buttons

    bool IsMouseButtonDown(Window*, MouseButton);
    bool IsMouseButtonPressed(Window*, MouseButton);
    bool IsMouseButtonReleased(Window*, MouseButton);

    // Mouse

    Vector2 GetMousePosition(Window*);
    Vector2 GetMouseDelta(Window*);
    Vector2 GetMouseScroll(Window*);

    // Cursor

    void ShowCursor(Window*);
    void HideCursor(Window*);
    void LockCursor(Window*);
    void UnlockCursor(Window*);

    // Cursor position

    void SetMousePosition(Window*, Vector2);

    // Mouse wheel

    Vector2 GetMouseScroll(Window* window);

    // Text input

    //char GetCharPressed();
}