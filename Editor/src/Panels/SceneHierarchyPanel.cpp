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

		// or this...
		/*	
		m_Context->m_Registry.view<entt::entity>().each([](auto entity)
			{
				TagComponent tagComponent = m_Context->m_Registry.get<TagComponent>(entity);
				ImGui::Text("%s", tagComponent.Tag.c_str());
			});
		*/

		// they should do the same thing

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		TagComponent& tagComponent = entity.GetComponent<TagComponent>();
		TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity.GetID(), flags, tagComponent.Tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (opened)
		{

			ImGui::DragFloat3("Translation", glm::value_ptr(transformComponent.Transform[3]), 0.1f);
			ImGui::TreePop();
		}

		
	}
}