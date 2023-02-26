#include "Utils.h"
#include "IOException.h"

namespace gl {
void initializeGLFW() {
    glfwInit();
}

void setOpenGLVersion(int majorVersion, int minorVersion) {
    // Set the major and minor versions of OpenGL we are using
    // This ensures that when a user does not have the proper OpenGL version
    // GLFW fails to run
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
}

void setGLFWUseCoreProfile() {
    /*
    * We also tell GLFW we want to explicitly use the core-profile. Telling GLFW
    we want to use the core-profile means we'll get access to a smaller subset
    of OpenGL features without backwards-compatible features we no longer need.
    */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow *createWindow(double width, double height, std::string title) {
    GLFWwindow *window =
        glfwCreateWindow(width, height,
                         title.c_str(), NULL, NULL);

    return window;
}

std::pair<int, int> calculateWindowWidthHeight(double uWidth, double uHeight) {
    std::pair<int, int> frameBufferSize = calculateFrameBufferSize();
    double fWidth = 1536;
    double fHeight = 864;

    double wScale = frameBufferSize.first / fWidth;
    double hScale = frameBufferSize.second / fHeight;

    return std::make_pair(uWidth * wScale, uHeight * hScale);
}

std::pair<int, int> calculateFrameBufferSize() {
    std::pair<int, int> screenSize = getScreenSize();

    GLFWwindow *window =
        glfwCreateWindow(screenSize.first, screenSize.second, "DESTROY THIS WINDOW AFTER OPENING", NULL, NULL);

    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(window, &width, &height);

    glfwDestroyWindow(window);

    return std::make_pair(width, height);
}

std::pair<int, int> getScreenSize() {
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    const GLFWvidmode *vidMode = glfwGetVideoMode(primary);

    int width = vidMode->width;
    int height = vidMode->height;

    return std::make_pair(width, height);
}

void terminate() {
    glfwTerminate();
}

void updateCurrentContext(GLFWwindow *window) {
    /*
     * OpenGL is a state machine. An OpenGL context holds that state.
     *
     * When you set the current context, you are switching all the state from
     * the old context to the new context. OpenGL is working with the state of
     * whatever context you set to be current. This context belongs to the thread of the window we specify
     *
     * TODO. Learn what state is actually stored in an OpenGL context
     */
    glfwMakeContextCurrent(window);
}

void initializeGLAD() {
    // Because OpenGL is only a specification of what different operations are
    // supposed to do
    // it is up to the graphics card manufacturers to implement the
    // specification. Since there are many different versions of OpenGL drivers,
    // the location of most of its functions is not known at compile-time and
    // needs to be queried at run-time. The location of these function pointers
    // are stored in function pointers for later use. This is a tedious process,
    // so the library GLAD loads them all for us
    bool didGLADSuccessfullyLoad =
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (!didGLADSuccessfullyLoad) {
        throw IOException("Failed to load GLAD");
    }
}

void setViewport(int x, int y, int width, int height) {
    /*
     * The viewport is the region inside the window where OpenGL actually
     * renders For example, we could have OpenGL render in a smaller rect in our
     * window, and then render things outside that rect (splitscreen, for
     * example)
     */
    glViewport(x, y, width, height);
}

void setWindowResizeEvent(GLFWwindow *window) {
    // When the screen is resized, we also need to resize the viewport.
    // Set callback that will do this
    glfwSetFramebufferSizeCallback(window, onWindowResize);
}

void onWindowResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool shouldWindowClose(GLFWwindow *window) {
    return glfwWindowShouldClose(window);
}

void swapBuffers(GLFWwindow *window) {
    /*
    * When an application draws in a single buffer the resulting image may display flickering issues.
    This is because the resulting output image is not drawn in an instant, but drawn pixel by pixel and usually
    from left to right and top to bottom. Because this image is not displayed at an instant to the user while 
    still being rendered to, the result may contain artifacts. To circumvent these issues, windowing applications
    apply a double buffer for rendering. The front buffer contains the final output image that is shown at the screen,
    while all the rendering commands draw to the back buffer. As soon as all the rendering commands are finished we swap
    the back buffer to the front buffer so the image can be displayed without still being rendered to, removing all the
    aforementioned artifacts.
    */
    glfwSwapBuffers(window);
}

void pollEvents() {
    /*
    * Checks if any events are triggered (like keyboard input or mouse movement events),
    updates the window state, and calls the corresponding functions (which we can register via callback methods).
    */
    glfwPollEvents();
}

void processInput(GLFWwindow *window) {
    //Retrieve the state of the escape key from the OpenGL context
    if (getKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //If key is pressed, tell OpenGL context the window should close
        setShouldWindowClose(window, true);
    }
}

int getKey(GLFWwindow *window, int key) {
    //Returns last state of key
    //GLFW_RELEASE or GLFW_PRESS
    return glfwGetKey(window, key);
}

void setShouldWindowClose(GLFWwindow *window, bool shouldWindowClose) {
    //Tell the OpenGL context that the window should be closed
    glfwSetWindowShouldClose(window, true);
}

void setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    //Tell the OpenGL context that, when and if it clears the color buffer,
    //fill the color buffer with this color
    glClearColor(r, g, b, a);
}

void clearColorBuffer() {
    //Tell the OpenGL context to clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
}

unsigned int getShaderProgram(const char *vertexShaderSource, const char * fragmentShaderSource) {
    unsigned int vertexShader = getCompiledShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = getCompiledShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    unsigned int shaderProgram = getLinkedShaderProgram(vertexShader, fragmentShader);

    deleteShader(vertexShader);
    deleteShader(fragmentShader);

    return shaderProgram;
}

unsigned int getCompiledShader(auto shaderType, const char * shaderSource) {
    unsigned int shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // check for shader compile errors
    int didShaderCompileSuccessfully;
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &didShaderCompileSuccessfully);

    if (!didShaderCompileSuccessfully) {
        std::string errorMessage = getShaderCompilationErrorMessage(shader);

        throw IOException(errorMessage);
    }

    return shader;
}

std::string getShaderCompilationErrorMessage(unsigned int shader) {
    char infoLog[512];

    glGetShaderInfoLog(shader, 512, NULL, infoLog);

    return std::string(infoLog);
}

unsigned int getLinkedShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    // link shaders
    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int didShaderLinkSuccessfully;
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &didShaderLinkSuccessfully);
    if (!didShaderLinkSuccessfully) {
        std::string errorMessage = getShaderProgramLinkerErrorMessage(shaderProgram);

        throw IOException(errorMessage);
    }

    return shaderProgram;
}

std::string getShaderProgramLinkerErrorMessage(unsigned int shaderProgram) {
    char infoLog[512];

    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

    return std::string(infoLog);
}

void deleteShader(unsigned int shader) {
    glDeleteShader(shader);
}

glm::mat4 createTransformationMatrix() {
    return glm::mat4(1.0f);
}

glm::mat4 translate(glm::mat4 transform, float x, float y, float z) {
    return glm::translate(transform, glm::vec3(x, y, z));
}

glm::mat4 rotate(glm::mat4 transform, float angle, float axisX, float axisY, float axisZ) {
    return glm::rotate(transform, angle, glm::vec3(axisX, axisY, axisZ));
}

}

