#pragma once
#include "KCore.h"
class Sample : public KCore
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};
