#pragma once
//#include "Shader.h"
//#include "CurveGraphics.h"
#include "ImguiController.h"
#include <math.h>
#include "Scene.h"
CurveGraphics* graphics = new CurveGraphics();
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
    Shader shader;
    ImguiController _imgui;
    //Scene scene;

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
public:

    bool Clean();
    void processInput();
    void start();
    bool init();
    void setShader(Shader& s);

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

