#include "Shader.h"

#include <glad/glad.h>
#include <iostream>

namespace TAPI
{

    Shader::Shader()
    {
        m_ID = 0;
    }

    Shader::~Shader()
    {
        if (m_ID != 0)
            glDeleteProgram(m_ID);
    }

    bool Shader::Compile(
        const std::string& vertexSource,
        const std::string& fragmentSource)
    {

        // Create vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const char* vertexCode = vertexSource.c_str();

        glShaderSource(
            vertexShader,
            1,
            &vertexCode,
            nullptr
        );

        glCompileShader(vertexShader);

        // Check vertex shader
        GLint success;

        glGetShaderiv(
            vertexShader,
            GL_COMPILE_STATUS,
            &success
        );

        if (!success)
        {
            char infoLog[512];

            glGetShaderInfoLog(
                vertexShader,
                512,
                nullptr,
                infoLog
            );

            std::cout
                << "Vertex Shader Error:\n"
                << infoLog
                << std::endl;

            return false;
        }

        // ============================
        // Fragment Shader
        // ============================

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* fragmentCode = fragmentSource.c_str();

        glShaderSource(
            fragmentShader,
            1,
            &fragmentCode,
            nullptr
        );

        glCompileShader(fragmentShader);

        // Check fragment shader

        glGetShaderiv(
            fragmentShader,
            GL_COMPILE_STATUS,
            &success
        );

        if (!success)
        {
            char infoLog[512];

            glGetShaderInfoLog(
                fragmentShader,
                512,
                nullptr,
                infoLog
            );


            std::cout
                << "Fragment Shader Error:\n"
                << infoLog
                << std::endl;


            return false;
        }

        m_ID = glCreateProgram();


        glAttachShader(
            m_ID,
            vertexShader
        );


        glAttachShader(
            m_ID,
            fragmentShader
        );


        glLinkProgram(m_ID);

        glGetProgramiv(
            m_ID,
            GL_LINK_STATUS,
            &success
        );

        if (!success)
        {
            char infoLog[512];

            glGetProgramInfoLog(
                m_ID,
                512,
                nullptr,
                infoLog
            );


            std::cout
                << "Shader Linking Error:\n"
                << infoLog
                << std::endl;


            return false;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return true;
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ID);
    }


    void Shader::Unbind() const
    {
        glUseProgram(0);
    }


    unsigned int Shader::GetID() const
    {
        return m_ID;
    }
}