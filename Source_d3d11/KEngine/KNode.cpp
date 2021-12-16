#include "KNode.h"
int   KNode::g_iNewCounter = 0;

void		KNode::SetRect(float x, float y, float w, float h)
{
	this->m_tRect.wh = KVector2(w, h);
	this->m_tRect.p0 = KVector2(x, y);
	this->m_tRect.p1.x = this->m_tRect.p0.x + w;
	this->m_tRect.p1.y = this->m_tRect.p0.y - h;
	m_vCenter.x = x + (w / 2.0f);
	m_vCenter.y = 0.0f;
	m_vCenter.z = y - (h / 2.0f);
}
bool KNode::AddObject(float fX, float fY)
{
	m_ObjectList.push_back(KVector2(fX, fY));
	return true;
}

bool KNode::AddObject(KVector2 pos)
{
	m_ObjectList.push_back(pos);
	return true;
}
bool		KNode::IsRect(KVector2 pos)
{
	if (this->m_tRect.p0.x <= pos.x &&
		this->m_tRect.p1.x >= pos.x &&
		this->m_tRect.p1.y <= pos.y &&
		this->m_tRect.p0.y >= pos.y)
	{
		return true;
	}
	return false;
}
KNode::KNode(UINT x, UINT y, UINT w, UINT h)
{
	m_pParent = nullptr;
	m_iDepth = 0;
	m_bLeaf = false;
	m_CornerList.push_back(x);
	m_CornerList.push_back(y);
	m_CornerList.push_back(w);
	m_CornerList.push_back(h);
}
KNode::~KNode()
{
	for (int iChild = 0; iChild < 4; iChild++)
	{
		if (m_pChild[iChild] != nullptr)
		{
			delete m_pChild[iChild];
			m_pChild[iChild] = nullptr;
			KNode::g_iNewCounter--;
		}
	}	
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
}