#pragma once
#include "KStd.h"
//뎁스 스텐실 상태, 블렌드 상태, 레스터라이즈 상태, 샘플러 상태 관리
//수정가능한것들을 한쪽으로 몬다. 국지화라고 함
class KDXState
{
public:
	static ID3D11DepthStencilState* g_pCurrentDSS;
	static ID3D11SamplerState*		g_pCurrentSS[3];
	static ID3D11RasterizerState*	g_pCurrentRS;
	static ID3D11BlendState*		g_pCurrentBS;


	//RS
	static	ID3D11RasterizerState* g_pRSSolid;
	static	ID3D11RasterizerState* g_pRSWireFrame;
	//DSS
	static	ID3D11DepthStencilState* g_pLeDSS;
	//BS
	static	ID3D11BlendState*	g_pBlendState;
	static	ID3D11SamplerState* g_pClampSS;
	static	ID3D11SamplerState* g_pWrapSS;
public:
	static	HRESULT		CreateDepthStenState();
	static	HRESULT		CreateBlendState();
	static	HRESULT		CreateRasterizeState();
	static	HRESULT		CreateSamplerState();
public:
	static	bool		Init();
	static	bool		Release();
};

static ID3D11DepthStencilState* ApplyDSS(ID3D11DeviceContext* pContext,
	ID3D11DepthStencilState* pState)
{
	ID3D11DepthStencilState* pDSS = KDXState::g_pCurrentDSS;
	pContext->OMSetDepthStencilState(pState, 0x01);
	KDXState::g_pCurrentDSS = pState;
	return pDSS;
}
static ID3D11SamplerState* ApplySS(ID3D11DeviceContext* pContext,
	ID3D11SamplerState* pState, UINT StartSlot = 0)
{
	ID3D11SamplerState* pSS = KDXState::g_pCurrentSS[StartSlot];
	pContext->PSSetSamplers(StartSlot, 1, &pState);
	KDXState::g_pCurrentSS[StartSlot] = pState;
	return pSS;
}

static ID3D11RasterizerState* ApplyRS(ID3D11DeviceContext* pContext,
	ID3D11RasterizerState* pState)
{
	ID3D11RasterizerState* pRS = KDXState::g_pCurrentRS;
	pContext->RSSetState(pState);
	KDXState::g_pCurrentRS = pState;
	return pRS;
}
static ID3D11BlendState* ApplyBS(ID3D11DeviceContext* pContext,
	ID3D11BlendState* pState)
{
	ID3D11BlendState* pBS = KDXState::g_pCurrentBS;
	float BlendFactor[] = { 0.0f,0.0f,0.0f,1.0f };
	pContext->OMSetBlendState(pState, BlendFactor, 0xffffff);
	KDXState::g_pCurrentBS = pState;
	return pBS;
}