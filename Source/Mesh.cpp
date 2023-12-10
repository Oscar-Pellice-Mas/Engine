#include "Mesh.h"

#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

#include "SDL.h"
#include <GL/glew.h>
#include "MathGeoLib.h"
#include "Geometry/AABB.h"

Mesh::Mesh()
{
	meshAABB = new AABB();

}

// Destructor
Mesh::~Mesh()
{
	// Cleanup OpenGL buffers when the Mesh is destroyed
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	delete meshAABB;
}

void Mesh::LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	name = mesh.name;
	// Initialize variables for vertex buffer size and offsets
	size_t bufferSize = 0;
	size_t posByteOffset = 0, posByteStride = 0, texByteOffset = 0, texByteStride = 0;

	// Check if POSITION, TEXCOORD_0, and NORMAL attributes exist in the primitive
	const auto& itPosition = primitive.attributes.find("POSITION");
	const auto& itTexture = primitive.attributes.find("TEXCOORD_0");
	const auto& itNormal = primitive.attributes.find("NORMAL");

	// Calculate the size of the vertex buffer based on the attributes
	if (itPosition != primitive.attributes.end()) {
		bufferSize += sizeof(float) * 3;
	}
	if (itTexture != primitive.attributes.end()) {
		bufferSize += sizeof(float) * 2;
	}
	if (itNormal != primitive.attributes.end()) {
		bufferSize += sizeof(float) * 3;
	}

	// Handle POSITION attribute
	if (itPosition != primitive.attributes.end())
	{
		// Extract information from the glTF model
		const tinygltf::Accessor& posAcc = model.accessors[itPosition->second];
		vertexCount = posAcc.count;

		// Assert that the data type and component type are as expected
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);

		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		posByteOffset = posView.byteOffset;
		posByteStride = posView.byteStride;

		// Generate and bind the vertex buffer
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, bufferSize * vertexCount, nullptr, GL_STATIC_DRAW);

		// Map the buffer and copy data from the glTF model
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		for (size_t i = 0; i < posAcc.count; i++) {
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			if (posView.byteStride != 0) {
				bufferPos += posView.byteStride;
			}
			else {
				bufferPos += sizeof(float) * 3;
			}

		}

		meshAABB->SetFrom(ptr, vertexCount);
		Sphere sphere = meshAABB->MinimalEnclosingSphere();
		Frustum* frustum = App->GetCamera()->frustum;
		float3 pos = sphere.Centroid() + float3(sphere.Diameter());
		App->GetCamera()->SetPosition(pos);
		App->GetCamera()->LookAt(float3::zero);
		App->GetCamera()->SetSpeed(sphere.r);

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	// Handle TEXCOORD_0 attribute
	if (itTexture != primitive.attributes.end()) {
		const tinygltf::Accessor& texCoordAcc = model.accessors[itTexture->second];
		SDL_assert(texCoordAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texCoordAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& texCoordView = model.bufferViews[texCoordAcc.bufferView];
		const tinygltf::Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
		const unsigned char* bufferTexCoord = &(texCoordBuffer.data[texCoordAcc.byteOffset + texCoordView.byteOffset]);

		texByteOffset = texCoordAcc.byteOffset + texCoordView.byteOffset;
		texByteStride = texCoordView.byteStride;

		float2* ptr = reinterpret_cast<float2*>(reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) + sizeof(float) * 3 * vertexCount);

		for (size_t i = 0; i < texCoordAcc.count; i++) {
			ptr[i] = *reinterpret_cast<const float2*>(bufferTexCoord);
			if (texCoordView.byteStride != 0) {
				bufferTexCoord += texCoordView.byteStride;
			}
			else {
				bufferTexCoord += sizeof(float) * 2;
			}

		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}

	// Handle TEXCOORD_0 attribute
	if (itNormal != primitive.attributes.end()) {
		const tinygltf::Accessor& normalAcc = model.accessors[itNormal->second];
		SDL_assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(normalAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& normalView = model.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = model.buffers[normalView.buffer];
		const unsigned char* bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);

		float3* ptr = nullptr;
		if (itTexture != primitive.attributes.end()) {
			ptr = reinterpret_cast<float3*>(reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) + sizeof(float) * 5 * vertexCount);
		}
		else {
			ptr = reinterpret_cast<float3*>(reinterpret_cast<char*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY)) + sizeof(float) * 3 * vertexCount);
		}

		for (size_t i = 0; i < normalAcc.count; i++) {
			ptr[i] = *reinterpret_cast<const float3*>(bufferNormal);
			if (normalView.byteStride != 0) {
				bufferNormal += normalView.byteStride;
			}
			else {
				bufferNormal += sizeof(float) * 3;
			}

		}
		glUnmapBuffer(GL_ARRAY_BUFFER);

	}
}

void Mesh::LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	if (primitive.indices >= 0)
	{
		// Extract information from the glTF model
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		indexCount = indAcc.count;
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);

		// Generate and bind the element buffer
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		// Map indices data to the element buffer
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void Mesh::CreateVAO()
{
	// Generate and bind the Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Bind the vertex buffer and element buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Enable and set up vertex attribute pointers for position, texture coordinates, and normals
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(sizeof(float) * 3 * vertexCount));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)(sizeof(float) * 5 * vertexCount));

	glBindVertexArray(0);
}

void Mesh::Draw(const std::vector<unsigned>& textures)
{
	// Get the shader program from the application
	program = App->GetProgram()->program;
	glUseProgram(program);

	// Cache uniform locations
	static GLint diffuseLocation = glGetUniformLocation(program, "diffuse");

	// Activate a texture unit and bind the texture
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, textures[material]);
	glUniform1i(diffuseLocation, 0);

	// Bind the Vertex Array Object and draw the mesh using the element buffer
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Mesh::DestroyBuffers() {
	glDeleteBuffers(1, &vbo);
	if (indexCount != 0) {
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vao);
	}
}