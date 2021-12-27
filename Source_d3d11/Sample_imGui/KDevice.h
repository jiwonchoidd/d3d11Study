#pragma once
#include "KStd.h"
#include <vector>
class KDevice
{
public:
	ID3D11Device*			m_pd3dDevice;		// 디바이스 객체
	IDXGISwapChain*			m_pSwapChain;		// 스왑체인 객체
	ID3D11RenderTargetView* m_pRenderTargetView;// 메인 랜더타켓 뷰
	D3D11_VIEWPORT			m_ViewPort;			// 뷰포트 
	D3D_DRIVER_TYPE         m_driverType;		// 디바이스 타입( 디폴트:하드웨어 가속 )
	IDXGIFactory*			m_pGIFactory;		// DXGI 객체
	ID3D11DeviceContext*	m_pImmediateContext;// 디바이스 컨텍스트
	D3D_DRIVER_TYPE			m_DriverType;		// 디바이스 타입
	D3D_FEATURE_LEVEL       m_FeatureLevel;		// DirectX의 기능수준
public:
	bool	SetDevice();
	//--------------------------------------------------------------------------------------
	// 디바이스 및 스왑체인 생성 : 
	// 1, CreateDevice()
	// 2, CreateGIFactory()
	// 3, CreateSwapChain()
	// 4, SetRenderTargetView()
	// 5, SetViewPort()
	//--------------------------------------------------------------------------------------
	HRESULT		CreateDeviceAndSwapChain();
	HRESULT		CreateGIFactory();
	HRESULT		SetRenderTargetView();
	HRESULT		SetViewPort();
	bool		CleanupDevice();

	bool DrawTestTriangle()
	{
		namespace wrl = Microsoft::WRL;
		HRESULT hr = S_OK;
		struct Vertex
		{
			float x;
			float y;
			float z;
		};
		const Vertex vertices[] =
		{
			{0.0f,0.5f},
			{0.5f,-0.5f},
			{-0.5f, -0.5f}
		};
		

		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(vertices)*sizeof(Vertex);
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = vertices;
		hr=g_pd3dDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
		if (FAILED(hr)) return false;

		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		m_pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
		
		wrl::ComPtr<ID3DBlob> pBlob;

		//1. 픽셀 셰이더 생성
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		hr=D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
		if (FAILED(hr)) return false;
		hr = g_pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), nullptr, &pPixelShader);
		if (FAILED(hr)) return false;

		//2. 바인드 픽셀 셰이더
		m_pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		//3. 버텍스 셰이더 생성
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		hr=D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
		if (FAILED(hr)) return false;
		hr = g_pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(), nullptr, &pVertexShader);
		if (FAILED(hr)) return false;

		//4. 바인드 버텍스 셰이더
		m_pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		//5. Input 버텍스 레이아웃 (2D 위치만)
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		hr = g_pd3dDevice->CreateInputLayout(layout, _countof(layout),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		);
		if (FAILED(hr)) return false;

		//6. 바인드 버텍스 레이아웃
		m_pImmediateContext->IASetInputLayout(pInputLayout.Get());

		//바인드 렌더 타겟
		m_pImmediateContext->OMSetRenderTargets(1u,&m_pRenderTargetView, nullptr);

		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//뷰포트 설정
		m_pImmediateContext->RSSetViewports(1u, &m_ViewPort);

		m_pImmediateContext->Draw((UINT)(sizeof(vertices)),0u);
		
		return true;
	};

	KDevice();
};

