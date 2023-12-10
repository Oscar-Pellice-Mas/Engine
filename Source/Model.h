#ifndef _MODEL_H_
#define _MODEL_H_

#include "Globals.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include "Geometry/AABB.h"

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

	void Update();
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel, std::string path);
	void Clear();

	const tinygltf::Model* GetSrcModel() const { return srcModel; }
	const std::vector<Mesh*>* GetMeshes() const { return &meshes; }
	const std::vector<DirectX::ScratchImage*> GetScrImages() const { return scrImages; }
	const AABB* GetAABB() const { return modelAABB; }
	inline const std::string* GetName() const { return &name; }

private:
	tinygltf::Model* srcModel = nullptr;
	std::vector<DirectX::ScratchImage*> scrImages;
	std::vector<Mesh*> meshes;
	std::vector<unsigned int> textures;
	AABB* modelAABB;
	std::string name = "";
};

#endif /*_MODEL_H_*/