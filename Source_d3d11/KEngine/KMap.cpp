#include "KMap.h"
bool    KMap::Load(KMapInfo& info, std::wstring vs, std::wstring ps)
{
    m_info = info;
    m_info.m_iNumColCell = m_info.m_iNumCol - 1;
    m_info.m_iNumRowCell = m_info.m_iNumRow - 1;
    m_info.m_iNumVertex = m_info.m_iNumCol * m_info.m_iNumRow;
    return CreateModel(vs, ps, info.szDefaultTexture);
}
bool	KMap::CreateVertexData()
{
    m_pVertexList.resize(m_info.m_iNumVertex);

    int iIndex = 0;
    for (int iRow=0; iRow < m_info.m_iNumRow; iRow++)
    {
        for (int iCol = 0; iCol < m_info.m_iNumCol; iCol++)
        {
            int iIndex = iRow * m_info.m_iNumRow + iCol;
            m_pVertexList[iIndex].pos.x = m_info.m_fCellDistance * iCol;
            m_pVertexList[iIndex].pos.y = randstep(0.0f, 1.6f);
            m_pVertexList[iIndex].pos.z = -m_info.m_fCellDistance * iRow;
            m_pVertexList[iIndex].color = 
                KVector4(
                    randstep(0.3f, 0.6f),
                    randstep(0.3f, 0.6f),
                    randstep(0.3f, 0.6f),1.0f);     
            //UV texture x, y
            m_pVertexList[iIndex].tex.x = ((float)iCol / (float)m_info.m_iNumCol)*2.0f;
            m_pVertexList[iIndex].tex.y = ((float)iRow / (float)m_info.m_iNumRow)*2.0f;

        }
    }
    if (m_pVertexList.size() > 0) return true;
    return false;
}
bool	KMap::CreateIndexData()
{
    //16ÀÏ¶§ 2°öÇÏ±â 3°öÇÔ 1536
    m_IndexList.resize(m_info.m_iNumColCell * m_info.m_iNumRowCell * 2 * 3);

    int iIndex = 0;
    for (int iRow = 0; iRow < m_info.m_iNumRowCell; iRow++)
    {
        for (int iCol = 0; iCol < m_info.m_iNumColCell; iCol++)
        {
            int iCurrentIndex = iRow * m_info.m_iNumRow + iCol;
            int iNextRow = (iRow+1) * m_info.m_iNumRow + iCol;
            m_IndexList[iIndex+0] = iCurrentIndex;
            m_IndexList[iIndex+1] = iCurrentIndex+1;
            m_IndexList[iIndex+2] = iNextRow;

            m_IndexList[iIndex+3] = m_IndexList[iIndex + 2];
            m_IndexList[iIndex+4] = m_IndexList[iIndex + 1];
            m_IndexList[iIndex+5] = iNextRow + 1;
            iIndex += 6;
        }
    }
    if (m_IndexList.size() > 0) return true;
    return false;
}
