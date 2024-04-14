#include "crpch.h"
#include "Entity.h"

namespace Crane
{
	Entity::Entity(entt::entity entityID, Scene* scene)
		: m_EntityID(entityID), m_Scene(scene)
	{
	}
}
