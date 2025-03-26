#include "Shader.h"
#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath):
    m_FilePath(filepath),
    m_RendererID(0)
{
    Shaders shaders = GetShaderSource(filepath);
    m_RendererID = CreateShader(shaders.VertexShader, shaders.FragmentShader);
}

Shader::~Shader()
{
    GLCheckError(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCheckError(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCheckError(glUseProgram(0));
}

unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
    unsigned int shader = glCreateShader(type);
    const char* src = &source[0];
    GLCheckError(glShaderSource(shader, 1, &src, nullptr));
    glCompileShader(shader);

    //error handling
    int result;
    GLCheckError(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        int msgLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgLength);
        char* errorMsg = (char*)malloc(msgLength * sizeof(char));
        glGetShaderInfoLog(shader, msgLength, &msgLength, errorMsg);
        std::cout << "Error creating " <<
            (type == GL_VERTEX_SHADER ? "Vertex Shader." : "Fragment Shader.") << std::endl;
        std::cout << errorMsg << std::endl;

        free(errorMsg);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

Shaders Shader::GetShaderSource(const std::string& filepath)
{
    std::ifstream inputStream(filepath);


    enum class Shadertype
    {
        None = -1, Vertex = 0, Fragment = 1
    };

    Shadertype type = Shadertype::Vertex;
    std::stringstream ss[2];
    std::string line;
    while (getline(inputStream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = Shadertype::Vertex;
            else if (line.find("fragment") != std::string::npos)
                type = Shadertype::Fragment;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCheckError(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCacheMap.find(name) != m_UniformLocationCacheMap.end())
        return m_UniformLocationCacheMap[name];

    GLCheckError(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: Uniform '" << name << "' not found!" << std::endl;

    m_UniformLocationCacheMap[name] = location;
    return location;
}
