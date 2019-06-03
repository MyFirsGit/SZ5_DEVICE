#pragma once
#include "windows.h"
// TOKEN建立连接参数结构
typedef struct _token_connect_param
{
	DWORD ComPort;			// 串口号
	DWORD Speed;			// 波特率		
	DWORD MaxSendCnt;		// 发送队列消息数量最大值
	DWORD MaxSendBuffSize;	// 发送数据缓存区最大长度
	DWORD MaxRecvCnt;		// 接收队列消息数量最大值
	DWORD MaxRecvBuffSize;	// 接收数据缓存区最大长度
} TOKEN_CONNECT_PARAM, *LPTOKEN_CONNECT_PARAM;

// 串口通信模块类声明
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

