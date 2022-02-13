#pragma once
#include "KStd.h"
class KTexture
{
public:
	wrl::ComPtr <ID3D11Resource>			m_pResourceTexture;
	wrl::ComPtr <ID3D11ShaderResourceView>	m_pSRVTexture;
public:
	HRESULT LoadTexture(std::wstring texfile);
	void Frame();
	void Render();
	void Release();
public:
	KTexture();
	~KTexture();
};

