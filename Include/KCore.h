#pragma once
#include "KWindow.h"
#include "KDevice.h"
#include "KTimer.h"
#include "KInput.h"
#include "KSound.h"
#include "KWrite.h"
#include "KCamera.h"
#include "KFbxObj.h"
#include "KTexture.h"
#include "KDXState.h"
class KCore : public KWindow
{
public:
	KTimer			m_Timer;
	KSound			m_Sound;
	KWrite			m_Write;
	KDebugCamera	m_DebugCamera;
	bool			m_bDebugText = false;
	bool			m_isChecked = false;
private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;

public:
	void	FrameCamera();
	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
public:
};

