#include <TimAPI/TimAPI.h>

#include <iostream>
#include <string>

int main()
{
    TAPI::Init();

    auto window =
        TAPI::InitWindow(
            800,
            600,
            "DrawLine Test"
        );

    TAPI::InitGraphics(window);

    while (!TAPI::WindowShouldClose(window))
    {
        TAPI::PollEvents();

        TAPI::ClearBuffers();

        TAPI::SetClearColor(TAPI::Colors::Black);

        TAPI::DrawLine(TAPI::Vector2(0.0f, 0.0f), TAPI::Vector2(1.0f, 1.0f), TAPI::Colors::Red);

        TAPI::SwapBuffers(window);
    }

    TAPI::DestroyWindow(window);
    TAPI::DeInit();
}

//int main()
//{
//    // Initialize TimAPI
//    if (!TAPI::Init())
//    {
//        std::cout << "Failed to initialize TimAPI.\n";
//        return -1;
//    }
//
//    // Create the window
//    TAPI::Window* window = TAPI::InitWindow(
//        1280,
//        720,
//        "TimAPI 0.0.3 Delta Time Test"
//    );
//
//    if (window == nullptr)
//    {
//        std::cout << "Failed to create window.\n";
//        TAPI::DeInit();
//        return -1;
//    }
//
//    // Initialize graphics
//    if (!TAPI::InitGraphics(window))
//    {
//        std::cout << "Failed to initialize graphics.\n";
//        TAPI::DestroyWindow(window);
//        TAPI::DeInit();
//        return -1;
//    }
//
//    // Optional
//    TAPI::SetVSync(window, true);
//
//    TAPI::Color backgroundColor = TAPI::Colors::Black;
//
//    float x = 0.0f;
//    float y = 0.0f;
//    float speed = 500.0f; // Units per second
//
//    while (!TAPI::WindowShouldClose(window))
//    {
//        // Updates events, input, delta time, FPS, etc.
//        TAPI::PollEvents();
//
//        // Clear the screen
//        TAPI::SetClearColor(backgroundColor);
//        TAPI::ClearBuffers();
//
//        //---------------------------------
//        // Movement Test
//        //---------------------------------
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::D))
//            x += speed * TAPI::GetDeltaTime();
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::A))
//            x -= speed * TAPI::GetDeltaTime();
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::W))
//            y -= speed * TAPI::GetDeltaTime();
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::S))
//            y += speed * TAPI::GetDeltaTime();
//
//        //---------------------------------
//        // Change background colors
//        //---------------------------------
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::B))
//            backgroundColor = TAPI::Colors::Blue;
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::G))
//            backgroundColor = TAPI::Colors::Green;
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::R))
//            backgroundColor = TAPI::Colors::Red;
//
//        if (TAPI::IsKeyDown(window, TAPI::Key::Space))
//            backgroundColor = TAPI::Colors::White;
//
//        //---------------------------------
//        // Update the title every frame
//        //---------------------------------
//
//        std::string title =
//            "TimAPI 0.0.3 | FPS: " +
//            std::to_string(TAPI::GetFPS()) +
//            " | DT: " +
//            std::to_string(TAPI::GetDeltaTime()) +
//            " | X: " +
//            std::to_string(x) +
//            " | Y: " +
//            std::to_string(y);
//
//        TAPI::SetWindowTitle(window, title);
//
//        // Present the frame
//        TAPI::SwapBuffers(window);
//    }
//
//    TAPI::DestroyWindow(window);
//    TAPI::DeInit();
//
//    return 0;
//}