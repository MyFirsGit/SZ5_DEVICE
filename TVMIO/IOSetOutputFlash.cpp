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
// @brief				���캯��
//                                                                 
// @param			(i)IO_OUTPUT_PORT_NO OutputNo IO����˿ں�
//  @param			(i)IO_OUTPUT_VALUE OutputValue  IO���ֵ
//
// @retval			��
//
// @exception    ��
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
// @brief      ��������
//                                                                
// @param      (i)��
//
// @retval      ��
//
// @exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOSetOneOutputFlash::~CIOSetOneOutputFlash(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief				�����������
//                                                                 
// @param			��
//
// @retval			long       0 : �ɹ�  ��0 : ʧ��
//
// @exception		��
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOSetOneOutputFlash::DoSetOutputFun()
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	BYTE cmd[4] = {0x00};
//	memset(cmd,0x00,sizeof(cmd));
//	cmd[0] = IO_COMMAND_CODE_SET_OUTPUT_FLASH;
//	cmd[1] = 0x00;	//AQ������������
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
// @brief				ִ������
//                                                                 
// @param			��
//
// @retval			long    0 : �ɹ�  ��0 : ʧ��
//
// @exception		��
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOSetOneOutputFlash::ExecuteCommand(void)
//{
//   long lRet = 1;
//   lRet = DoSetOutputFun();
//   this->OnComplete(lRet);
//   return lRet;
//}