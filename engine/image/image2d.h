#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include "../abstract_resource.h"
#include "../texture/texture_type.h"
#include <gli/gli.hpp>

namespace Rendy
{
	class Image2D final : public AbstractResource
	{
	public:
		Image2D(const char* memory, uint32_t length);
		~Image2D();

		void* get_data();
		glm::uvec2 get_size() const;
		uint32_t get_width() const;
		uint32_t get_height() const;
		TextureType get_type() const;
		uint32_t get_channel_count() const;
		virtual void reload() override;
		virtual bool validate() const override;
		bool uses_transparency() const;
	private:
		void load_image(const char* memory, uint32_t length);
		void analyze_alpha_channel();

		void* data_ptr = nullptr;
		gli::texture gli_tex;
		TextureType type;
		//uint32_t length = 0;
		glm::uvec2 size{ 0, 0 };
		uint32_t channel_count = 0;
		bool has_useful_alpha = false;
	};

	using Image2DRef = std::shared_ptr<Image2D>;
};