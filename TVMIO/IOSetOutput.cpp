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
//CIOSetOneSlotOutput::CIOSetOneSlotOutput(IO_OUTPUT_PORT_NO OutputNo,IO_OUTPUT_VALUE	 OutputValue)
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
//CIOSetOneSlotOutput::~CIOSetOneSlotOutput(void)
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
//long CIOSetOneSlotOutput::SetOutputFunction()
//{
//	IOTRS_RESULT result = IOTRS_RES_OK;
//	BYTE cmd[4] = {0x00};
//	memset(cmd,0x00,sizeof(cmd));
//	cmd[0] = IO_COMMAND_CODE_SET_OUTPUT_ON_OFF;
//	cmd[1] = m_OutputNo;
//	cmd[2] = m_OutputValue;
//	BYTE recvbuff[MAX_RSP_LEN];
//	DWORD recvlen;
//	result = SendOneCommand(IO_COMMAND_LEN_SET_OUTPUT_ON_OFF,cmd, recvbuff,recvlen);	
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
//long CIOSetOneSlotOutput::ExecuteCommand(void)
//{
//   long lRet = 1;
//   lRet = SetOutputFunction();
//   this->OnComplete(lRet);
//   return lRet;
//}