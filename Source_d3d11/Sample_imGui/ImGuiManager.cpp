#include "ImGuiManager.h"
#include "ImGui/imgui.h"
void ImGuiManager::Frame()
{

}

void ImGuiManager::Render()
{
}

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
