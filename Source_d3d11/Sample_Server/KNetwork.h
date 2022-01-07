#pragma once
#include "KNetworkUser.h"
class KNetwork
{
public:
	SOCKET m_ListenSocket;
public:
	bool	InitNetwork();
	bool	CloseNetwork();
	bool	InitServer(int protocol, int iport, const char* ip = nullptr);
};

