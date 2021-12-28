#pragma once
#include "KStd.h"
//���� ���ٽ� ����, ���� ����, �����Ͷ����� ����, ���÷� ���� ����
//���������Ѱ͵��� �������� ���. ����ȭ��� ��
class KState
{
public:
	//DSS
	static ID3D11DepthStencilState* g_pCurrentDSS;
	static ID3D11DepthStencilState* g_pDSState;

public:
	static HRESULT		CreateDepthStenState();

public:
	static bool		SetState();
	static bool		ReleaseState();
};
static ID3D11DepthStencilState* ApplyDSS(ID3D11DeviceContext* pContext,
	ID3D11DepthStencilState* pState)
{
	ID3D11DepthStencilState* pDSS = KState::g_pCurrentDSS;
	pContext->OMSetDepthStencilState(pState, 1);
	KState::g_pCurrentDSS = pState;
	return pDSS;
}

