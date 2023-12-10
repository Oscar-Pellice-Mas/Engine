#pragma once

#include<list>
#include<vector>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleCamera;
class ModuleTexture;
class ModuleExercice;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleProgram* GetProgram() { return program; }
    ModuleDebugDraw* GetDebugDraw() { return debugDraw; }
    ModuleCamera* GetCamera() { return camera; }
    ModuleTexture* GetTexture() { return texture; }
    ModuleExercice* GetExercice() { return exercice; }
    ModuleEditor* GetEditor() { return editor; }

    const std::vector<float>* const GetFrameRate() { return &frameRate; };
    const std::vector<float>* const GetMilliseconds() { return &milliSeconds; };

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleProgram* program = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;
    ModuleCamera* camera = nullptr;
    ModuleTexture* texture = nullptr;
    ModuleExercice* exercice = nullptr;

    std::list<Module*> modules;

    std::vector<float> frameRate;
    std::vector<float> milliSeconds;
    int vectorIndex = 0;

};

extern Application* App;
