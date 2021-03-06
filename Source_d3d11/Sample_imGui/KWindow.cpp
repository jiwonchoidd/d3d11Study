#include "KWindow.h"
#include <assert.h>
#include <sstream>
#include "resource.h"
#include "ImGui/imgui_impl_win32.h"
KWindow* g_pWindow = nullptr;

HWND  g_hWnd;
RECT  g_rtClient;

//RESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 메세지 핸들링
    assert(g_pWindow);
    return g_pWindow->MsgProc(hWnd, message, wParam, lParam);    
}
LRESULT  KWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    //case WM_KEYDOWN:
    //    if (wParam == 'F')
    //    {
    //        SetWindowText(g_hWnd, L"F을 눌렀다가");
    //    }
    //    break;
    //case WM_KEYUP:
    //    if (wParam == 'F')
    //    {
    //        SetWindowText(g_hWnd, L"F를 떼었습니다.");
    //    }
    //    break;
    //case WM_CHAR:
    //{
    //    static std::wstring title;
    //    title.push_back((char)wParam);
    //    SetWindowText(g_hWnd, title.c_str());
    //}
    //    break;
    case WM_LBUTTONDOWN:
    {
        POINTS pt = MAKEPOINTS(lParam);
        std::ostringstream oss;
        oss << "(" << pt.x << "," << pt.y << ")";
        SetWindowTextA(g_hWnd, oss.str().c_str());
    }
    break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// 1. 윈도우 클래스 등록
// 2. 윈도우 생성
bool   KWindow::InitWindows(
	HINSTANCE hInstance,
	int nCmdShow,
	const WCHAR* strWindowTitle, int width, int height)
{
	m_hInstance = hInstance;

    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; //메시지 핸들러 등록
    wcex.hInstance = hInstance;
    wcex.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wcex.lpszClassName = L"dd_ch";
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hIcon = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wcex.hIconSm = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

    if (!RegisterClassExW(&wcex))
    {
        return false;
    }
    RECT rc = { 0, 0, width, height };
    // 작업영역(  타이틀 바/경계선/메뉴/스크롤 바 등의 영역을 제외한 영역), 윈도우 스타일, 메뉴여부
    // Chilil가 이게 베스트 세팅이라고 함
    AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// 윈도우 생성 함수
	m_hWnd = CreateWindowExW(
        0,
        L"dd_ch",
        strWindowTitle,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        0,
        0,
        rc.right-rc.left,
        rc.bottom-rc.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (m_hWnd == NULL)
    {
        return false;
    }
   
    GetWindowRect(m_hWnd, &m_rtWindow);
    GetClientRect(m_hWnd, &m_rtClient);
    
    g_hWnd = m_hWnd;
    g_rtClient = m_rtClient;
    
    // SW_SHOW, nCmdShow
    
    ShowWindow(m_hWnd, SW_SHOW);

    ImGui_ImplWin32_Init(m_hWnd);

	return true;
}

bool	KWindow::Run()
{
    //다이렉트 초기화
    GameInit();
    //메세지 const
    MSG msg;
    while (m_bGameRun)
    {
        //
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_bGameRun = GameRun();            
        }
    }
    GameRelease();
	return true;
}
bool	KWindow::GameRun()
{
    return true;
}
bool	KWindow::GameInit()
{
    return true;
}
bool	KWindow::GameRelease() 
{
    return true;
}
KWindow::KWindow() : m_bGameRun(true)
{
    g_pWindow = this;
}

KWindow::~KWindow()
{
    ImGui_ImplWin32_Shutdown();
}
   