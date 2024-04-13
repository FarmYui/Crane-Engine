#include "crpch.h"
#include "Scene.h"

#include "Components.h"

#include "Crane/Renderer/Renderer2D.h"

namespace Crane
{
	Scene::Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			
			Renderer2D::DrawQuad(transform, { sprite.Color.r, sprite.Color.g, sprite.Color.b }, sprite.Color.a);
		}
	}
}


	