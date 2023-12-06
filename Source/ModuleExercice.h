#pragma once
#ifndef _MODULE_EXERCICE_H_
#define _MODULE_EXERCICE_H_

#include "Module.h"
#include "Globals.h"


class ModuleExercice : public Module
{
public:
	ModuleExercice();
	~ModuleExercice();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();


private:
	unsigned vbo_id = 0;
	unsigned texture = 0;
};
#endif /*_MODULE_EXERCICE_H_*/