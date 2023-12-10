#ifndef _MODEL_H_
#define _MODEL_H_

#include "Globals.h"
#include "Geometry/AABB.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

class Mesh;
namespace DirectX
{
	class ScratchImage;
}

class Model
{
public:
	Model();
	~Model();

	// Public member functions
	void Update();
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& model, std::string path);
	void Clear();

	// Accessor functions
	const tinygltf::Model* GetSrcModel() const { return model; }
	const std::vector<Mesh*>* GetMeshes() const { return &meshes; }
	const std::vector<DirectX::ScratchImage*> GetScrImages() const { return images; }
	const std::string* GetName() const { return &name; }
	const AABB* GetAABB() const { return aabb; }

private:
	tinygltf::Model* model = nullptr;
	std::vector<DirectX::ScratchImage*> images;
	std::vector<Mesh*> meshes;
	std::vector<unsigned int> textures;
	std::string name = "";
	AABB* aabb;
};

#endif /*_MODEL_H_*/