#pragma once
#include "../abstract_texture2d.h"
#include "../../sampler2d/es2/sampler2d.h"

namespace Rendy
{
	namespace ES2
	{
		class Texture2D final: public AbstractTexture2D
		{
		public:
			Texture2D(Image2DRef image);
			~Texture2D();
			virtual void reload() override;
			virtual bool validate() const override;
			virtual void bind(uint32_t slot) override;
			virtual void unbind(uint32_t slot) override;	
			virtual void set_min_filter(MinFilter filter) override;
			virtual void set_mag_filter(MagFilter filter) override;
			virtual void set_wrap_mode(WrapMode mode) override;
			virtual bool uses_transparency() const override;
			virtual MinFilter get_min_filter() const override;
			virtual MagFilter get_mag_filter() const override;
			virtual WrapMode get_wrap_mode() const override;
			virtual AbstractSampler2DRef get_sampler() override;
			virtual glm::uvec2 get_size() const override;
			//virtual TextureFormat get_format() const override;
			//virtual TextureType get_type() const override;
		private:
			bool load_from_image();
			TextureFormat parse_format(int c) const;
			void reset();

			Image2DRef image;
			AbstractSampler2DRef sampler;
			TextureFormat format = TextureFormat::Invalid;
			TextureType type = TextureType::Invalid;
			uint32_t id = 0;
		};
	};
};