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
	const float GetScreenHeight() const { return screenHeight; }
	const float GetScreenWidth() const { return screenWidth; }
	SDL_Window* GetWindow() const { return window; }
	SDL_Surface* GetSurface() const { return screen_surface; }
	
	// Setters
	void SetFullScreen(bool fullScreen);
	void SetResizable(bool resizable);
	void SetScreenSize(float height, float width);

private:

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	//Screen dimensions
	float screenHeight = 0.0f;
	float screenWidth = 0.0f;
};

#endif // __ModuleWindow_H__