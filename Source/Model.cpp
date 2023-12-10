#include "Model.h"

#include "Application.h"
#include "ModuleTexture.h"
#include "Mesh.h"
#include <.\GL\glew.h>
#include "DirectXTex.h"

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

Model::Model()
{
	srcModel = new tinygltf::Model;
}

Model::~Model()
{
	for (int i = 0; i < meshes.size(); i++) delete meshes[i];
	meshes.clear();

	for (int i = 0; i < textures.size(); i++) glDeleteTextures(1, &textures[i]);
	textures.clear();
}

void Model::Update() {
	for (int i = 0; i < meshes.size(); ++i) {
		meshes[i]->Draw(textures);
	}
}



void Model::Load(const char* assetFileName)
{
	std::string filePath = "";
	filePath = assetFileName;
	size_t pos = filePath.rfind('/');
	if (pos != std::string::npos) {
		name = filePath.substr(pos + 1);
		filePath.erase(pos + 1, filePath.size() - 1);
	}
	else {
		pos = filePath.rfind('\\');
		if (pos != std::string::npos) {
			name = filePath.substr(pos + 1);
			filePath.erase(pos + 1, filePath.size() - 1);
		}
	}

	tinygltf::TinyGLTF gltfContext;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(srcModel, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
		return;
	}
	
	LoadMaterials(*srcModel, filePath);
	for (const auto& srcMesh : srcModel->meshes)
	{
		for (const auto& primitive : srcMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->SetMaterial(primitive.material);
			mesh->LoadVBO(*srcModel, srcMesh, primitive);
			mesh->LoadEBO(*srcModel, srcMesh, primitive);
			mesh->CreateVAO();
			meshes.push_back(mesh);
		}
	}
}

void Model::LoadMaterials(const tinygltf::Model& srcModel, std::string filePath)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];

			std::string path = filePath + image.uri;

			DirectX::ScratchImage* scrImage = new DirectX::ScratchImage();
			App->GetTexture()->LoadTextureData(*scrImage, path);
			textureId = App->GetTexture()->LoadTexture(scrImage);
			scrImages.push_back(scrImage);
		}
		textures.push_back(textureId);
	}
}

void Model::Clear() {

	for (int i = 0; i < textures.size(); i++) glDeleteTextures(1, &textures[i]);
	textures.clear();

	delete srcModel;
	srcModel = new tinygltf::Model();

	for (int i = 0; i < scrImages.size(); i++) {
		delete scrImages[i];
	}
	scrImages.clear();

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i]->DestroyBuffers();
		delete meshes[i];
	}
	meshes.clear();
}