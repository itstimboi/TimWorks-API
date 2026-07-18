#pragma once

namespace TAPI
{
    struct Window;

    void UpdateInput(Window* window);

    void CreateInputState(Window* window);
    void DestroyInputState(Window* window);
}