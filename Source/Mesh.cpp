#include "Mesh.h"

#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

#include "SDL.h"
#include <GL/glew.h>
#include "MathGeoLib.h"

Mesh::Mesh()
{

}

// Destructor
Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	size_t stride = 0;
	int vertexSize = 0;

	const auto& itPos = primitive.attributes.find("POSITION");
	const unsigned char* bufferPos = nullptr;
	
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		vertexCount = posAcc.count;
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
		stride = posView.byteStride;
		vertexSize += 3;
	}

	const auto& itNorm = primitive.attributes.find("NORMAL");
	const unsigned char* bufferNorm = nullptr;

	if (itNorm != primitive.attributes.end())
	{
		const tinygltf::Accessor& normAcc = model.accessors[itNorm->second];
		SDL_assert(normAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(normAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& normView = model.bufferViews[normAcc.bufferView];
		const tinygltf::Buffer& normBuffer = model.buffers[normView.buffer];
		bufferNorm = &(normBuffer.data[normAcc.byteOffset + normView.byteOffset]);
		vertexSize += 3;
	}

	const auto& itTex = primitive.attributes.find("TEXCOORD_0");
	const unsigned char* bufferTex = nullptr;

	if (itTex != primitive.attributes.end())
	{
		const tinygltf::Accessor& texAcc = model.accessors[itTex->second];
		SDL_assert(texAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
		const tinygltf::Buffer& texBuffer = model.buffers[texView.buffer];
		bufferTex = &(texBuffer.data[texAcc.byteOffset + texView.byteOffset]);
		vertexSize += 2;
	}
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize * indexCount, nullptr, GL_STATIC_DRAW);
		
	// TODO
	float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (size_t i = 0; i < vertexCount; ++i)
	{
		if (bufferPos != nullptr) {
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += stride;
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		indexCount = indAcc.count;
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);

		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));

	glBindVertexArray(0);
}

void Mesh::Draw(const std::vector<unsigned>& textures)
{
	program = App->GetProgram()->program;
	glUseProgram(program);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures[material]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}