#include "ModuleWindow.h"
#include "SDL/include/SDL.h"

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;
	Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;

	if(FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, flags);

	if(window == nullptr)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Get window surface
	screen_surface = SDL_GetWindowSurface(window);

	return true;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetFullScreen(bool fullScreen)
{
	SDL_SetWindowFullscreen(window, fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void ModuleWindow::SetResizable(bool resizable)
{
	SDL_SetWindowResizable(window, resizable ? SDL_TRUE : SDL_FALSE);
}

void ModuleWindow::SetScreenSize(int width, int height) {
	screenHeight = height;
	screenWidth = width;
}