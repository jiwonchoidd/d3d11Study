#include "KTexture.h"

HRESULT KTexture::LoadTexture(std::wstring texfile)
{
	//L"..\\..\\data\\texture\\Skybox_dd.dds"
	HRESULT hr = S_OK;
	hr = CreateDDSTextureFromFile(
		g_pd3dDevice,
		texfile.c_str(),
		&m_pResourceTexure,
		&m_pSRVTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

void KTexture::Frame()
{
}

void KTexture::Render()
{
	
}

void KTexture::Release()
{
	m_pResourceTexure.Reset();
	m_pSRVTexture.Reset();
}

KTexture::KTexture()
{
	m_pResourceTexure = nullptr;
	m_pSRVTexture = nullptr;
}

KTexture::~KTexture()
{
	m_pResourceTexure.Reset();
		m_pSRVTexture.Reset();
}
