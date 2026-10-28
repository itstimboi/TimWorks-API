#pragma once

#include <string>

namespace TAPI
{
    class Shader
    {

    public:

        Shader();
        ~Shader();


        bool Compile(
            const std::string& vertexSource,
            const std::string& fragmentSource
        );


        void Bind() const;
        void Unbind() const;


        unsigned int GetID() const;


    private:

        unsigned int m_ID = 0;

    };
}