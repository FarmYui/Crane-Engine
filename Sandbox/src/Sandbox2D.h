#pragma once
#include "Crane.h"

namespace Crane
{
	class Tile;

	class Sandbox2D : public Layer
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event& e) override;
		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;

	private:
		Timestep m_Timestep;
		OrthographicCameraController m_CameraController;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_TextureAtlas;
		std::unordered_map<char, Ref<TextureRegion2D>> m_MapTiles;
		uint32_t m_MapWidth, m_MapHeight;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewportSize;
	};
}