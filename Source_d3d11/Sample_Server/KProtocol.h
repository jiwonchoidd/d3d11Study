#pragma once
#include <windows.h>
/*
* 패킷 앞에 길이와 타입을 정한 정보를 전송
* TCP의 패킷 분할되는 특성을 이용해
* PACKET_HEADER를 붙여서 신뢰성을 보장함.
* 메세지에 대한 길이 + 패킷에 대한 헤더 사이즈
* WORD WORD 4바이트 + strlen(msg)
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
	char	name[20];		// 이름
	short	damage;
	char	message[256];	// 내용
};
#define PACKET_HEADER_SIZE	4
#define	PACKET_CHAT_MSG		1000