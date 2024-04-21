#include "crpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <glm/glm.hpp>

#include <fstream>

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& vec3)
		{
			Node node;
			node.push_back(vec3.x);
			node.push_back(vec3.y);
			node.push_back(vec3.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& vec3)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			vec3.x = node[0].as<float>();
			vec3.y = node[1].as<float>();
			vec3.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& vec4)
		{
			Node node;
			node.push_back(vec4.x);
			node.push_back(vec4.y);
			node.push_back(vec4.z);
			node.push_back(vec4.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& vec4)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			vec4.x = node[0].as<float>();
			vec4.y = node[1].as<float>();
			vec4.z = node[2].as<float>();
			vec4.w = node[3].as<float>();

			return true;
		}
	};
}


namespace Crane
{

	YAML::Emitter& operator<<(YAML::Emitter& out,const glm::vec3& vec3)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec3.x << vec3.y << vec3.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vec4.x << vec4.y << vec4.z << vec4.w << YAML::EndSeq;
		return out;
	}

	template<typename T, typename F>
	static void SerializeComponent(Entity entity,F func)
	{
		if (entity.HasComponent<T>())
		{
			T& component = entity.GetComponent<T>();
			func(component);
		}
	}

	template<typename T, typename F>
	static void DeserializeComponent(Entity entity, F func)
	{
		if (entity.HasComponent<T>())
		{
			T& component = entity.GetComponent<T>();
			func(component);
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // entity global unique id goes here, once we have it ofc

		SerializeComponent<TagComponent>(entity, [&](TagComponent& tagComponent)
			{
				out << YAML::Key << "TagComponent";
				out << YAML::BeginMap; // TagComponent

				out << YAML::Key << "Tag" << YAML::Value << tagComponent.Tag;

				out << YAML::EndMap; // TagComponent
			});

		SerializeComponent<TransformComponent>(entity, [&](TransformComponent& transformComponent)
			{
				out << YAML::Key << "TransformComponent";
				out << YAML::BeginMap; // TransformComponent

				out << YAML::Key << "Translation" << YAML::Value << transformComponent.Translation;
				out << YAML::Key << "Rotation"    << YAML::Value << transformComponent.Rotation;
				out << YAML::Key << "Scale"       << YAML::Value << transformComponent.Scale;

				out << YAML::EndMap; // TransformComponent
			});

		SerializeComponent<SpriteRendererComponent>(entity, [&](SpriteRendererComponent& spriteRendererComponent) 
			{
				out << YAML::Key << "SpriteRendererComponent";
				out << YAML::BeginMap; // SpriteRendererComponent

				out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

				out << YAML::EndMap; // SpriteRendererComponent
			});

		SerializeComponent<CameraComponent>(entity, [&](CameraComponent& cameraComponent)
			{
				SceneCamera& camera = cameraComponent.Camera;

				out << YAML::Key << "CameraComponent";
				out << YAML::BeginMap; // CameraComponent

				out << YAML::Key << "Camera" << YAML::Value;
				out << YAML::BeginMap; // Camera
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
				out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
				out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
				out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
				out << YAML::EndMap; // Camera

				out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
				out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

				out << YAML::EndMap; // CameraComponent
			});


		out << YAML::EndMap; // Entity
	}


	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		// go thru every entity
		for (entt::entity entityID : m_Scene->m_Registry.view<entt::entity>())
		{
			Entity entity{ entityID, m_Scene.get() };
			
			/* 
			huh ?
			if (!entity)
				return;
			this is not needed, i think we only get valid entities
			*/

			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		CR_CORE_ASSERT(false, "Serialization type not supported");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CR_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		YAML::Node entitiesNode = data["Entities"];
		if (entitiesNode)
		{
			for (auto entityNode : entitiesNode)
			{
				uint64_t uuid = entityNode["Entity"].as<uint64_t>(); // TODOS

				std::string name;
				YAML::Node tagComponentNode = entityNode["TagComponent"];
				if (tagComponentNode)
					name = tagComponentNode["Tag"].as<std::string>();

				CR_CORE_TRACE("Deserializing Entity with ID: {0}, Name: {1}", uuid, name);
				
				Entity deserializedEntity = m_Scene->CreateEntity(name);

				YAML::Node transformComponentNode = entityNode["TransformComponent"];
				if (transformComponentNode)
				{
					//Since entities get created with a TransformComponent by default we use GetComponent method
					TransformComponent& transformComponent = deserializedEntity.GetComponent<TransformComponent>();
					
					transformComponent.Translation = transformComponentNode["Translation"].as<glm::vec3>();
					transformComponent.Rotation    = transformComponentNode["Rotation"].as<glm::vec3>();
					transformComponent.Scale       = transformComponentNode["Scale"].as<glm::vec3>();
				}

				YAML::Node spriteRendererComponentNode = entityNode["SpriteRendererComponent"];
				if (spriteRendererComponentNode)
				{
					SpriteRendererComponent& spriteRendererComponent = deserializedEntity.AddComponent<SpriteRendererComponent>();

					spriteRendererComponent.Color = spriteRendererComponentNode["Color"].as<glm::vec4>();
				}

				YAML::Node cameraComponentNode = entityNode["CameraComponent"];
				if (cameraComponentNode)
				{
					YAML::Node cameraNode = cameraComponentNode["Camera"];

					CameraComponent& cameraComponent = deserializedEntity.AddComponent<CameraComponent>();
					SceneCamera& camera = cameraComponent.Camera;
					
					camera.SetProjectionType((SceneCamera::ProjectionType)cameraNode["ProjectionType"].as<int>());

					camera.SetPerspectiveVerticalFOV(cameraNode["PerspectiveFOV"].as<float>());
					camera.SetPerspectiveNearClip(cameraNode["PerspectiveNear"].as<float>());
					camera.SetPerspectiveFarClip(cameraNode["PerspectiveFar"].as<float>());

					camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
					camera.SetOrthographicNearClip(cameraNode["OrthographicNear"].as<float>());
					camera.SetOrthographicFarClip(cameraNode["OrthographicFar"].as<float>());

					cameraComponent.Primary = cameraComponentNode["Primary"].as<bool>();
					cameraComponent.FixedAspectRatio = cameraComponentNode["FixedAspectRatio"].as<bool>();

				}
				
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		CR_CORE_ASSERT(false, "Deserialization type not supported");
		return false;
	}


}