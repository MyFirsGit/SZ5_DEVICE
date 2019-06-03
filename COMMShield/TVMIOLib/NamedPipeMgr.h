#pragma once
#include <windows.h>
#include "IOLib.h"

#define PIPE_INSTANCES 1			// max. instances  
#define IOBUFFERSIZE 3000           // output buffer size
#define PIPE_TIMEOUT 0              // client time-out 

class NamedPipeMgr  
{
public:
	
	NamedPipeMgr();
	virtual ~NamedPipeMgr();

	int create();												// ���������ܵ���������
	DWORD write(const char* buff, unsigned int len);			// ������д��ܵ�
	DWORD read(char* buff, unsigned int maxlen);				// �ӹܵ��ж�ȡ����
	bool close();												// �ر������ܵ�
	long GetStat();												// ȡ�õ�ǰͨ��״̬
	HANDLE pipe_;
private:
	OVERLAPPED olw_;
	OVERLAPPED olr_;
	char *name_;			// �ܵ�����
	long m_pipeStat;		// ��ǰͨ��״̬
};
