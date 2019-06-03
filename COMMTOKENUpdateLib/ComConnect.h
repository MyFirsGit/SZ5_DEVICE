#pragma once
#include "windows.h"
// TOKEN�������Ӳ����ṹ
typedef struct _token_connect_param
{
	DWORD ComPort;			// ���ں�
	DWORD Speed;			// ������		
	DWORD MaxSendCnt;		// ���Ͷ�����Ϣ�������ֵ
	DWORD MaxSendBuffSize;	// �������ݻ�������󳤶�
	DWORD MaxRecvCnt;		// ���ն�����Ϣ�������ֵ
	DWORD MaxRecvBuffSize;	// �������ݻ�������󳤶�
} TOKEN_CONNECT_PARAM, *LPTOKEN_CONNECT_PARAM;

// ����ͨ��ģ��������
class  ComConnect
{
public:
	int open(TOKEN_CONNECT_PARAM Para,HANDLE& hCom);
	int close(HANDLE& hCom);
	int read(int max_sz, LPBYTE buff,HANDLE& hCom);
	int write(int sz, LPBYTE buff,HANDLE& hCom);
	ComConnect(void);
	~ComConnect(void);
};

