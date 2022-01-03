#include "Sample.h"

bool Sample::Init()
{
    m_Camera.CreateViewMatrix(KVector3(0, 0, -20), KVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.25f, (float)g_rtClient.right / (float)g_rtClient.bottom);
    return true;
}
bool Sample::Frame()
{
    return true;
}
bool Sample::Render()
{
    DrawTestBox(g_fSecTimer * 2.5 * m_Speed,
        0.0f, 0.0f, 1.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
    DrawTestBox(g_fSecTimer * 2 * m_Speed,
        0.0f, 3.0f, 2.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
    DrawTestBox(g_fSecTimer * 1.5 * m_Speed,
        0.0f, 6.0f, 3.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
    DrawTestBox(g_fSecTimer * 1 * m_Speed,
        0.0f, 9.0f, 4.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);
    return true;
}
bool Sample::Release()
{
    m_Camera.Release();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(Å×½ºÆ®, 800, 600);