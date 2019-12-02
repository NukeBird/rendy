#pragma once
#include "../rendy.h"
#include <vector>

namespace Rendy
{
	namespace ES2
	{
		class Engine final: public AbstractEngine
		{
		public:
			Engine(VFSRef vfs);
			virtual void reload() override;
			virtual GAPIRef get_gapi() const override;
			virtual AbstractMaterialRef make_material(ImageSetRef image_set) override;
		private:
			GAPIRef gapi;
			AbstractShaderRef generic_shader;
			AbstractTextureCubeRef iem;
			VFSRef vfs;
		};
	};
};