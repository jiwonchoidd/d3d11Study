#include "KInput.h"

INPUT_MAP g_InputData;

bool KInput::Init()
{    
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
    // ��ġ�� ��ȯ ������ ����
    m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
    // ��ġ�� ���� �� ���� ����(��������)
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

bool KInput::ShutDownDirectInput()
{
    if (m_pMouseDevice)
    {
        m_pMouseDevice->Unacquire();
        m_pMouseDevice->Release();
        m_pMouseDevice = NULL;
    }
    if (m_pKeyDevice)
    {
        m_pKeyDevice->Unacquire();
        m_pKeyDevice->Release();
        m_pKeyDevice = NULL;
    }
    if (m_pDI)
    {
        m_pDI->Release();
        m_pDI = NULL;
    }
    return true;
}


bool KInput::Frame()
{
    // ȭ����ǥ
    GetCursorPos(&m_ptPos);
    // Ŭ���̾�Ʈ ��ǥ
    ScreenToClient(g_hWnd, &m_ptPos);

    #pragma region ���̷�Ʈ ��ǲ ���� �о����
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

    #pragma region  ���콺 ����
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
  
    g_InputData.bWKey = GetKey(DIK_W);
    g_InputData.bAKey = GetKey(DIK_A);
    g_InputData.bSKey = GetKey(DIK_S);
    g_InputData.bDKey = GetKey(DIK_D);

    g_InputData.bLShift = GetKey(DIK_LSHIFT);

    g_InputData.bLeftKey = GetKey(DIK_LEFT);
    g_InputData.bRightKey = GetKey(DIK_RIGHT);
    g_InputData.bUpKey = GetKey(DIK_UP);
    g_InputData.bDownKey = GetKey(DIK_DOWN);
    g_InputData.bExit = GetKey(DIK_ESCAPE);
    g_InputData.bSpace = GetKey(DIK_SPACE);
    g_InputData.bExit = GetKey(DIK_ESCAPE);

    if (GetKey(DIK_F5) == KEY_HOLD) 	
    g_InputData.bChangeFillMode = true;

    return true;
}

BYTE KInput::GetKey(BYTE dwKey)
{
    BYTE sKey;
    sKey = m_KeyState[dwKey];
    if (sKey & 0x80)
    {
        if (m_KeyStateOld[dwKey] == KEY_FREE)
            m_KeyStateOld[dwKey] = KEY_PUSH;
        else
            m_KeyStateOld[dwKey] = KEY_HOLD;
    }
    else
    {
        if (m_KeyStateOld[dwKey] == KEY_PUSH ||
            m_KeyStateOld[dwKey] == KEY_HOLD)
            m_KeyStateOld[dwKey] = KEY_UP;
        else
            m_KeyStateOld[dwKey] = KEY_FREE;
    }
    return m_KeyStateOld[dwKey];
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
    ShutDownDirectInput();
}