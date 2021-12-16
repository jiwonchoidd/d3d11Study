#pragma once
#include "TWindow.h"
#include "TDevice.h"
#include "TTimer.h"
#include "TInput.h"
#include "TSound.h"
#include "TWrite.h"
#include "KCamera.h"
#include "KFbxObj.h"
#include "KTexture.h"
#include "KRState.h"
class TCore : public TWindow
{
public:
	TTimer			m_Timer;
	TSound			m_Sound;
	TWrite			m_Write;
	KDebugCamera	m_DebugCamera;
	KRState			m_RState;
	KFbxObj			m_KFbxObj;
	KTexture		m_KTexture;
	bool			m_bDebugText = false;
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

