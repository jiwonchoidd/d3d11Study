#include "KDevice.h"
ID3D11Device* g_pd3dDevice = nullptr;		// µð¹ÙÀÌ½º °´Ã¼
bool	KDevice::SetDevice()
{
	HRESULT hr;
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateDeviceAndSwapChain()))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (FAILED(SetViewPort()))
	{
		return false;
	}
	return true;
}
HRESULT KDevice::CreateGIFactory()
{
	HRESULT hr = S_OK;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory),
		(void**)(&m_pGIFactory));
	return hr;
}

HRESULT KDevice::CreateDeviceAndSwapChain()
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == NULL) return S_FALSE;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_12_0,
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);
	m_DriverType = driverTypes[0];

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = g_rtClient.right;
	sd.BufferDesc.Height = g_rtClient.bottom;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	//µª½º½ºÅÙ½Ç count¶û ½º¿ÒÃ¼ÀÎ count¶û ¸ÂÃç¾ßÇÔ,
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	//sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		m_DriverType,
		NULL,
		createDeviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pd3dDevice,
		&m_FeatureLevel,
		&m_pImmediateContext);

	g_pd3dDevice = m_pd3dDevice;

	return hr;
}

HRESULT KDevice::SetRenderTargetView()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* pBackBuffer; // ¹é¹öÆÛ
	if (FAILED(hr = m_pSwapChain->GetBuffer(
		0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer), hr))
	{
		return hr;
	}

	hr=m_pd3dDevice->CreateRenderTargetView(
		pBackBuffer, NULL, 
		&m_pRenderTargetView);
	if (FAILED(hr))
	{
		pBackBuffer->Release();
		return hr;
	}
	pBackBuffer->Release();	
	return hr;
}

HRESULT KDevice::SetDepthStencilView()
{
	// 1)ÅØ½ºÃ³ »ý¼º : ±íÀÌ,½ºÅÙ½Ç °ªÀ» ÀúÀåÇÏ´Â ¹öÆÛ¿ë
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC SDesc;
	m_pSwapChain->GetDesc(&SDesc);
	ID3D11Texture2D* pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = SDesc.BufferDesc.Width;
	DescDepth.Height = SDesc.BufferDesc.Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	//RGB´Â ÅØ½ºÃÄ ¸®¼Ò½º D24´Â µª½º
	DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&DescDepth, nullptr, &pDSTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	if (pDSTexture == nullptr)
	{
		return E_FAIL;
	}
	//µª½º ½ºÅÙ½Ç ºä ¸¸µë
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(pDSTexture, &descDSV,
		&m_DepthStencilView);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

HRESULT KDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	// Setup the viewport    
	m_ViewPort.Width = Desc.BufferDesc.Width;
	m_ViewPort.Height = Desc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	return hr;
}

bool KDevice::CleanupDevice()
{
	//½º¸¶Æ® Æ÷ÀÎÅÍ »ç¿ë
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pGIFactory) m_pGIFactory->Release();
	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pImmediateContext = nullptr;
	m_pGIFactory = nullptr;
	return true;
}

KDevice::KDevice()
{
	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;;
	m_pRenderTargetView = nullptr;;
	m_pGIFactory = nullptr;
	m_pImmediateContext = nullptr;
}
