#include <TimAPI/TimAPI.h>
#include <iostream>

int main()
{
	// This initiates TimAPI
	TAPI::Init();

	// This creates a window that is 800 pixels wide by 600 pixels high with a title of "Title"
	TAPI::Window* mainWindow = TAPI::InitWindow(800, 600, "Title");

	// This initializes OpenGL and allows functions like TAPI::PollEvents()
	TAPI::InitGraphics(mainWindow);

	// This automatically updates the OpenGL viewport whenever the window is resized
	//TAPI::SetWindowSizeCallback(mainWindow);

	// This sets the background color (RGBA)
	TAPI::SetClearColor(TAPI::Color(0.0f, 0.0f, 0.0f, 1.0f));

	TAPI::Color chosenRenderColor = TAPI::Colors::Black;
	
	std::cout << "This is " << TAPI::GetVersion() << std::endl;

	// This is the main loop
	while (!TAPI::WindowShouldClose(mainWindow))
	{
		// This checks for input and clears the color and depth buffers
		TAPI::PollEvents();
		
		// This clears the color and depth buffers
		TAPI::Clear();

		// This changes the background color using one of TimAPI's built-in colors
		TAPI::SetClearColor(chosenRenderColor);

		if(TAPI::IsKeyDown(mainWindow, TAPI::Key::W))
		{
			TAPI::SetClearColor(TAPI::Colors::White);
			chosenRenderColor = TAPI::Colors::White;
		}
		if (TAPI::IsKeyDown(mainWindow, TAPI::Key::B))
		{
			TAPI::SetClearColor(TAPI::Colors::Blue);
			chosenRenderColor = TAPI::Colors::Blue;
		}
		if (TAPI::IsKeyDown(mainWindow, TAPI::Key::LeftControl))
		{
			TAPI::SetClearColor(TAPI::Colors::Black);
			chosenRenderColor = TAPI::Colors::Black;
		}

		// This presents the finished frame to the screen (this ends the frame)
		TAPI::SwapBuffers(mainWindow);
	}

	// This closes the window
	TAPI::DestroyWindow(mainWindow);

	// This shuts down TimAPI
	TAPI::DeInit();

	return 0;
}