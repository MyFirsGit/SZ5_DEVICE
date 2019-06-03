#pragma once
#include "UpgradeApi.h"

class  CTHCommBase
{
public:
	CTHCommBase(void);
	~CTHCommBase(void);
	 int THUpdate(UINT comPort,UINT speed,UINT updateSpeed, char* pFilePath);

private:
	int Open(UINT comPort,UINT comBaudRate,UINT workBaudRate);
	int ClosePort();

	int DownloadFw(char* pFilePathName);
	/*int  Open(TH_CONNECT_PARAM Para,HANDLE& hCom);
	int  ClosePort(HANDLE& hCom);
	int  WriteOneMsg(LPBYTE dataBuf,WORD nSendLen,HANDLE hCom);
	int  ReadOneMsg(LPBYTE rbuf,WORD *nrLen,DWORD WaitTimeMax,HANDLE hCom);	
	int  ExecuteCmd(LPBYTE lpSendBuffer,long wSendLength);
	int  CmdDownLoadFile(char *fileContent,long fileSize,int blockCnt);
	bool CheckResponse(BYTE sendCommand,BYTE recvCommand);*/

private:
	//ComConnect *m_pConn;
	//HANDLE m_hCom;
	//char* m_pFilePath;
protected:
};
