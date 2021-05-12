#pragma once
#include <pxl/math/rect.h>
#include <pxl/graphics/texture.h>

namespace pxl
{
	class Subtexture
	{
	public:
		Subtexture();
		Subtexture(const TextureRef& texture);
		Subtexture(const TextureRef& texture, const Rect &rect);
		void set(const Rect& rect);
		TextureRef texture() const;
		Rect rect() const;
		Rect texturecoordinates() const;
	private:
		TextureRef _texture;
		Rect _rect;
	};
}