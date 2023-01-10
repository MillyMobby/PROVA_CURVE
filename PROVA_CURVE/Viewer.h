#pragma once
#include <math.h>
#include "Scene.h"

struct WindowSize {

public:
    
    WindowSize() { isChanged = false; width = 1920; height = 1080; }
    bool isChanged;
    int width;
    int height;

}; 
class Viewer
{ 
    //WINDOW
    GLFWwindow* _window = nullptr;   
    WindowSize _windowSize;
    //static float deltaTime/* = 0.0f*/;	 //Time between current frame and last frame
    //float lastFrame = 0.0f;
    bool isMoving = false;
   
public:
    
    bool Clean();
    bool processInput();
    void start();
    bool init();

    void setupCallbacks();
    static void frameBufferSizeCB(GLFWwindow* window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double x, double y);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

