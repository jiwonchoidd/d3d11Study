#pragma once
#include <windows.h>
#include "TDevice.h"
class TWindow : public TDevice
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
		const WCHAR* strWindowTitle=L"Default Name", int iWidth=800, int iHeight=600 );
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual bool	GameInit();
	virtual bool	GameRun();
	virtual bool	GameRelease();
	bool	Run();
public:
	TWindow();
};

