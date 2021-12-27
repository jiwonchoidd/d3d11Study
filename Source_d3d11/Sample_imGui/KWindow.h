#pragma once
#include <windows.h>
#include "KDevice.h"
class KWindow : public KDevice
{
public:
	RECT m_rtWindow;
	RECT m_rtClient;

	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	bool		m_bGameRun;
public:
	bool   InitWindows(HINSTANCE hInstance,
		int nCmdShow,
		const WCHAR* strWindowTitle=L"Untitled", int width = 800, int height = 600);
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool	Run();
	virtual bool	GameInit();
	virtual bool	GameRun();
	virtual bool	GameRelease();
public:
	KWindow();
};

