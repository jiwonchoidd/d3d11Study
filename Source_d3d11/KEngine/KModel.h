#pragma once
#include "KTexture.h"
struct PC_VERTEX
{
	KVector3 pos;
	KVector4 color;
	PC_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PCT_VERTEX
{
	KVector3 pos;
	KVector4 color;
	KVector2 tex;
	PCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
	}
};
struct PNCT_VERTEX
{
	KVector3 pos;
	KVector3 normal;
	KVector4 color;
	KVector2 tex;
	PNCT_VERTEX()
	{
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		color.w = 1.0f;
		normal.x = 0.0f;
		normal.y = 0.0f;
		normal.z = 0.0f;
		tex.x = 0.0f;
		tex.y = 0.0f;
	}
};
struct CB_DATA
{
	KMatrix  matWorld;
	KMatrix  matView;
	KMatrix  matProj;
	KMatrix  matNormal;
	KVector4 vLightDir;
	KVector4 vValue;
};
class KModel 
{
public:
	KTexture			m_Tex;
	UINT				m_iNumIndex=0;
	UINT				m_iVertexSize=0;
	CB_DATA				m_cbData;
	ID3DBlob*			m_pVSBlob = nullptr;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	ID3D11Buffer*		m_pConstantBuffer;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*	m_pPS;
	ID3D11PixelShader*	m_pMainPS;
	//PNCT가 기본형이다.

	std::vector< PNCT_VERTEX>		m_pVertexList;
	std::vector< DWORD>				m_IndexList;
public:
	KMatrix		m_matWorld;
public:
	virtual bool		CreateVertexData();
	virtual bool		CreateIndexData();
	virtual HRESULT		CreateConstantBuffer();
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT		CreateVertexLayout();
	virtual HRESULT		LoadShader(std::wstring vsFile, std::wstring psFile);
	virtual bool		LoadObject(std::wstring filename);
	virtual void		SetMatrix(KMatrix* pMatWorld,
						KMatrix* pMatView, KMatrix* pMatProj);
	virtual bool		LoadTexture(std::wstring szTextureName);
public:
	virtual bool	CreateModel(wstring vsFile, wstring psFile, wstring szTextureName);
	virtual bool    Init();
	virtual bool    Frame();
	virtual bool    PreRender(ID3D11DeviceContext* pContext);
	virtual bool    Render(ID3D11DeviceContext* pContext);
	virtual bool    PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex=0);
	virtual bool    Release();
public:
	static ID3DBlob*	LoadShaderBlob(std::wstring vs,
		std::string function, std::string version);
public:
	KModel();
};

