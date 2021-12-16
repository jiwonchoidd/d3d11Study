#include "KShape.h"
bool KPlaneShape::CreateVertexData()
{
    m_pVertexList.resize(4);

    m_pVertexList[0].pos = KVector3(-1.0f, 1.0f, 0.0f);
    m_pVertexList[0].normal = KVector3(0.0f, 0.0f, -1.0f);
    m_pVertexList[0].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_pVertexList[0].tex = KVector2(0.0f, 0.0f);

    m_pVertexList[1].pos = KVector3(1.0f, 1.0f, 0.0f);
    m_pVertexList[1].normal = KVector3(0.0f, 0.0f, -1.0f);
    m_pVertexList[1].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_pVertexList[1].tex = KVector2(1.0f, 0.0f);


    m_pVertexList[2].pos = KVector3(-1.0f, -1.0f, 0.0f);
    m_pVertexList[2].normal = KVector3(0.0f, 0.0f, -1.0f);
    m_pVertexList[2].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_pVertexList[2].tex = KVector2(0.0f, 1.0f);


    m_pVertexList[3].pos = KVector3(1.0f, -1.0f, 0.0f);
    m_pVertexList[3].normal = KVector3(0.0f, 0.0f, -1.0f);
    m_pVertexList[3].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
    m_pVertexList[3].tex = KVector2(1.0f, 1.0f);
    return true;
}
bool KPlaneShape::CreateIndexData()
{
    m_IndexList.resize(6);
    m_IndexList[0] = 0;
    m_IndexList[1] = 1;
    m_IndexList[2] = 2;
    m_IndexList[3] = 2;
    m_IndexList[4] = 1;
    m_IndexList[5] = 3;
    return true;
}