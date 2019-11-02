#include "texture_cube.h"
#include "../common.h"
#include <optick.h>

Rendy::ES2::TextureCube::TextureCube(const std::string& filename)
{
	OPTICK_EVENT();
	gli_tex = gli::load(filename);
	process();
}

Rendy::ES2::TextureCube::TextureCube(const void* memory, uint32_t size)
{
	OPTICK_EVENT();
	gli_tex = gli::load(reinterpret_cast<const char*>(memory), 
		static_cast<size_t>(size));
	process();
}

Rendy::ES2::TextureCube::~TextureCube()
{
	OPTICK_EVENT();
	reset();
}

void Rendy::ES2::TextureCube::reload()
{
	OPTICK_EVENT();
	if (!validate())
	{
		reset();
		load();
	}
}

bool Rendy::ES2::TextureCube::validate() const
{
	OPTICK_EVENT();
	if (gli_tex.empty())
	{
		return false;
	}

	if (gli_tex.target() != gli::texture::target_type::TARGET_CUBE)
	{
		return false;
	}

	if (!glIsTexture(id))
	{
		return false;
	}

	return true;
}

void Rendy::ES2::TextureCube::bind(uint32_t slot)
{
	OPTICK_EVENT();
	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Rendy::ES2::TextureCube::unbind(uint32_t slot)
{
	OPTICK_EVENT();
	assert(slot < 8);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

uint32_t Rendy::ES2::TextureCube::get_max_level() const
{
	return static_cast<uint32_t>(gli_tex.max_level());
}

glm::uvec3 Rendy::ES2::TextureCube::get_size() const
{
	OPTICK_EVENT();
	const auto t = gli_tex.extent(0);
	return glm::uvec3
	{
		static_cast<uint32_t>(t.x), 
		static_cast<uint32_t>(t.y), 
		static_cast<uint32_t>(t.z)
	};
}

Rendy::TextureFormat Rendy::ES2::TextureCube::get_format() const
{
	OPTICK_EVENT();
	return TextureFormat::RGB; //TODO
}

Rendy::TextureType Rendy::ES2::TextureCube::get_type() const
{
	OPTICK_EVENT();
	return TextureType::UnsignedByte; //TODO
}

bool Rendy::ES2::TextureCube::load()
{
	OPTICK_EVENT();
	if (gli_tex.empty())
	{
		return false;
	}

	if (gli_tex.target() != gli::texture::target_type::TARGET_CUBE)
	{
		return false;
	}

	if (!glIsTexture(id))
	{
		glGenTextures(1, &id);
	}

	gli::gl GL(gli::gl::PROFILE_ES20);
	gli::gl::format const format = GL.translate(gli_tex.format(), gli_tex.swizzles());
	GLenum target = GL.translate(gli_tex.target());

	bind(0);

	glm::ivec3 Extent(gli_tex.extent());
	GLsizei const FaceTotal = static_cast<GLsizei>(gli_tex.layers() * gli_tex.faces());

	for (std::size_t layer = 0; layer < gli_tex.layers(); ++layer)
	{
		for (std::size_t level = 0; level < gli_tex.levels(); ++level)
		{
			for (std::size_t face = 0; face < gli_tex.faces(); ++face)
			{
				GLsizei const LayerGL = static_cast<GLsizei>(layer);
				glm::ivec3 Extent(gli_tex.extent(level));
				target = static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face);

				if (gli::is_compressed(gli_tex.format()))
				{
					glCompressedTexImage2D(
						target, static_cast<GLint>(level),
						format.Internal,
						Extent.x,
						gli_tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
						0,
						static_cast<GLsizei>(gli_tex.size(level)),
						gli_tex.data(layer, face, level));
				}
				else
				{
					glTexImage2D(
						target, static_cast<GLint>(level),
						format.Internal,
						Extent.x,
						gli_tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
						0,
						format.External, format.Type,
						gli_tex.data(layer, face, level));
				}
			}
		}
	}

	unbind(0);

	return true;
}

void Rendy::ES2::TextureCube::reset()
{
	OPTICK_EVENT();
	if (glIsTexture(id))
	{
		glDeleteTextures(1, &id);
	}

	id = 0;
}

void Rendy::ES2::TextureCube::process()
{
	OPTICK_EVENT();
	if (!gli_tex.empty())
	{
		if (gli_tex.target() != gli::texture::target_type::TARGET_CUBE)
		{
			gli_tex.clear();
			return;
		}

		//gli_tex = gli::convert(gli_tex, gli::format::FORMAT_RGB8_UINT_PACK8); //TODO: RGBA?
		load();
	}
}
