#include "KNetwork.h"

bool KNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}

bool KNetwork::InitServer(int protocol, int iport, const char* ip)
{
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iport);
	if (ip == nullptr)
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sa.sin_addr.s_addr = inet_addr(ip);
	}

	//소켓에 주소 할당
	int iRet = bind(m_Sock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return false;

	//클라이언트로 부터 연결 요청을 처리할수있는 상태를 만듬 
	iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return false;
	return true;
}
int KNetwork::SendMsg(SOCKET sock, char* msg, WORD type)
{
	return 0;
}
int KNetwork::SendMsg(SOCKET sock, UPACKET& packet)
{
	return 0;
}
int KNetwork::AddUser(SOCKET sock)
{
	return 0;
}
int KNetwork::RecvUser(KNetworkUser& user)
{
	return 0;
}
bool KNetwork::CloseNetwork()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}
