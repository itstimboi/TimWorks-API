#pragma once

namespace TAPI
{
    enum class Key
    {
        Unknown = -1,

        Space,

        A, B, C, D, E, F, G,
        H, I, J, K, L, M, N,
        O, P, Q, R, S, T,
        U, V, W, X, Y, Z,

        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        Escape,
        Enter,
        Tab,
        Backspace,

        Left,
        Right,
        Up,
        Down,

        LeftShift,
        RightShift,

        LeftControl,
        RightControl,

        LeftAlt,
        RightAlt,

        Count
    };


    enum class MouseButton
    {
        Left,
        Right,
        Middle,

        Button4,
        Button5,
        Button6,
        Button7,
        Button8,

        Count
    };
}