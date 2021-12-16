#pragma once
#include "KModel.h"
#include "KStd.h"
#include "KTexture.h"
#include "KMesh.h"


class KFbxObj
{
	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
public:
	KMatrix			m_matWorld;
	KAnimMatrix     m_matAnimMatrix;
	// todo:게임 오브젝트로 옮겨야함
	//KVector3 m_MovePos;
	//float	m_fYRot = 0;
	bool	m_bAnimControl = false;
	bool	m_bAnimPlay = false;
	float   m_fElpaseTime = 0.0f;
	int		m_iAnimIndex = 0;
	float	m_fStartTime = 0.0f;
	float	m_fEndTime = 0.0f;
	float	m_fSampleTime = 0.0f;
	KVector3 m_vLook;
public:
	void	SelectedAnim(float start, float end);
	//
	std::vector<FbxNode*>	m_pFbxNodeList;
	std::vector<KMtrl*>		m_pFbxMaterialList;
	std::vector<KMesh*>		m_pMeshList;
	CB_DATA					m_cbData;
public:
	KMesh*		GetFindIndex(FbxNode* pNode);
	bool		LoadObject(string filename, string shadername);
	KMatrix     DxConvertMatrix(KMatrix m);
	KMatrix     ConvertMatrix(FbxMatrix& m);
	KMatrix		ConvertAMatrix(FbxAMatrix& m);
	bool	Frame();
	bool    Release();
public:
	void	SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj);
	bool    Render(ID3D11DeviceContext* pContext);
	int     GetRootMtrl(FbxSurfaceMaterial* pFbxMaterial);
	void	LoadMaterial(KMtrl* pMtrl);
	// 일반함수 리스트
public:
	void	PreProcess(FbxNode* pNode);
	void	ParseMesh(FbxNode* pNode, KMesh* pMesh);
	void	ParseNode(FbxNode* pNode, KMesh* pParentMesh);
	KMatrix	ParseTransform(FbxNode* pNode, KMatrix& matParent);
public:
	void	ParseAnimStack(FbxString* szData);
	void	ParseAnimation();
	void	ParseAnimationNode();
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, KMesh* pMesh, KSkinData* pSkindata);
public:
	FbxVector2  ReadTextureCoord(FbxMesh* pFbxMesh, DWORD dwVertexTextureCount, FbxLayerElementUV* pUVSet, int vertexIndex, int uvIndex);
	FbxVector4  ReadNormal(const FbxMesh* mesh, DWORD dwVertexNormalCount, FbxLayerElementNormal* VertexNormalSets,
		int controlPointIndex, int dwVertexIndex);
	FbxColor	ReadColor(const FbxMesh* mesh, DWORD dwVertexColorCount, FbxLayerElementVertexColor* VertexColorSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4	ReadTangent(const FbxMesh* pFbxMesh,
		DWORD dwVertexTangentCount, FbxGeometryElementTangent* VertexTangentSets,
		DWORD dwDCCIndex, DWORD dwVertexIndex);
	void  ChangePixelShader(ID3D11PixelShader* ps = nullptr,
		KMatrix* matNormal = nullptr, KVector3 vLight = KVector3(0, 0, 1));
};

