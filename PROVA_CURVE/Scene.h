#pragma once
#include "Camera.h"
#include "Shader.h"
#include "CurveGraphics.h"
#include "ImguiController.h"
//CurveGraphics* graphics = new CurveGraphics();
static double p[] = { 0.1, 0.1,0.0, 0.2, 0.3,0.0, 0.3, 0.4,0.0, 0.4, 0.2,0.0, 0.5, 0.1, 0.0,0.6, 0.2,0.0 };

class Scene {
private:
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
	void run(float deltaTime);
	void clean();
	void handleMouseEvents(int& button, int& action, double& clipX, double& clipY, int& width, int& height);
	void updatePoints(double& dotX, double& dotY);
	void handleKeyEvents(int key);
	void windowShouldCloseIMGUI(GLFWwindow* _window);
	Camera& getCamera();
	void updateCamera(int w, int h);
	void checkForTransformations(bool wasMoved);
	void BSplineBasisGraphic();
};

