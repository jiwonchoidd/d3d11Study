#pragma once
#include "KStd.h"
#include "KMap.h"
struct Rect
{
	float  x, y, w, h;
};
struct TRect
{
	KVector2  p0;
	KVector2  p1;
	KVector2  wh;
	// x1,y1(p0)
	//
	//          x2, y2(p1)
};
typedef std::vector<DWORD>  DWORD_VECTOR;
struct TLodPatch
{
	UINT   iLodLevel;
	DWORD_VECTOR		IndexList[16];
	ID3D11Buffer* IndexBufferList[16] = { nullptr, };
	void Release()
	{
		for (int iBuffer = 0; iBuffer < 16; iBuffer++)
		{
			if (IndexBufferList[iBuffer])
			{
				IndexBufferList[iBuffer]->Release();
				IndexBufferList[iBuffer] = nullptr;
			}

		}
	}
	TLodPatch() {}
	~TLodPatch()
	{
		Release();
	}
};
// 8421 code 1111
// 0 ~ 16
//     1
// 8   x   2
//     4
// 
class KNode
{
public:
	UINT			m_iLodLevel;
public:
	static      int g_iNewCounter;
	int			m_iIndex;
	POINT		m_Element;
	TRect		m_tRect;
	std::vector<KVector2>  m_ObjectList;
	std::vector<DWORD>  m_CornerList;
	std::vector<DWORD>  m_IndexList;
	ID3D11Buffer*		m_pIndexBuffer;	
	std::vector<PNCT_VERTEX>  m_pVertexList;
	ID3D11Buffer*		m_pVertexBuffer;
	KVector3			m_vCenter;
	KNode*		m_pChild[4];
	KNode*		m_NeighborList[4];
	int			m_iDepth;
	bool		m_bLeaf;
	KNode*		m_pParent;
public:	
	bool		AddObject(float fX, float fY);
	bool		AddObject(KVector2 pos);
	bool		IsRect(KVector2 pos);
	void		SetRect(float x, float y, float w, float h);
public:
	KNode() {};
	KNode(UINT x, UINT y, UINT w, UINT h);
	~KNode();
};

