#include <iostream>

//We need to include glad before glfw
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Utils.h"
#include "IOException.h"

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


int main() {
    gl::initializeGLFW();
    gl::setOpenGLVersion(3, 3);
    gl::setGLFWUseCoreProfile();
    
    // Need to have a properly scaled window based on this monitor's DPI
    std::pair<int, int> windowSize = gl::calculateWindowWidthHeight(800, 600);
    int width = windowSize.first;
    int height = windowSize.second;

    GLFWwindow *window = gl::createWindow(width, height, "HELLO GLFW 2");

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        gl::terminate();
        return -1;
    }

    gl::updateCurrentContext(window);

    try {
        gl::initializeGLAD();
    }
    catch (gl::IOException &ex) {
        gl::terminate();
        return -1;
    }

    unsigned int shaderProgram = gl::getShaderProgram(vertexShaderSource, fragmentShaderSource);

    ///////////////////////////////////////////////////////////////////////////////

        // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f  // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound
    // element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    gl::setViewport(0, 0, width, height);

    gl::setWindowResizeEvent(window);



    while (!gl::shouldWindowClose(window)) {
        gl::processInput(window);
        gl::setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::clearColorBuffer();

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        gl::pollEvents();
        gl::swapBuffers(window);
    }

    gl::terminate();

	return 0;
}