#include "ModuleExercice.h"

#include "Application.h"

#include "ModuleOpenGL.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"

#include "SDL.h"
#include <GL/glew.h>


ModuleExercice::ModuleExercice()
{

}

ModuleExercice::~ModuleExercice()
{
}

unsigned CreateTriangleVBO();
void RenderVBO(unsigned vbo, unsigned program, unsigned texture);
void DestroyVBO(unsigned vbo);

// Called before render is available
bool ModuleExercice::Init()
{
	//vbo_id = CreateTriangleVBO();
	//texture = App->GetTexture()->LoadTexture("Baboon.ppm");

	model.Load("BackerHouse.gltf");

	return true;
}

update_status ModuleExercice::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleExercice::Update()
{
	App->GetDebugDraw()->Draw(App->GetCamera()->frustum->ViewMatrix(), 
		App->GetCamera()->frustum->ProjectionMatrix(), 
		App->GetOpenGL()->getWidth(), 
		App->GetOpenGL()->getHeight());
	
	//RenderVBO(vbo_id, App->GetProgram()->program, texture);
	unsigned program = App->GetProgram()->program;
	float4x4 modelMatrix = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f), float4x4::RotateZ(0.0f), float3(40.0f, 40.0f, 40.0f));

	glUseProgram(program);
	glUniformMatrix4fv(0, 1, GL_TRUE, &modelMatrix[0][0]);
	model.Update();

	return UPDATE_CONTINUE;
}

update_status ModuleExercice::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleExercice::CleanUp()
{
	DestroyVBO(vbo_id);

	return true;
}

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

