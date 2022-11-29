#include "ImguiController.h"

void ImguiController::setIMGUI(GLFWwindow* _window) {
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

void ImguiController::displayInputGuide() {
	ImGui::Begin("Curve Settings");
	//ImGui::BulletText("Double-click on title bar to collapse window.");
	ImGui::Text("Keyboard input guide:");
	ImGui::BulletText("[F] = remove first control point");
	ImGui::BulletText("[L] = remove last control point");
	ImGui::Text("Mouse input guide:");
	ImGui::BulletText("[right] = add control point");
	ImGui::BulletText("[left] = move selected control point\n");
}

void ImguiController::DegreeRangeWidget() {
	
	static int max;
	if (maxDegree > 0) minDegree = 1; max = maxDegree;
	ImGui::DragIntRange2("Degree Range", &minDegree, &maxDegree, 5, 0, 1000, "Min: %d", "Max: %d");
	if (maxDegree > max) maxDegree = max;
}

void ImguiController::degreeSlider() {
	if (maxDegree >= 1 && grado <= maxDegree) {
		//ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(1.0f, 0.0f, 0.0f));
		ImGui::SliderInt("Degree Choiche", &grado, 1, 10);
	}
}

void ImguiController::NurbsWeightEditor() {
	if (makeNURBS == true) {
		if (maxDegree >= grado) {
			ImGui::BulletText("NURBS weight slider:\n");
			for (int i = 0; i < maxDegree + minDegree; i++)
			{

				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
				ImGui::VSliderFloat("##v", ImVec2(18, 160), &weights[i], 0.0f, 1.0f, "w");
				if (ImGui::IsItemActive() || ImGui::IsItemHovered())
					ImGui::SetTooltip("%.3f", weights[i]);
				ImGui::PopStyleColor(4);
				ImGui::PopID();
			}
		}
	}
	else if (makeNURBS == false) {
		if (maxDegree >= grado) { for (int i = 0; i < maxDegree + minDegree; i++) { weights[i] = 1.0f; } }
	}
}

void ImguiController::windowIMGUI(GLFWwindow* _window) {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	
	if (show_window)
		//ImGui::ShowDemoWindow(&show_demo_window);
		//Begin/End pair to createa named window.
	{
		displayInputGuide();
		DegreeRangeWidget();
		degreeSlider();
		
		ImGui::Text("Control Points = % d\n", (maxDegree + minDegree));
		
		ImGui::Checkbox("Make Bezier", &makeBezier);
		ImGui::SameLine();
		ImGui::Text("(Degree %d)", maxDegree);
		ImGui::Checkbox("Make NURBS (Add Weights)", &makeNURBS);
		NurbsWeightEditor();		
		
		ImGui::End();
	}

	// 3. Show another simple window.
	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Make Bezier")) {
	//		
	//	}
			//show_another_window = false;
	/*	ImGui::End();
	*/
}

void ImguiController::renderIMGUI() {
	// Rendering
	ImGui::Render();
	int display_w, display_h;
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void ImguiController::cleanupIMGUI() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}




















//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		
		////IMGUI_DEMO_MARKER("Widgets/Basic/RadioButton");
		//static int e = 0;
		//ImGui::RadioButton("radio a", &e, 0); ImGui::SameLine();
		//ImGui::RadioButton("radio b", &e, 1); ImGui::SameLine();
		//ImGui::RadioButton("radio c", &e, 2);
		//if (ImGui::IsItemHovered())
		//	ImGui::SetTooltip("I am a tooltip");

		//for (int i = 0; i < 7; i++)
		//{
		//	if (i > 0)
		//		ImGui::SameLine();
		//	ImGui::PushID(i);
		//	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f));
		//	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.7f));
		//	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.8f, 0.8f));
		//	ImGui::Button("Click");
		//	ImGui::PopStyleColor(3);
		//	ImGui::PopID();
		//}
