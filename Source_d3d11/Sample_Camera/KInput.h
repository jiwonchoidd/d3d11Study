#define DIRECTINPUT_VERSION 0x0800
#pragma once
#include "KStd.h"
#include <dinput.h>

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
//#pragma comment (lib, "uuid.lib")
// Ű����, ���콺 ->������API ���
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
	LPDIRECTINPUT8		  m_pDI;
	LPDIRECTINPUTDEVICE8  m_pKeyDevice; // Ű����
	LPDIRECTINPUTDEVICE8  m_pMouseDevice;// ���콺
	DIMOUSESTATE		  m_DIMouseState;
	BYTE				  m_MouseState[3];
	BYTE				  m_BeforeMouseState[3];
	BYTE				  m_KeyState[256];
public:
	DWORD   GetKey(DWORD dwKey);
	bool	OnMouseMove(float x, float y);
	bool	OnMouseDragBegin(float x, float y);
	bool	OnMouseDragEnd();
public:
	bool	Init();
	bool	InitDirectInput();
	bool	Frame();
	bool	Render();
	bool	Release();
private:
	KInput();
public:
	~KInput();
};

#define g_Input KInput::Get()