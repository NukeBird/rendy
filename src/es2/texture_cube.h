#pragma once
#include "../abstract_texture_cube.h"
#include <gli/gli.hpp>
#include <string>

namespace Rendy
{
	namespace ES2
	{
		class TextureCube final : public AbstractTextureCube
		{
		public:
			TextureCube(const std::string& filename);
			TextureCube(const void* memory, uint32_t size);
			~TextureCube();
			virtual void reload() override;
			virtual bool validate() const override;
			virtual void bind(uint32_t slot) override;
			virtual void unbind(uint32_t slot) override;
			virtual uint32_t get_max_level() const override;
			virtual glm::uvec3 get_size() const override;
			//virtual TextureFormat get_format() const override;
			virtual TextureType get_type() const override;
		private:
			bool load();
			void reset();
			void process();

			gli::texture gli_tex;
			uint32_t id = 0;
		};
	};
};