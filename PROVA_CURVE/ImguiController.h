#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "PredefinedCurves.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
class ImguiController
{
private:
	bool show_window = true;
	bool show_demo_window = true;
	bool isChanged = false;
 

public:
	ImguiController(){  };
	ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	int grado = 2;
	bool makeBezier = false, makeNURBS = false, circle = false;
	int maxDegree = 0,  minDegree = 0;
	int itemCurrent = 0;
	int item = 0;
	std::vector<float>weights = std::vector<float>(100, 1);
	
	std::vector<float> values;
	
	
	
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
	void setPredefinedCurve();
};

