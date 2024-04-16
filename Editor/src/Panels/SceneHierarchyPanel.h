#pragma once

#include "Crane/Core/Core.h"
#include "Crane/Scene/Scene.h"

#include "Crane/Scene/Entity.h"

namespace Crane
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& Scene);

		void SetScene(const Ref<Scene>& Scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Scene;
		Entity m_SelectedEntity;
	};
}