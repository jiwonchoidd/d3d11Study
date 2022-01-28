#pragma once
#include "KCore.h"
#include "KBoxObj.h"
class Sample : public KCore
{
public:
	KBoxObj		m_box;
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

