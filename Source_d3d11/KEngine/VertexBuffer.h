//#pragma once
//#include "TStd.h"
//
//struct NormalVertex 
//{
//	Vector3 pos;
//	Vector4 color;
//	NormalVertex(Vector3 p, Vector4 c)
//	{
//		pos = p;
//		color = c;
//	}
//	NormalVertex()
//	{
//		pos = { 0.5f,0.5f,0.5f };
//		color = { 0.5f,0.5f,0.5f,1.0f };
//	}
//};
//
//struct CBuffer_DATA
//{
//	Matrix	matWorld;
//	Matrix	matView;
//	Matrix	matProj;
//};
//
//class VertexBuffer 
//{
//	//버퍼 필수 멤버 변수
//	//상수 버퍼
//	ID3D11Buffer* m_pConstBuffer;
//	ID3D11InputLayout* m_pInputLayout;
//	ID3D11VertexShader* m_pVertexShader;
//	ID3D11PixelShader* m_pPixelShader;
//	ID3D11Buffer* m_pVertexBuffer;
//	ID3DBlob* m_pBlob = nullptr;
//	CBuffer_DATA	m_ConstBufferData;
//	KVector3			m_vCamPos;
//	KVector3			m_vCamTarget;
//	std::vector<NormalVertex> m_VertexList;
//public:
//	HRESULT	CreateConstBuffer();
//	HRESULT	CreateVertexBuffer();
//	HRESULT CreateInputLayout();
//	HRESULT LoadShader();
//	bool LoadFile(std::wstring filename);
//public:
//	bool	Init();
//	bool	Frame()
//	{
//		m_ConstBufferData.matWorld = KMatrix::CreateRotationY(g_fTimer * 5);
//		KVector3 vUp = { 0,1,0.0f };
//		m_ConstBufferData.matView = KMatrix::CreateLookAt(m_vCamPos, m_vCamTarget, vUp);
//		m_ConstBufferData.matProj = KMatrix::PerspectiveFovLH(1.0f,
//			100.0f, 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);
//
//		m_ConstBufferData.matWorld = m_ConstBufferData.matWorld.Transpose();
//		m_ConstBufferData.matView = m_ConstBufferData.matView.Transpose();
//		m_ConstBufferData.matProj = m_ConstBufferData.matProj.Transpose();
//		//로컬->월드->뷰->프로젝션->뷰포트
//		//상수버퍼 바꿔주는 것
//
//		return false;
//	}
//	bool	Render(ID3D11DeviceContext* pContext);
//	bool	Release();
//public:
//	VertexBuffer();
//};
//
