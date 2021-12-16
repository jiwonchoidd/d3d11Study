#pragma once
#include <assert.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <windows.h>
#include "KMath.h"
#include "d3dtk/SimpleMath.h"
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <fbxsdk.h>
/////////////////////////////////////
#include <bitset>
#include <limits>
#include <functional>
#include <unordered_map> // unordered_map
#include <codecvt>
#include <mmsystem.h> //timeGetTime
#include <wrl.h>//Windows Runtime C++ Template Library (WRL).
/////////////////    ������ �ֿ� �ɼ� //////////////
// error C7510: 'Callback': ������ ���ø� �� ��
// 1)C++ --> ��� -->  �ؼ���� : �ƴϿ� 
// 2)C++ --> �Ϲ� -->  ��� ������ ó��  : �ƴϿ�
#include <memory>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace std;

#pragma comment (lib, "D3DCompiler.lib")
#pragma comment	(lib, "d3d11.lib")
#pragma comment		(lib, "dxgi.lib")
#pragma comment (lib, "EngineCore.lib")
#pragma comment	(lib, "DirectXTK.lib")

using namespace DirectX;
using namespace DirectX::SimpleMath;

#pragma region wWinMain ��ũ��

//�⺻ ��ũ��
#define WinSTART int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow){
#define WinRUN Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L"lib"); g_Sample.Run();}
#define WinMain WinSTART WinRUN

//��Ʈ��ȭ�����ڸ� �̿��� ��ũ��
#define WinRUN_OPT(NAME, WIDTH, HEIGHT) Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#NAME, WIDTH, HEIGHT); g_Sample.Run();}
#define WinMain_OPT(NAME, WIDTH, HEIGHT) WinSTART WinRUN_OPT(NAME, WIDTH, HEIGHT)
//��ǲ ��ũ��

#pragma endregion

extern float g_fTimer;
extern float g_fSecPerFrame;
extern HWND  g_hWnd;
extern RECT  g_rtClient;
extern ID3D11Device* g_pd3dDevice;		// ����̽� ��ü �ܺκ���

//�̱���
template<class T>
class TSingleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
//////////////////////////////////////////////
// ��ü �� �迭 �Ҵ�� ���� �� �Ҹ� ��ũ��
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

namespace TBASIS
{
	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STG;
	typedef basic_string<char>  C_STR;
	typedef vector<T_STR>		T_STR_VECTOR;
	typedef basic_string<TCHAR>::iterator	T_ITOR;
	typedef basic_string<wchar_t>::iterator	W_ITOR;
	typedef basic_string<char>::iterator	C_ITOR;
	typedef vector<T_STR>		T_ARRAY_ITOR;
	typedef vector<DWORD>				DWORD_VECTOR;
	typedef	vector< DWORD >::iterator	DWORD_VECTOR_ITOR;
	typedef list<DWORD>					DWORD_LIST;
	typedef list<DWORD>::iterator		DWORD_LIST_ITOR;
	typedef list< HANDLE >				HANDLE_LIST;
	typedef	list< HANDLE >::iterator	HANDLE_LIST_ITOR;

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// static
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//��� �ھƵα�
	static TCHAR g_szDataPath[] = L"../../data/";

	// ��Ƽ����Ʈ �������� �����
	// ABCȫ�浿 = 9
	// [A][B][C] [0]
	// �����ڵ����Ʈ �������� �����
	// ABCȫ�浿 = 12
	// [A][0][B][0][C][0] [][][][][][]
	static std::wstring mtw(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(str);
	}
	//#include <codecvt>
	static std::string wtm(std::wstring str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(str);
	}
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  �ҽ�
			retData, iLength, // ���
			NULL, NULL);
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			src, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			src, -1,  //  �ҽ�
			pDest, iLength, // ���
			NULL, NULL);
		if (iRet == 0) return false;
		return true;
	}
	static WCHAR* GetMtW(char* data)
	{
		WCHAR retData[4096] = { 0 };
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			data, -1,  //  �ҽ�
			retData, iLength); // ���
		return retData;
	}
	static bool GetMtW(char* pSrc, WCHAR* pDest)
	{
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1,  //  �ҽ�
			pDest, iLength); // ���		
		if (iRet == 0) return false;
		return true;
	}
	static void PRINT(char* fmt, ...) // ����������
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}

	static void ErrorQuit(TCHAR* msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}
}