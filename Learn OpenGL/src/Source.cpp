#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Source.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCheckError(x) GLClearErrors();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__))

#if 0
int count = 0;

void HandleKeyboardInput(GLFWwindow * window)
{
    int key = glfwGetKey(window, GLFW_KEY_W);
    if (key == GLFW_PRESS)
    {
        std::cout << key << "Ara Ara" << count << std::endl;
        ++count;
    }
}
#endif
static void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file,int line)
{
    while (GLenum errors = glGetError())
    {
        std::cout << "[Opengl Error]  " << function <<
            "\n\t\t"<<file<< "\n\t\tLine : " << line << std::endl;
        return false;
    }
    return true;
}

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
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

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

    //for the vertices array
    unsigned int buffer;
    GLCheckError(glGenBuffers(1, &buffer));
    GLCheckError(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCheckError(glBufferData(GL_ARRAY_BUFFER, 5 * sizeof(float) * 4  , vertices, GL_STATIC_DRAW));

    //for the indices array
    unsigned int indexBuffer;
    GLCheckError(glGenBuffers(1, &indexBuffer));
    GLCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCheckError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int) , indices, GL_STATIC_DRAW));

    //for vertex positions
    GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0));
    GLCheckError(glEnableVertexAttribArray(0));

    //for vertex colors
    GLCheckError(glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(2*sizeof(float))));
    GLCheckError(glEnableVertexAttribArray(1));

    Shaders shaders = GetShaderSource("resources\\shaders\\BasicShader.shader");
    unsigned int shader = CreateShader(shaders.VertexShader, shaders.FragmentShader);
    glUseProgram(shader);

    //uniforms
    //int uniformLocation = glGetUniformLocation(shader, "translateVal");
    //ASSERT(uniformLocation != -1);

    //int rotation = glGetUniformLocation(shader, "rotate");
    //ASSERT(uniformLocation != -1);
    //
    //float b = 0.5f;
    //float increment = 0.01f;

    ////uniform for scaling
    //int uniformScaleLocation = glGetUniformLocation(shader,"u_ScaleValues");
    //ASSERT(uniformScaleLocation!=-1);

    //int count = 0;
    //float x = 0.01f;
    //float y = 0.01f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //HandleKeyboardInput(window);
        //int keyw = glfwGetKey(window, GLFW_KEY_W);
        //if (keyw == GLFW_PRESS)
        //{
        //    //std::cout << keyw << "Ara Ara" << count << std::endl;
        //    //++count;
        //    y = y + 0.01f;
        //    //glUniform4f(uniformLocation, std::cos(b), f, b, 1.0f);
        //}
        //int keys = glfwGetKey(window, GLFW_KEY_S);
        //if (keys == GLFW_PRESS)
        //{
        //    //std::cout << keys << "Ara Ara" << count << std::endl;
        //    //++count;
        //    y = y - 0.01f;
        //    //glUniform4f(uniformLocation, std::cos(b), f, b, 1.0f);
        //}
        //int keya = glfwGetKey(window, GLFW_KEY_A);
        //if (keya == GLFW_PRESS)
        //{
        //    //std::cout << keyw << "Ara Ara" << count << std::endl;
        //    //++count;
        //    x = x - 0.01f;
        //    //glUniform4f(uniformLocation, std::cos(b), f, b, 1.0f);
        //}
        //int keyd = glfwGetKey(window, GLFW_KEY_D);
        //if (keyd == GLFW_PRESS)
        //{
        //    //std::cout << keyw << "Ara Ara" << count << std::endl;
        //    //++count;
        //    x = x + 0.01f;
        //    //glUniform4f(uniformLocation, std::cos(b), f, b, 1.0f);
        //}
        //glUniform3f(uniformLocation, x,y,0.0f);
        //GLCheckError(glUniform4f(uniformLocation, std::cos(b), std::sin(b), b, 1.0f));
        GLCheckError(glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,(void*)0));
        //glDrawArrays(GL_QUADS,0,4*5);
        /*if (b > 1.0f)
            increment = -0.01f;
        else if (b < 0.0f)
            increment = 0.01f;

        b = b + increment;*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}