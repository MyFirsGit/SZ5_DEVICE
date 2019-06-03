#include "stdafx.h"
#include "PrinterCommand.h"
#include "PrinterException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ���캯��
                                                                 
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

CPrinterClose::CPrinterClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������
                                                                 
 @param      (i)��

 @retval      ��

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

CPrinterClose::~CPrinterClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      �رմ���
                                                                 
 @param      (i)��

 @retval      int   \n
                  0 : �ɹ�  ��0 : ʧ�� 

 @exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CPrinterClose::CloseFunction(){
	int iRet = 1;
	if ( m_hPrinter != NULL) {
		if (!CloseHandle(m_hPrinter)) {
			iRet = CPrinterException::ERROR_Printer_CLOSE;
			return iRet;
		}
		else{
			iRet = 0;
		    m_hPrinter = NULL;
		}
	}
	else {
		iRet = CPrinterException::ERROR_Printer_NOTOPEN ;
	}
	
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ִ������
                                                                 
 @param      (i)��

 @retval      long   \n 
                  0 : �ɹ�   ��0 : ʧ�� 

 @exception   
*/
//////////////////////////////////////////////////////////////////////////

long CPrinterClose::ExecuteCommand(){
	long iRet = 1;
	iRet = CloseFunction();
	this->OnComplete(iRet);
	return iRet;
}