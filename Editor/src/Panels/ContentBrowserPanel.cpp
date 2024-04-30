#include "ContentBrowserPanel.h"

#include <ImGui/imgui.h>

namespace Crane
{
	static const std::filesystem::path s_AssetsDirectory = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetsDirectory)
	{
	}

	void ContentBrowserPanel::OpenDirectory()
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(filenameString.c_str(), { 100,100 }))
				{
					// '/=' is an overloaded operator to add onto std::filename::path
					m_CurrentDirectory /= path.filename();
				
				}
				
			}
			if (directoryEntry.is_regular_file())
			{
				ImGui::Button(filenameString.c_str(), { 100,100 });
			}

			ImGui::SameLine();
		}

		ImGui::NewLine();

		if (m_CurrentDirectory.has_parent_path() && ImGui::Button("<-", {70,50}))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		// instead of doing this every frame 
		// we might do this only if something in the directory changes or the directory changes
		// and store the new filenames in a vector
		OpenDirectory();


		ImGui::End();
	}
}