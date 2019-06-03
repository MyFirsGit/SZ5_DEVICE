#pragma once
#include "ComConnect.h"


#define STX				0xAA		// 报文开始标记码
#define	MAX_BUFFER_SIZE	3000		// 最大缓冲区长度
#define MAX_RECEIVE_DATA_LENGTH 256	// 最大接收数据长度
#define RECEIVE_DATA_LENGTH 4		// 接收数据长度
#define BUFF_SIZE 512				// 缓冲区长度
#define MAX_CONN_SEND   10			// 发送队列消息数量最大值
#define MAX_CONN_RECEIVE  10		// 接收队列消息数量最大值
#define MAX_RETRY_COUNT		10		// 最大重试次数

class  CTOKENCommBase
{
public:
	CTOKENCommBase(void);
	~CTOKENCommBase(void);
	 int TOKENUpdate(DWORD comPort,DWORD speed, char* data,long bufferLength);

private:
	int  Open(TOKEN_CONNECT_PARAM Para,HANDLE& hCom);
	int  ClosePort(HANDLE& hCom);
	int  WriteOneMsg(LPBYTE dataBuf,WORD nSendLen,HANDLE hCom);
	int  ReadOneMsg(LPBYTE rbuf,WORD *nrLen,DWORD WaitTimeMax,HANDLE hCom);	
	int  ExecuteCmd(LPBYTE lpSendBuffer,long wSendLength);
	int  CmdDownLoadFile(char *fileContent,long fileSize,int blockCnt);
	bool CheckResponse(BYTE sendCommand,BYTE recvCommand);

private:
	ComConnect *m_pConn;
	HANDLE m_hCom;
protected:
};
