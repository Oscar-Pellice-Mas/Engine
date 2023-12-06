#include "Model.h"

#include "Application.h"
#include "ModuleTexture.h"
#include "Mesh.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

Model::Model()
{

}

Model::~Model()
{
}

void Model::Update() {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i]->Draw(textures);
	}
}

void Model::Load(const char* assetFileName)
{
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model srcModel;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&srcModel, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
	}
	
	LoadMaterials(srcModel);
	for (const auto& srcMesh : srcModel.meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->LoadVBO(srcModel, srcMesh, primitive);
			mesh->LoadEBO(srcModel, srcMesh, primitive);
			mesh->CreateVAO();
			mesh->SetMaterial(primitive.material);
			meshes.push_back(mesh);
		}
	}
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			textureId = (App->GetTexture()->LoadTexture(image.uri));
		}
		textures.push_back(textureId);
	}
}