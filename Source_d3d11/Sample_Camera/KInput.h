#pragma once
#include "KStd.h"
// 키보드, 마우스 ->윈도우API 사용
enum  KeyState
{
	KEY_FREE = 0,
	KEY_UP,
	KEY_PUSH,
	KEY_HOLD,
};
class KInput : public Singleton<KInput>
{
	friend class Singleton<KInput>;
private:
	DWORD	m_dwKeyState[256];
	POINT   m_ptPos;
	int		m_iWheel = 0;
public:
	POINT	m_pMouseMove;
	POINT	m_pPrevMouse;
	bool	m_bMouseEnabled=true;
	bool	m_bDrag = false;
public:
	LRESULT	MsgProc_Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	DWORD   GetKey(DWORD dwKey);
	bool	OnMouseMove(float x, float y);
	bool	OnMouseDragBegin(float x, float y);
	bool	OnMouseDragEnd();
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	KInput();
public:
	~KInput();
};

#define g_Input KInput::Get()