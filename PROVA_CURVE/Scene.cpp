#include "Scene.h"

void Scene::initScene() {
	_shader.processShader();
	_graphics->setupGL();
	_graphics->initGL();
	// ci andra' anche la camera
}

void Scene::loadCamera(int w, int h) {
//_camera.setFov(attr);
// 
_camera.setNear(0.1); 
_camera.setFar(100.0); 

	

_camera.setEye(Vec3f(0.0f, 0.0f, 2.0f));
_camera.setLookAt(Vec3f(0.0f, 0.0f, 1.0f));
_camera.setup(Vec3f(0.0f, 1.0f, 0.0f));
_camera.computeAspectRatio(w,h);
	
	_camera.update();

}

void Scene::setIMGUI(GLFWwindow* window) {
	_imgui.setIMGUI(window);
}

void Scene::BG() { glClearColor(_imgui.clear_color.x * _imgui.clear_color.w, _imgui.clear_color.y * _imgui.clear_color.w, _imgui.clear_color.z * _imgui.clear_color.w, _imgui.clear_color.w); }

void Scene::run() {
	glUseProgram(_shader.getShaderProgram());
	//glUniform2f(glGetUniformLocation(shader.getShaderProgram(), "windowCoords"), _windowSize.width, _windowSize.height);
	_graphics->rendering(_imgui.grado, _imgui.makeBezier, _imgui.makeNURBS, _imgui.weights);
}

void Scene::clean() {
	_graphics->cleanGL(_shader.getShaderProgram());
	_imgui.cleanupIMGUI();
}

void Scene::handleMouseEvents(int& button, int& action, double& clipX, double& clipY, int& width,int& height) {
	auto& io = ImGui::GetIO();
	if (io.WantCaptureMouse || io.WantCaptureKeyboard) { return; }

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << " mouse in " << clipX << ", " << clipY << std::endl;
		_graphics->AddPoint(/*xpos, ypos*/clipX, clipY);
		_imgui.maxDegree = _graphics->getPointsNumber() - 1;
		
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			double minDist = 10000.0;
			int minI;
			for (int i = 0; i < _graphics->getPointsNumber(); i++) {
				double thisDistX = (/*xpos*/clipX - _graphics->getControlPt_X(i)) * 0.5f * (double)width;
				double thisDistY = (/*ypos*/clipY - _graphics->getControlPt_Y(i)) * 0.5f * (double)height;
				double thisDist = sqrtf(thisDistX * thisDistX + thisDistY * thisDistY);
				if (thisDist < minDist) {
					minDist = thisDist;
					minI = i;
				}
				if (minDist <= 10.0) {minI; }
			}			
			_graphics->setSelectedVert(minI);
			_graphics->ChangePoint(minI, /*xpos, ypos*/clipX, clipY);
		}
		else if (action == GLFW_RELEASE) {
			_graphics->setSelectedVert(-1);
		}
	}
}

void Scene::updatePoints(double& dotX, double& dotY) {
	if (_graphics->getSelectedVert() == -1) { return; }
	
	_graphics->ChangePoint(_graphics->getSelectedVert(), dotX, dotY);
}

void Scene::handleKeyEvents(int key) {
	if (key == GLFW_KEY_F) {		
		if (_graphics->getSelectedVert() != 0)
		{
			_graphics->RemoveFirstPoint();
			_imgui.maxDegree = _graphics->getPointsNumber() - 1;
			if (_graphics->getSelectedVert() < 0) {
				//graphics->setSelectedVert(graphics->getSelectedVert());
			}
			else _graphics->setSelectedVert(-1);
		}
	}
	else if (key == GLFW_KEY_L) {
		_graphics->RemoveLastPoint();
		_imgui.maxDegree = _graphics->getPointsNumber() - 1;
	}
}

void Scene::windowShouldCloseIMGUI(GLFWwindow* _window) {
	_imgui.windowIMGUI(_window);
	_imgui.renderIMGUI();
}