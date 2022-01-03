#pragma once
class ImGuiManager
{
private:
	bool m_bImguiEnable = true;
public:
	void OnOffImgui();
	bool isImguiEnable();
public:
	void Frame();
	void Render();
public:
	ImGuiManager();
	~ImGuiManager();
};

