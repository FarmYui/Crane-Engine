#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Crane
{
	class TextureRegion2D
	{
	public:
		TextureRegion2D(const glm::vec2& min, const glm::vec2& max);
		TextureRegion2D() = default;

		const glm::vec2* GetTextureCoordinates() const { return m_TextureCoordinates; }

		static TextureRegion2D& CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2 coords, const glm::vec2 spriteOffset, const glm::vec2 spriteUnitSize = {1,1});


	private:
		glm::vec2 m_TextureCoordinates[4];
	};
}