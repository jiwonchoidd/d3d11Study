#pragma once
#include "TStd.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
class TWrite
{
public:
	ID2D1Factory*		m_pd2dFactory;
	IDWriteFactory*		m_pdWriteFactory;
	ID2D1RenderTarget*	m_pRT;// 3d ¿¬µ¿
	IDWriteTextFormat*	m_pTextFormat;
	IDWriteTextFormat*  m_pTextFormat50;
	ID2D1SolidColorBrush* m_pTextBrush;
	float				toastTime;
	float					alpha;
public:
	HRESULT CreateDeviceResources(IDXGISurface1* pSurface);
	bool	DrawText(RECT rt, const TCHAR* text, 
		D2D1::ColorF color, IDWriteTextFormat* pTextFormat=nullptr);

	bool	BlinkMessage(wstring text);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
};

