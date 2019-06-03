//#include "stdafx.h"
//#include "IOCommand.h"
//#include "IOException.h"
//
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      构造函数
//                                                                 
// @param    无
//
// @retval     无  
//
// @exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOInitialize::CIOInitialize(void){
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      析构函数
//                                                                 
// @param    无
//
// @retval     无  
//
// @exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOInitialize::~CIOInitialize(void){
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      执行命令
//                                                                 
// @param    无
//
// @retval     long  0 : 成功  非0 : 失败
//
// @exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOInitialize::ExecuteCommand(void)
//{
//	long lRet = 0;
//	lRet = InitializeFunction();
//	this->OnComplete(lRet);
//	return lRet;
//}
////////////////////////////////////////////////////////////////////////////
///**
// @brief      初始化处理函数
//                                                                 
// @param    无
//
// @retval     long    0 : 成功  非0 : 失败
//
// @exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOInitialize::InitializeFunction()
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	BYTE cmd = IO_COMMAND_CODE_INIT;
//	BYTE recvbuff[MAX_RSP_LEN];
//	DWORD recvlen;
//	result = SendOneCommand(IO_COMMAND_LEN_INIT,&cmd, recvbuff,recvlen);	
//	return result;
//}
