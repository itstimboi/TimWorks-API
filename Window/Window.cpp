#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Window.h"

namespace TAPI
{

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	struct Window
	{
		GLFWwindow* Handle = nullptr;

		int Width = 0;
		int Height = 0;
		std::string Title;

		bool ShouldClose = false;
	};

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
		window->ShouldClose = false;

		glfwMakeContextCurrent(glfwWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwDestroyWindow(glfwWindow);
			delete window;
			return nullptr;
		}

		glViewport(0, 0, windowWidth, windowHeight);

		return window;
	}
	
	void DestroyWindow(Window* window)
	{
		if (window == nullptr)
			return;

		GLFWwindow* glfwWindow = window->Handle;

		glfwDestroyWindow(glfwWindow);

		delete window;
	}

	void SetWindowSizeCallback(Window* window)
	{
		glfwSetFramebufferSizeCallback(window->Handle, framebuffer_size_callback);
	}
}