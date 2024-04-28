#pragma once

#include "Crane/Core/Log.h"
#include "Crane/Scene/Scene.h"

#include "Crane/Scene/Entity.h"

namespace Crane
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& Scene);

		Entity GetSelectedEntity() const { return m_SelectedEntity; }
		void SetSelectedEntity(uint32_t entityID) { m_SelectedEntity = { (entt::entity)entityID, m_Scene.get() }; }

		void SetScene(const Ref<Scene>& Scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedEntity;
	};
	
}