//#include "stdafx.h"
//#include "IOCommand.h"
//#include "IOException.h"
//#include "DebugLog.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���캯��
//
//@param      (i)��
//
//@retval     ��  
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOGetStatus::CIOGetStatus(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��������
//
//@param      (i)��
//
//@retval     ��  
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOGetStatus::~CIOGetStatus(void){
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief				ִ������
//
//@param			��
//
//@retval				long  0 : �ɹ�  ��0 : ʧ��
//
//@exception		��
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOGetStatus::ExecuteCommand(void)
//{
//	long iRet = 1;
//	iRet = GetIOStatus();
//	this->OnComplete(iRet);
//	return iRet;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ȡIO״̬
//
//@param    none
//
//@retval     long   0 : �ɹ�  ��0 : ʧ��
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOGetStatus::GetIOStatus()
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	BYTE cmd = IO_COMMAND_CODE_GET_STATUS;
//	BYTE recvbuff[MAX_RSP_LEN];
//	BYTE *pRecvBuff = recvbuff; 
//	DWORD recvlen;
//	result = SendOneCommand(IO_COMMAND_LEN_INIT,&cmd, pRecvBuff,recvlen);	
//	if(result == IOTRS_RES_OK){
//		m_IOStatusInfo.Deserialize(pRecvBuff+3);
//	}
//	return result;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �õ�RW������Ϣ
//
//@param      (o)BYTE* pResponse    ��д��״̬     
//
//@retval     void     
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//void CIOGetStatus::GetIOResponse(IO_RSP_STATUS_INFO *pResponse)
//{
//	try {
//		if(pResponse == NULL){
//			throw CIOException(CIOException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
//		}
//		else{
//			*pResponse = m_IOStatusInfo;
//		}
//	}
//	catch(CSysException&) {
//		throw;
//	}
//	catch(...) {
//		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
//	}
//}