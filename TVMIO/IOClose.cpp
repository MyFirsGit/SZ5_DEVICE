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
// @param			��
//
// @retval			��
//
// @exception		��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOClose::CIOClose(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief				��������
//                                                                 
// @param			��
//
// @retval			��
//
// @exception		��
//*/
////////////////////////////////////////////////////////////////////////////
//CIOClose::~CIOClose(void)
//{
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief			�رմ���
//                                                                 
// @param      ��
//
// @retval		long   0 : �ɹ�  ��0 : ʧ�� 
//
// @exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOClose::CloseFunction()
//{
//	IOTRS_RESULT lRet = 0;
//	lRet = IOTrs_Close(m_hTVMIO);
//	if(lRet != IOTRS_RES_OK){
//		lRet = CIOException::ERROR_IO_CLOSE;
//	}
//	return lRet;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
// @brief      ִ������
//                                                                 
// @param      (i)��
//
// @retval      long   \n 
//                  0 : �ɹ�   ��0 : ʧ�� 
//
// @exception   
//*/
////////////////////////////////////////////////////////////////////////////
//long CIOClose::ExecuteCommand()
//{
//	long iRet = 1;
//	iRet = CloseFunction();
//	this->OnComplete(iRet);
//	return iRet;
//}