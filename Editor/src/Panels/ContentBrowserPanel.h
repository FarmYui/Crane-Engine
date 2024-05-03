#pragma once

#include "Crane/Core/Core.h"
#include "Crane/Renderer/Texture.h"

#include <filesystem>

namespace Crane
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		void OpenDirectory();
	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;


	};
}