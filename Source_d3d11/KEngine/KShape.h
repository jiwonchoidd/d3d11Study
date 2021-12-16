#pragma once
#include "KModel.h"

class KShape : public KModel
{
};
class KPlaneShape : public KShape
{
public:
    bool CreateVertexData() override;
    bool CreateIndexData() override;
};
class KBoxShape : public KShape
{
};
