#pragma once
#include "KModel.h"
struct KMapInfo
{
    int m_iNumRow; // 2^n+1
    int m_iNumCol;
    int m_iNumRowCell;
    int m_iNumColCell;
    int m_iNumVertex;
    float m_fCellDistance;
    wstring szDefaultTexture;
};
class KMap :public KModel
{
public: 
    KMapInfo m_info;
public:
    bool    Load(KMapInfo& info, std::wstring vs, std::wstring ps);
	bool	CreateVertexData() override;
    bool	CreateIndexData() override;
   
};

