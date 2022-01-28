#include "KBoxObj.h"

bool KBoxObj::Init()
{
	const KVector3 vertices[] =
	{
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{-1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},
		{-1.0f,-1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
		{-1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},
	};
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4,
	};
	for (int num = 0; num < sizeof(vertices); num++)
	{
		PNCT_VERTEX pnct;
		pnct.pos = vertices[num];
		m_VertexList.push_back(pnct);
	}

	for (int num = 0; num < sizeof(indices); num++)
	{
		m_IndexList.push_back(indices[num]);
	}


	if (CreateObject(L"VertexShader.hlsl",
		L"PixelShader.hlsl",
		L"..\\..\\data\\texture\\Skybox_dd.dds"))
	{
		return true;
	}

	return false;
}

bool KBoxObj::Frame()
{
	return true;
}

bool KBoxObj::Render()
{
	return true;
}

bool KBoxObj::Release()
{
	return true;
}

KBoxObj::KBoxObj()
{
}

KBoxObj::~KBoxObj()
{
}
