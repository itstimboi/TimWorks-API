#pragma once

#include "Input.h"

namespace TAPI
{
    struct InputState
    {
        bool Keys[350]{};
        bool PreviousKeys[350]{};


        bool MouseButtons[8]{};
        bool PreviousMouseButtons[8]{};


        Vector2 MousePosition;
        Vector2 PreviousMousePosition;


        Vector2 MouseDelta;
    };
}