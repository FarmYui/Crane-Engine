#include "crpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Crane/Renderer/Renderer2D.h"


namespace Crane
{
	Scene::Scene()
	{
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


	Entity Scene::CreateEntity(const std::string& name = "")
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		TagComponent& tag = entity.AddComponent<TagComponent>(name);
		tag = name.empty() ? "Entity" : name;

		return entity;
	}

}
	