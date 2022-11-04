#include "Viewer.h"
void main() {
	Viewer view;
	view.start();
}

void Viewer::start() {
		init();
		shader.setName("color");
		shader.processShader();
		graphics.setupGL();
		graphics.initGL();

		while (!glfwWindowShouldClose(_window)) {
			//processMouseInput();		
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			const float clearDepth = 1.0f;
			glClearBufferfv(GL_DEPTH, 0, &clearDepth);	
			glUseProgram(shader.getShaderProgram());	

			graphics.renderScene();
			//processInput();

			
			glfwPollEvents();

			//windowIMGUI();
			//renderIMGUI();
			glfwSwapBuffers(_window);
			
		}
		//cleanupIMGUI();
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
	//glViewport(0.0f, 0.0f, _windowSize.width, _windowSize.height); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	//glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	//glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	//glOrtho(0, _windowSize.width, 0, _windowSize.height, 0, 1); // essentially set coordinate system
	//glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	//glLoadIdentity(); // same as above comment
	setIMGUI();
	glfwSetFramebufferSizeCallback(_window, Viewer::frameBufferSizeCB);
	glfwSetWindowUserPointer(_window, (void*)&_windowSize);
	setupCallbacks();
	return 1;
}

void Viewer::processInput() {

}

bool Viewer::Clean() {
	glfwTerminate();
	return true;
}

void Viewer::setupCallbacks() {
	glfwSetErrorCallback(glfw_error_callback);
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
	float clipX =  (2.0f * (float)xpos / (float)(windowSize->width )) - 1.0f;
	float clipY = 1.0f - (2.0f * (float)ypos / (float)(windowSize->height ));

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		graphics.AddPoint(clipX, clipY);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			float minDist = 10000.0f;
			int minI;
			for (int i = 0; i < graphics.getPointsNumber(); i++) {
				float thisDistX =  (clipX - graphics.getControlPt_X(i)) * 0.5f *(float)windowSize->width;
				float thisDistY =  (clipY - graphics.getControlPt_Y(i)) * 0.5f *(float)windowSize->height;
				float thisDist = sqrtf(thisDistX * thisDistX + thisDistY * thisDistY);
				if (thisDist < minDist) {
					minDist = thisDist;
					minI = i;
				}
			}
			if (minDist <= 10.0) {     
				std::cout << "vertice selezionato" << std::endl;
				graphics.setSelectedVert(minI);
				graphics.ChangePoint(minI, clipX, clipY);
			}
		}
		else if (action == GLFW_RELEASE) {
			graphics.setSelectedVert(-1);
		}
	}
}

void Viewer::cursorPosCallback(GLFWwindow* window, double x, double y) {

	WindowSize* windowSize = (WindowSize*)glfwGetWindowUserPointer(window);
	if (graphics.getSelectedVert() == -1) {
		return;
	}
	float dotX = (2.0f * (float)x / (float)(windowSize->width - 1)) - 1.0f;
	float dotY = 1.0f - (2.0f * (float)y / (float)(windowSize->height - 1));

	graphics.ChangePoint(graphics.getSelectedVert(), dotX, dotY);
}

void Viewer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		return;			
	}
	if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_X) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_F) {
		if (graphics.getSelectedVert() != 0) {   
			graphics.RemoveFirstPoint();
			if (graphics.getSelectedVert() < 0) {
				graphics.setSelectedVert(graphics.getSelectedVert());
			}		
			else graphics.setSelectedVert(-1);
		}
	}
	else if (key == GLFW_KEY_L) {
			graphics.RemoveLastPoint();		
	}
}

void Viewer::renderIMGUI() {
// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Viewer::glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Viewer::setIMGUI() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	
	
}

void Viewer::windowIMGUI() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

}

void Viewer::cleanupIMGUI() {
// Cleanup
ImGui_ImplOpenGL3_Shutdown();
ImGui_ImplGlfw_Shutdown();
ImGui::DestroyContext();
}
