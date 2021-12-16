#include "KTexture.h"

bool  KTexture::LoadTexture(std::wstring texFileName)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        g_pd3dDevice,
        texFileName.c_str(),
        &m_pTexture,
        &m_pTextureSRV);
    if (FAILED(hr))
    {
        hr = DirectX::CreateDDSTextureFromFile(
            g_pd3dDevice,
            texFileName.c_str(),
            &m_pTexture,
            &m_pTextureSRV);
    }
    if (FAILED(hr)) return false;
    return true;
}
bool KTexture::Init()
{
    return false;
}
bool KTexture::Frame()
{
    return false;
}
bool KTexture::Render()
{
    return false;
}
bool KTexture::Release()
{
    SAFE_RELEASE(m_pTexture);
    SAFE_RELEASE(m_pTextureSRV);
    return true;
}
KTexture::KTexture()
{
    m_pTexture=nullptr;
    m_pTextureSRV = nullptr;
}