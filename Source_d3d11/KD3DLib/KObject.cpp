#include "KObject.h"
void KObject::SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)
{
    if (pMatWorld != nullptr)
    {
        m_cbData.matWorld = pMatWorld->Transpose();
    }
    if (pMatView != nullptr)
    {
        m_cbData.matView = pMatView->Transpose();
    }
    if (pMatProj != nullptr)
    {
        m_cbData.matProj = pMatProj->Transpose();
    }
    m_cbData.matNormal = m_cbData.matNormal.Transpose();
}

bool KObject::Init()
{
    return true;
}

bool KObject::Frame()
{
    m_cbData.vValue.z = g_fSecTimer;
    return true;
}

bool KObject::PreRender(ID3D11DeviceContext* pContext)
{
    if (m_VertexList.size() <= 0) return true;
    //리소스 업데이트 데이터와 리소스 버퍼의 저장
    pContext->UpdateSubresource(
        m_pConstantBuffer.Get(), 0, NULL, &m_cbData, 0, 0);
    pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
    pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

    pContext->PSSetShaderResources(0, 1, &m_Texture.m_pSRVTexture);
    //쉐이더
    pContext->VSSetShader(m_pVS.Get(), NULL, 0);
    pContext->PSSetShader(m_pPS.Get(), NULL, 0);

    pContext->IASetInputLayout(m_pVertexLayout.Get());
    UINT pStrides = m_iVertexSize;
    UINT pOffsets = 0;
    //정점버퍼 바인딩 인덱스버퍼 바인딩 
    pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer,
        &pStrides, &pOffsets);
    pContext->IASetIndexBuffer(
        m_pIndexBuffer.Get(),
        DXGI_FORMAT_R32_UINT, 0);
    return true;
}

bool KObject::Render(ID3D11DeviceContext* pContext)
{
    if (PreRender(pContext) == false) return false;
    if (PostRender(pContext, m_iNumIndex) == false) return false;
    return true;
}

bool KObject::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
    if (iNumIndex > 0)
    {
        pContext->DrawIndexed(iNumIndex, 0, 0);
    }
    else
    {
        pContext->Draw(m_VertexList.size(), 0);
    }
    return false;
}

HRESULT KObject::LoadShader(std::wstring vsFile, std::wstring psFile)
{
    HRESULT hr = S_OK;
    ID3DBlob* error = nullptr;
    m_pVSBlob = LoadShaderBlob(vsFile, "VS", "vs_5_0");
    if (m_pVSBlob == nullptr)
    {
        return hr;
    }
    hr = g_pd3dDevice->CreateVertexShader(
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        NULL, &m_pVS);
    if (FAILED(hr)) return hr;

    ID3DBlob* PSBlob = nullptr;
    PSBlob = LoadShaderBlob(psFile, "PS", "ps_5_0");
    if (PSBlob == nullptr)
    {
        return hr;
    }
    hr = g_pd3dDevice->CreatePixelShader(
        PSBlob->GetBufferPointer(),
        PSBlob->GetBufferSize(),
        NULL, &m_pPS);
    if (FAILED(hr)) return hr;
    PSBlob->Release();
    return hr;
}

bool KObject::LoadTexture(std::wstring filename)
{
    if (!filename.empty())
    {
        
        HRESULT hr= m_Texture.LoadTexture(filename);
        if (FAILED(hr))
        {
            return false;
        }
    }
    return true;
}

bool KObject::CheckVertexData()
{
    if (m_VertexList.size() > 0)
    {
        return true;
    }
    return false;
}

bool KObject::CheckIndexData()
{
    if (m_IndexList.size() > 0)
    {
        return true;
    }
    return false;
}

HRESULT KObject::CreateConstantBuffer()
{
    HRESULT hr = S_OK;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(CB_DATA);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_cbData;
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pConstantBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT KObject::CreateVertexBuffer()
{
    HRESULT hr = S_OK;
    if (m_VertexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = m_iVertexSize * m_VertexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_VertexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
    if (FAILED(hr)) return hr;
    return hr;
}

HRESULT KObject::CreateIndexBuffer()
{
    HRESULT hr = S_OK;
    //if (m_IndexList.size() <= 0) return hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
    bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA data;
    ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
    data.pSysMem = &m_IndexList.at(0);
    hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
    if (FAILED(hr)) return hr;

    m_iNumIndex = m_IndexList.size();
    return hr;
}

HRESULT KObject::CreateVertexLayout()
{
    HRESULT hr = S_OK;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
        m_pVSBlob->GetBufferPointer(),
        m_pVSBlob->GetBufferSize(),
        &m_pVertexLayout);
    if (FAILED(hr)) return hr;

    m_pVSBlob->Release();
    return hr;
}

bool KObject::CreateObject(std::wstring vsFile, 
    std::wstring psFile, std::wstring szTextureName)
{
    //버텍스 데이터 생성
    if (CheckVertexData())
    {
        CreateVertexBuffer();
        if (CheckIndexData())
        {
            CreateIndexBuffer();
        }
        LoadTexture(szTextureName);
        if (SUCCEEDED(LoadShader(vsFile, psFile)))
        {
            if (SUCCEEDED(CreateVertexLayout()))
            {
                CreateConstantBuffer();
                return true;
            }
        }
    }
    return false;
}

ID3DBlob* KObject::LoadShaderBlob(std::wstring vs, std::string function, std::string version)
{
    HRESULT hr = S_OK;
    ID3DBlob* ret = nullptr;
    ID3DBlob* error = nullptr;
    hr = D3DCompileFromFile(
        vs.c_str(),
        nullptr,
        nullptr,
        function.c_str(),
        version.c_str(),
        0,
        0,
        &ret,
        &error);
    if (FAILED(hr))
    {
        MessageBoxA(NULL,
            (char*)error->GetBufferPointer(),
            "error", MB_OK);
        return ret;
    }
    return ret;
}

bool KObject::Release()
{
    m_pPS.Reset();
    m_pVS.Reset();
    m_pVertexBuffer.Reset();
    m_pIndexBuffer.Reset();
    m_pConstantBuffer.Reset();
    m_pVertexLayout.Reset();
    m_pVSBlob.Reset();
    return true;
}