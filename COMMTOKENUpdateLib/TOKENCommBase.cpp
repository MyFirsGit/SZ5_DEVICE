#include "StdAfx.h"
#include "TOKENCommBase.h"
#include "common.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommBase::CTOKENCommBase(void)
{
	m_pConn = new ComConnect();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommBase::~CTOKENCommBase(void)
{
	if (m_pConn != NULL)
	{
		delete	m_pConn;
		m_pConn = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		TOKEN�̼�����

@param      (i)DWORD port �˿ں�
@param      (i)DWORD speed ������
@param      (i)char *data�ļ�����
@param      (i)long bufferLength�ļ����ݳ���

@retval     int 0:ִ�гɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::TOKENUpdate(DWORD comPort,DWORD speed, char* data,long bufferLength)
{
	// �򿪴���
	TOKEN_CONNECT_PARAM connectParm;
	connectParm.ComPort = comPort;
	connectParm.Speed = speed;
	connectParm.MaxSendCnt = MAX_CONN_SEND; 
	connectParm.MaxRecvCnt = MAX_CONN_SEND; 
	connectParm.MaxSendBuffSize = MAX_BUFFER_SIZE;
	connectParm.MaxRecvBuffSize = MAX_BUFFER_SIZE;
	int retOpen = Open(connectParm,m_hCom);
	if (retOpen != TOKENTRS_RES_OK)
	{
		return retOpen;
	}

	// ���Ϳ�ʼ��������
	BYTE startDownload[10] = {0};
	startDownload[0] = 0x31;
	startDownload[1] = 0x00;
	if (ExecuteCmd(startDownload,2) != TOKENTRS_RES_OK)
	{
		// ���Ϳ�ʼ���ر�������
		BYTE startDownloadEx[10] = {0};
		startDownloadEx[0] = 0xF0;
		startDownloadEx[0] = 0X00;
		int retStartDownLoadEx = ExecuteCmd(startDownloadEx,2);
		if (retStartDownLoadEx != TOKENTRS_RES_OK)
		{
			return retStartDownLoadEx;
		}
	}	

	//// ��ȡ�ļ�
	//FILE *fp;
	//fp = fopen(fullFilePath,"rb"); 
	//if (NULL == fp)
	//{
	//	return MIMTRS_OPEN_FILE_FAIL;
	//}
	//fseek(fp,0,SEEK_END);		//��λ���ļ�ĩ 
	//long fileSize = ftell(fp);	//�ļ����� 
	//fseek(fp,0,SEEK_SET);		//��λ���ļ�ͷ
	//char *fileContent = new char[fileSize];
	//size_t readCount = fread(fileContent,1,fileSize,fp);
	//if (0 == readCount)
	//{
	//	return MIMTRS_READ_FILE_FAIL;
	//}
	// �ָ����
	long blockCnt = bufferLength/64;
	int surplusLen = bufferLength%64;
	if (0 != surplusLen) 
	{
		blockCnt++;
	}
	// �·��̼���������
	BYTE downloadCount[6] = {0};
	downloadCount[0] = 0xE0;
	downloadCount[1] = 0x04;
	char lenData[4] = {0};
	long fileLlen = lrev(bufferLength);
	memcpy(lenData, &fileLlen, sizeof(fileLlen));
	memcpy(downloadCount+2,lenData,4);

	int retDownloadCount = ExecuteCmd(downloadCount,6);
	if (retDownloadCount != TOKENTRS_RES_OK)
	{
		return retDownloadCount;
	}

	// ���͹̼������·�����
	int retDownloadFile = CmdDownLoadFile(data,bufferLength,blockCnt);
	if (retDownloadFile != TOKENTRS_RES_OK)
	{
		return retDownloadFile;
	}

	//	Sleep(2000);
	// ���͹̼��·��������
	BYTE downloadEnd[10] = {0};
	downloadEnd[0] = 0xE2;
	downloadEnd[1] = 0x00;
	int retDownLoadEnd = ExecuteCmd(downloadEnd,2);
	if (retDownLoadEnd != TOKENTRS_RES_OK)
	{
		return retDownLoadEnd;
	}

	Sleep(5000);
	// �����л���������������
	BYTE switchToNormal[10] = {0};
	switchToNormal[0] = 0xE9;
	switchToNormal[1] = 0x00;
	int retSwitchToNormal = ExecuteCmd(switchToNormal,2);
	if (retSwitchToNormal != TOKENTRS_RES_OK)
	{
		return retSwitchToNormal;
	}
	// �رմ���
	ClosePort(m_hCom);

	return TOKENTRS_RES_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ļ���TOKEN

@param      (i)char *fileContent �ļ�����
@param      (i)long fileSize �ļ�����
@param      (i)long blockCnt �ļ��ֿ�����

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int  CTOKENCommBase::CmdDownLoadFile(char *fileContent,long fileSize,int blockCnt)
{		
	int retDownload = TOKENTRS_RES_OK;
	for (int i=0;i<blockCnt;i++) 
	{
		// Firmware����
		BYTE firmwareData[66] = {0};
		firmwareData[0] = 0xE1;
		if ( i == blockCnt-1) 
		{			
			firmwareData[1] = fileSize-64*(blockCnt-1);
			int buffLen = fileSize-64*(blockCnt-1);
			memcpy(firmwareData+2,fileContent+64*i,buffLen);
			retDownload = ExecuteCmd(firmwareData,buffLen+2);
		}
		else 
		{
			firmwareData[1] = 0x40;
			memcpy(firmwareData+2,fileContent+64*i,64);
			retDownload = ExecuteCmd(firmwareData,64+2);
		}
		// �м����ʧ����ֹͣ����
		if(TOKENTRS_RES_OK != retDownload)
		{
			return retDownload;
		}
	}	
	return TOKENTRS_RES_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ִ��

@param      (i)LPBYTE lpSendBuffer ����
@param      (i)long wSendLength �����

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::ExecuteCmd(LPBYTE lpSendBuffer,long wSendLength)
{
	int result = TOKENTRS_RES_OK;
	WORD wRecvLen;
	bool bSentDone;
	BYTE lpReceivedBuffer[MAX_BUFFER_SIZE];        // ��������
	WORD pwReceivedLength = 0;         // �������ݳ���		
	result = TOKENTRS_RES_OK;
	bSentDone = false;
	wRecvLen = 0;
	int reTry = MAX_RETRY_COUNT;
	do 
	{
		do{
			if(false == bSentDone){
				Sleep(50);
				result = WriteOneMsg(lpSendBuffer, wSendLength,m_hCom);
				bSentDone = true;
			}
			else{
				Sleep(50);
				result = ReadOneMsg(lpReceivedBuffer, &pwReceivedLength,5000,m_hCom);	
				// ���ɹ�
				if (result==TOKENTRS_RES_OK){
					// Ӧ�����
					if(CheckResponse(lpSendBuffer[0], lpReceivedBuffer[1]))
					{
						break;
					}					
				}
				else
				{
					result = TOKENTRS_CMD_EXECUTE_FAIL;
				}
			}
			reTry--;
		} while (1);
	} while (result != TOKENTRS_RES_OK && reTry>0);

	return result;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ӧ�������Ƿ����

@param      (i)BYTE CmdSeq  ���������е��������к�  

@retval     bool  true:���  false: δ���     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTOKENCommBase::CheckResponse(BYTE sendCommand,BYTE recvCommand)
{
	bool bReturn = false;

	// ����Ƿ�Ӧ�����
	switch(sendCommand){	
	case CMD_STATR_DOWNLOAD:		// IAP ���ؿ�ʼ			
	case CMD_STATR_DOWNLOAD_EX:		// ����IAP ���ؿ�ʼ
		if ( recvCommand == RSP_START_DOWNLOAD_NORMAL ) {
			bReturn = true; 
		}
		break;			
	case CMD_DOWNLOAD_COUNT:		// �·��̼���������		
	case CMD_DOWNLOAD_FILE:			// �̼������·�	
	case CMD_DOWNLOAD_END:			// �̼��·����
		if ( recvCommand == RSP_DOWNLOAD_NORMAL ) {
			bReturn = true; 
		}
		break;	
	case CMD_SWITCH_TO_NORMAL:		// �л�����������
		if ( recvCommand == RSP_SWITCH_TO_NORMAL_NORMAL ) {
			bReturn = true; 
		}
		break;		
	default:
		break;			
	}
	return bReturn;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪴���

@param      (i)TOKEN_CONNECT_PARAM Para ���ڲ���
@param      (o)HANDLE& hCom���ھ��

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::Open(TOKEN_CONNECT_PARAM Para,HANDLE& hCom)
{
	int iRet = m_pConn->open(Para,hCom);
	if(iRet != 0){
		//	theTOKEN_TRACE->WriteData(_T("Open Port Error��"),NULL,0);
		return iRet;
	}
	//	theTOKEN_TRACE->WriteData(_T("Open Port OK��"),NULL,0);
	return TOKENTRS_RES_OK;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �رմ���

@param      ��

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::ClosePort(HANDLE& hCom)
{
	m_pConn->close(hCom);
	//theTOKEN_TRACE->WriteData(_T("Close Port��"),NULL,0);
	return TOKENTRS_RES_OK;
}

//****************************************************************************
// ����һ���������ȡ������Ϣ,����TOKEN���Թ����з��������ݣ����������������������Ͷ�������Ϣ���ʹ��
// function:	WriteOneMsg  
// input:		char *dataBuf				�����͵�����
//				UINT16 nSendLen				�������ݵĳ���
// output:		int 						���͵������ݳ��� ��0����ȷ��0����������
//****************************************************************************
int CTOKENCommBase::WriteOneMsg(LPBYTE dataBuf,UINT16 nSendLen,HANDLE hCom)
{
	BYTE sendBuf[BUFF_SIZE];
	memset(sendBuf,0,BUFF_SIZE);
	LPBYTE lpData = sendBuf;
	*lpData = STX;
	lpData++;
	memcpy(lpData,dataBuf,nSendLen);
	lpData += nSendLen;
	*lpData = BccCalc((LPBYTE)sendBuf+1, nSendLen);
	lpData++;
	int iRet = m_pConn->write(nSendLen+2, sendBuf,hCom);
	if(iRet!=0){
		//theTOKEN_TRACE->WriteData(_T("->"),(LPBYTE)sendBuf,iRet);
	}
	return iRet;
}
//****************************************************************************
// ��ȡ������Ϣ������TOKEN���Թ����з��������ݣ����������������
// function:	WriteOneMsg  
// input:		char *dataBuf				�����͵�����
//				UINT16 nSendLen				�������ݵĳ���
// output:		int 						���յ������ݳ��� 0����ȷ����0����������
//****************************************************************************
int CTOKENCommBase::ReadOneMsg(LPBYTE rbuf,UINT16 *nrLen,DWORD WaitTimeMax,HANDLE hCom)
{	
	int  datalen = 0;                            // һ�ζ����ڽ��յ������ݳ���
	BYTE readBuffer[BUFF_SIZE];

	memset(readBuffer,0x00,sizeof(readBuffer));

	// ��ȡ��ʼ��
	datalen = m_pConn->read(RECEIVE_DATA_LENGTH, readBuffer,hCom);

	if((datalen<=0) || (readBuffer[0] != STX))
	{ 	
		return TOKENTRS_RECV_EMPTY;
	}
	else
	{
		memcpy(rbuf,readBuffer,datalen);
	}
	return TOKENTRS_RES_OK;
}
