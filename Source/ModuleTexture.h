#pragma once
#ifndef _MODULE_TEXTURE_H_
#define _MODULE_TEXTURE_H_

#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h" //only for string
#include "DirectXTex.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	
	bool LoadTexture(const std::string& fileDir);
	unsigned GetTexture();

private:

	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	unsigned texture = 0;

	int format = 0;
	int internalFormat = 0;
	int type = 0;

	void GetImageFormat();
	bool LoadTextureData(const std::string& fileDir);

};
#endif /*_MODULE_TEXTURE_H_*/