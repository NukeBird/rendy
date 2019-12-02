#include "sampler2d_factory.h"
#include "sampler2d_es2.h"
#include "sampler2d_es3.h"

Rendy::Sampler2DFactory::Sampler2DFactory(OGL version)
{
	this->version = version;
}

Rendy::AbstractSampler2DRef Rendy::Sampler2DFactory::make()
{
	switch (version)
	{
		case Rendy::OGL::ES20:
		{
			return std::make_shared<ES2::Sampler2D>();
		}
		case Rendy::OGL::ES31:
		{
			return std::make_shared<ES3::Sampler2D>();
		}
	}

	//TODO: choose best
	return std::make_shared<ES3::Sampler2D>();
}