#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Source.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct Shaders
{
    std::string VertexShader;
    std::string FragmentShader;
};

static Shaders GetShaderSource(const std::string& filepath)
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

static unsigned int CompileShader(const std::string& source, unsigned int type)
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

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(10);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float vertices[] = {    //anti clockwise
                               // 2-position      3-color
                                -0.5f, -0.5f,  1.0f,0.0f,0.0f,   //bottom left
                                 0.5f, -0.5f,  0.0f,1.0f,0.0f,   //bottom right
                                 0.5f,  0.5f,  0.0f,0.0f,1.0f,   //top right
                                -0.5f,  0.5f,  0.0f,1.0f,0.0f    //top left
        };

        unsigned int indices[] = {
                0,1,2,
                0,2,3
        };

        VertexBuffer vertexBuffer(vertices, 5 * sizeof(float) * 4);

        IndexBuffer indexBuffer(indices, 6);

        //for vertex positions
        GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0));
        GLCheckError(glEnableVertexAttribArray(0));

        //for vertex colors
        GLCheckError(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
        GLCheckError(glEnableVertexAttribArray(1));

        Shaders shaders = GetShaderSource("resources\\shaders\\BasicShader.shader");
        unsigned int shader = CreateShader(shaders.VertexShader, shaders.FragmentShader);
        glUseProgram(shader);

        //uniforms
        GLCheckError(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);

        float redChannel = 0.f;
        float increment = 0.1f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            if (redChannel > 1.f)
                increment = -0.1f;
            else if (redChannel < 0.f)
                increment = +0.1f;

            redChannel += increment;

            GLCheckError(glUniform4f(location, redChannel, 0.5f, 0.8f, 1.f));
            GLCheckError(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glDeleteProgram(shader);
    }
    glfwTerminate();
    return 0;
};