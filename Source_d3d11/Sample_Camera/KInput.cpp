#include "KInput.h"
LRESULT KInput::MsgProc_Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int iMouseX = (short)LOWORD(lParam);
    int iMouseY = (short)HIWORD(lParam);
    switch (message)
    {
    case WM_MOUSEMOVE:
        if (m_bMouseEnabled)
        {
            OnMouseMove(iMouseX, iMouseY);
        }
        return TRUE;
    case WM_RBUTTONDOWN:
        SetCapture(g_hWnd);
        OnMouseDragBegin(iMouseX, iMouseY);
        return TRUE;
    case WM_RBUTTONUP:
        ReleaseCapture();
        OnMouseDragEnd();
        return TRUE;
    case WM_MOUSEWHEEL:
    {
        //���� -120~120
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

DWORD  KInput::GetKey(DWORD dwKey)
{
    return m_dwKeyState[dwKey];
}
bool KInput::OnMouseMove(float x, float y)
{
    if (m_bDrag)
    {
        m_pMouseMove.x = x - m_pPrevMouse.x;
        m_pMouseMove.y = y - m_pPrevMouse.y;
    }
    m_pPrevMouse.x = x;
    m_pPrevMouse.y = y;

    m_pMouseMove.x -= 5.0 * g_fSecPerFrame;
    m_pMouseMove.y -= 5.0 * g_fSecPerFrame;
    return true;
}
bool KInput::OnMouseDragBegin(float x, float y)
{
    //Ŭ�� �����Ҷ�
    m_bDrag = true;
    m_pPrevMouse.x = x;
    m_pPrevMouse.y = y;
    return true;
}
bool KInput::OnMouseDragEnd()
{
    //Ŭ���� ����Ǹ� x,y ���� 0��
    m_bDrag = false;
    m_pMouseMove.x = 0;
    m_pMouseMove.y = 0;
    return true;
}
bool KInput::Frame()
{
    // ȭ����ǥ
    GetCursorPos(&m_ptPos);
    // Ŭ���̾�Ʈ ��ǥ
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