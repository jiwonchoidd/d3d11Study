#pragma once
#include "KNetwork.h"
//�޼��� ����� ����
#define NETWORK_MSG WM_USER+1
class KAsyncSelect : public KNetwork
{
public:
	bool	m_bConnect = false;
public:
	bool Connect(HWND hWnd, int protocol, int iport, const char* ip);
	LRESULT  Select_MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

