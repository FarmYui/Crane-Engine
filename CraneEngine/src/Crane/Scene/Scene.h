#pragma once

#include "entt.hpp"

#include "Crane/Core/Timestep.h"

namespace Crane
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		Entity CreateEntity(const std::string& name);

		void OnUpdate(Timestep ts);
	
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
	
}