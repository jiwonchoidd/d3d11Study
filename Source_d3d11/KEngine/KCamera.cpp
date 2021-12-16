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
        m_vCameraPos.z += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos.z -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_PUSH)
    {
        m_vCameraPos.x -= m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_HOLD)
    {
        m_vCameraPos.x += m_pSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('Q') >= KEY_PUSH)
    {
        m_vCameraPos.y -= m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('E') >= KEY_HOLD)
    {
        m_vCameraPos.y += m_pSpeed * g_fSecPerFrame;
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
    m_pSpeed = 20.0f;
    m_pMouseSensitivity = 50;
    m_pOriginSpeed = m_pSpeed;
}
KCamera::~KCamera()
{
}
#pragma endregion

#pragma region Camera>>DebugCamera
bool KDebugCamera::Frame()
{
    if (g_Input.GetKey('W') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('S') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('D') >= KEY_PUSH)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * m_pSpeed * g_fSecPerFrame;
    }
    if (g_Input.GetKey('A') >= KEY_HOLD)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * -m_pSpeed * g_fSecPerFrame;
    }

    g_Input.GetKey(VK_SHIFT) >= KEY_HOLD ?
        m_pSpeed+=60.0f*g_fSecPerFrame : m_pSpeed -=60.0f*g_fSecPerFrame;

    if (m_pSpeed >= m_pOriginSpeed * 5.0f)
    {
        m_pSpeed = m_pOriginSpeed*5.0f;
    }
    if (m_pSpeed <= m_pOriginSpeed)
    {
        m_pSpeed = m_pOriginSpeed;
    }
    return true;
}
KMatrix KDebugCamera::Update(KVector4 vValue)
{
    m_fYaw += vValue.y;
    m_fPitch += vValue.x;
    m_fRoll += vValue.z;
    m_fRadius += vValue.w;
    #pragma region Shake & Clamp
    m_fYaw += randstep(0.01f, 0.0f) * g_fSecPerFrame;
    m_fPitch += randstep(-0.01f, 0.0f) * g_fSecPerFrame;

    if (m_fPitch > 1.0f)m_fPitch = 1.0f;
    if (m_fPitch < -1.0f)m_fPitch = -1.0f;
    #pragma endregion   
    KQuaternion q;
    //사원수를 행렬로 변환하고 역행렬로 카메라
    D3DXQuaternionRotationYawPitchRoll(&q, m_fYaw, m_fPitch, m_fRoll);
    KMatrix matRotation;
    D3DKMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DKMatrixInverse(&m_matView, NULL, &matRotation);
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




