#include <iostream>

//We need to include glad before glfw
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Utils.h"
#include "IOException.h"

int main() {
    gl::initializeGLFW();

    gl::setOpenGLVersion(3, 3);

    gl::setGLFWUseCoreProfile();

    // Need to have a properly scaled window based on this monitor's DPI
    std::pair<int, int> windowSize = gl::calculateWindowWidthHeight(800, 600);
    int width = windowSize.first;
    int height = windowSize.second;

    GLFWwindow *window = gl::createWindow(width, height, "HELLO GLFW");

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

    gl::setViewport(0, 0, width, height);

    gl::setWindowResizeEvent(window);


    while (!gl::shouldWindowClose(window)) {
        gl::processInput(window);

        gl::setClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::clearColorBuffer();

        gl::pollEvents();
        gl::swapBuffers(window);
    }

    gl::terminate();

	return 0;
}