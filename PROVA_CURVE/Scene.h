#pragma once
#include "Camera.h"
#include "Shader.h"
#include "CurveGraphics.h"
//CurveGraphics* graphics = new CurveGraphics();
class Scene {
private:
	Camera camera;
	CurveGraphics* graphics;
	Shader* shader;
	 

public:
	Scene(CurveGraphics* Graphics, Shader* Shader) {
		graphics = Graphics;
		//graphics = new CurveGraphics();
		shader = Shader;
	};
	~Scene() { graphics->~CurveGraphics(); };

	void initScene();
	void run();
	void clean();
	void handleMouseEvents(int button, int action, int clipX, int clipY, int width, int heights);
	void updatePoints(int dotX, int dotY);
	void handleKeyEvents(int key);
};

