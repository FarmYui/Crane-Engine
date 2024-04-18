#include "SceneHierarchyPanel.h"

#include "Crane/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

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


	static void	DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		
		//x
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);


		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// y
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.8f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.8f,0.15f,1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// z
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.15f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.2f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.15f,0.8f,1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		
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

		DrawComponent<TransformComponent>("Transform", [&]() 
			{
				TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

				DrawVec3Control("Translation", transformComponent.Translation, 0.0f);
				glm::vec3 rotation = glm::degrees(transformComponent.Rotation);
				DrawVec3Control("Rotation",    rotation,    0.0f);
				transformComponent.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale",       transformComponent.Scale,       1.0f);
			});
		

		DrawComponent<SpriteRendererComponent>("Sprite Renderer",[&]()
			{
				SpriteRendererComponent& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRendererComponent.Color));
			});

		DrawComponent<CameraComponent>("Camera", [&]()
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
			});
	}

	template<typename T, typename F>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, F func)
	{
		if (m_SelectedEntity.HasComponent<T>())
		{
			if (ImGui::TreeNodeEx((const void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str()))
			{
				func();
				ImGui::TreePop();
			}
		}
	}
}