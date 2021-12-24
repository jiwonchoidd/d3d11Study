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
 /*   int check=MessageBox(g_hWnd, L"프로그램 시작?", L"WARNING", MB_ICONQUESTION | MB_OKCANCEL);
    if (check == IDOK)
    {
        MessageBox(g_hWnd, L"정말로?", L"WARNING", MB_ICONQUESTION | MB_OKCANCEL);
    }
    else
    {
        return 0;
    }*/
    g_Sample.Run();
}