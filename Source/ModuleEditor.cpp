#include "ModuleEditor.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleExercice.h"
#include "Mesh.h"

#include "SDL.h"
#include "DirectXTex.h"
#include <GL/glew.h>
#include "MathGeoLib.h"
#include "ImGui\imgui-1.89.9-docking\imgui.h"
#include "ImGui\imgui-1.89.9-docking\imgui_impl_opengl3.h"
#include "ImGui\imgui-1.89.9-docking\imgui_impl_sdl2.h"

ModuleEditor::ModuleEditor()
{
	logs = new ImGuiTextBuffer;
}

// Destructor
ModuleEditor::~ModuleEditor()
{
	delete logs;
	logs = nullptr;
}

// Called before render is available
bool ModuleEditor::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->GetWindow()->GetWindow(), App->GetOpenGL()->context);
    ImGui_ImplOpenGL3_Init("#version 460");

	cameraSpeed = App->GetCamera()->GetSpeed();
	mouseSensitivity = App->GetCamera()->GetSensitivity();
	zoomSpeed = App->GetCamera()->GetZoomSpeed();

	return true;
}

update_status ModuleEditor::PreUpdate()
{
    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->GetWindow()->GetWindow());
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();
		
	
	static bool show_main_menu = true;
	if (show_main_menu)
	{
		bool ret = ShowMainMenu(show_main_menu);
		if (ret == true) return UPDATE_STOP;
	}

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    return UPDATE_CONTINUE;
}

bool ModuleEditor::ShowMainMenu(bool& show_main_menu)
{
	bool exit = false;
	static bool show_logs = true;
	ImGui::Begin("Main Menu", &show_main_menu, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("About"))
		{
			ShowAbout();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GitHub"))
		{
			ImGui::SeparatorText("Find the repository here!");
			if (ImGui::Button("Open on browser"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/Oscar-Pellice-Mas/Engine", NULL, NULL, 0);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Logs")) 
		{
			if (show_logs)
			{
				// Open the Logs window when show_logs is true
				ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
				ImGui::Begin("Logs", &show_logs);

				// Logs content
				ImGui::Separator();
				ImGui::TextUnformatted(logs->begin(), logs->end());

				// Ensure the logs window is scrolled to the bottom
				if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
					ImGui::SetScrollHereY(1.0f);

				// Close button to hide logs
				if (ImGui::Button("CLOSE"))
				{
					show_logs = false;
					ImGui::End();
				}

				ImGui::End();
			}

			ImGui::EndMenu();
			
		}
		if (ImGui::BeginMenu("Quit"))
		{
			ImGui::EndMenu();
			exit = true;
		}

		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("Help"))
	{
		ShowMovementHelp();
	}
	
	if (ImGui::CollapsingHeader("Application"))
	{
		ShowApplication();
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		ShowWindow();
	}

	if (ImGui::CollapsingHeader("Camera")) 
	{
		ShowCamera();
	}

	if (ImGui::CollapsingHeader("Properties"))
	{
		ShowProperties();
	}

	if (ImGui::CollapsingHeader("Software"))
	{
		ShowSoftware();
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ShowHardware();
	}

	ImGui::End();
	return exit;
}

void ModuleEditor::ShowMovementHelp()
{
	ImGui::Text("Movement Controls:");
	ImGui::Text("W / S: Move forward / backward");
	ImGui::Text("A / D: Move left / right");
	ImGui::Text("Q / E: Move up / down");
	ImGui::Text("Arrow Keys: Rotate camera");
	ImGui::Text("Right Mouse Button: Drag to rotate camera");
	ImGui::Text("Hold F: Activate Focus mode. Mouse motion follows orbital.");
	ImGui::Text("Hold Shift: Increase movement speed");
	ImGui::Text("Hold Ctrl: Decrease movement speed");
	ImGui::Text("Scroll Wheel: Zoom in / out");
}

void ModuleEditor::ShowHardware()
{
	GLfloat total_ram, available_ram;
	glGetFloatv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_ram);
	glGetFloatv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_ram);

	// Additional Information
	ImGui::Text("Number of CPU Cores: %i", SDL_GetCPUCount());

	// Memory Information
	ImGui::Text("Total RAM: %.1fGB", total_ram * 1.0f);
	ImGui::Text("Available Physical Memory: %.1fGB", SDL_GetSystemRAM() * 1.0f);
	ImGui::Text("Memory Usage: %.2f%%", (total_ram - available_ram) / total_ram * 100.0f);
	ImGui::Separator();

	// GPU Information
	const char* gpuVendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	const char* gpuRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

	ImGui::Text("GPU Vendor: %s", gpuVendor);
	ImGui::Text("GPU Renderer: %s", gpuRenderer);

}

void ModuleEditor::ShowSoftware()
{
	SDL_version version;
	SDL_VERSION(&version);

	ImGui::Text("SDL Version: %u.%u.%u", version.major, version.minor);
	ImGui::Text("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	ImGui::Text("OpenGL Supported Version: %s", glGetString(GL_VERSION));
	ImGui::Text("Glew Version: %s", glewGetString(GLEW_VERSION));
	ImGui::Text("ImGui Version: %s", ImGui::GetVersion());
	ImGui::Text("DirectXTex Version: %u", DIRECTX_TEX_VERSION);
}

void ModuleEditor::ShowProperties()
{
	if (ImGui::TreeNode("Geometry"))
	{
		const std::vector<Mesh*>* meshes = App->GetExercice()->GetModel()->GetMeshes();
		ImGui::Text("Model name: %s", App->GetExercice()->GetModel()->GetName()->c_str());
		for (int i = 0; i < meshes->size(); i++) {
			ImGui::Separator();
			ImGui::Text("Mesh name: %s", meshes->at(i)->GetName()->c_str());
			ImGui::Text("Indices: %i", meshes->at(i)->GetIndexCount());
			ImGui::SameLine();
			ImGui::Text("Vertices: %i", meshes->at(i)->GetVertexCount());
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Textures"))
	{

		const tinygltf::Model* model = App->GetExercice()->GetModel()->GetSrcModel();
		std::vector<DirectX::ScratchImage*> images = App->GetExercice()->GetModel()->GetScrImages();

		if (!images.empty()) {
			for (int i = 0; i < images.size(); i++) {
				ImGui::Separator();
				int imageSize = images[i]->GetMetadata().width * images[i]->GetMetadata().height * 
					DirectX::BitsPerPixel(images[i]->GetMetadata().format) / 8000000;
				
				ImGui::Text("Size: %i MB", imageSize);
				ImGui::Text("Width: %i", images[i]->GetMetadata().width);
				ImGui::SameLine();
				ImGui::Text("Height: %i", images[i]->GetMetadata().height);
				
			}
		}
		
		ImGui::TreePop();
	}
}

void ModuleEditor::ShowCamera()
{
	if (ImGui::InputFloat("Camera Speed", &cameraSpeed, 0.05f, 1.0f)) {
		App->GetCamera()->SetSpeed(cameraSpeed);
	}
	if (ImGui::InputFloat("Camera Sensitivity", &mouseSensitivity, 0.01f, 0.1f)) {
		App->GetCamera()->SetSensitivity(mouseSensitivity);
	}
	if (ImGui::InputFloat("Zoom Speed", &zoomSpeed, 0.01f, 0.1f)) {
		App->GetCamera()->SetZoomSpeed(zoomSpeed);
	}
}

void ModuleEditor::ShowWindow()
{
	if (resizable) {
		width = App->GetWindow()->GetScreenWidth();
		if (ImGui::SliderInt("Width", &width, 1, 1920))
		{
			SDL_SetWindowSize(App->GetWindow()->GetWindow(), width, height);
			App->GetWindow()->SetScreenSize(width, height);
		}

		height = App->GetWindow()->GetScreenHeight();
		if (ImGui::SliderInt("Height", &height, 1, 1080))
		{
			SDL_SetWindowSize(App->GetWindow()->GetWindow(), width, height);
			App->GetWindow()->SetScreenSize(width, height);

		}
	}
	
	if (ImGui::Checkbox("Fullscreen", &fullScreen))
	{
		App->GetWindow()->SetFullScreen(fullScreen);

	}

	if (ImGui::Checkbox("Resizable", &resizable))
	{
		App->GetWindow()->SetResizable(resizable);
	}

}

void ModuleEditor::ShowApplication()
{
	const std::vector<float>& frameRates = *(App->GetFrameRate());
	if (!frameRates.empty()) {
		const float fps = frameRates.back();

		char title[50];
		sprintf_s(title, 50, "Framerate: %.1f FPS", fps);

		ImGui::PlotHistogram("##FrameRate", frameRates.data(), frameRates.size(), 0, title, 0.0f, 100.0f, ImVec2(310.0f, 100.0f));
	}

	const std::vector<float>& milliseconds = *(App->GetMilliseconds());
	if (!milliseconds.empty()) {
		const float ms = milliseconds.back();

		char title[50];
		sprintf_s(title, 50, "Milliseconds: %.1f ms", ms);

		ImGui::PlotHistogram("##Milliseconds", milliseconds.data(), milliseconds.size(), 0, title, 0.0f, 40.0f, ImVec2(310.0f, 100.0f));
	}
}

void ModuleEditor::ShowAbout()
{
	ImGui::Text("UPC Game Engine");
	ImGui::Text("Description: Game Engine created for UPC project");
	ImGui::Text("Author: Oscar Pellicé Mas");
	ImGui::Text("Libraries: ");
	ImGui::Text("License: MIT");
}

void ModuleEditor::AddLog(const char* str)
{ 
	if (logs!= nullptr) logs->appendf(str); 
}

update_status ModuleEditor::PostUpdate()
{   
    return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return true;
}
