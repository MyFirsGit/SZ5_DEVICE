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
 @brief      构造函数
                                                                 
 @param      (i)无

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////

CPrinterClose::CPrinterClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      析构函数
                                                                 
 @param      (i)无

 @retval      无

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////

CPrinterClose::~CPrinterClose(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      关闭串口
                                                                 
 @param      (i)无

 @retval      int   \n
                  0 : 成功  非0 : 失败 

 @exception   无
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
 @brief      执行命令
                                                                 
 @param      (i)无

 @retval      long   \n 
                  0 : 成功   非0 : 失败 

 @exception   
*/
//////////////////////////////////////////////////////////////////////////

long CPrinterClose::ExecuteCommand(){
	long iRet = 1;
	iRet = CloseFunction();
	this->OnComplete(iRet);
	return iRet;
}