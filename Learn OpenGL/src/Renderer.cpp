#include <iostream>

#include "Renderer.h"

void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum errors = glGetError())
    {
        std::cout << "[Opengl Error]  " << function <<
            "\n\t\t" << file << "\n\t\tLine : " << line << std::endl;
        return false;
    }
    return true;
}