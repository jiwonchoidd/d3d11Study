#pragma once
#include "KObject.h"
class KBoxObj : public KObject
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	KBoxObj();
	~KBoxObj();
};

