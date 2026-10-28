#pragma once

#include <xstring>

#include "../../TimAPI.h"
#include "../Shader/Shader.h"

namespace TAPI
{
    class Renderer;

    TAPI_API Renderer* GetRenderer();

    void CreateRenderer();
    void DestroyRenderer();

    class Renderer
    {

    public:

        TAPI_API bool InitRenderer();

        TAPI_API void ShutdownRenderer();

        TAPI_API bool Begin();

        TAPI_API void End();

        TAPI_API void DrawLine(
            Vector2 start,
            Vector2 end,
            Color color
        );

    private:

        unsigned int VAO = 0;
        unsigned int VBO = 0;

        int colorLocation = -1;

        TAPI::Shader shader;
    };
}