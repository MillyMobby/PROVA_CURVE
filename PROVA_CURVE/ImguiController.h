#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include <vector>;
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
class ImguiController
{
private:
	bool show_window = true;
	/*show_another_window = true*/
 

public:
	ImguiController(){  };
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	int grado = 2;
	bool makeBezier = false, makeNURBS = false;
	int maxDegree = 0,  minDegree = 0;
	 std::vector<float>weights = std::vector<float>(100, 1);
	
	~ImguiController() {};
	void setIMGUI(GLFWwindow* _window);
	void displayInputGuide();
	void DegreeRangeWidget();
	void degreeSlider();
	void NurbsWeightEditor();
	void windowIMGUI(GLFWwindow* _window) ;
	void renderIMGUI();
	void cleanupIMGUI();

	bool imguiWantsToCaptureMouse();
};

