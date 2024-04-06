#include <crpch.h>
#include "TextureRegion2D.h"

namespace Crane
{

	TextureRegion2D::TextureRegion2D(const glm::vec2& min, const glm::vec2& max)
	{
		m_TextureCoordinates[0] = {min.x, min.y};
		m_TextureCoordinates[1] = {max.x, min.y};
		m_TextureCoordinates[2] = {max.x, max.y};
		m_TextureCoordinates[3] = {min.x, max.y};
	}

	TextureRegion2D& TextureRegion2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2 coords, const glm::vec2 cellSize, const glm::vec2 spriteUnitSize)
	{
		glm::vec2 min, max;
		min.x = coords.x * cellSize.x / (float)texture->GetWidth();
		min.y = coords.y * cellSize.y / (float)texture->GetHeight();
		
		max.x = (coords.x + spriteUnitSize.x) * cellSize.x / (float)texture->GetWidth();
		max.y = (coords.y + spriteUnitSize.y) * cellSize.y / (float)texture->GetHeight();

		return TextureRegion2D(min, max);
	}
}