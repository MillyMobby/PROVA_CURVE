#pragma once
#include "Camera.h"
#include "Shader.h"
#include "CurveGraphics.h"
#include "ImguiController.h"
#include "LineStripGraphics.h"
class Scene {
private:
	CurveGraphics* _graphics;	
	std::vector<LineStripGraphics> lsg;
	LineStripGraphics* ls;
	ImguiController _imgui;
	Shader _shader;
	Camera _camera;
	
	double startX = 0, startY = 0;
public:

	Scene() {
		_graphics = new CurveGraphics();
		ls = new LineStripGraphics();
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
	void updatePoints(double& dotX, double& dotY, float& deltaTime);
	void handleKeyEvents(int key);
	void windowShouldCloseIMGUI(GLFWwindow* _window);
	Camera& getCamera();
	void updateCamera(int w, int h);
	void checkForTransformations(bool wasMoved);
	void BSplineBasisGraphic();
};

