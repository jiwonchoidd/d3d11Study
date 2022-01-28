#pragma once
#include "KBoxObj.h"
class KSkyObj : public KBoxObj
{
public:
	void SetMatrix(KMatrix* pWorld, KMatrix* pView, KMatrix* pProj);
	KSkyObj();
	~KSkyObj();
};

