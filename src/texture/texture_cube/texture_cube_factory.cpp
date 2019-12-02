#include "texture_cube_factory.h"
#include "texture_cube_es2.h"
#include "texture_cube_es3.h"

Rendy::TextureCubeFactory::TextureCubeFactory(OGL version)
{
	this->version = version;
}

Rendy::AbstractTextureCubeRef Rendy::TextureCubeFactory::make(const std::string& filename)
{
	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::TextureCube>(filename);
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::TextureCube>(filename);
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::TextureCube>(filename);
}

Rendy::AbstractTextureCubeRef Rendy::TextureCubeFactory::make(const void* memory, 
	uint32_t size)
{
	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::TextureCube>(memory, size);
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::TextureCube>(memory, size);
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::TextureCube>(memory, size);
}