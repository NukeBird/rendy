#pragma once
#include "../abstract_texture2d.h"

namespace ES2
{
	class Texture final: public AbstractTexture2D
	{
	public:
		Texture(Image2DRef image);
		~Texture();
		virtual void reload() override;
		virtual bool validate() const override;
		virtual void bind(uint32_t slot) override;
		virtual void unbind(uint32_t slot) override;
		virtual glm::uvec2 get_size() const override;
		virtual TextureFormat get_format() const override;
		virtual TextureType get_type() const override;
	private:
		bool load_from_image();
		TextureFormat parse_format(int c) const;
		void reset();

		Image2DRef image;
		TextureFormat format = TextureFormat::Invalid;
		TextureType type = TextureType::Invalid;
		uint32_t id = 0;
	};
};