#include "Scene.h"
void Scene::initScene() {

	shader->setName("color");
	shader->processShader();
	graphics->setupGL();
	graphics->initGL();
	// ci andra' anche la camera
}

void Scene::run() {
	glUseProgram(shader->getShaderProgram());

	//glUniform2f(glGetUniformLocation(shader.getShaderProgram(), "windowCoords"), _windowSize.width, _windowSize.height);
	graphics->renderScene();
}

void Scene::clean() {
	//graphics->cleanGL(shader->getShaderProgram());
}

void Scene::handleMouseEvents(int button, int action, int clipX, int clipY, int width,int height) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//std::cout << " mouse in " << xpos << ", " << ypos << std::endl;
		graphics->AddPoint(/*xpos, ypos*/clipX, clipY);
		
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			double minDist = 10000.0;
			int minI;
			for (int i = 0; i < graphics->getPointsNumber(); i++) {
				double thisDistX = (/*xpos*/clipX - graphics->getControlPt_X(i)) * 0.5f * (double)width;
				double thisDistY = (/*ypos*/clipY - graphics->getControlPt_Y(i)) * 0.5f * (double)height;
				double thisDist = sqrtf(thisDistX * thisDistX + thisDistY * thisDistY);
				if (thisDist < minDist) {
					minDist = thisDist;
					minI = i;
				}
			}
			if (minDist <= 10.0) {
				graphics->setSelectedVert(minI);
				graphics->ChangePoint(minI, /*xpos, ypos*/clipX, clipY);
			}
		}
		else if (action == GLFW_RELEASE) {
			graphics->setSelectedVert(-1);
		}
	}
}

void Scene::updatePoints(int dotX, int dotY) {
	if (graphics->getSelectedVert() == -1) { return; }
	
	graphics->ChangePoint(graphics->getSelectedVert(), dotX, dotY);
}

void Scene::handleKeyEvents(int key) {
	if (key == GLFW_KEY_F) {		
		if (graphics->getSelectedVert() != 0)
		{
			graphics->RemoveFirstPoint();
			if (graphics->getSelectedVert() < 0) {
				graphics->setSelectedVert(graphics->getSelectedVert());
			}
			else graphics->setSelectedVert(-1);
		}
	}
	else if (key == GLFW_KEY_L) {
		graphics->RemoveLastPoint();
	}
}