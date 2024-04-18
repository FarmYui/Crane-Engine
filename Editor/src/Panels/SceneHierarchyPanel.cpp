#include "SceneHierarchyPanel.h"

#include "Crane/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGui/imgui.h>

#include <entt/entt.hpp>

namespace Crane
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& Scene)
	{
	}

	void SceneHierarchyPanel::SetScene(const Ref<Scene>& Scene)
	{ 
		m_Scene = Scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		// this
		for (entt::entity entityID : m_Scene->m_Registry.view<entt::entity>()) 
		{
			Entity entity{ entityID, m_Scene.get() };
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};
		

		ImGui::End();


		
		ImGui::Begin("Properties");
		
		if (m_SelectedEntity.GetID() != entt::null)
			DrawComponents(m_SelectedEntity);
		

		ImGui::End();
		
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		TagComponent& tagComponent = entity.GetComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetID(), flags, tagComponent.Tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				glm::mat4& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Translation", glm::value_ptr(transform[3]), 0.1f);
			
				ImGui::TreePop();
			}			
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite"))
			{
				glm::vec4& color = entity.GetComponent<SpriteRendererComponent>().Color;
				ImGui::ColorEdit4("Color", glm::value_ptr(color));
				
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
				SceneCamera& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(uint32_t)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (uint32_t i = 0; i < 2; i++)
					{
						bool isSelected = projectionTypeStrings[i] == currentProjectionTypeString;
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
						
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical Fov", &verticalFov, 0.1f, 10.0f, 170.0f))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip, 0.1f))
						camera.SetPerspectiveNearClip(nearClip);

					float farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip, 0.1f))
						camera.SetPerspectiveFarClip(farClip);
				}


				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float size = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &size, 0.1f, 0.5f, 100.0f))
						camera.SetOrthographicSize(size);

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip, 0.1f))
						camera.SetOrthographicNearClip(nearClip);
					
					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip, 0.1f))
						camera.SetOrthographicFarClip(farClip);
	
					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
				}


				ImGui::TreePop();
			}
		}

	}
}