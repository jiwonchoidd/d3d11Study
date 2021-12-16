#pragma once
#include "KStd.h"
class KRState
{
private:
	bool m_isChecked = false;
	ID3D11RasterizerState* m_pRSSolid;
	ID3D11RasterizerState* m_pRSWireFrame;

public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render(ID3D11DeviceContext* context);
	virtual bool		Release();

public:
	KRState();
	virtual ~KRState();
};

