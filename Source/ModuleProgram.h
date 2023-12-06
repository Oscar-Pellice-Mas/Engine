#ifndef _MODULE_PROGRAM_H_
#define _MODULE_PROGRAM_H_

#include "Module.h"
#include "Globals.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	unsigned program = 0;
	
};

#endif /*_MODULE_PROGRAM_H_*/