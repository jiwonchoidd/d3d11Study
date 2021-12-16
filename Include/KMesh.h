#pragma once
#include "KModel.h"
#include "KTexture.h"
const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

struct KMtrl
{
	FbxNode* m_pFbxNode;
	FbxSurfaceMaterial* m_pFbxSurfaceMtrl;
	KTexture			m_Texture;
	vector<KMtrl*> m_pSubMtrl;
	KMtrl() {}
	//생성자로 노드와 매터리얼 저장 
	KMtrl(FbxNode* pFbxNode, FbxSurfaceMaterial* pFbxMtrl)
	{
		m_pFbxNode = pFbxNode;
		m_pFbxSurfaceMtrl = pFbxMtrl;
	}
	void Release()
	{
		m_Texture.Release();
		for (auto data : m_pSubMtrl)
		{
			data->Release();
			delete data;
		}
	}
};
//레이어는 하나 일 것이다.
struct KLayer
{
	FbxLayerElementVertexColor* pColor;
	FbxLayerElementNormal* pNormal;
	FbxLayerElementUV* pUV;
	FbxLayerElementMaterial* pMaterial;
};

struct KWeight
{
	std::vector<int>     m_IndexList;
	std::vector<float>   m_WeightList;
};

struct KSkinData
{
	std::vector<FbxNode*>  m_MatrixList;
	//정점당 가중치, 인덱스
	std::vector<KWeight>   m_VertexList;
};
struct PNCTIW_VERTEX
{
	float   index[4]; 
	float   weight[4];
};
struct KAnimMatrix
{
	KMatrix   matAnimation[255]; //최대 255
};
class KMesh :public KModel
{
public:
	//인덱스 정렬된
	vector<FbxNode*>		m_pFbxNodeList;
	vector<KMatrix>			m_matBindPoseList;
	vector<KMesh*>			m_pMeshLinkList;

	KAnimMatrix				m_matAnimMatrix;
	ID3D11Buffer*			m_pAnimCB = nullptr;
	ID3D11Buffer*			m_pIWVertrexBuffer=nullptr;
	//정점당 가중치 인덱스 4개씩 쉐이더에 넣기 위함 
	vector<PNCTIW_VERTEX>	m_iwList;
	FbxNode*				m_pFbxNode = nullptr;

	KMesh* m_pParent = nullptr; // 부모
	wstring				m_szName;
	wstring				m_szParentName;
	OBJECTCLASSTYPE     m_ClassType;
	vector<KLayer>		m_LayerList;
	int					m_iNumLayer;
	//해당하는 서브매쉬에 서브 매터리얼 연결해야함
	int					m_iMtrlRef;
	KMatrix				m_matWorld;
	vector<KMatrix>		m_AnimationTrack;
	vector<KMesh*>		m_pSubMesh;
public:
	virtual HRESULT		CreateVertexLayout() override;
	virtual HRESULT		CreateConstantBuffer()override;
	virtual HRESULT		CreateVertexBuffer()override;
	virtual bool		PreRender(ID3D11DeviceContext* pContext)override;
	bool Release() override;
public:
	KMesh();
	virtual ~KMesh();
};

