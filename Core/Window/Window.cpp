#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../Input/InputState.h"
#include "../Input/InputInternal.h"
#include "Window.h"

namespace TAPI
{
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		(void)window;

		glViewport(0, 0, width, height);
	}

	static GLFWwindow* GetGLFWWindow(Window* window)
	{
		return static_cast<GLFWwindow*>(window->Handle);
	}

	bool Init()
	{
		if (!glfwInit())
		{
			std::cout << "Error initializing GLFW" << std::endl << "Closing..." << std::endl;
			return false;
		}

		return true;
	}
	
	void DeInit()
	{
		glfwTerminate();
	}

	Window* InitWindow(int windowWidth, int windowHeight, const std::string& Title)
	{
		Window* window = new Window;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // !__APPLE__


		GLFWwindow* glfwWindow = glfwCreateWindow(windowWidth, windowHeight, Title.c_str(), nullptr, nullptr);

		if (glfwWindow == nullptr)
		{
			std::cout << "Error initializing Window" << std::endl << "Closing..." << std::endl;
			delete window;
			return nullptr;
		}

		window->Handle = glfwWindow;
		window->Width = windowWidth;
		window->Height = windowHeight;
		window->Title = Title;

		glfwMakeContextCurrent(glfwWindow);

		//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		//{
		//	std::cout << "Failed to initialize GLAD" << std::endl;
		//	glfwDestroyWindow(glfwWindow);
		//	delete window;
		//	return nullptr;
		//}

		//glViewport(0, 0, windowWidth, windowHeight);

		//glEnable(GL_DEPTH_TEST);

		CreateInputState(window);

		return window;
	}
	
	void DestroyWindow(Window* window)
	{
		if (!window)
			return;

		if (window->Handle)
		{
			glfwDestroyWindow(static_cast<GLFWwindow*>(window->Handle));
			window->Handle = nullptr;
		}

		DestroyInputState(window);

		delete window;
	}

	void SetWindowSizeCallback(Window* window)
	{
		if (window == nullptr)
			return;

		glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(window->Handle), framebuffer_size_callback);
	}

	void PollEvents()
	{
		glfwPollEvents();
	}

	void SwapBuffers(Window* window)
	{
		if (window == nullptr)
			return;

		glfwSwapBuffers(static_cast<GLFWwindow*>(window->Handle));
	}

	bool WindowShouldClose(Window* window)
	{
		if (window == nullptr)
			return true;

		return glfwWindowShouldClose(static_cast<GLFWwindow*>(window->Handle));
	}

	void SetClearColor(Color color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool InitGraphics(Window* window)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		glViewport(0, 0, window->Width, window->Height);

		glEnable(GL_DEPTH_TEST);

		return true;
	}

	Vector2Int GetWindowSize(Window* window)
	{
		if (window == nullptr)
			return Vector2Int();


		int width;
		int height;


		glfwGetWindowSize(
			static_cast<GLFWwindow*>(window->Handle),
			&width,
			&height
		);


		return Vector2Int(
			width,
			height
		);

		
	}

	void SetWindowSize(Window* window, int newWindowWidth, int newWindowHeight)
	{
		if (window == nullptr || newWindowWidth <= 0 || newWindowHeight <= 0)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwSetWindowSize(
			GetGLFWWindow(window),
			newWindowWidth,
			newWindowHeight
		);

		glfwGetWindowSize(
			GetGLFWWindow(window),
			&window->Width,
			&window->Height
		);
	}
	
	std::string GetWindowTitle(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return "";
		}

		return window->Title;
	}

	void SetWindowTitle(Window* window, const std::string& newTitle)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		window->Title = newTitle;

		glfwSetWindowTitle(
			static_cast<GLFWwindow*>(window->Handle),
			newTitle.c_str()
		);
	}
	
	void RequestClose(Window* window)
	{
		if (!window)
			return;


		glfwSetWindowShouldClose(
			GetGLFWWindow(window),
			GLFW_TRUE
		);
	}

	void CancelClose(Window* window)
	{
		if (!window)
			return;


		glfwSetWindowShouldClose(
			GetGLFWWindow(window),
			GLFW_FALSE
		);
	}

	Vector2Int GetWindowPosition(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return { 0, 0 };
		}

		int x;
		int y;

		glfwGetWindowPos(GetGLFWWindow(window), &x, &y);

		window->xPos = x;
		window->yPos = y;

		return Vector2Int(x, y);
	}

	void SetWindowPosition(Window* window, int newXPos, int newYPos)
	{
		if (window == nullptr || newXPos <= 0 || newYPos <= 0)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		window->xPos = newXPos;
		window->yPos = newYPos;

		glfwSetWindowPos(GetGLFWWindow(window), newXPos, newYPos);
	}

	void SetVSync(Window* window, bool enabled)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwSwapInterval(enabled ? 1 : 0);

		window->vSync = enabled ? 1 : 0;
	}

	void ShowCursor(Window* window)
	{
		glfwSetInputMode(
			GetGLFWWindow(window),
			GLFW_CURSOR,
			GLFW_CURSOR_NORMAL
		);
	}

	void HideCursor(Window* window)
	{
		glfwSetInputMode(
			GetGLFWWindow(window),
			GLFW_CURSOR,
			GLFW_CURSOR_DISABLED
		);
	}	

	void SetFullscreen(Window* window, bool fullscreen)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		window->fullscreen = fullscreen;

		if (fullscreen == true)
		{
			window->RestoreXPos = window->xPos;
			window->RestoreYPos = window->yPos;

			GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);

			glfwSetWindowMonitor(GetGLFWWindow(window), mainMonitor, window->xPos, window->yPos, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);

			glfwSetWindowMonitor(GetGLFWWindow(window), nullptr, window->RestoreXPos, window->RestoreYPos, window->RestoreWidth, window->RestoreHeight, mode->refreshRate);
		}
	}

	bool IsFullscreen(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return false;
		}

		return window->fullscreen;
	}

	void MaximizeWindow(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwMaximizeWindow(GetGLFWWindow(window));
	}

	void MinimizeWindow(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwIconifyWindow(GetGLFWWindow(window));
	}

	void RestoreWindow(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwRestoreWindow(GetGLFWWindow(window));
	}

	void ShowWindow(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwShowWindow(GetGLFWWindow(window));
	}

	void HideWindow(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwHideWindow(GetGLFWWindow(window));
	}

	void FocusWindow(Window* window)
	{
		if (window == nullptr)
		{
			std::cout << "Didn't Get Proper References." << std::endl;
			return;
		}

		glfwFocusWindow(GetGLFWWindow(window));
	}

	void BeginFrame(Window* window)
	{
		if (window == nullptr)
			return;

		UpdateKeyboard(window);
		UpdateMouse(window);

		PollEvents();
	}

	void EndFrame(Window* window)
	{
		if (window == nullptr)
		{
			return;
		}

		SwapBuffers(window);
	}
}