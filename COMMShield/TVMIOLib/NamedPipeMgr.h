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

	int create();												// 创建命名管道服务器端
	DWORD write(const char* buff, unsigned int len);			// 将数据写入管道
	DWORD read(char* buff, unsigned int maxlen);				// 从管道中读取数据
	bool close();												// 关闭命名管道
	long GetStat();												// 取得当前通信状态
	HANDLE pipe_;
private:
	OVERLAPPED olw_;
	OVERLAPPED olr_;
	char *name_;			// 管道名称
	long m_pipeStat;		// 当前通信状态
};
