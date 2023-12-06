#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

	//SDL_Window

public:
	//The window we'll be rendering to
	struct SDL_Window* window = NULL;

	//The surface contained by the window
	struct SDL_Surface* screen_surface = NULL;
};

#endif // __ModuleWindow_H__