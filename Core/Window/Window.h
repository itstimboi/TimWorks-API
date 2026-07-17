#pragma once

#include <string>
#include "../externalVars.h"

#ifdef TIMAPI_EXPORTS
#define TAPI_API __declspec(dllexport)
#else
#define TAPI_API __declspec(dllimport)
#endif

namespace TAPI
{
	struct Window;
	TAPI_API bool Init();
	TAPI_API void DeInit();
	TAPI_API Window* InitWindow(int windowWidth, int windowHeight, const std::string& Title);
	TAPI_API void DestroyWindow(Window* window);
	TAPI_API void SetWindowSizeCallback(Window* window);
	TAPI_API void PollEvents();
	TAPI_API void SwapBuffers(Window* window);
	TAPI_API bool WindowShouldClose(Window* window);
	TAPI_API void SetClearColor(Color color);
	TAPI_API void ClearWindow();
}