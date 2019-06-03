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
// @brief				构造函数
//                                                                 
// @param			(i)IO_OUTPUT_PORT_NO OutputNo IO输出端口号
//  @param			(i)IO_OUTPUT_VALUE OutputValue  IO输出值
//
// @retval			无
//
// @exception    无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOSetOneOutputFlash::CIOSetOneOutputFlash(IO_OUTPUT_PORT_NO OutputNo,IO_OUTPUT_VALUE	OutputValue)
//{
//	m_OutputNo = OutputNo;
//	m_OutputValue = OutputValue;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      析构函数
//                                                                
// @param      (i)无
//
// @retval      无
//
// @exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CIOSetOneOutputFlash::~CIOSetOneOutputFlash(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief				设置输出函数
//                                                                 
// @param			无
//
// @retval			long       0 : 成功  非0 : 失败
//
// @exception		无
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOSetOneOutputFlash::DoSetOutputFun()
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	BYTE cmd[4] = {0x00};
//	memset(cmd,0x00,sizeof(cmd));
//	cmd[0] = IO_COMMAND_CODE_SET_OUTPUT_FLASH;
//	cmd[1] = 0x00;	//AQ参数，不设置
//	cmd[2] = m_OutputNo;
//	cmd[3] = m_OutputValue;
//	BYTE recvbuff[MAX_RSP_LEN];
//	DWORD recvlen;
//	result = SendOneCommand(IO_COMMAND_LEN_SET_OUTPUT_FLASH,cmd, recvbuff,recvlen);	
//	return result;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief				执行命令
//                                                                 
// @param			无
//
// @retval			long    0 : 成功  非0 : 失败
//
// @exception		无
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOSetOneOutputFlash::ExecuteCommand(void)
//{
//   long lRet = 1;
//   lRet = DoSetOutputFun();
//   this->OnComplete(lRet);
//   return lRet;
//}