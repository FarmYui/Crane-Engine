#include "crpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Crane/Renderer/Renderer2D.h"


namespace Crane
{
	Entity Scene::CreateEntity(const std::string& name = "")
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();

		TagComponent& tag = entity.AddComponent<TagComponent>(name);
		tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.GetID());
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](const entt::entity entity, NativeScriptComponent& nativeScriptComponent)
				{
					// move this to scene::play
					if (!nativeScriptComponent.Instance)
					{
						nativeScriptComponent.Instance = nativeScriptComponent.InstantiateScript();
						nativeScriptComponent.Instance->m_Entity = { entity, this };
						nativeScriptComponent.Instance->OnCreate();
					}

					nativeScriptComponent.Instance->OnUpdate(ts);
					
				});
		}


		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent,CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			
				Renderer2D::DrawQuad(transform.GetTransform(), {sprite.Color.r, sprite.Color.g, sprite.Color.b}, sprite.Color.a);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
			
		}
	}



}
	