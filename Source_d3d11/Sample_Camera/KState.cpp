#include "KState.h"
ID3D11DepthStencilState* KState::g_pDSState = nullptr;
ID3D11DepthStencilState* KState::g_pCurrentDSS = nullptr;

HRESULT KState::CreateDepthStenState()
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
    hr = g_pd3dDevice->CreateDepthStencilState(&dsd, &g_pDSState);
    if (FAILED(hr))
    {
        return hr;
    }
    //���� ���ٽ� ���� ������ ��� ������� �����ϴ� ���̱⿡ OM
    return hr;
}

bool KState::SetState()
{
    CreateDepthStenState();
	return true;
}

bool KState::ReleaseState()
{
    if(g_pDSState)
    g_pDSState->Release();
	return true;
}
