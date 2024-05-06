#include "crpch.h"
#include "ContentBrowserPanel.h"

#include <ImGui/imgui.h>

namespace Crane
{
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_FolderIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OpenDirectory()
	{
		bool isContentBrowserHovered = ImGui::IsWindowHovered();

		static float thumbnailSize = 100.0f;
		static float padding = 7.0f;
		float cellSize = thumbnailSize + padding;
		float panelSizeWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = (int)(panelSizeWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = path.filename().string();

			bool isDirectory = directoryEntry.is_directory();
			ImTextureID iconID = isDirectory ? (ImTextureID)(uint64_t)m_FolderIcon->GetRendererID() : (ImTextureID)(uint64_t)m_FileIcon->GetRendererID();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.12f ,0.12f ,0.12f ,1.0f });
			ImGui::ImageButton(filenameString.c_str(), iconID, {thumbnailSize, thumbnailSize}, { 0,1 }, { 1,0 });
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", (const void*)itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::SetTooltip("%s", filenameString.c_str());
				
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (isDirectory)
				{
					// '/=' is an overloaded operator to add onto directory
					m_CurrentDirectory /= path.filename();
				}
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		if (m_CurrentDirectory.has_parent_path() && ImGui::ArrowButton("<-", ImGuiDir_Left))
			m_CurrentDirectory = m_CurrentDirectory.parent_path();

		//ImGui::DragFloat("Thumbnail Size", &thumbnailSize, 1.0f, 60.0f, 300.0f);
		//ImGui::DragFloat("Padding", &padding, 1.0f, 5.0f, 50.0f);

		// TODO status directory bar
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