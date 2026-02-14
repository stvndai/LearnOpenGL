#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"

//const char* vertexShaderSource = 
//"#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
//"out vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos, 1.0);\n"
//"   ourColor = aColor;\n"  
//"}\0";
//
////const char* fragmentShaderSource =
////"#version 330 core\n"
////"out vec4 FragColor;\n"
////"in vec4 vertexColor;\n"
////"void main()\n"
////"{\n"
////"    FragColor = vertexColor;\n"
////"}\0";
//
//const char* fragmentShaderSource =
//"#version 330 core\n"
//"out vec4 FragColor;\n"
//"in vec3 ourColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(ourColor, 1.0f);\n"
//"}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);


    void framebuffer_size_callback(GLFWwindow * window, int width, int height);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bool isFill = true;
    void processInput(GLFWwindow * window);
    void inputModeSwap(GLFWwindow * window, bool& isFill);

    float vertices[] = {
        // positions         // colors
         0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };


    // use the shader class made
    Shader ourShader("slideRightShader.txt", "fragmentShader.txt");

    // VBO and VAO

    // create VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // create VAO  vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);


    // 2. bind the VAO(vertex array)
    glBindVertexArray(VAO);

    // bind the buffer and upload data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 2. set the vertex atrribute pointers
    // this is position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    //main loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        inputModeSwap(window, isFill);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        ourShader.use();

        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //glUseProgram(shaderProgram);
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        float timeValue = glfwGetTime();
        float xOffset = sin(timeValue) * 0.5f;
        ourShader.setFloat("xOffset", xOffset);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void inputModeSwap(GLFWwindow* window, bool& isFill) {

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (isFill) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            isFill = false;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            isFill = true;
        }
    }
}