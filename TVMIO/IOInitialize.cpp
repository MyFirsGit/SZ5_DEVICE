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
// @brief      ���캯��
//                                                                 
// @param    ��
//
// @retval     ��  
//
// @exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOInitialize::CIOInitialize(void){
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      ��������
//                                                                 
// @param    ��
//
// @retval     ��  
//
// @exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOInitialize::~CIOInitialize(void){
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      ִ������
//                                                                 
// @param    ��
//
// @retval     long  0 : �ɹ�  ��0 : ʧ��
//
// @exception   ��
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
// @brief      ��ʼ��������
//                                                                 
// @param    ��
//
// @retval     long    0 : �ɹ�  ��0 : ʧ��
//
// @exception   ��
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
