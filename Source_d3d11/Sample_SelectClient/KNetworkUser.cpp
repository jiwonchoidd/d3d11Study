#include "KNetworkUser.h"

int KNetworkUser::DispatchRead(char* sRecvBuffer, int iRecvByte)
{
	//m_szRecvBuffer 2048�� ������ �޸� ��ġ �ٲ�, �ʱ�ȭ
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}

	memcpy(&m_szRecvBuffer[m_iWritePos], sRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;// ���ۿ� ������ ����� ��ġ
	m_iReadPos += iRecvByte; // ��Ŷ���� ��ġ�κ��� ���� ����Ʈ
	
	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// ��Ŷ �ؼ� ����
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// ��� 1���� ��Ŷ�� �����ߴٸ�
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				KPacket kPacket(pPacket->ph.type);
				memcpy(&kPacket.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);
				m_lPacketPool.push_back(kPacket);

				// ������Ŷ ó��
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);
		}
	}
	return 1;
}

void KNetworkUser::set(SOCKET sock, SOCKADDR_IN addr)
{
	m_bConnect = true;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;

	m_Sock = sock;
	m_Addr = addr;
	//��Ʈ��ũ �ּ� ��ȯ �Լ� �򿣵��(32bit) ->  
	m_csName = inet_ntoa(addr.sin_addr);
	//������� �޸� ���ӵ� ����� �迭�ϴ� ������� 
	// Network Byte ������ To Host�� Byte ������ �ٲ�
	m_iPort = ntohs(addr.sin_port);
}
