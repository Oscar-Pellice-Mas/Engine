#include "ModuleExercice.h"

#include "Application.h"

#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"

#include "SDL.h"
#include <GL/glew.h>
#include "MathGeoLib.h" 


ModuleExercice::ModuleExercice()
{
	model = new Model();
}

ModuleExercice::~ModuleExercice()
{
	delete model;
}

bool ModuleExercice::Init()
{
	unsigned program = App->GetProgram()->program;
	glUseProgram(program);

	//model->Load("./Models/Avocado/Avocado.gltf");
	model->Load("./Models/BakerHouse/BakerHouse.gltf");
	//model->Load("./Models/Duck/Duck.gltf");

	return true;
}

update_status ModuleExercice::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status ModuleExercice::Update()
{
	float4x4 model_matrix, view_matrix, proj_matrix;
	model_matrix = float4x4::FromTRS(float3::zero, float4x4::RotateZ(0), float3::one);
	view_matrix = App->GetCamera()->frustum->ViewMatrix();
	proj_matrix = App->GetCamera()->frustum->ProjectionMatrix();

	App->GetDebugDraw()->Draw(view_matrix, proj_matrix, App->GetWindow()->GetScreenHeight(), App->GetWindow()->GetScreenWidth());

	glUseProgram(App->GetProgram()->program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &model_matrix[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view_matrix[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj_matrix[0][0]);
	
	model->Update();

	return UPDATE_CONTINUE;
}

update_status ModuleExercice::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleExercice::CleanUp()
{
	return true;
}

void ModuleExercice::LoadModel(char* file) 
{
	model->Load(file);
}

void ModuleExercice::ClearModel()
{
	model->Clear();
}
