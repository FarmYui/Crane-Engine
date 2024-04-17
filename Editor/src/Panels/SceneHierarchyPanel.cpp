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
			glm::vec4& color = entity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
		}

	}
}