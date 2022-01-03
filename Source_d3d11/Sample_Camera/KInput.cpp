#include "KInput.h"

INPUT_MAP g_InputData;

bool KInput::Init()
{    
    ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
    InitDirectInput();
    return true;
}
bool KInput::InitDirectInput()
{
    HRESULT hr = S_OK;
    if (FAILED(hr = DirectInput8Create(g_hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&m_pDI, NULL)))
    {
        return false;
    }
    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard,
        &m_pKeyDevice, NULL)))
    {
        return false;
    }
    // 장치별 반환 데이터 설정
    m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
    // 장치별 독점 및 비독점 설정(협조레벨)
    if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(
        g_hWnd,
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND |
        DISCL_NOWINKEY)))
    {
        return false;
    }

    while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);


    if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse,
        &m_pMouseDevice, NULL)))
    {
        return false;
    }
    m_pMouseDevice->SetDataFormat(&c_dfDIMouse);

    if (FAILED(hr = m_pMouseDevice->SetCooperativeLevel(
        g_hWnd,
        DISCL_NONEXCLUSIVE |
        DISCL_FOREGROUND)))
    {
        return true;
    }
    while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
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
    return true;
}
bool KInput::OnMouseDragBegin(float x, float y)
{
    //클릭 시작할때
    m_bDrag = true;
    m_pPrevMouse.x = x;
    m_pPrevMouse.y = y;
    return true;
}
bool KInput::OnMouseDragEnd()
{
    //클릭이 종료되면 x,y 값을 0을
    m_bDrag = false;
    m_pMouseMove.x = 0;
    m_pMouseMove.y = 0;
    return true;
}

bool KInput::Frame()
{
    // 화면좌표
    GetCursorPos(&m_ptPos);
    // 클라이언트 좌표
    ScreenToClient(g_hWnd, &m_ptPos);

    #pragma region 다이렉트 인풋 상태 읽어오기
    HRESULT hr;
    if (m_pMouseDevice == NULL || m_pKeyDevice == NULL) return false;

    if (FAILED(hr = m_pKeyDevice->GetDeviceState(256, m_KeyState)))
    {
        while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);
    }
    if (FAILED(hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
    {
        while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
    }
    #pragma endregion

    #pragma region  마우스 상태
    for (int iButton = 0; iButton < 3; iButton++)
    {
        m_BeforeMouseState[iButton] = m_DIMouseState.rgbButtons[iButton];
    }
    for (int iButton = 0; iButton < 3; iButton++)
    {
        if (m_BeforeMouseState[iButton] & 0x80)
        {
            if (m_MouseState[iButton] == KEY_FREE)
                m_MouseState[iButton] = KEY_PUSH;
            else
                m_MouseState[iButton] = KEY_HOLD;
        }
        else
        {
            if (m_MouseState[iButton] == KEY_PUSH ||
                m_MouseState[iButton] == KEY_HOLD)
                m_MouseState[iButton] = KEY_UP;
            else
                m_MouseState[iButton] = KEY_FREE;
        }
    }

    ZeroMemory(&g_InputData, sizeof(INPUT_MAP));

    if (m_MouseState[0] == KEY_PUSH) 	g_InputData.bLeftClick = true;
    if (m_MouseState[1] == KEY_PUSH) 	g_InputData.bRightClick = true;
    if (m_MouseState[2] == KEY_PUSH) 	g_InputData.bMiddleClick = true;

    if (m_MouseState[0] >= KEY_PUSH) 	g_InputData.bLeftHold = true;
    if (m_MouseState[1] >= KEY_PUSH) 	g_InputData.bRightHold = true;
    if (m_MouseState[2] >= KEY_PUSH) 	g_InputData.bMiddleHold = true;

    g_InputData.iMouseValue[0] = m_DIMouseState.lX;
    g_InputData.iMouseValue[1] = m_DIMouseState.lY;
    g_InputData.iMouseValue[2] = m_DIMouseState.lZ;
    #pragma endregion
  
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
                m_dwKeyState[iKey] == KEY_HOLD)
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
KInput::KInput() 
{

}
KInput::~KInput() 
{

}