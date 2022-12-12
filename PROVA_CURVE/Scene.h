#pragma once
#include "Camera.h"
#include "Shader.h"
#include "CurveGraphics.h"
#include "ImguiController.h"
//CurveGraphics* graphics = new CurveGraphics();
class Scene {
private:
	//Camera camera;
	CurveGraphics* _graphics;	
	ImguiController _imgui;
	Shader _shader;
	Camera _camera;
	

public:

	Scene() {
		_graphics = new CurveGraphics();
	};
	~Scene() {  };
	inline Shader getShader() { return _shader; }
	void initScene();
	void loadCamera(int w, int h);
	void setIMGUI(GLFWwindow* window);
	void BG();
	void run();
	void clean();
	void handleMouseEvents(int& button, int& action, double& clipX, double& clipY, int& width, int& height);
	void updatePoints(double& dotX, double& dotY);
	void handleKeyEvents(int key);
	void windowShouldCloseIMGUI(GLFWwindow* _window);
};

