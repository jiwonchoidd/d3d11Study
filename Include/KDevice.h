#pragma once
#include "KDepthStencil.h"
#include "KRenderTarget.h"
class KDevice
{
	// ����̽� �� ����ü�� ���� : 
	// 1, CreateDevice()
	// 2, CreateGIFactory()
	// 3, CreateSwapChain()
	// 4, SetRenderTargetView()
	// 5, SetDepthStencilView
	// 6, SetViewPort()
public:
	ID3D11Device*			m_pd3dDevice;		// ����̽� ��ü
	IDXGISwapChain*			m_pSwapChain;		// ����ü�� ��ü
	
	D3D11_VIEWPORT			m_ViewPort;			// ����Ʈ 
	D3D_DRIVER_TYPE         m_driverType;		// ����̽� Ÿ��( ����Ʈ:�ϵ���� ���� )
	IDXGIFactory*			m_pGIFactory;		// DXGI ��ü
	ID3D11DeviceContext*	m_pImmediateContext;// ����̽� ���ؽ�Ʈ
	D3D_DRIVER_TYPE			m_DriverType;		// ����̽� Ÿ��
	D3D_FEATURE_LEVEL       m_FeatureLevel;		// DirectX�� ��ɼ���
public:
	KDepthStencil			m_DefaultDS;
	KRenderTarget			m_DefaultRT;
public:
	bool		SetDevice();
	HRESULT		CreateDevice();
	HRESULT		CreateGIFactory();
	HRESULT		CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeigh);
	//--------------------------------------------------------------------------------------
	HRESULT		SetRenderTargetView();
	HRESULT		SetDepthStencilView();
	HRESULT		SetViewPort();
	bool		CleanupDevice();

};

