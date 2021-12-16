#include "KInput.h"

void   KInput::OnMove(int iX, int iY)
{
    if (m_bDrag)
    {
        m_pDrag.x = iX - m_pDragDown.x;
        m_pDrag.y = iY - m_pDragDown.y;
    }
    m_pDragDown.x = iX;
    m_pDragDown.y = iY;
    return;
}
void   KInput::OnBegin(int iX, int iY)
{
    //클릭 시작할때
    m_bDrag = true;
    m_pDragDown.x = iX;
    m_pDragDown.y = iY;
    return;
}
void   KInput::OnEnd()
{
    //클릭이 종료되면 x,y 값을 0을
    m_bMove = false;
    m_bDrag = false;
    m_pDrag.x = 0;
    m_pDrag.y = 0;
    return;
}
LRESULT KInput::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int iMouseX = (short)LOWORD(lParam);
    int iMouseY = (short)HIWORD(lParam);
    switch (message)
    {
    case  WM_MOUSEMOVE:
        OnMove(iMouseX, iMouseY);
        return TRUE;
    case WM_LBUTTONDOWN:
        SetCapture(g_hWnd);
        OnBegin(iMouseX, iMouseY);
        return TRUE;
    case WM_LBUTTONUP:
        ReleaseCapture();
        OnEnd();
        return TRUE;
    case WM_MOUSEWHEEL:
    {
        //휠은 -120~120
        m_iWheel = GET_WHEEL_DELTA_WPARAM(wParam);   
        return 0;
    }
    }
    return 0;
}

bool KInput::Init()
{    
    ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
    
    return true;
}
// OR(논리합)
// b   0    1     a
// 0   0    1
// 1   1    1 
// AND(논리곱)
// b   0    1     a
// 0   0    0
// 1   0    1 
// sKey 1000 0000 0000 0000
//   &  1000 0000 0000 0000
DWORD  KInput::GetKey(DWORD dwKey)
{
    return m_dwKeyState[dwKey];
}
bool KInput::Frame()
{
    // 화면좌표
    GetCursorPos(&m_ptPos);
    // 클라이언트 좌표
    ScreenToClient(g_hWnd, &m_ptPos);

    for (int iKey = 0; iKey < 256; iKey++)
    {
        SHORT sKey = GetAsyncKeyState(iKey);
        if (sKey & 0x8000)
        {
            if (m_dwKeyState[iKey] == KEY_FREE)
            {
                m_dwKeyState[iKey] = KEY_PUSH;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_HOLD;
            }
        }
        else
        {
            if (m_dwKeyState[iKey] == KEY_PUSH ||
                m_dwKeyState[iKey] == KEY_HOLD )
            {
                m_dwKeyState[iKey] = KEY_UP;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_FREE;
            }
        }
    }
    return true;
}

bool KInput::Render()
{
    return true;
}

bool KInput::Release()
{
    return true;
}
KInput::KInput() {}
KInput::~KInput() {}

