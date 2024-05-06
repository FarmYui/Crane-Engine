#include "crpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Crane/Renderer/Renderer2D.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Crane
{
	static b2BodyType ToBox2DRigidbodyType(RigidBody2DComponent::BodyType type)
	{
		switch (type)
		{
		case Crane::RigidBody2DComponent::BodyType::Static:
			return b2_staticBody;
		case Crane::RigidBody2DComponent::BodyType::Dynamic:
			return b2_dynamicBody;
		case Crane::RigidBody2DComponent::BodyType::Kinematic:
			return b2_kinematicBody;
		default:
			CR_CORE_ASSERT(false, "Unknown Body type");
			return b2_staticBody;
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

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.GetID());
	}

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({0.0f, -9.8f});

		m_Registry.view<RigidBody2DComponent>().each([=](const entt::entity entity, RigidBody2DComponent& rigidBodyComponent) {
			
				Entity e{ entity, this };
			
				TransformComponent& transformComponent = e.GetComponent<TransformComponent>();
				
				b2BodyDef properties;
				properties.type = ToBox2DRigidbodyType(rigidBodyComponent.Type);
				properties.position.Set(transformComponent.Translation.x, transformComponent.Translation.y);
				properties.angle = transformComponent.Rotation.z;
				
				b2Body* body = m_PhysicsWorld->CreateBody(&properties);
				body->SetFixedRotation(rigidBodyComponent.FixedRotation);
				rigidBodyComponent.RuntimeBody = (void*)body;

				if (e.HasComponent<BoxCollider2DComponent>())
				{
					BoxCollider2DComponent& colliderComponent = e.GetComponent<BoxCollider2DComponent>();

					b2PolygonShape boxShape;
					boxShape.SetAsBox(transformComponent.Scale.x * colliderComponent.Size.x, transformComponent.Scale.y * colliderComponent.Size.y);

					b2FixtureDef fixtureDef;
					fixtureDef.shape = &boxShape;
					fixtureDef.density = colliderComponent.Density;
					fixtureDef.friction = colliderComponent.Friction;
					fixtureDef.restitution = colliderComponent.Restitution;
					fixtureDef.restitutionThreshold = colliderComponent.RestitutionThreshold;
					
					body->CreateFixture(&fixtureDef);

					colliderComponent.RuntimeFixture = (void*)&fixtureDef;
				}
			});
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
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

		// Update Physics
		{
			const uint32_t velocityIterations = 6;
			const uint32_t positionIterations = 2;
			
			m_PhysicsWorld->Step(ts.GetSeconds(), velocityIterations, positionIterations);

			// Retreive transform from b2d
			m_Registry.view<RigidBody2DComponent>().each([=](const entt::entity entity, RigidBody2DComponent& rigidBodyComponent)
				{
					Entity e{ entity, this };
					TransformComponent& transformComponent = e.GetComponent<TransformComponent>();

					b2Body* body = (b2Body*)rigidBodyComponent.RuntimeBody;
					
					const b2Vec2& position = body->GetPosition();
					transformComponent.Translation.x = position.x;
					transformComponent.Translation.y = position.y;

					transformComponent.Rotation.z = body->GetAngle();

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
	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{}
	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{}

}
	