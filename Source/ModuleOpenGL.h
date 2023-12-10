#ifndef _MODULE_OPENGL_H_
#define _MODULE_OPENGL_H_

#include "Module.h"
#include "Globals.h"

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void* context = nullptr;

private:

};
#endif /*_MODULE_OPENGL_*/