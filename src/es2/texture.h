#pragma once
#include "../abstract_texture.h"

namespace ES2
{
	class Texture final: public AbstractTexture
	{
	public:
		Texture(const char* memory, uint32_t length);
		~Texture();
		virtual void reload() override;
		virtual bool validate() const override;
		virtual void bind(uint32_t slot) override;
		virtual void unbind(uint32_t slot) override;
		virtual glm::uvec3 get_size() const override;
		virtual TextureFormat get_format() const override;
		virtual TextureType get_type() const override;
	private:
		bool load_from_memory(const char* memory, uint32_t length);
		TextureFormat parse_format(int c) const;
		void reset();

		void* data = nullptr;
		uint32_t length = 0;
		glm::uvec3 size = { 0, 0, 0 };
		TextureFormat format = TextureFormat::Invalid;
		TextureType type = TextureType::Invalid;
		uint32_t id = 0;
	};
};