#pragma once

#include "GL/glew.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCheckError(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int line);

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
    void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
    void Clear() const;
};