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

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
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
			
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
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


	Entity Scene::GetPrimaryCameraEntity()
	{
		// this
		auto view = m_Registry.view<CameraComponent>();
		for (entt::entity entityID : view)
		{
			const CameraComponent& cameraComponent = view.get<CameraComponent>(entityID);
			if (cameraComponent.Primary == true)
				return Entity{entityID, this};
		}
		
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		CR_CORE_ASSERT(false, "Component not found");
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{}
}
	