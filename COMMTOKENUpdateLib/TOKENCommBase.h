#pragma once
#include "ComConnect.h"


#define STX				0xAA		// ���Ŀ�ʼ�����
#define	MAX_BUFFER_SIZE	3000		// ��󻺳�������
#define MAX_RECEIVE_DATA_LENGTH 256	// ���������ݳ���
#define RECEIVE_DATA_LENGTH 4		// �������ݳ���
#define BUFF_SIZE 512				// ����������
#define MAX_CONN_SEND   10			// ���Ͷ�����Ϣ�������ֵ
#define MAX_CONN_RECEIVE  10		// ���ն�����Ϣ�������ֵ
#define MAX_RETRY_COUNT		10		// ������Դ���

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
