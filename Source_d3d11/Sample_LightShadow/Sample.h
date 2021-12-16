#pragma once
#include "KCore.h"
#include "KShape.h"
class KLight
{
    //ī�޶�� ���� �ٲ㵵�Ǵµ� ī�޶�� �Ӽ��� �޶�
public:
    KVector3    m_vPos;
    KVector3    m_vTarget;
    KVector3    m_vDir;
    KMatrix     m_matView;
    KMatrix     m_matProj;
    KMatrix     m_matWorld;
public:
    void    SetLight(KVector3 vPos, KVector3 vTarget)
    {
        //�̰� ������ ������� ����� ����.
        m_vPos = vPos;
        m_vTarget = vTarget;
        //���� ����ȭ
        //(vTarget - vPos).Normalize(m_vDir);
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        //����ĸ����
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        //�������� ������� �����
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.25f, 1.0f, 1.0f, 500.0f);
    }
    bool    Frame()
    {
        D3DKMatrixRotationY(&m_matWorld, XM_PI * g_fSecPerFrame * 0.1f);
        //������ ����� ���ϴ� �Լ� coord�� w���� 1�� ��
        D3DXVec3TransformCoord(&m_vPos, &m_vPos, &m_matWorld);
        //error C2102: '&'�� l-value�� �־�� �մϴ�.
        m_vDir = m_vTarget - m_vPos;
        D3DXVec3Normalize(&m_vDir, &m_vDir);
        KVector3 vUp(0, 1, 0);
        D3DKMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);
        D3DKMatrixPerspectiveFovLH(&m_matProj, XM_PI * 0.25f, 1.0f, 1.0f, 500.0f);

        return true;
    }
};                    
class KMiniMap : public KPlaneShape
{
public:
    bool		LoadTexture()
    {
        m_Tex.LoadTexture(L"../../data/charport.bmp");
    }
    bool CreateVertexData()
    {
        m_pVertexList.resize(4);

        m_pVertexList[0].pos = KVector3(-1.0f, -0.5f, 0.0f);
        m_pVertexList[0].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[0].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[0].tex = KVector2(0.0f, 0.0f);

        m_pVertexList[1].pos = KVector3(-0.5f, -0.5f, 0.0f);
        m_pVertexList[1].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[1].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[1].tex = KVector2(1.0f, 0.0f);


        m_pVertexList[2].pos = KVector3(-1.0f, -1.0f, 0.0f);
        m_pVertexList[2].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[2].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[2].tex = KVector2(0.0f, 1.0f);


        m_pVertexList[3].pos = KVector3(-0.5f, -1.0f, 0.0f);
        m_pVertexList[3].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[3].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[3].tex = KVector2(1.0f, 1.0f);
        return true;
    }
};
struct cbDataShadow
{
    KMatrix g_matShadow1; // Lw * Lv * Lp	
};
class Sample : public KCore
{
    float               m_fYRot = 0;
	KVector3			m_MovePos;
    KMatrix             m_matTex;
    cbDataShadow        m_ShadowCB;
    KLight              m_Light1;
    KMiniMap            m_MiniMap;
	KPlaneShape			m_MapObj;
	KRenderTarget		m_Rt;
	KFbxObj				m_FbxObjA;
	KFbxObj				m_FbxObjB;
	KMatrix				m_matShadow;
    ID3D11PixelShader* m_pPSShadow = nullptr;
    ID3D11PixelShader*	m_pPSShadowMap = nullptr;
    ID3D11Buffer*       m_pShadowCB = nullptr;
public:
    HRESULT     CreateConstantBuffer();
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

