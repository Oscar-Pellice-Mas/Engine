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


ModuleExercice::ModuleExercice()
{
	model = new Model();

}

ModuleExercice::~ModuleExercice()
{
	delete model;
}



// Called before render is available
bool ModuleExercice::Init()
{
	//vbo_id = CreateTriangleVBO();
	//texture = App->GetTexture()->LoadTexture("Textures/Baboon.ppm");
	
	unsigned program = App->GetProgram()->program;
	glUseProgram(program);



	model->Load("Models/BakerHouse/BakerHouse.gltf");

	return true;
}

update_status ModuleExercice::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleExercice::Update()
{
	float4x4 model_matrix, view_matrix, proj_matrix;
	//model_matrix = float4x4::FromTRS(float3::zero, float4x4::RotateZ(0), float3::one);
	model_matrix = float4x4::FromTRS(float3::zero, float4x4::RotateZ(0), float3(40,40,40));
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

// Called before quitting
bool ModuleExercice::CleanUp()
{
	return true;
}



/*
unsigned CreateTriangleVBO();
void RenderVBO(unsigned vbo, unsigned program, unsigned texture);
void DestroyVBO(unsigned vbo);
// This function must be called one time at creation of vertex buffer
unsigned CreateTriangleVBO()
{
	//float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	float buffer_data[] = {
		-1.0f, -1.0f, 0.0f, // v0 pos
		 1.0f, -1.0f, 0.0f, // v1 pos
		-1.0f,  1.0f, 0.0f, // v2 pos
		 -1.0f, 1.0f, 0.0f, // v3 pos
		 1.0f,  -1.0f, 0.0f, // v4 pos
		 1.0f,  1.0f, 0.0f, // v5 pos

		 0.0f,  0.0f, // v0 texcoord
		 1.0f,  0.0f, // v1 texcoord
		 0.0f,  1.0f, // v2 texcoord
		 0.0f,  1.0f, // v3 texcoord
		 1.0f,  0.0f, // v4 texcoord
		 1.0f,  1.0f  // v5 texcoord
	};

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);

	return vbo;
}

// This function must be called each frame for drawing the triangle
void RenderVBO(unsigned vbo, unsigned program, unsigned texture)
{
	if (texture == -1) return;

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active 

	// Attribute pointer for position (3 floats per vertex)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Attribute pointer for texture coordinates (2 floats per vertex)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3 * 2));

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glUniform1i(glGetUniformLocation(program, "mytexture"), 0);


	// 1 triangle to draw = 3 vertices 
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// This function must be called one time at destruction of vertex buffer
void DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}
*/
