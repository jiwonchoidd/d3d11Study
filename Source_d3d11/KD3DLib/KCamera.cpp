#include "KCamera.h"
#include "KInput.h"

#pragma region Camera

bool KCamera::Init()
{
    return true;
}
bool KCamera::Frame()
{
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

    if (g_InputData.bWKey)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * m_fSpeed * g_fSecPerFrame;
    }
    if (g_InputData.bSKey)
    {
        m_vCameraPos = m_vCameraPos + m_vLook * -m_fSpeed * g_fSecPerFrame;
    }
    if (g_InputData.bDKey)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * m_fSpeed * g_fSecPerFrame;
        m_vCameraTarget.x += m_fSpeed * g_fSecPerFrame;
    }
    if (g_InputData.bAKey)
    {
        m_vCameraPos = m_vCameraPos + m_vSide * -m_fSpeed * g_fSecPerFrame;
        m_vCameraTarget.x -= m_fSpeed * g_fSecPerFrame;
    }

    g_InputData.bWKey ?
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
        m_fYaw += XMConvertToRadians(g_InputData.iMouseValue[0] * 90.0f * g_fSecPerFrame);
        m_fPitch += XMConvertToRadians(g_InputData.iMouseValue[1] * 90.0f * g_fSecPerFrame);
    }
    m_fRoll += 0;
    m_fRadius += 0;

    KQuaternion q;
    //������� ��ķ� ��ȯ�ϰ� ����ķ� ī�޶�
    D3DXQuaternionRotationYawPitchRoll(&q, m_fYaw, m_fPitch, m_fRoll);
    KMatrix matRotation;
    D3DKMatrixAffineTransformation(&matRotation, 1.0f, NULL, &q, &m_vCameraPos);
    D3DKMatrixInverse(&m_matView, NULL, &matRotation);
    //�� ������ ���� ������� �����.
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




