#ifndef _MODULE_INPUT_H_
#define _MODULE_INPUT_H_

#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;
class Model;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	const Uint8 *keyboard = nullptr;
	const Model* model = nullptr;
};

#endif /*_MODULE_INPUT_H_*/