#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Crane
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entityID, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityID);
		}

		template<typename T, typename...Args>
		T& AddComponent(Args&&... args)
		{
			CR_CORE_ASSERT(!HasComponent<T>(), "Entity alredy has Component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			CR_CORE_ASSERT(HasComponent<T>(), "Entity does not have Component!");
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			CR_CORE_ASSERT(HasComponent<T>(), "Entity does not have Component!");
			return m_Scene->m_Registry.remove<T>(m_EntityID);
		}

		bool operator==(const Entity& other) const 
		{ 
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene; 
		}

		bool operator!=(const Entity& other) const
		{
			return !operator==(other);
		}

		operator bool() { return m_EntityID != entt::null; }

		uint32_t GetID() const { return (uint32_t)m_EntityID; }
	private:
		entt::entity m_EntityID{ entt::null };
		Scene* m_Scene = nullptr;
	};

}