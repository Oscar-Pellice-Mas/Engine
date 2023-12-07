#include "ModuleTexture.h"

#include "Globals.h"

#include "DirectXTex.h"
#include <GL/glew.h>

ModuleTexture::ModuleTexture()
{

}

// Destructor
ModuleTexture::~ModuleTexture()
{
}

// Called before render is available
bool ModuleTexture::Init()
{

	return true;
}

update_status ModuleTexture::PreUpdate()
{

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleTexture::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
    if (texture != 0) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }

	return true;
}

void ModuleTexture::GetImageFormat() {
	switch (metadata.format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}
}

bool ModuleTexture::LoadTextureData(const std::string& fileDir) {
    
    std::wstring filename = std::wstring(fileDir.begin(), fileDir.end());
    
    // Try loading DDS
    HRESULT hr = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS_NONE, &metadata, image);

    if (FAILED(hr)) {
        // Try loading TGA if DDS fails
        hr = DirectX::LoadFromTGAFile(filename.c_str(), &metadata, image);

        if (FAILED(hr)) {
            // Try loading using WIC if TGA also fails
            hr = DirectX::LoadFromWICFile(filename.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, image);

            if (FAILED(hr)) {
                LOG("Material convertor error: texture loading failed (%s)", fileDir.c_str());
                return false;
            }
            else {
                LOG("Texture loaded successfully using WIC.");
                return true;
            }
        }
        else {
            LOG("Texture loaded successfully using TGA.");
            return true;
        }
    }
    else {
        LOG("Texture loaded successfully using DDS.");
        return true;
    }
}

bool ModuleTexture::LoadTexture(const std::string& fileDir) {
    bool loaded = LoadTextureData(fileDir);

    if (!loaded) return false;

    GetImageFormat();

    // Flip the image vertically (if needed)
    DirectX::ScratchImage flippedImage;
    DirectX::FlipRotate(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FR_FLIP_VERTICAL, flippedImage);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(flippedImage.GetMetadata().width),
        static_cast<GLsizei>(flippedImage.GetMetadata().height), 0, format, type, flippedImage.GetPixels());

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

GLuint ModuleTexture::GetTexture() {
    return texture;
}