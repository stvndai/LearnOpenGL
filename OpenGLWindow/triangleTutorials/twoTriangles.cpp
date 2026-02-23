#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


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
        0.5f,  0.5f, 0.0f,  //0 top right
        0.5f, -0.5f, 0.0f,  //1 bottom right
       -0.5f, -0.5f, 0.0f,  //2 bottom left
       -0.5f,  0.5f, 0.0f,   //3 top left 
       -1.0f, -0.5f, 0.0f,  // 4
        0.0f, -0.5f, 0.0f,  // 5
        1.0f, -0.5f, 0.0f, // 6

    };
    unsigned int indices[] = {  // note that we start from 0!
        5, 4, 3,   // first triangle
        6, 5, 0    // second triangle
    };

    // working on shaders
    // vertex shaders for shape and position
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragmentShader for color
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //I must link the two shaders by creating a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPLIATION_FAILED\n" << infoLog << std::endl;
    }
    


    // VBO and VAO

    // create VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
      
    // create VAO  vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VAO2;


    // 2. bind the VAO(vertex array)
    glBindVertexArray(VAO);

    // bind the buffer and upload data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 2. set the vertex atrribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // creating element buffer object EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        inputModeSwap(window, isFill);

        // clear screen FIRST
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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