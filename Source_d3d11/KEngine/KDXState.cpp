#include "KDXState.h"
#include "KInput.h"
ID3D11DepthStencilState* KDXState::g_pCurrentDSS = nullptr;
ID3D11SamplerState* KDXState::g_pCurrentSS[3] = { nullptr,nullptr,nullptr };
ID3D11RasterizerState* KDXState::g_pCurrentRS = nullptr;
ID3D11BlendState* KDXState::g_pCurrentBS = nullptr;

ID3D11BlendState* KDXState::g_pBlendState = nullptr;
ID3D11DepthStencilState* KDXState::g_pLeDSS = nullptr;
ID3D11SamplerState* KDXState::g_pClampSS = nullptr;
ID3D11SamplerState* KDXState::g_pWrapSS = nullptr;
ID3D11RasterizerState* KDXState::g_pRSSolid = nullptr;
ID3D11RasterizerState* KDXState::g_pRSWireFrame = nullptr;

bool KDXState::Init()
{
    CreateDepthStenState();
    CreateSamplerState();
    CreateRasterizeState();
    CreateBlendState();
    return true;
}
HRESULT KDXState::CreateDepthStenState()
{
    HRESULT hr = S_OK;
    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsd.DepthEnable = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //1.0, 0.6�� 0.5�� ���ؼ� ũ�� ����
    //���� �տ� �ִ� ���� �ѷ����� �ڿ��ִ¾ֵ��� ���� �ȵ�
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    dsd.StencilEnable = TRUE;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    hr = g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pLeDSS);
    if (FAILED(hr))
    {
        return hr;
    }
    //���� ���ٽ� ���� ������ ��� ������� �����ϴ� ���̱⿡ OM
    return hr;
}


HRESULT KDXState::CreateBlendState()
{
    HRESULT hr = S_OK;
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.AlphaToCoverageEnable = false;
    bd.IndependentBlendEnable = false;
    bd.RenderTarget[0].BlendEnable = false;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = g_pd3dDevice->CreateBlendState(&bd, &g_pBlendState);
    if (FAILED(hr))
    {
        return hr;
    }
}

HRESULT KDXState::CreateRasterizeState()
{
    HRESULT hr = S_OK;
    D3D11_RASTERIZER_DESC rd;
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_WIREFRAME;
    rd.CullMode = D3D11_CULL_BACK;
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSWireFrame);
    ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.DepthClipEnable = TRUE; // Clipping ȿ�� �⺻�� False��
    hr = g_pd3dDevice->CreateRasterizerState(&rd, &g_pRSSolid);
    return hr;
}
HRESULT KDXState::CreateSamplerState()
{
    D3D11_SAMPLER_DESC sd;
    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    HRESULT hr = g_pd3dDevice->CreateSamplerState(&sd,
        &g_pClampSS);

    ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    hr = g_pd3dDevice->CreateSamplerState(&sd, &g_pWrapSS);
    return hr;
}



bool KDXState::Release()
{
    SAFE_RELEASE(g_pBlendState);
    SAFE_RELEASE(g_pLeDSS);
    SAFE_RELEASE(g_pClampSS);
    SAFE_RELEASE(g_pWrapSS);
    SAFE_RELEASE(g_pRSSolid);
    SAFE_RELEASE(g_pRSWireFrame);
    return true;
}

