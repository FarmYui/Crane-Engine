#pragma once

#include "entt.hpp"

#include "Crane/Core/Timestep.h"

namespace Crane
{
	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		entt::entity CreateEntity();
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
	};
}