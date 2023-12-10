#ifndef _MESH_H_
#define _MESH_H_

#include "Globals.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include "Geometry/AABB.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void LoadVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void CreateVAO();
	void Draw(const std::vector<unsigned>& textures);
	void DestroyBuffers();

	void SetMaterial(int id) { material = id; }
	inline const AABB* GetAABB() const { return meshAABB; }
	inline const int GetIndexCount() const { return indexCount; }
	inline const int GetVertexCount() const { return vertexCount; }
	inline const std::string* GetName() const { return &name; }

private:
	unsigned ebo = 0;
	unsigned vao = 0;
	unsigned vbo = 0;
	unsigned program = 0;
	unsigned material = 0;

	int indexCount = 0;
	int vertexCount = 0;
	AABB* meshAABB;
	std::string name = "";
};

#endif /*_MESH_H_*/