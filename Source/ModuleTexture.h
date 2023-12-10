#ifndef _MODULE_TEXTURE_H_
#define _MODULE_TEXTURE_H_

#include "Module.h"
#include "Globals.h"

#include "MathGeoLib.h" //only for string
//#include "DirectXTex.h"

namespace DirectX
{
	class ScratchImage;
}

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
	
	unsigned LoadTexture(DirectX::ScratchImage* img);
	bool LoadTextureData(DirectX::ScratchImage& image, const std::string& fileDir);
private:

};
#endif /*_MODULE_TEXTURE_H_*/