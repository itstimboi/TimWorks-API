#pragma once

namespace TAPI
{
	struct Color
	{
		float r = (float)0;
		float g = (float)0;
		float b = (float)0;
		float a = (float)0;
	};

	namespace Colors
	{
		inline constexpr Color White = { 1.0f, 1.0f, 1.0f, 1.0f };
		inline constexpr Color Black = { 0.0f, 0.0f, 0.0f, 1.0f };
		inline constexpr Color Red = { 1.0f, 0.0f, 0.0f, 1.0f };
		inline constexpr Color Green = { 0.0f, 1.0f, 0.0f, 1.0f };
		inline constexpr Color Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	}
}