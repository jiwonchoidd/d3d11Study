#pragma once
#include "KNode.h"
#include <queue>

class KQuadtree
{
public:
	UINT		m_iMaxDepth;
	UINT		m_iNumCell;
	UINT		m_iNumPatch;
	vector<DWORD>  m_IndexList;
	ID3D11Buffer* m_pIndexBuffer;
	vector<TLodPatch>   m_LodPatchList;
public:
	KNode*		m_pRootNode;
	int			m_iNumCol;
	int			m_iNumRow;
	queue<KNode*>  m_Queue;
	map<int, KNode*>  m_pLeafList;
	KMap*		m_pMap;
public:
	bool	UpdateIndexList(KNode* pNode);
	HRESULT CreateIndexBuffer(KNode* pNode);
	bool	UpdateVertexList(KNode* pNode);
	HRESULT CreateVertexBuffer(KNode* pNode);
public:
	void    Build(KMap* pMap);
	KNode*  CreateNode(KNode* pParent, float x, float y, float w, float h);
	void	Buildtree(KNode*);
	bool    AddObject(KVector2 pos);
	KNode*  FindNode(KNode* pNode, KVector2 pos);
	
	KNode*  FindPlayerNode(KVector2 pos);
	bool	SubDivide(KNode* pNode);
	void	SetNeighborNode();
	bool    LoadObject(std::wstring filename);
public:
	bool    Init();
	bool	Frame();
	bool	Render(ID3D11DeviceContext* pContext, KVector3* vCamera);
	bool    Release();
	bool    ComputeStaticLodIndex(int iMaxCells);
	HRESULT CreateIndexBuffer(TLodPatch& patch, int iCode);
	template <typename OutputIterator>
	void	Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first);
public:
	KQuadtree();
	virtual ~KQuadtree();
};

