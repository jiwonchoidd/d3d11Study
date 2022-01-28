#pragma once
#include "KTexture.h"
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
namespace wrl = Microsoft::WRL;
class KObject
{
public:
	KTexture			m_Texture;
	UINT				m_iNumIndex = 0;
	UINT				m_iVertexSize = 0;
	CB_DATA				m_cbData;
public:
	std::vector <PNCT_VERTEX>		m_VertexList;
	std::vector <DWORD>				m_IndexList;
public:
	wrl::ComPtr <ID3D11PixelShader>	 m_pPS;
	wrl::ComPtr <ID3D11VertexShader> m_pVS;
	wrl::ComPtr <ID3D11Buffer>	 	 m_pVertexBuffer;
	wrl::ComPtr <ID3D11Buffer>		 m_pIndexBuffer;
	wrl::ComPtr <ID3D11Buffer>		 m_pConstantBuffer;
	wrl::ComPtr <ID3D11InputLayout>  m_pVertexLayout;
	wrl::ComPtr <ID3DBlob>			 m_pVSBlob = nullptr;
public:
	virtual void		SetMatrix(KMatrix* pMatWorld,
		KMatrix* pMatView, KMatrix* pMatProj);
	virtual HRESULT		LoadShader(std::wstring vsFile, std::wstring psFile);
	virtual bool		LoadTexture(std::wstring filename);
public:
	virtual bool		CheckVertexData();
	virtual bool		CheckIndexData();
	virtual HRESULT		CreateConstantBuffer();
	virtual HRESULT		CreateVertexBuffer();
	virtual HRESULT		CreateIndexBuffer();
	virtual HRESULT		CreateVertexLayout();
public:
	virtual bool		CreateObject(std::wstring vsFile, std::wstring psFile, std::wstring szTextureName);
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		PreRender(ID3D11DeviceContext* pContext);
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex = 0);
	virtual bool		Release();
	static ID3DBlob*	LoadShaderBlob(std::wstring vs,
		std::string function, std::string version);
};

