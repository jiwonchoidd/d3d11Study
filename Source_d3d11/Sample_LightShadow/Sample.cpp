#include "Sample.h"
HRESULT Sample::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(cbDataShadow);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_ShadowCB;
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &m_pShadowCB);
	if (FAILED(hr)) return hr;
	return hr;
}
bool		Sample::Init()
{
	CreateConstantBuffer();
	m_DebugCamera.CreateViewMatrix(KVector3(-50, 100, -100), KVector3(0, 0, 0));
	m_DebugCamera.CreateProjMatrix(1.0f, 500.0f, XM_PI * 2.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
	
	//텍스쳐 변환행렬
	m_matTex._11 = 0.5f; m_matTex._22 = -0.5f;
	m_matTex._41 = 0.5f; m_matTex._42 = 0.5f;
	//라이트 설정
	m_Light1.SetLight(KVector3(150, 150, 150), KVector3(0, 0, 0));

	m_MiniMap.CreateModel(L"../../data/shader/PlaneVS.txt",
		L"../../data/shader/PlanePS.txt", L"");

	m_MapObj.CreateModel(L"DepthShadow.hlsl",
		L"DepthShadow.hlsl", L"../../data/object/wal156S.bmp");

	KMatrix matWorld, matScale;
	//맵크기를 키움
	D3DKMatrixRotationX(&matWorld, XM_PI / 2.0f);
	D3DKMatrixScaling(&matScale, 100.0f, 100.0f, 100.0f);
	m_MapObj.m_matWorld = matScale * matWorld;
	//그림자 렌더타겟 이미지 
	m_Rt.Create(512, 512);

	//캐릭터 블랍 만듬
	ID3DBlob* PSBlob = nullptr;
	PSBlob = KModel::LoadShaderBlob(L"CharacterShader.hlsl",
		"PSShadow", "ps_5_0");
	if (PSBlob != nullptr)
	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(
			PSBlob->GetBufferPointer(),
			PSBlob->GetBufferSize(),
			NULL, &m_pPSShadow);
		if (FAILED(hr)) return hr;
		PSBlob->Release();
	}
	PSBlob = KModel::LoadShaderBlob(L"DepthShadow.hlsl",
		"PSDepth", "ps_5_0");
	if (PSBlob != nullptr)
	{
		HRESULT hr = S_OK;
		hr = g_pd3dDevice->CreatePixelShader(
			PSBlob->GetBufferPointer(),
			PSBlob->GetBufferSize(),
			NULL, &m_pPSShadowMap);
		if (FAILED(hr)) return hr;
		PSBlob->Release();
	}

	m_FbxObjA.LoadObject("../../data/object/Man.fbx", "CharacterShader.hlsl");
	return true;
}

bool		Sample::Frame()
{
	if (g_Input.GetKey(VK_F4) == KEY_PUSH)
	{
		(m_FbxObjA.m_bAnimPlay) ? m_FbxObjA.m_bAnimPlay = false : m_FbxObjA.m_bAnimPlay = true;
	}
	if (m_FbxObjA.m_bAnimPlay == false)
	{
		#pragma region 캐릭터 이동 애니메이션
		if (g_Input.GetKey(VK_RIGHT))
		{
			m_fYRot += 3 * g_fSecPerFrame;
		}
		if (g_Input.GetKey(VK_LEFT))
		{
			m_fYRot -= 3 * g_fSecPerFrame;
		}
		m_FbxObjA.m_matWorld._11 = cos(m_fYRot);
		m_FbxObjA.m_matWorld._13 = -sin(m_fYRot);
		m_FbxObjA.m_matWorld._31 = sin(m_fYRot);
		m_FbxObjA.m_matWorld._33 = cos(m_fYRot);
		//look vector
		//_31,_32,_33 월드행렬 하고 정규화
		m_FbxObjA.m_vLook.x = m_FbxObjA.m_matWorld._31;
		m_FbxObjA.m_vLook.y = m_FbxObjA.m_matWorld._32;
		m_FbxObjA.m_vLook.z = m_FbxObjA.m_matWorld._33;

		if (g_Input.GetKey(VK_UP) >= KEY_PUSH)
		{
			m_MovePos -= m_FbxObjA.m_vLook * g_fSecPerFrame * 120.0f;
			//애니메이션
			m_FbxObjA.m_fElpaseTime += 1.0f * g_fSecPerFrame;
			m_FbxObjA.m_iAnimIndex = (m_FbxObjA.m_fElpaseTime + 3.1f) * 30.0f;
			if (m_FbxObjA.m_fElpaseTime > 0.75f)
			{
				m_FbxObjA.m_fElpaseTime = 0.0f;
			}
		}
		else if (g_Input.GetKey(VK_DOWN) >= KEY_PUSH)
		{
			m_MovePos += m_FbxObjA.m_vLook * g_fSecPerFrame * 40.0f;
			//애니메이션
			m_FbxObjA.m_fElpaseTime += 1.0f * g_fSecPerFrame;
			m_FbxObjA.m_iAnimIndex = (m_FbxObjA.m_fElpaseTime + 2.1f) * 30.0f;
			if (m_FbxObjA.m_fElpaseTime > 0.8f)
			{
				m_FbxObjA.m_fElpaseTime = 0.0f;
			}
		}
		else
		{
			m_FbxObjA.m_iAnimIndex = 0;
			m_FbxObjA.m_fElpaseTime = 0.0f;
		}
		m_FbxObjA.m_matWorld._41 = m_MovePos.x;
		m_FbxObjA.m_matWorld._42 = m_MovePos.y;
		m_FbxObjA.m_matWorld._43 = m_MovePos.z;
		#pragma endregion
	}


	m_FbxObjA.Frame();
	m_Light1.Frame();
	//쉐도우 뷰행렬, 프로젝션 행렬, 텍스쳐행렬 곱한것
	m_ShadowCB.g_matShadow1 = m_Light1.m_matView * m_Light1.m_matProj * m_matTex;
	return true;
}
bool		Sample::Render()
{
	//레스터라이즈 솔리드
	ApplyRS(m_pImmediateContext, KDXState::g_pRSSolid);
	//렌더타겟 객체 저장해놓고 나중에 end로 복원
	// 그림자 라이트에서 카메라로 촬영한 텍스쳐
	if (m_Rt.Begin(m_pImmediateContext))
	{
		m_MapObj.SetMatrix(&m_MapObj.m_matWorld,
			&m_Light1.m_matView, &m_Light1.m_matProj);
		m_MapObj.PreRender(m_pImmediateContext);
		m_pImmediateContext->PSSetShader(m_pPSShadowMap, NULL, 0);
		m_MapObj.PostRender(m_pImmediateContext,
			m_MapObj.m_iNumIndex);//
		m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld,
			&m_Light1.m_matView, &m_Light1.m_matProj);
		m_FbxObjA.ChangePixelShader(m_pPSShadow);
		m_FbxObjA.Render(m_pImmediateContext);
		//복원 작업
		m_Rt.End(m_pImmediateContext);
	}
	m_ShadowCB.g_matShadow1 = m_ShadowCB.g_matShadow1.Transpose();
	m_pImmediateContext->UpdateSubresource(
		m_pShadowCB, 0, NULL, &m_ShadowCB, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pShadowCB);

	//샘플러 상태 : 클램프 -> 그림자용
	//ApplyRS(m_pImmediateContext,KDXState::g_pRSSolid);
	ApplySS(m_pImmediateContext, KDXState::g_pClampSS, 1);

	D3DKMatrixInverse(&m_MapObj.m_cbData.matNormal, NULL,
		&m_MapObj.m_matWorld);
	m_MapObj.m_cbData.vLightDir.x = m_Light1.m_vDir.x;
	m_MapObj.m_cbData.vLightDir.y = m_Light1.m_vDir.y;
	m_MapObj.m_cbData.vLightDir.z = m_Light1.m_vDir.z;
	m_MapObj.SetMatrix(&m_MapObj.m_matWorld, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Rt.m_pTextureSRV);
	m_MapObj.Render(m_pImmediateContext);

	m_MiniMap.SetMatrix(nullptr, nullptr, nullptr);
	m_MiniMap.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_Rt.m_pTextureSRV);
	m_MiniMap.PostRender(m_pImmediateContext, m_MiniMap.m_iNumIndex);

	D3DKMatrixInverse(&m_FbxObjA.m_cbData.matNormal,
		NULL, &m_FbxObjA.m_matWorld);
	m_FbxObjA.SetMatrix(&m_FbxObjA.m_matWorld, &m_DebugCamera.m_matView, &m_DebugCamera.m_matProj);
	m_FbxObjA.ChangePixelShader(nullptr,&m_FbxObjA.m_cbData.matNormal, m_Light1.m_vDir);
	m_pImmediateContext->PSSetShaderResources(1, 1, &m_Rt.m_pTextureSRV);
	m_FbxObjA.Render(m_pImmediateContext);

	if (g_Input.GetKey(VK_F5) == KEY_PUSH)
	{
		m_Rt.Save(m_pImmediateContext, L"frame.dds");
	}
	return true;
	//사용자 설명
	m_Write.BlinkMessage(L"F1 - 프레임,  F4 - 전체 애니메이션,  방향키 - 이동");
	return true;
}
bool		Sample::Release()
{
	m_MapObj.Release();
	m_MiniMap.Release();
	m_Rt.Release();
	m_FbxObjA.Release();
	SAFE_RELEASE(m_pPSShadow);
	return true;
}
Sample::Sample()
{
}
Sample::~Sample()
{
}
WinMain_OPT(뎁스 쉐도우, 1200, 675);