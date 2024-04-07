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

		const std::array<glm::vec2,4>& GetTextureCoordinates() const { return m_TextureCoordinates; }

		static Ref<TextureRegion2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2 coords, const glm::vec2 spriteOffset, const glm::vec2 spriteUnitSize = { 1,1 });

	private:
		std::array<glm::vec2, 4> m_TextureCoordinates;
	};

}