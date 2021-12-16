//**********************************************************************
//
// ShaderFramework.cpp
// 
// ���̴� ���� ���� C��Ÿ���� �ʰ��� �����ӿ�ũ�Դϴ�.
// (���� ������ �ڵ��Ͻ� ���� ���� �̷��� �����ӿ�ũ��
// �ۼ��Ͻø� �ȵ˴ϴ�. -_-)
//
// Author: Pope Kim
//
//**********************************************************************

#include "ShaderFramework.h"
#include <stdio.h>

#define PI           3.14159265f
#define FOV          (PI/4.0f)							// �þ߰�
#define ASPECT_RATIO (WIN_WIDTH/(float)WIN_HEIGHT)		// ȭ���� ��Ⱦ��
#define NEAR_PLANE   1									// ���� ���
#define FAR_PLANE    10000								// ���Ÿ� ���


//----------------------------------------------------------------------
// ��������
//----------------------------------------------------------------------

// D3D ����
LPDIRECT3D9             gpD3D			= NULL;				// D3D
LPDIRECT3DDEVICE9       gpD3DDevice		= NULL;				// D3D ��ġ

// ��Ʈ
ID3DXFont*              gpFont			= NULL;

// ��
LPD3DXMESH				gpTorus		= NULL;
LPD3DXMESH				gpDisc		= NULL;

// ���̴�
LPD3DXEFFECT			gpApplyShadowShader	= NULL;
LPD3DXEFFECT			gpCreateShadowShader	= NULL;

// �ؽ�ó

// ���α׷� �̸�
const char*				gAppName		= "�ʰ��� ���̴� ���� �����ӿ�ũ";

// ȸ����
float					gRotationY		= 0.0f;

// ���� ��ġ
D3DXVECTOR4				gWorldLightPosition(500.0f, 500.0f, -500.0f, 1.0f);

// ī�޶� ��ġ
D3DXVECTOR4				gWorldCameraPosition( 0.0f, 0.0f, -200.0f, 1.0f ); 

// ��ü�� ����
D3DXVECTOR4				gTorusColor(1, 1, 0, 1);
D3DXVECTOR4				gDiscColor(0, 1, 1, 1);

// �׸��ڸ� ����Ÿ��
LPDIRECT3DTEXTURE9		gpShadowRenderTarget = NULL;
LPDIRECT3DSURFACE9		gpShadowDepthStencil = NULL;

//-----------------------------------------------------------------------
// ���α׷� ������/�޽��� ����
//-----------------------------------------------------------------------

// ������
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    // ������ Ŭ������ ����Ѵ�.
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      gAppName, NULL };
    RegisterClassEx( &wc );

    // ���α׷� â�� �����Ѵ�.
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    HWND hWnd = CreateWindow( gAppName, gAppName,
                              style, CW_USEDEFAULT, 0, WIN_WIDTH, WIN_HEIGHT,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	// Client Rect ũ�Ⱑ WIN_WIDTH, WIN_HEIGHT�� ������ ũ�⸦ �����Ѵ�.
	POINT ptDiff;
	RECT rcClient, rcWindow;
	
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWindow);
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(hWnd,rcWindow.left, rcWindow.top, WIN_WIDTH + ptDiff.x, WIN_HEIGHT + ptDiff.y, TRUE);

    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );

	// D3D�� ����� ��� ���� �ʱ�ȭ�Ѵ�.
	if( !InitEverything(hWnd) )
		PostQuitMessage(1);

	// �޽��� ����
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while(msg.message!=WM_QUIT)
    {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else // �޽����� ������ ������ ������Ʈ�ϰ� ����� �׸���
		{
			PlayDemo();
		}
    }

    UnregisterClass( gAppName, wc.hInstance );
    return 0;
}

// �޽��� ó����
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
	case WM_KEYDOWN:
		ProcessInput(hWnd, wParam);
		break;

    case WM_DESTROY:
		Cleanup();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

// Ű���� �Է�ó��
void ProcessInput( HWND hWnd, WPARAM keyPress)
{
	switch(keyPress)
	{
	// ESC Ű�� ������ ���α׷��� �����Ѵ�.
	case VK_ESCAPE:
		PostMessage(hWnd, WM_DESTROY, 0L, 0L);
		break;
	}
}

//------------------------------------------------------------
// ���ӷ���
//------------------------------------------------------------
void PlayDemo()
{
	Update();
    RenderFrame();
}

// ���ӷ��� ������Ʈ
void Update()
{
}

//------------------------------------------------------------
// ������
//------------------------------------------------------------

void RenderFrame()
{
	D3DCOLOR bgColour = 0xFF0000FF;	// ������ - �Ķ�

    gpD3DDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), bgColour, 1.0f, 0 );

    gpD3DDevice->BeginScene();
    {
		RenderScene();				// 3D ��ü���� �׸���.
		RenderInfo();				// ����� ���� ���� ����Ѵ�.
    }
	gpD3DDevice->EndScene();

    gpD3DDevice->Present( NULL, NULL, NULL, NULL );
}


// 3D ��ü���� �׸���.
void RenderScene()
{
	// ����-�� ����� �����.
	D3DXMATRIXA16 matLightView;
	{
		D3DXVECTOR3 vEyePt( gWorldLightPosition.x, gWorldLightPosition.y, gWorldLightPosition.z ); 
		D3DXVECTOR3 vLookatPt( 0.0f, 0.0f,  0.0f );
		D3DXVECTOR3 vUpVec(    0.0f, 1.0f,  0.0f );
 		D3DXMatrixLookAtLH( &matLightView, &vEyePt, &vLookatPt, &vUpVec );
	}

	// ����-���� ����� �����.
	D3DXMATRIXA16 matLightProjection;
	{
		D3DXMatrixPerspectiveFovLH( &matLightProjection, D3DX_PI / 4.0f, 1, 1, 3000 );
	}

	// ��/��������� �����.
	D3DXMATRIXA16 matViewProjection;
	{
		// �� ����� �����.
		D3DXMATRIXA16 matView;
		D3DXVECTOR3 vEyePt( gWorldCameraPosition.x, gWorldCameraPosition.y, gWorldCameraPosition.z ); 
		D3DXVECTOR3 vLookatPt( 0.0f, 0.0f,  0.0f );
		D3DXVECTOR3 vUpVec(    0.0f, 1.0f,  0.0f );
 		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		
		// ��������� �����.
		D3DXMATRIXA16			matProjection;
		D3DXMatrixPerspectiveFovLH( &matProjection, FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE );

		D3DXMatrixMultiply(&matViewProjection, &matView, &matProjection);
	}

	// ��ȯü�� ��������� �����.
	D3DXMATRIXA16			matTorusWorld;
	{
		// �����Ӹ��� 0.4���� ȸ���� ��Ų��.
		gRotationY += 0.4f * PI / 180.0f;
		if ( gRotationY > 2 * PI )
		{
			gRotationY -= 2 * PI;
		}

		D3DXMatrixRotationY(&matTorusWorld, gRotationY);
	}

	// ��ũ�� ��������� �����.
	D3DXMATRIXA16			matDiscWorld;
	{
		D3DXMATRIXA16 matScale;
		D3DXMatrixScaling(&matScale, 2, 2, 2);
		
		D3DXMATRIXA16 matTrans;
		D3DXMatrixTranslation(&matTrans, 0, -40, 0);

		D3DXMatrixMultiply(&matDiscWorld, &matScale, &matTrans);
	}

	// ���� �ϵ���� �����ۿ� ���̹���
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	gpD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	gpD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);

	//////////////////////////////
	// 1. �׸��� �����
	//////////////////////////////

	// �׸��� ���� ����Ÿ��� ���̹��۸� ����Ѵ�.
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	if( SUCCEEDED( gpShadowRenderTarget->GetSurfaceLevel( 0, &pShadowSurface ) ) )
	{
		gpD3DDevice->SetRenderTarget( 0, pShadowSurface );
		pShadowSurface->Release();
		pShadowSurface = NULL;
	}
	gpD3DDevice->SetDepthStencilSurface( gpShadowDepthStencil );

	// ���� �����ӿ� �׷ȴ� �׸��� ������ ����
	gpD3DDevice->Clear( 0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0 );

	// �׸��� ����� ���̴� ������������ ����
	gpCreateShadowShader->SetMatrix("gWorldMatrix", &matTorusWorld);
	gpCreateShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	gpCreateShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	// �׸��� ����� ���̴��� ����
	{
		UINT numPasses = 0;
		gpCreateShadowShader->Begin(&numPasses, NULL);
		{
			for (UINT i = 0; i < numPasses; ++i )
			{
				gpCreateShadowShader->BeginPass(i);
				{
					// ��ȯü�� �׸���.
					gpTorus->DrawSubset(0);
				}
				gpCreateShadowShader->EndPass();
			}
		}
		gpCreateShadowShader->End();
	}


	//////////////////////////////
	// 2. �׸��� ������
	//////////////////////////////

	// �ϵ���� �����/���̹��۸� ����Ѵ�.
	gpD3DDevice->SetRenderTarget( 0, pHWBackBuffer );
	gpD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);

	pHWBackBuffer->Release();
	pHWBackBuffer = NULL;
	pHWDepthStencilBuffer->Release();
	pHWDepthStencilBuffer = NULL;


	// �׸��� ������ ���̴� ������������ ����
	gpApplyShadowShader->SetMatrix("gWorldMatrix", &matTorusWorld);	//��ȯü
	gpApplyShadowShader->SetMatrix("gViewProjectionMatrix",  &matViewProjection);
	gpApplyShadowShader->SetMatrix("gLightViewMatrix", &matLightView);
	gpApplyShadowShader->SetMatrix("gLightProjectionMatrix", &matLightProjection);

	gpApplyShadowShader->SetVector("gWorldLightPosition", &gWorldLightPosition);

	gpApplyShadowShader->SetVector("gObjectColor", &gTorusColor);

	gpApplyShadowShader->SetTexture("ShadowMap_Tex", gpShadowRenderTarget);


	// ���̴��� �����Ѵ�.
	UINT numPasses = 0;
	gpApplyShadowShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i )
		{
			gpApplyShadowShader->BeginPass(i);
			{
				// ��ȯü�� �׸���.
				gpTorus->DrawSubset(0);

				// ��ũ�� �׸���.
				gpApplyShadowShader->SetMatrix("gWorldMatrix", &matDiscWorld);
				gpApplyShadowShader->SetVector("gObjectColor", &gDiscColor);
				gpApplyShadowShader->CommitChanges();
				gpDisc->DrawSubset(0);
			}
			gpApplyShadowShader->EndPass();
		}
	}
	gpApplyShadowShader->End();
}

// ����� ���� ���� ���.
void RenderInfo()
{
	// �ؽ�Ʈ ����
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,255,255,255);    

	// �ؽ�Ʈ�� ����� ��ġ
	RECT rct;
	rct.left=5;
	rct.right=WIN_WIDTH / 3;
	rct.top=5;
	rct.bottom = WIN_HEIGHT / 3;
	 
	// Ű �Է� ������ ���
	gpFont->DrawText(NULL, "���� �����ӿ�ũ\n\nESC: ��������", -1, &rct, 0, fontColor );
}

//------------------------------------------------------------
// �ʱ�ȭ �ڵ�
//------------------------------------------------------------
bool InitEverything(HWND hWnd)
{
	// D3D�� �ʱ�ȭ
	if( !InitD3D(hWnd) )
	{
		return false;
	}

	// ����Ÿ���� �����.
	const int shadowMapSize = 2048;
	if(FAILED(gpD3DDevice->CreateTexture( shadowMapSize, shadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &gpShadowRenderTarget, NULL ) ))
	{
		return false;
	}
	
	// �׸��� �ʰ� ������ ũ���� ���̹��۵� �������� �Ѵ�.
	if(FAILED(gpD3DDevice->CreateDepthStencilSurface(shadowMapSize, shadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE,
		&gpShadowDepthStencil, NULL)))
	{
		return false;
	}
	
	// ��, ���̴�, �ؽ�ó���� �ε�
	if( !LoadAssets() )
	{
		return false;
	}

	// ��Ʈ�� �ε�
    if(FAILED(D3DXCreateFont( gpD3DDevice, 20, 10, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                           OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, (DEFAULT_PITCH | FF_DONTCARE), 
                           "Arial", &gpFont )))
	{
		return false;
	}

	return true;
}

// D3D ��ü �� ��ġ �ʱ�ȭ
bool InitD3D(HWND hWnd)
{
    // D3D ��ü
    gpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !gpD3D )
	{
		return false;
	}

    // D3D��ġ�� �����ϴµ� �ʿ��� ����ü�� ä���ִ´�.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

	d3dpp.BackBufferWidth				= WIN_WIDTH;
	d3dpp.BackBufferHeight				= WIN_HEIGHT;
	d3dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount				= 1;
    d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality			= 0;
    d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow					= hWnd;
    d3dpp.Windowed						= TRUE;
    d3dpp.EnableAutoDepthStencil		= TRUE;
    d3dpp.AutoDepthStencilFormat		= D3DFMT_D24X8;
    d3dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
    d3dpp.FullScreen_RefreshRateInHz	= 0;
    d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_ONE;

    // D3D��ġ�� �����Ѵ�.
    if( FAILED( gpD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                    &d3dpp, &gpD3DDevice ) ) )
    {
        return false;
    }

    return true;
}

bool LoadAssets()
{
	// �ؽ�ó �ε�

	// ���̴� �ε�
	gpApplyShadowShader = LoadShader("ApplyShadow.fx");
	if ( !gpApplyShadowShader )
	{
		return false;
	}

	gpCreateShadowShader = LoadShader("CreateShadow.fx");
	if ( !gpCreateShadowShader )
	{
		return false;
	}


	// �� �ε�
	gpTorus = LoadModel("torus.x");
	if ( !gpTorus )
	{
		return false;
	}

	gpDisc = LoadModel("disc.x");
	if ( !gpDisc )
	{
		return false;
	}

	return true;
}

// ���̴� �ε�
LPD3DXEFFECT LoadShader(const char * filename )
{
	LPD3DXEFFECT ret = NULL;

	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(gpD3DDevice, filename,
		NULL, NULL, dwShaderFlags, NULL, &ret, &pError);

	// ���̴� �ε��� ������ ��� outputâ�� ���̴�
	// ������ ������ ����Ѵ�.
	if(!ret && pError)
	{
		int size	= pError->GetBufferSize();
		void *ack	= pError->GetBufferPointer();

		if(ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete [] str;
		}
	}

	return ret;
}

// �� �ε�
LPD3DXMESH LoadModel(const char * filename)
{
	LPD3DXMESH ret = NULL;
	if ( FAILED(D3DXLoadMeshFromX(filename,D3DXMESH_SYSTEMMEM, gpD3DDevice, NULL,NULL,NULL,NULL, &ret)) )
	{
		OutputDebugString("�� �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	};

	return ret;
}

// �ؽ�ó �ε�
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename)
{
	LPDIRECT3DTEXTURE9 ret = NULL;
	if ( FAILED(D3DXCreateTextureFromFile(gpD3DDevice, filename, &ret)) )
	{
		OutputDebugString("�ؽ�ó �ε� ����: ");
		OutputDebugString(filename);
		OutputDebugString("\n");
	}

	return ret;
}
//------------------------------------------------------------
// ������ �ڵ�.
//------------------------------------------------------------

void Cleanup()
{
	// ��Ʈ�� release �Ѵ�.
	if(gpFont)
	{
		gpFont->Release();
		gpFont = NULL;
	}

	// ���� release �Ѵ�.
	if ( gpTorus )
	{
		gpTorus->Release();
		gpTorus = NULL;
	}

	if ( gpDisc )
	{
		gpDisc->Release();
		gpDisc = NULL;
	}

	// ���̴��� release �Ѵ�.
	if ( gpApplyShadowShader )
	{
		gpApplyShadowShader->Release();
		gpApplyShadowShader = NULL;
	}

	if ( gpCreateShadowShader )
	{
		gpCreateShadowShader->Release();
		gpCreateShadowShader = NULL;
	}

	// �ؽ�ó�� release �Ѵ�.
	if ( gpShadowRenderTarget )
	{
		gpShadowRenderTarget->Release();
		gpShadowRenderTarget = NULL;
	}
	
	if ( gpShadowDepthStencil )
	{
		gpShadowDepthStencil->Release();
		gpShadowDepthStencil = NULL;
	}

	// D3D�� release �Ѵ�.
    if(gpD3DDevice)
	{
        gpD3DDevice->Release();
		gpD3DDevice = NULL;
	}

    if(gpD3D)
	{
        gpD3D->Release();
		gpD3D = NULL;
	}
}

