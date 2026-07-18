#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>

#include "InputState.h"
#include "Input.h"
#include "../Window/Window.h"
//#include "InputInternal.h"

namespace TAPI
{
    static int ToGLFWKey(Key key)
    {
        switch (key)
        {
        case Key::Space: return GLFW_KEY_SPACE;

        case Key::A: return GLFW_KEY_A;
        case Key::B: return GLFW_KEY_B;
        case Key::C: return GLFW_KEY_C;
        case Key::D: return GLFW_KEY_D;
        case Key::E: return GLFW_KEY_E;
        case Key::F: return GLFW_KEY_F;
        case Key::G: return GLFW_KEY_G;

        case Key::H: return GLFW_KEY_H;
        case Key::I: return GLFW_KEY_I;
        case Key::J: return GLFW_KEY_J;
        case Key::K: return GLFW_KEY_K;
        case Key::L: return GLFW_KEY_L;
        case Key::M: return GLFW_KEY_M;
        case Key::N: return GLFW_KEY_N;

        case Key::O: return GLFW_KEY_O;
        case Key::P: return GLFW_KEY_P;
        case Key::Q: return GLFW_KEY_Q;
        case Key::R: return GLFW_KEY_R;
        case Key::S: return GLFW_KEY_S;
        case Key::T: return GLFW_KEY_T;

        case Key::U: return GLFW_KEY_U;
        case Key::V: return GLFW_KEY_V;
        case Key::W: return GLFW_KEY_W;
        case Key::X: return GLFW_KEY_X;
        case Key::Y: return GLFW_KEY_Y;
        case Key::Z: return GLFW_KEY_Z;


        case Key::Escape: return GLFW_KEY_ESCAPE;
        case Key::Enter: return GLFW_KEY_ENTER;
        case Key::Tab: return GLFW_KEY_TAB;
        case Key::Backspace: return GLFW_KEY_BACKSPACE;


        case Key::Left: return GLFW_KEY_LEFT;
        case Key::Right: return GLFW_KEY_RIGHT;
        case Key::Up: return GLFW_KEY_UP;
        case Key::Down: return GLFW_KEY_DOWN;


        case Key::LeftShift: return GLFW_KEY_LEFT_SHIFT;
        case Key::RightShift: return GLFW_KEY_RIGHT_SHIFT;

        case Key::LeftControl: return GLFW_KEY_LEFT_CONTROL;
        case Key::RightControl: return GLFW_KEY_RIGHT_CONTROL;

        case Key::LeftAlt: return GLFW_KEY_LEFT_ALT;
        case Key::RightAlt: return GLFW_KEY_RIGHT_ALT;
        }

        return GLFW_KEY_UNKNOWN;
    }

    static int ToGLFWMouseButton(MouseButton button)
    {
        switch (button)
        {
        case MouseButton::Left:
            return GLFW_MOUSE_BUTTON_LEFT;

        case MouseButton::Right:
            return GLFW_MOUSE_BUTTON_RIGHT;

        case MouseButton::Middle:
            return GLFW_MOUSE_BUTTON_MIDDLE;

        case MouseButton::Button4:
            return GLFW_MOUSE_BUTTON_4;

        case MouseButton::Button5:
            return GLFW_MOUSE_BUTTON_5;

        case MouseButton::Button6:
            return GLFW_MOUSE_BUTTON_6;

        case MouseButton::Button7:
            return GLFW_MOUSE_BUTTON_7;

        case MouseButton::Button8:
            return GLFW_MOUSE_BUTTON_8;
        }


        return GLFW_MOUSE_BUTTON_LEFT;
    }

    static std::unordered_map<Window*, InputState> g_InputStates;

    void CreateInputState(Window* window)
    {
        if (!window)
        {
            return;
        }

        g_InputStates.emplace(
            window,
            InputState{}
        );
    }

    void DestroyInputState(Window* window)
    {
        if (!window)
        {
            return;
        }

        g_InputStates.erase(window);
    }

    void UpdateInput(Window* window)
    {
        if (window == nullptr)
            return;


        InputState& input =
            g_InputStates[window];


        GLFWwindow* glfwWindow =
            static_cast<GLFWwindow*>(window->Handle);



        // Copy old keyboard state

        for (int i = 0; i < 350; i++)
        {
            input.PreviousKeys[i] =
                input.Keys[i];


            input.Keys[i] =
                glfwGetKey(
                    glfwWindow,
                    i
                )
                == GLFW_PRESS;
        }



        // Mouse buttons

        for (int i = 0; i < 8; i++)
        {
            input.PreviousMouseButtons[i] =
                input.MouseButtons[i];


            input.MouseButtons[i] =
                glfwGetMouseButton(
                    glfwWindow,
                    i
                )
                == GLFW_PRESS;
        }



        // Mouse position

        double x;
        double y;


        glfwGetCursorPos(
            glfwWindow,
            &x,
            &y
        );


        input.PreviousMousePosition =
            input.MousePosition;


        input.MousePosition =
            Vector2(
                (float)x,
                (float)y
            );


        input.MouseDelta =
            Vector2(
                input.MousePosition.x -
                input.PreviousMousePosition.x,

                input.MousePosition.y -
                input.PreviousMousePosition.y
            );
    }

    bool IsKeyDown(Window* window, Key key)
    {
        if (window == nullptr)
            return false;


        return glfwGetKey(
            static_cast<GLFWwindow*>(window->Handle),
            ToGLFWKey(key)
        )
            == GLFW_PRESS;
    }



    bool IsMouseButtonDown(Window* window, MouseButton button)
    {
        if (window == nullptr)
            return false;


        return glfwGetMouseButton(
            static_cast<GLFWwindow*>(window->Handle),
            ToGLFWMouseButton(button)
        )
            == GLFW_PRESS;
    }

    bool IsKeyPressed(Window* window, Key key)
    {
        InputState& input =
            g_InputStates[window];


        int glfwKey =
            ToGLFWKey(key);


        return
            input.Keys[glfwKey] &&
            !input.PreviousKeys[glfwKey];
    }

    bool IsKeyReleased(Window* window, Key key)
    {
        InputState& input =
            g_InputStates[window];


        int glfwKey =
            ToGLFWKey(key);


        return
            !input.Keys[glfwKey] &&
            input.PreviousKeys[glfwKey];
    }

    bool IsMouseButtonReleased(Window* window, MouseButton mouseButton)
    {
        InputState& input =
            g_InputStates[window];


        int glfwMouseButton =
            ToGLFWMouseButton(mouseButton);


        return
            !input.MouseButtons[glfwMouseButton] &&
            input.PreviousMouseButtons[glfwMouseButton];
    }

    Vector2 GetMouseDelta(Window* window)
    {
        if (window == nullptr)
            return Vector2();


        return g_InputStates[window].MouseDelta;
    }

    Vector2 GetMousePosition(Window* window)
    {
        if (window == nullptr)
            return Vector2();


        double x;
        double y;


        glfwGetCursorPos(
            static_cast<GLFWwindow*>(window->Handle),
            &x,
            &y
        );


        return Vector2(
            (float)x,
            (float)y
        );
    }



    //Vector2 GetMouseDelta(Window* window)
    //{
    //    InputState& input = g_InputStates[window];

    //    static double lastX = 0;
    //    static double lastY = 0;


    //    if (window == nullptr)
    //        return Vector2();


    //    double x;
    //    double y;


    //    glfwGetCursorPos(
    //        static_cast<GLFWwindow*>(window->Handle),
    //        &x,
    //        &y
    //    );


    //    Vector2 delta(
    //        (float)(x - lastX),
    //        (float)(y - lastY)
    //    );


    //    lastX = x;
    //    lastY = y;

    //    input.PreviousMousePosition = Vector2((float)lastX, (float)lastY);

    //    return delta;
    //}

    Vector2 GetMouseScroll(Window* window)
    {
        if (window == nullptr)
        {
            std::cout << "Didn't Get Proper References." << std::endl;
            return Vector2();
        }

        return Vector2();
    }

}