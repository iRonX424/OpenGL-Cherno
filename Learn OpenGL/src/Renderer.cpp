#include <iostream>

#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
    vertexArray.Bind();
    indexBuffer.Bind();
    shader.Bind();

    GLCheckError(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, (void*)0));
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
