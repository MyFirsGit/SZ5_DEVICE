#include "StdAfx.h"
#include "THCommBase.h"
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
CTHCommBase::CTHCommBase(void)
{

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHCommBase::~CTHCommBase(void)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief		TH�̼�����

@param      (i)DWORD port �˿ں�
@param      (i)DWORD speed ������
@param      (i)char *data�ļ�����
@param      (i)long bufferLength�ļ����ݳ���

@retval     int 0:ִ�гɹ�����0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTHCommBase::THUpdate(UINT comPort,UINT speed,UINT updateSpeed, char* pFilePath)
{
	// �򿪴���
	int nRet = FWDCommOpen(comPort,updateSpeed,speed);

	if(nRet != 0){
		return nRet;
	}

	if(NULL == pFilePath){
		return -1;
	}

	nRet = DownloadFw(pFilePath);

	if(nRet != 0){
		return nRet;
	}

	nRet = ClosePort();
	return MIMTRS_RES_OK;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ӳ���̼�����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTHCommBase::DownloadFw(char* pFilePathName){
	return FWDownload(pFilePathName);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �رմ���ͨ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTHCommBase::ClosePort(){
	return FWDCommClose();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ļ���TH

@param      (i)char *fileContent �ļ�����
@param      (i)long fileSize �ļ�����
@param      (i)long blockCnt �ļ��ֿ�����

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int  CTHCommBase::CmdDownLoadFile(char *fileContent,long fileSize,int blockCnt)
//{		
//	int retDownload = MIMTRS_RES_OK;
//	for (int i=0;i<blockCnt;i++) 
//	{
//		// Firmware����
//		BYTE firmwareData[66] = {0};
//		firmwareData[0] = 0xE1;
//		if ( i == blockCnt-1) 
//		{			
//			firmwareData[1] = fileSize-64*(blockCnt-1);
//			int buffLen = fileSize-64*(blockCnt-1);
//			memcpy(firmwareData+2,fileContent+64*i,buffLen);
//			retDownload = ExecuteCmd(firmwareData,buffLen+2);
//		}
//		else 
//		{
//			firmwareData[1] = 0x40;
//			memcpy(firmwareData+2,fileContent+64*i,64);
//			retDownload = ExecuteCmd(firmwareData,64+2);
//		}
//		// �м����ʧ����ֹͣ����
//		if(MIMTRS_RES_OK != retDownload)
//		{
//			return retDownload;
//		}
//	}	
//	return MIMTRS_RES_OK;
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ִ��

@param      (i)LPBYTE lpSendBuffer ����
@param      (i)long wSendLength �����

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int CTHCommBase::ExecuteCmd(LPBYTE lpSendBuffer,long wSendLength)
//{
//	int result = MIMTRS_RES_OK;
//	WORD wRecvLen;
//	bool bSentDone;
//	BYTE lpReceivedBuffer[MAX_BUFFER_SIZE];        // ��������
//	WORD pwReceivedLength = 0;         // �������ݳ���		
//	result = MIMTRS_RES_OK;
//	bSentDone = false;
//	wRecvLen = 0;
//	int reTry = MAX_RETRY_COUNT;
//	do 
//	{
//		do{
//			if(false == bSentDone){
//				Sleep(50);
//				result = WriteOneMsg(lpSendBuffer, wSendLength,m_hCom);
//				bSentDone = true;
//			}
//			else{
//				Sleep(50);
//				result = ReadOneMsg(lpReceivedBuffer, &pwReceivedLength,5000,m_hCom);	
//				// ���ɹ�
//				if (result==MIMTRS_RES_OK){
//					// Ӧ�����
//					if(CheckResponse(lpSendBuffer[0], lpReceivedBuffer[1]))
//					{
//						break;
//					}					
//				}
//				else
//				{
//					result = MIMTRS_CMD_EXECUTE_FAIL;
//				}
//			}
//			reTry--;
//		} while (1);
//	} while (result != MIMTRS_RES_OK && reTry>0);
//
//	return result;
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ӧ�������Ƿ����

@param      (i)BYTE CmdSeq  ���������е��������к�  

@retval     bool  true:���  false: δ���     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//bool CTHCommBase::CheckResponse(BYTE sendCommand,BYTE recvCommand)
//{
//	bool bReturn = false;
//
//	// ����Ƿ�Ӧ�����
//	switch(sendCommand){	
//	case CMD_STATR_DOWNLOAD:		// IAP ���ؿ�ʼ			
//	case CMD_STATR_DOWNLOAD_EX:		// ����IAP ���ؿ�ʼ
//		if ( recvCommand == RSP_START_DOWNLOAD_NORMAL ) {
//			bReturn = true; 
//		}
//		break;			
//	case CMD_DOWNLOAD_COUNT:		// �·��̼���������		
//	case CMD_DOWNLOAD_FILE:			// �̼������·�	
//	case CMD_DOWNLOAD_END:			// �̼��·����
//		if ( recvCommand == RSP_DOWNLOAD_NORMAL ) {
//			bReturn = true; 
//		}
//		break;	
//	case CMD_SWITCH_TO_NORMAL:		// �л�����������
//		if ( recvCommand == RSP_SWITCH_TO_NORMAL_NORMAL ) {
//			bReturn = true; 
//		}
//		break;		
//	default:
//		break;			
//	}
//	return bReturn;
//}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪴���

@param      (i)TH_CONNECT_PARAM Para ���ڲ���
@param      (o)HANDLE& hCom���ھ��

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int CTHCommBase::Open(TH_CONNECT_PARAM Para,HANDLE& hCom)
//{
//	int iRet = m_pConn->open(Para,hCom);
//	if(iRet != 0){
//		//	theTH_TRACE->WriteData(_T("Open Port Error��"),NULL,0);
//		return iRet;
//	}
//	//	theTH_TRACE->WriteData(_T("Open Port OK��"),NULL,0);
//	return MIMTRS_RES_OK;
//}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �رմ���

@param      ��

@retval     int  0:�ɹ�  ��0: ʧ��     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int CTHCommBase::ClosePort(HANDLE& hCom)
//{
//	m_pConn->close(hCom);
//	//theTH_TRACE->WriteData(_T("Close Port��"),NULL,0);
//	return MIMTRS_RES_OK;
//}

//****************************************************************************
// ����һ���������ȡ������Ϣ,����TH���Թ����з��������ݣ����������������������Ͷ�������Ϣ���ʹ��
// function:	WriteOneMsg  
// input:		char *dataBuf				�����͵�����
//				UINT16 nSendLen				�������ݵĳ���
// output:		int 						���͵������ݳ��� ��0����ȷ��0����������
//****************************************************************************
//int CTHCommBase::WriteOneMsg(LPBYTE dataBuf,UINT16 nSendLen,HANDLE hCom)
//{
//	BYTE sendBuf[BUFF_SIZE];
//	memset(sendBuf,0,BUFF_SIZE);
//	LPBYTE lpData = sendBuf;
//	*lpData = STX;
//	lpData++;
//	memcpy(lpData,dataBuf,nSendLen);
//	lpData += nSendLen;
//	*lpData = BccCalc((LPBYTE)sendBuf+1, nSendLen);
//	lpData++;
//	int iRet = m_pConn->write(nSendLen+2, sendBuf,hCom);
//	if(iRet!=0){
//		//theTH_TRACE->WriteData(_T("->"),(LPBYTE)sendBuf,iRet);
//	}
//	return iRet;
//}
//****************************************************************************
// ��ȡ������Ϣ������TH���Թ����з��������ݣ����������������
// function:	WriteOneMsg  
// input:		char *dataBuf				�����͵�����
//				UINT16 nSendLen				�������ݵĳ���
// output:		int 						���յ������ݳ��� 0����ȷ����0����������
//****************************************************************************
//int CTHCommBase::ReadOneMsg(LPBYTE rbuf,UINT16 *nrLen,DWORD WaitTimeMax,HANDLE hCom)
//{	
//	int  datalen = 0;                            // һ�ζ����ڽ��յ������ݳ���
//	BYTE readBuffer[BUFF_SIZE];
//
//	memset(readBuffer,0x00,sizeof(readBuffer));
//
//	// ��ȡ��ʼ��
//	datalen = m_pConn->read(RECEIVE_DATA_LENGTH, readBuffer,hCom);
//
//	if((datalen<=0) || (readBuffer[0] != STX))
//	{ 	
//		return MIMTRS_RECV_EMPTY;
//	}
//	else
//	{
//		memcpy(rbuf,readBuffer,datalen);
//	}
//	return MIMTRS_RES_OK;
//}
