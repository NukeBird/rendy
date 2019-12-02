#pragma once
#include "../rendy.h"
#include <vector>

namespace Rendy
{
	namespace ES3
	{
		class Engine final: public AbstractEngine
		{
		public:
			Engine(VFSRef vfs);
			virtual void reload() override;
			virtual GAPIRef get_gapi() const override;
			virtual AbstractMaterialRef make_material(ImageSetRef image_set) override;
		private:
			AbstractTextureCubeRef read_texture_cube(const std::string& path);

			GAPIRef gapi;
			AbstractShaderRef generic_shader;
			AbstractTextureCubeRef iem;
			AbstractTextureCubeRef pmrem;
			AbstractTexture2DRef lut;
			VFSRef vfs;
		};
	};
};