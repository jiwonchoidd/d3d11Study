#pragma once
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include <string>
#include "KPacket.h"
#pragma comment	(lib, "ws2_32.lib")

class KNetworkUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	bool		m_bConnect = false;
	// buffer ����� �ʿ���
	char		m_szRecvBuffer[2048];
	int			m_iPacketPos; // ��Ŷ�� ���� �ּ�
	int			m_iWritePos; // ������ ���� ��ġ 
	int			m_iReadPos; // ��Ŷ ���� ��ġ�� ���� ���� ����Ʈ
	std::list<KPacket> m_lPacketPool;
	int DispatchRead(char* sRecvBuffer, int iRecvByte);
	void set(SOCKET sock, SOCKADDR_IN addr);
};

