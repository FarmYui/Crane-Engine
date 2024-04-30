#pragma once

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
	};
}