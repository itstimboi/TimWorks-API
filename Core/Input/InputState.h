#pragma once

#include "Input.h"

namespace TAPI
{
    struct InputState
    {
        bool Keys[(int)Key::Count] = {};
        bool PreviousKeys[(int)Key::Count] = {};

        bool MouseButtons[(int)MouseButton::Count] = {};
        bool PreviousMouseButtons[(int)MouseButton::Count] = {};

        Vector2 MousePosition;
        Vector2 PreviousMousePosition;
        Vector2 MouseDelta;

        Vector2 MouseScroll;
    };
}