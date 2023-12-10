#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_

#include "Module.h"
#include "Globals.h"
#include "Math/float2.h"

struct ImGuiIO;
class ImGuiTextBuffer;

class ModuleEditor : public Module
{
public:
	
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void AddLog(char str[]);

private:
	bool ShowMainMenu(bool& show_main_menu);
	void ShowHardware();
	void ShowSoftware();
	void ShowProperties();
	void ShowCamera();
	void ShowWindow();
	void ShowApplication();
	void ShowAbout();
	void ShowMovementHelp();

	bool fullScreen = false, resizable = true;
	int width = 0, height = 0;
	const ImGuiIO* GetIO() const { return io; };
	float cameraSpeed = 0;
	float mouseSensitivity = 0;
	float zoomSpeed = 0;

	void* context = nullptr;
	ImGuiIO* io = nullptr;
	ImGuiTextBuffer* logs = nullptr;
};

#endif