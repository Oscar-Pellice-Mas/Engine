#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleExercice.h"
#include "SDL/include/SDL.h"
#include "ImGui\imgui-1.89.9-docking\imgui_impl_sdl2.h"

ModuleInput::ModuleInput()
{
}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                return UPDATE_STOP;

            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->GetWindow()->SetScreenSize(sdlEvent.window.data1, sdlEvent.window.data2);
                if (sdlEvent.window.event == SDL_WINDOWEVENT_CLOSE) {
                    return UPDATE_STOP;
                }
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                App->GetCamera()->HandleKeyboardInput(sdlEvent.key);
                break;

            case SDL_MOUSEMOTION:
                App->GetCamera()->HandleMouseMotion(sdlEvent.motion);
                break;

            case SDL_MOUSEWHEEL:
                App->GetCamera()->HandleMouseWheel(sdlEvent.wheel);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                App->GetCamera()->HandleMouseButton(sdlEvent.button);
                break; 

            case SDL_DROPFILE:
                LOG("FILE DROPPED: %s", sdlEvent.drop.file);
                App->GetExercice()->ClearModel();
                App->GetExercice()->LoadModel(sdlEvent.drop.file);
                break;

            default:
                break;
        }
    }

    keyboard = SDL_GetKeyboardState(NULL);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
