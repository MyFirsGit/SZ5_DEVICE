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
//@brief      构造函数
//
//@param      (i)无
//
//@retval     无  
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOGetStatus::CIOGetStatus(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      析构函数
//
//@param      (i)无
//
//@retval     无  
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOGetStatus::~CIOGetStatus(void){
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief				执行命令
//
//@param			无
//
//@retval				long  0 : 成功  非0 : 失败
//
//@exception		无
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
//@brief      读取IO状态
//
//@param    none
//
//@retval     long   0 : 成功  非0 : 失败
//
//@exception   无
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
//@brief      得到RW返回信息
//
//@param      (o)BYTE* pResponse    读写器状态     
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
//		throw CIOException(CIOException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
//	}
//}