#pragma once
//#include "Shader.h"
//#include "CurveGraphics.h"
//#include "ImguiController.h"
#include <math.h>
#include "Scene.h"
//CurveGraphics* graphics = new CurveGraphics();
//ImguiController _imgui;
//std::vector<int> selectedPoints;
//float currentFrame = static_cast<float>(glfwGetTime());
//    float deltaTime = 0.0f;	// Time between current frame and last frame
//    float lastFrame = 0.0f;
 //Scene* scene ;

struct WindowSize {

public:
    
    WindowSize() { isChanged = false; width = 1280; height = 720; }
    bool isChanged;
    int width;
    int height;

}; 
class Viewer
{
    
    //WINDOW
    GLFWwindow* _window = nullptr;   
    WindowSize _windowSize;
    //Shader shader;
    //static ImguiController _imgui ;
    //static CurveGraphics graphics /*= new = CurveGraphics()*/;
    //static Scene scene;
   
public:

    bool Clean();
    void processInput();
    void start();
    bool init();

    void windowShouldCloseIMGUI();   

    void setupCallbacks();
    static void frameBufferSizeCB(GLFWwindow* window, int width, int height);
    /*static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double x, double y) ;
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);*/
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double x, double y);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

