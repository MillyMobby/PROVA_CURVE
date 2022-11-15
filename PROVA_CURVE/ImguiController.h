#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

class ImguiController
{
private:
	bool show_demo_window = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
	ImguiController() {};
	~ImguiController() {};
	void setIMGUI(GLFWwindow* _window);
	void windowIMGUI(GLFWwindow* _window) ;
	void renderIMGUI();
	void cleanupIMGUI();
};

