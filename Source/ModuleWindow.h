#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

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

	//Getters
	int GetScreenHeight() const { return screenHeight; }
	int GetScreenWidth() const { return screenWidth; }
	SDL_Window* GetWindow() const { return window; }
	SDL_Surface* GetSurface() const { return screen_surface; }
	
	// Setters
	void SetFullScreen(bool fullScreen);
	void SetResizable(bool resizable);
	void SetScreenSize(int height, int width);

private:

	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	//Screen dimensions
	int screenHeight = 0;
	int screenWidth = 0;
};

#endif // __ModuleWindow_H__