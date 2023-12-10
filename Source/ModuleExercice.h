#pragma once
#ifndef _MODULE_EXERCICE_H_
#define _MODULE_EXERCICE_H_

#include "Globals.h"
#include "Module.h"
#include "Model.h"

class Model;

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


	void ClearModel();
	inline const Model* GetModel() const { return model; }
	void LoadModel(char* file);

private:
	unsigned vbo_id = 0;
	unsigned texture = 0;
	Model* model;
};
#endif /*_MODULE_EXERCICE_H_*/