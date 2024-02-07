#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "shaderClass/shaderClass.h"
#include "vboClass/vboClass.h"
#include "vaoClass/vaoClass.h"
#include "eboClass/eboClass.h"

// Vertex Shader source code
// const char* vertexShaderSource = "#version 330 core\n"
// "layout (location = 0) in vec3 aPos;\n"
// "void main()\n"
// "{\n"
// "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
// "}\0";
//Fragment Shader source code
// const char* fragmentShaderSource = "#version 330 core\n"
// "out vec4 FragColor;\n"
// "void main()\n"
// "{\n"
// "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
// "}\n\0";

// Triforce
GLfloat vertices[] = {
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

GLuint indices[] = {
    0, 3, 5,
    3, 2, 4,
    5, 4, 1
};

// Square
// GLfloat vertices[] = {
//     0.5f, 0.5f, 0.0f,   // Top Right
//     -0.5f, -0.5f, 0.0f, // Bottom Left
//     0.5f, -0.5f, 0.0f,  // Bottom Right
//     -0.5f, 0.5f, 0.0f,  // Top Left
// };

// GLuint indices[] = {
//     0, 1, 2,
//     0, 3, 1
// };

const int WINDOW_STARTING_WIDTH = 800;
const int WINDOW_STARTING_HEIGHT = 800;

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_STARTING_WIDTH, WINDOW_STARTING_HEIGHT, "Test", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, 800, 800);

    shaderClass shaderProgram("../resources/shaders/default.vert", "../resources/shaders/default.frag");

    vaoClass VAO;
    VAO.Bind();

    vboClass VBO(vertices, sizeof(vertices));
    eboClass EBO(indices, sizeof(indices));

    VAO.LinkVBO(VBO, 0);
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    int w, h, lastW, lastH;
    lastW = WINDOW_STARTING_WIDTH;
    lastH = WINDOW_STARTING_HEIGHT;

    while (!glfwWindowShouldClose(window)) {
        glfwGetWindowSize(window, &w, &h);
        // If window size changes, set view port to knew size
        if (w != lastW || h != lastH) {
            std::cout << "Window size changed\n";
            std::cout << "Width: " << w << "\nHeight: " << h << "\n";
            lastW = w;
            lastH = h;
            glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        }
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.Activate();
        VAO.Bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // Processes all pending GLFW events
        glfwPollEvents();
    }

    // Deleting all objects that were created
    VAO.Delete();
    VBO.Delete();
    EBO.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

// #include <filesystem>
// #include <iostream>

// int main() {
//     const std::string frag_path = "resources/shaders/default.frag";
//     const std::string vert_path = "resources/shaders/default.vert";

//     if (!std::filesystem::exists(frag_path) || !std::filesystem::exists(vert_path)) {
//         std::cout << "Files not found in the specified directories." << std::endl;
//     }
//     else {
//         std::cout << "Files found";
//     }
//     return 0;
// }