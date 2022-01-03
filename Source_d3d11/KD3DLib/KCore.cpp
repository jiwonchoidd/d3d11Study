#include "KCore.h"
#include "ImGui/imgui.h"

bool	KCore::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool	KCore::GameInit()
{
    KDevice::SetDevice();
    KState::SetState();
    // TODO : create Timer
    m_Timer.Init();
    g_Input.Init();
    m_Write.Init();
    m_Camera.Init();

    IDXGISurface1* m_pBackBuffer;
    m_pSwapChain->GetBuffer(0, 
        __uuidof(IDXGISurface),
        (void**)&m_pBackBuffer);
    m_Write.CreateDeviceResources(m_pBackBuffer);
    if (m_pBackBuffer)m_pBackBuffer->Release();

    Init();
    return true;
}
bool	KCore::GameFrame() 
{
    // TODO : Frame Timer
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    m_Camera.Frame();
    m_ImGuiManager.Frame();
    if (g_Input.GetKey(DIK_F1) == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }
    if (g_Input.GetKey(DIK_F2) == KEY_PUSH)
    {
        m_ImGuiManager.OnOffImgui();   
    }

    static char buffer[1024];
    if (ImGui::Begin("Simulation Speed"))
    {
        ImGui::InputText("Input text",buffer,sizeof(buffer));
        ImGui::SliderFloat("Speed Factor", &m_Speed, 0.0f, 10.0f);
        ImGui::Text("Average %.3f ms/Frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();

    if (ImGui::Begin("Input Checker"))
    {
        ImGui::Text(" W : %d,\n A : %d,\n S : %d,\n D : %d,\n LShift : %d", g_InputData.bWKey,
            g_InputData.bAKey, g_InputData.bSKey, g_InputData.bDKey, g_InputData.bLShift);
    }
    ImGui::End();

    Frame();
    return true;
}
bool	KCore::GameRender() 
{
    PreRender();
        
        // TODO : Render Timer
        m_Timer.Render();
        g_Input.Render();
        m_Write.Render();
        if (m_bDebugText)
        {
            RECT  rt = { 0, 0, m_rtClient.right, m_rtClient.bottom };
            m_Write.DrawText(rt, m_Timer.m_szTimerString,
                D2D1::ColorF(1, 1, 1, 1));
        }
        
        Render();
        m_ImGuiManager.Render();
    PostRender();
    return true;
}
bool	KCore::PreRender() {
    float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);

    m_pImmediateContext->ClearDepthStencilView(
        m_DepthStencilView,
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pImmediateContext->OMSetRenderTargets(1,
        &m_pRenderTargetView, m_DepthStencilView);

    ApplyDSS(m_pImmediateContext, KState::g_pDSState);

    return true;
}
bool	KCore::Render() {
    return true;
}
bool	KCore::PostRender() {
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool	KCore::GameRelease() 
{
    Release();
    KState::ReleaseState();
    // TODO : Render Timer
    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
    m_Camera.Release();
    CleanupDevice();
    return true;
}
bool	KCore::Init()
{
    return true;
}
bool	KCore::Frame() {
    return true;
}
bool	KCore::Release() {
    return true;
}
