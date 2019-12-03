#include "texture2d_factory.h"
#include "texture2d_es2.h"
#include "texture2d_es3.h"
#include <optick.h>

Rendy::Texture2DFactory::Texture2DFactory(OGL version)
{
	OPTICK_EVENT();
	this->version = version;
}

Rendy::AbstractTexture2DRef Rendy::Texture2DFactory::make(Image2DRef image)
{
	OPTICK_EVENT();

	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::Texture2D>(image);
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::Texture2D>(image);
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::Texture2D>(image);
}