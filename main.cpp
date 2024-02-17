#include <GL/glew.h>

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>


// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;"
                                   "layout (location = 1) in vec3 color;"
                                   "out vec3 ourColor;"
                                   "void main()"
                                   "{"
                                   "gl_Position = vec4(position, 1.0);"
                                   "ourColor = color;"
                                   "}";
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "in vec3 ourColor;"
                                     "out vec4 color;"
                                     "void main()"
                                     "{"
                                     "color = vec4(ourColor, 1.0f);"
                                     "}";


GLuint LoadShaders()
{
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);                // 4x Сглаживание
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //Версия OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Не используем старый OpenGL



    // Window dimensions
    const GLuint WIDTH = 1024, HEIGHT = 768;

    // Создание окна приложения
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GLFW Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
     glfwMakeContextCurrent(window);


    // Инициализируем GLEW
    glewExperimental = true; // Флаг необходим в Core-режиме OpenGL
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    //Вызов функции
    GLuint shaderProgram = LoadShaders();

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions         // Colors
        -1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
         1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
          0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f   // Top
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO


    //Инициализация интерфейса ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //Режим отслеживания нажатия клавиш в приложении
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //Основной цикл, при нажатии на ECS или закрытии приложения работа завершается
    while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        !glfwWindowShouldClose(window))
    {
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        // Render
        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        //Запуск нового окна ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //Элементы интерфейса в окне
        ImGui::Text("Hello, ImGui!");
        ImGui::Text("Hello, ImGui!");
        ImGui::Text("Hello, ImGui!");
        if (ImGui::Button("Press Me"))
        {
            ImGui::Text("Hello, ImGui!");
        }

        //Отображение ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        //Обмен данными между передним и задним буфером
        glfwSwapBuffers(window);
        //Опрос и обработка событий
        glfwPollEvents();
    }

    //Освобождение ресурсов ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //Уничтожение контекста GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
