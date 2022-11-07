#pragma once
#include "Shader.h"
#include "CurveGraphics.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <math.h>

CurveGraphics graphics;

struct WindowSize {

public:

    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f;

    WindowSize() { isChanged = false; width = 1280; height = 720; }

    bool isChanged;
    int width;
    int height;

}; 
class Viewer
{
    //WINDOW
    GLFWwindow* _window = nullptr;
    GLFWwindow* _window2 = nullptr;
    WindowSize _windowSize;
    Shader shader;

    bool show_demo_window = true;
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
public:

    bool Clean();
    void processInput();
    void start();
    bool init();

    void setIMGUI();
    void windowIMGUI();
    void renderIMGUI();
    void cleanupIMGUI();

    void setupCallbacks();
    static void frameBufferSizeCB(GLFWwindow* window, int width, int height);
    //void processMouseInput();
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double x, double y) ;
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfw_error_callback(int error, const char* description);

};

