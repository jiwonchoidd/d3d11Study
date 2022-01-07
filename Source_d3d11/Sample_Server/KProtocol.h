#pragma once
#include <windows.h>
/*
* ��Ŷ �տ� ���̿� Ÿ���� ���� ������ ����
* TCP�� ��Ŷ ���ҵǴ� Ư���� �̿���
* PACKET_HEADER�� �ٿ��� �ŷڼ��� ������.
* �޼����� ���� ���� + ��Ŷ�� ���� ��� ������
* WORD WORD 4����Ʈ + strlen(msg)
*/
#pragma pack (push,1)
typedef struct
{
	WORD		len;
	WORD		type;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER	ph;
	char			msg[4096];
}UPACKET, * P_UPACKET;
#pragma pack(pop)

struct KChatting
{
	long	index;
	char	name[20];		// �̸�
	short	damage;
	char	message[256];	// ����
};
#define PACKET_HEADER_SIZE	4
#define	PACKET_CHAT_MSG		1000