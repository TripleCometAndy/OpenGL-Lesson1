#pragma once

//We need to include glad before glfw
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <string>

namespace gl {
void initializeGLFW();
void setOpenGLVersion(int majorVersion, int minorVersion);
void setGLFWUseCoreProfile();
GLFWwindow * createWindow(double width, double height, std::string title);
std::pair<int, int> calculateWindowWidthHeight(double uWidth, double uHeight);
std::pair<int, int> calculateFrameBufferSize();
std::pair<int, int> getScreenSize();
void terminate();
void updateCurrentContext(GLFWwindow *window);
void initializeGLAD();
void setViewport(int x, int y, int width, int height);
void setWindowResizeEvent(GLFWwindow *window);
void onWindowResize(GLFWwindow *window, int width, int height);
bool shouldWindowClose(GLFWwindow * window);
void swapBuffers(GLFWwindow *window);
void pollEvents();
void processInput(GLFWwindow *window);
int getKey(GLFWwindow *window, int key);
void setShouldWindowClose(GLFWwindow * window, bool shouldWindowClose);
void setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void clearColorBuffer();
}