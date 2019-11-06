#pragma once
#include "../abstract_sampler2d.h"
#include "../common.h"

namespace Rendy
{
	namespace ES2
	{
		class Sampler2D: public AbstractSampler2D
		{
		public:
			virtual void reload() override;
			virtual bool validate() const override;
			virtual void bind(uint32_t slot) override;
			virtual void unbind(uint32_t slot) override;
			virtual void set_min_filter(MinFilter filter) override;
			virtual void set_mag_filter(MagFilter filter) override;
			virtual void set_wrap_mode(WrapMode mode) override;
			virtual MinFilter get_min_filter() const override;
			virtual MagFilter get_mag_filter() const override;
			virtual WrapMode get_wrap_mode() const override;
		private:
			GLenum parse_min_filter(MinFilter filter) const; //TODO
			GLenum parse_mag_filter(MagFilter filter) const; //TODO
			GLenum parse_wrap_mode(WrapMode mode) const; //TODO

			MinFilter min_filter = MinFilter::Linear;
			MagFilter mag_filter = MagFilter::Linear;
			WrapMode wrap_mode = WrapMode::Repeat;
		};
	};
};