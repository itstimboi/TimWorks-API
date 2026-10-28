#include <glad/glad.h>
#include <iostream>

#include "PrimitiveRenderer.h"
#include "../../TimAPI.h"

#include <filesystem>

namespace TAPI
{
    static std::string PrimitiveVertexShader()
    {
        std::cout << std::filesystem::current_path() << std::endl;

        std::string shader = GetFileContents("Graphics/Shaders/Primitive.vert");

        return shader;
    }
        


    static std::string PrimitiveFragmentShader()
    {
        std::cout << std::filesystem::current_path() << std::endl;

        std::string shader = GetFileContents("Graphics/Shaders/Primitive.frag");

        return shader;
    }

    static Renderer* g_Renderer = nullptr;

    Renderer* GetRenderer()
    {
        return g_Renderer;
    }

    bool Renderer::InitRenderer()
    {

        if (!shader.Compile(
            PrimitiveVertexShader(),
            PrimitiveFragmentShader()))
        {
            std::cout
                << "Failed to create primitive shader."
                << std::endl;

            return false;
        }

        glGenVertexArrays(
            1,
            &VAO
        );


        glGenBuffers(
            1,
            &VBO
        );


        glBindVertexArray(VAO);


        glBindBuffer(
            GL_ARRAY_BUFFER,
            VBO
        );


        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float) * 2,
            nullptr,
            GL_DYNAMIC_DRAW
        );


        glVertexAttribPointer(
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(float) * 2,
            (void*)0
        );


        glEnableVertexAttribArray(0);


        glBindBuffer(
            GL_ARRAY_BUFFER,
            0
        );


        glBindVertexArray(0);

        shader.Bind();

        colorLocation = glGetUniformLocation(
            shader.GetID(),
            "uColor"
        );

        shader.Unbind();

        return true;
    }

    void TAPI::DrawLine(
        Vector2 start,
        Vector2 end,
        Color color)
    {
        GetRenderer()->DrawLine(
            start,
            end,
            color
        );
    }

    void Renderer::ShutdownRenderer()
    {
        glDeleteBuffers(
            1,
            &VBO
        );


        glDeleteVertexArrays(
            1,
            &VAO
        );
    }

    bool Renderer::Begin()
    {
        shader.Bind();

        return true;
    }

    void Renderer::End()
    {
        shader.Unbind();

        glBindVertexArray(0);
    }

    void Renderer::DrawLine(
        Vector2 start,
        Vector2 end,
        Color color
    )
    {
        GLfloat vertices[] =
        {
            start.x,
            start.y,

            end.x,
            end.y
        };


        shader.Bind();


        glBindVertexArray(VAO);


        glBindBuffer(
            GL_ARRAY_BUFFER,
            VBO
        );


        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            sizeof(vertices),
            vertices
        );


        glUniform4f(
            colorLocation,
            color.r,
            color.g,
            color.b,
            color.a
        );


        glDrawArrays(
            GL_LINES,
            0,
            2
        );


        glBindBuffer(
            GL_ARRAY_BUFFER,
            0
        );


        glBindVertexArray(0);


        shader.Unbind();
    }

    void CreateRenderer()
    {
        g_Renderer = new Renderer();
    }


    void DestroyRenderer()
    {
        delete g_Renderer;
        g_Renderer = nullptr;
    }
}