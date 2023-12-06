#ifndef _MODEL_H_
#define _MODEL_H_

#include "Globals.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

class Mesh;

class Model
{
public:
	Model();
	~Model();

	void Update();
	void Load(const char* assetFileName);
	void LoadMaterials(const tinygltf::Model& srcModel);
private:
	std::vector<Mesh*> meshes;
	std::vector<unsigned int> textures;
};

#endif /*_MODEL_H_*/