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
#include "VertexArray.h"
#include "Shader.h"

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

        //float vertices[] = {    //anti clockwise
        //    // 2-position      
        //     -0.5f, -0.5f,     //bottom left
        //      0.5f, -0.5f,     //bottom right
        //      0.5f,  0.5f,     //top right
        //     -0.5f,  0.5f,     //top left
        //};

        unsigned int indices[] = {
                0,1,2,
                0,2,3
        };

        VertexBuffer vertexBuffer(vertices, 5 * sizeof(float) * 4);

        IndexBuffer indexBuffer(indices, 6);


        VertexArray vertexArray;
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(3);
        vertexArray.AddBuffer(vertexBuffer, layout);


        ////for vertex positions
        //GLCheckError(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0));
        //GLCheckError(glEnableVertexAttribArray(0));

        ////for vertex colors
        //GLCheckError(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));
        //GLCheckError(glEnableVertexAttribArray(1));

        Shader shader = Shader("resources\\shaders\\BasicShader.shader");

        Renderer renderer;

        float redChannel = 0.f;
        float increment = 0.1f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            if (redChannel > 1.f)
                increment = -0.1f;
            else if (redChannel < 0.f)
                increment = +0.1f;

            redChannel += increment;

            shader.Bind();
            shader.SetUniform4f("u_Color", redChannel, 0.5f, 0.8f, 1.f);

            //vertexArray.Bind();
            //indexBuffer.Bind();

            //GLCheckError(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0));
            renderer.Draw(vertexArray, indexBuffer, shader);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();
    }
    glfwTerminate();
    return 0;
};