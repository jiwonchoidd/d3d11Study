#pragma once
#include <windows.h>
#include < tchar.h >
#include <string>
#include <map>
#include <vector>
#include <list>
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>

#pragma comment	(lib, "d3d11.lib")
#pragma comment		(lib, "dxgi.lib")

extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;

template<class T>
class Singleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};