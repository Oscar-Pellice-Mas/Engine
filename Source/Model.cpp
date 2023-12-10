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
	model = new tinygltf::Model;
}

Model::~Model()
{
	// Clean up meshes
	for (Mesh* mesh : meshes)
	{
		mesh->DestroyBuffers();
		delete mesh;
	}
	meshes.clear();

	// Delete textures
	for (unsigned int texture : textures)
	{
		glDeleteTextures(1, &texture);
	}
	textures.clear();
	
	// Delete scratch images
	for (DirectX::ScratchImage* image : images)
	{
		delete image;
	}
	images.clear();

	// Delete the source model
	delete model;
}

void Model::Update() 
{
	// Draw all meshes
	for (Mesh* mesh : meshes)
	{
		mesh->Draw(textures);
	}
}

void Model::Load(const char* assetFileName)
{
	std::string filePath = assetFileName;
	size_t pos = filePath.find_last_of('/');
	if (pos != std::string::npos) 
	{
		name = filePath.substr(pos + 1);
		filePath.erase(pos + 1, filePath.size() - 1);
	}
	else 
	{
		pos = filePath.find_last_of('\\');
		if (pos != std::string::npos) 
		{
			name = filePath.substr(pos + 1);
			filePath.erase(pos + 1, filePath.size() - 1);
		}
	}

	// Load glTF model
	tinygltf::TinyGLTF gltfContext;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
		return;
	}
	
	LoadMaterials(*model, filePath);
	for (const auto& mesh : model->meshes)
	{
		for (const auto& primitive : mesh.primitives)
		{
			Mesh* aux = new Mesh;
			aux->SetMaterial(primitive.material);
			aux->LoadVBO(*model, mesh, primitive);
			aux->LoadEBO(*model, mesh, primitive);
			aux->CreateVAO();
			meshes.push_back(aux);
		}
	}
}

void Model::LoadMaterials(const tinygltf::Model& model, std::string filePath)
{
	for (const auto& srcMaterial : model.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = model.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = model.images[texture.source];

			std::string path = filePath + image.uri;
			DirectX::ScratchImage* scrImage = new DirectX::ScratchImage();

			App->GetTexture()->LoadTextureData(*scrImage, path);
			textureId = App->GetTexture()->LoadTexture(scrImage);
			images.push_back(scrImage);
		}
		textures.push_back(textureId);
	}
}

void Model::Clear() {

	// Delete textures
	for (unsigned int texture : textures)
	{
		glDeleteTextures(1, &texture);
	}
	textures.clear();

	// Delete scratch images
	for (DirectX::ScratchImage* image : images)
	{
		delete image;
	}
	images.clear();

	// Clean up meshes
	for (Mesh* mesh : meshes)
	{
		mesh->DestroyBuffers();
		delete mesh;
	}
	meshes.clear();

	// Delete the source model
	delete model;
	model = new tinygltf::Model();
}