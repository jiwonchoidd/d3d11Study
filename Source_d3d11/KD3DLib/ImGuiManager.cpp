#include "ImGuiManager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
void ImGuiManager::Frame()
{
    if (m_bImguiEnable)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
}
void ImGuiManager::Render()
{
    if (m_bImguiEnable)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void ImGuiManager::OnOffImgui()
{
    m_bImguiEnable = !m_bImguiEnable;
}

bool ImGuiManager::isImguiEnable()
{
    return m_bImguiEnable;
}
ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
}

ImGuiManager::~ImGuiManager()
{
	ImGui::DestroyContext();
}
