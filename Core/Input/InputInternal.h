#pragma once

namespace TAPI
{
    struct Window;

    void UpdateKeyboard(Window* window);
    void UpdateMouse(Window* window);

    void CreateInputState(Window* window);
    void DestroyInputState(Window* window);
}