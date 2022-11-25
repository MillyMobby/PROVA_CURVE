#include "Viewer.h"
void main() {
	Viewer view;
	/*CurveGraphics* graphics = new CurveGraphics();
	Shader* shader = new Shader();
	shader->setName("color");
	shader->processShader();
	scene = new Scene(graphics, shader);
	scene->initScene();*/
	//view.setScene(scene);
	view.start();
	
}

void Viewer::start() {
		init();
		shader.setName("color");
		shader.processShader();
		graphics->setupGL();
		graphics->initGL();
		//graphics->initCurveGL();
		//scene.initScene();
		/*
		*/
		while (!glfwWindowShouldClose(_window)) {
			
			//processMouseInput();		
			glClearColor(_imgui.clear_color.x * _imgui.clear_color.w, _imgui.clear_color.y * _imgui.clear_color.w, _imgui.clear_color.z * _imgui.clear_color.w, _imgui.clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
			const float clearDepth = 1.0f;
			glClearBufferfv(GL_DEPTH, 0, &clearDepth);	

			//scene->run();
			glUseProgram(shader.getShaderProgram());	
			graphics->check(_imgui.grado, _imgui.makeBezier);
			////glUniform2f(glGetUniformLocation(shader.getShaderProgram(), "windowCoords"), _windowSize.width, _windowSize.height);
			graphics->renderScene();
			//graphics->generateFullCurve();
			graphics->renderCurve();
			windowShouldCloseIMGUI();
			
			glfwPollEvents();
			glfwSwapBuffers(_window);
			
		}
		_imgui.cleanupIMGUI();
		graphics->cleanGL(shader.getShaderProgram());
		//scene->clean();
		Clean();
		
}

bool Viewer::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_windowSize.isChanged = false;

	_window = glfwCreateWindow(_windowSize.width, _windowSize.height, "curve", NULL, NULL);

	glfwMakeContextCurrent(_window);
	if (_window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
				//glViewport(0,0, _windowSize.width, _windowSize.height); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
				//glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
				//glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
				//glOrtho(0, 1, 0, 1, 0, 1); // essentially set coordinate system
				//glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
				//glLoadIdentity(); // same as above comment
				//glfwSetFramebufferSizeCallback(_window, Viewer::frameBufferSizeCB);
	glfwSetWindowUserPointer(_window, (void*)&_windowSize);
	setupCallbacks();	
	_imgui.setIMGUI(_window);
	return 1;
}
//void Viewer::setScene(Scene& currentScene) { scene = currentScene; }
void Viewer::processInput() {}

bool Viewer::Clean() {
	
	glfwTerminate();
	return true;
}

void Viewer::setupCallbacks() {
	glfwSetFramebufferSizeCallback(_window, frameBufferSizeCB);
	glfwSetKeyCallback(_window, keyCallback);	
	glfwSetMouseButtonCallback(_window, mouseButtonCallback);
	glfwSetCursorPosCallback(_window, cursorPosCallback);
}

void Viewer::frameBufferSizeCB(GLFWwindow* window, int width, int height) {
	if (window != nullptr) {
		glViewport(0, 0, width, height);
		WindowSize* windowSize = (WindowSize*)glfwGetWindowUserPointer(window);
		if (windowSize != nullptr) {
			windowSize->width = width;
			windowSize->height = height;
			windowSize->isChanged = true;
		}
	}
}

void Viewer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	WindowSize* windowSize = (WindowSize*)glfwGetWindowUserPointer(window);
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// clip space [-1,1]. 
	double clipX =  (2.0f * (double)xpos / (double)(windowSize->width )) - 1.0f;
	double clipY = 1.0f - (2.0f * (double)ypos / (double)(windowSize->height )); 
	auto& io = ImGui::GetIO();

	if (io.WantCaptureMouse || io.WantCaptureKeyboard) { return; }
	else {
	//scene->handleMouseEvents(button, action, clipX, clipY, windowSize->width, windowSize->height);
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			std::cout << " mouse in " << clipX << ", " << clipY << std::endl;
			graphics->AddPoint(/*xpos, ypos*/clipX, clipY);
			//graphics->check(_imgui.i); // setCurve();
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				double minDist = 10000.0;
				int minI;
				for (int i = 0; i < graphics->getPointsNumber(); i++) {
					double thisDistX =  (/*xpos*/clipX - graphics->getControlPt_X(i)) * 0.5f *(double)windowSize->width;
					double thisDistY =  (/*ypos*/clipY - graphics->getControlPt_Y(i)) * 0.5f *(double)windowSize->height;
					double thisDist = sqrtf(thisDistX * thisDistX + thisDistY * thisDistY);
					if (thisDist < minDist) {
						minDist = thisDist;
						minI = i;
					}
				}
				if (minDist <= 10.0) {     				
					graphics->setSelectedVert(minI);
					graphics->ChangePoint(minI, /*xpos, ypos*/clipX, clipY);
				
				
				
				}//graphics->modifyCurve(minI);
			
			}
				else if (action == GLFW_RELEASE) {
					graphics->modifyCurve(/*graphics->getSelectedVert(), clipX, clipY*/);
					graphics->setSelectedVert(-1);//graphics->generateFullCurve();
				}
			}
	}
	
	}//

void Viewer::cursorPosCallback(GLFWwindow* window, double x, double y) {
	WindowSize* windowSize = (WindowSize*)glfwGetWindowUserPointer(window);
	double dotX = (2.0f * (double)x / (double)(windowSize->width - 1)) - 1.0f;
	double dotY = 1.0f - (2.0f * (double)y / (double)(windowSize->height - 1));
	if (graphics->getSelectedVert() == -1) { return; }
	//scene->updatePoints( dotX, dotY);
	graphics->ChangePoint(graphics->getSelectedVert(), dotX, dotY);
	graphics->modifyCurve(/*graphics->getSelectedVert(), dotX, dotY*/);
	//graphics->updateCurvePoints     magari con deboor locale (?)
}

void Viewer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		return;			
	}
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_X) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_F) {
		//scene->handleKeyEvents( key);
		if (graphics->getSelectedVert() != 0) 
		{
			graphics->RemoveFirstPoint();
			if (graphics->getSelectedVert() < 0) {
				//graphics->setSelectedVert(graphics->getSelectedVert());
			}		
			else graphics->setSelectedVert(-1);
		}
	}
	else if (key == GLFW_KEY_L) {
			graphics->RemoveLastPoint();	
	}
}

void Viewer::windowShouldCloseIMGUI() {
	_imgui.windowIMGUI(_window);
	_imgui.renderIMGUI();
}
