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
    MessageBoxW(NULL, L"�䷻Ʈ �����Դϴ�.", L"Warning.", MB_OK);
    //a ��Ƽ����Ʈ
    //w ���Ϲ���Ʈ
    g_Sample.Run();
}