#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImguiController
{
private:
	bool show_demo_window = true;
	/*show_another_window = true*/
 

public:
	ImguiController(){  };
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	int grado=3;
	bool makeBezier = false;
	//ImguiController() {};
	~ImguiController() {};
	void setIMGUI(GLFWwindow* _window);
	void windowIMGUI(GLFWwindow* _window) ;
	void renderIMGUI();
	void cleanupIMGUI();

	bool imguiWantsToCaptureMouse();
};

