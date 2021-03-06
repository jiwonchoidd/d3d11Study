#include "KCamera.h"
#include "KInput.h"

#pragma region Camera

bool KCamera::Init()
{
    return true;
}
bool KCamera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * -m_fSpeed * g_fSecPerFrame;
    }

    g_Input.GetKey(VK_SHIFT) >= KEY_HOLD ?
        m_fSpeed += 60.0f * g_fSecPerFrame : m_fSpeed -= 60.0f * g_fSecPerFrame;

    if (m_fSpeed >= m_fOriginSpeed * 5.0f)
    {
        m_fSpeed = m_fOriginSpeed * 5.0f;
    }
    if (m_fSpeed <= m_fOriginSpeed)
    {
        m_fSpeed = m_fOriginSpeed;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraPos.y -= m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('E') >= KEY_HOLD)
    {
        m_vCameraPos.y += m_fSpeed * g_fSecPerFrame;
    }
    //매 프레임 마다 뷰행렬을 만든다.
    m_matView = CreateViewMatrix(m_vCameraPos, m_vCameraTarget);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;
    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;
    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    return true;
}
//None
bool KCamera::Render()
{
    return true;
}
bool KCamera::Release()
{
    return true;
}
KVector3*   KCamera::GetCameraPos()
{
    return &m_vCameraPos;
}
KMatrix     KCamera::CreateViewMatrix(KVector3 vPos, KVector3 vTarget, KVector3 vUp)
{
    m_vCameraPos = vPos;
    m_vCameraTarget = vTarget;
    D3DKMatrixLookAtLH(&m_matView, &m_vCameraPos, &m_vCameraTarget, &vUp);
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;

    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;

    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    return m_matView;
}
KMatrix     KCamera::CreateProjMatrix(
    float fNear, float fFar, float fFov, float fAspect)
{
    D3DKMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
    return m_matProj;
}
KCamera::KCamera()
{
    m_vCameraPos = { 0, 20, -20.0f };
    m_vCameraTarget = { 0, 0, 0.0f };
    m_fSpeed = 20.0f;
    m_fMouseSensitivity = 50;
    m_fOriginSpeed = m_fSpeed;
}
KCamera::~KCamera()
{
}
#pragma endregion

#pragma region Camera>>DebugCamera
bool KDebugCamera::Frame()
{
    OnMouseRotation();

    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * m_fSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_fSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * -m_fSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_fSpeed * g_fSecPerFrame;
    }

    g_Input.GetKey(VK_SHIFT) >= KEY_HOLD ?
        m_fSpeed += 60.0f * g_fSecPerFrame : m_fSpeed -= 60.0f * g_fSecPerFrame;

    if (m_fSpeed >= m_fOriginSpeed * 5.0f)
    {
        m_fSpeed = m_fOriginSpeed * 5.0f;
    }
    if (m_fSpeed <= m_fOriginSpeed)
    {
        m_fSpeed = m_fOriginSpeed;
    }

    return true;
}
KMatrix KDebugCamera::OnMouseRotation()
{
    if (g_Input.m_DIMouseState.rgbButtons[1])
    {
        m_fYaw += XMConvertToRadians(g_InputData.iMouseValue[0] * 40.0f * g_fSecPerFrame);
        m_fPitch += XMConvertToRadians(g_InputData.iMouseValue[1] * 40.0f * g_fSecPerFrame);
    }
    m_fRoll += 0;
    m_fRadius += 0;

    KQuaternion q;
    //사원수를 행렬로 변환하고 역행렬로 카메라
    D3DXQuaternionRotationYawPitchRoll(&q, m_fYaw, m_fPitch, m_fRoll);
    KMatrix matRotation;
    D3DKMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DKMatrixInverse(&m_matView, NULL, &matRotation);
    //매 프레임 마다 뷰행렬을 만든다.
    m_vSide.x = m_matView._11;
    m_vSide.y = m_matView._21;
    m_vSide.z = m_matView._31;
    m_vUp.x = m_matView._12;
    m_vUp.y = m_matView._22;
    m_vUp.z = m_matView._32;
    m_vLook.x = m_matView._13;
    m_vLook.y = m_matView._23;
    m_vLook.z = m_matView._33;
    return matRotation;
}
#pragma endregion




