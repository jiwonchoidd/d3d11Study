#include "KTexture.h"

HRESULT KTexture::LoadTexture(std::wstring texfile)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        g_pd3dDevice,
        texfile.c_str(),
        &m_pResourceTexture,
        &m_pSRVTexture);
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(
            g_pd3dDevice,
            texfile.c_str(),
            &m_pResourceTexture,
            &m_pSRVTexture);
    }
    if (FAILED(hr)) return false;
    return true;
}

void KTexture::Frame()
{
}

void KTexture::Render()
{

}

void KTexture::Release()
{
    m_pResourceTexture.Reset();
    m_pSRVTexture.Reset();
}

KTexture::KTexture()
{
    m_pResourceTexture = nullptr;
    m_pSRVTexture = nullptr;
}

KTexture::~KTexture()
{
    m_pResourceTexture.Reset();
    m_pSRVTexture.Reset();
}
