#include "File.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace TAPI
{

    std::string GetFileContents(
        const std::string& filepath
    )
    {
        std::ifstream file(
            filepath,
            std::ios::in
        );


        if (!file.is_open())
        {
            throw std::runtime_error(
                "Failed to open file: " + filepath
            );
        }


        std::stringstream buffer;

        buffer << file.rdbuf();


        return buffer.str();
    }

}