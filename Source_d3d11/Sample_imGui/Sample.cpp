#include "Sample.h"

bool Sample::Init()
{
    return true;
}
bool Sample::Frame()
{
    return true;
}
bool Sample::Render()
{
    return true;
}
bool Sample::Release()
{
    return true;
}

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sample g_Sample;
    g_Sample.InitWindows(
        hInstance,
        nCmdShow,
        L"TEST");
    MessageBoxW(NULL, L"토렌트 버젼입니다.", L"Warning.", MB_OK);
    //a 멀티바이트
    //w 유니바이트
    g_Sample.Run();
}