#include "KPacket.h"

void KPacket::PutData(const char* pData, int iSize)
{
	m_uPacket.ph.len += iSize;
	memcpy(m_pOffset, pData, iSize);
	m_pOffset += iSize;
}

void KPacket::GetData(const char* pData, int iSize)
{
	memcpy(const_cast<char*>(pData), m_pOffset, iSize);
	m_pOffset += iSize;
}

//�а� ���� ����
void KPacket::Reset()
{
	m_pOffset = m_uPacket.msg;
}

KPacket::KPacket()
{
	//��Ŷ ���� ��Ŷ ��� ������� ����
	//�������� �޼����� ����
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_pOffset = m_uPacket.msg;
}

KPacket::KPacket(WORD iType)
{
	//Ÿ���� �߰������� �޾ƿ��� ������
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_uPacket.ph.type = iType;
	m_pOffset = m_uPacket.msg;
}

KPacket::KPacket(const KPacket& packet)
{
	//��Ŷ ��ü�� �޾ƿ��� ������
	m_uPacket.ph = packet.m_uPacket.ph;
	memcpy(m_uPacket.msg, packet.m_uPacket.msg, sizeof(char)*4096);
	//m_uPacket = packet.m_uPacket;
	m_pOffset = m_uPacket.msg;
}

KPacket::~KPacket()
{
}

#pragma region ������ ������ << ������ Putdata
KPacket& KPacket::operator << (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
KPacket& KPacket::operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
KPacket& KPacket::operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
KPacket& KPacket::operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
KPacket& KPacket::operator<<(byte data)
{
	PutData((char*)&data, sizeof(byte));
	return *this;
}
KPacket& KPacket::operator << (char* data)
{
	//���ڿ����� ���� ���Եȴ�.
	//���ڿ��� ������ null ���ڰ� ���ԵǾ� �ֱ� ����
	PutData(data, strlen(data) + 1);
	return *this;
}
KPacket& KPacket::operator<<(DWORD data)
{
	PutData((char*)&data, sizeof(DWORD));
	return *this;
}
KPacket& KPacket::operator << (std::string data)
{
	//���� ���Ե��� �ʾƼ� +1 ���ش�.
	PutData(data.c_str(), data.size() + 1);
	return *this;
}
#pragma endregion

#pragma region ������ ������ >> ������ Getdata
KPacket& KPacket::operator >> (int& data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
KPacket& KPacket::operator >> (long& data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
KPacket& KPacket::operator >> (short& data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
KPacket& KPacket::operator >> (float& data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
KPacket& KPacket::operator>>(byte& data)
{
	GetData((char*)&data, sizeof(byte));
	return *this;
}
KPacket& KPacket::operator >> (char* data)
{
	int iSize = strlen(m_pOffset) + 1;
	GetData(data, iSize);
	return *this;
}
KPacket& KPacket::operator>>(DWORD& data)
{
	GetData((char*)&data, sizeof(DWORD));
	return *this;
}
KPacket& KPacket::operator >> (std::string& data)
{
	int iSize = strlen(m_pOffset + 1);
	GetData(data.c_str(), iSize);
	return *this;
}
#pragma endregion


