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

 @retval     无  

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterOpenBox::CPrinterOpenBox(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      析构函数
                                                                 
 @param      (i)无

 @retval     无  

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterOpenBox::~CPrinterOpenBox(void){
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      执行命令
                                                                 
 @param      (i)无

 @retval     long  \n
               0 : 成功  非0 : 失败

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterOpenBox::ExecuteCommand(void)
{
	long iRet = 1;
	iRet = OpenBoxFunction();
	this->OnComplete(iRet);
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
/**
 @brief      打开钱箱
                                                                 
 @param      (i)无

 @retval     int  \n
             0 : 成功  非0 : 失败

 @exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterOpenBox::OpenBoxFunction()
{
	int iRet = 1;
	char* cCmd = NULL;
	DWORD dwBytesWritten = 10;
	BOOL  bWriteState = FALSE;
	OVERLAPPED overlapped;

	//判断打印机串口是否打开
	if ( m_hPrinter == NULL) {
		iRet = CPrinterException::ERROR_Printer_NOTOPEN;
		return iRet;
	}

	//生成OVERLAPPED事件
	memset(&overlapped,0,sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (overlapped.hEvent == NULL) {
		iRet = CPrinterException::ERROR_Printer_OVERLAPPED_EVENT;
	}
	
	//开钱箱指令
	cCmd = new char[10];
	memset(cCmd,0,10);
	cCmd[0] = 0x1B;
	cCmd[1] = 0x70;
	cCmd[2] = 0x30;
	cCmd[3] = 0x08;
	cCmd[4] = 0x09;

	//输出到打印机
	bWriteState = WriteFile(m_hPrinter,cCmd,dwBytesWritten,&dwBytesWritten,&overlapped);
    if (NULL != cCmd) {
        delete [] cCmd;
        cCmd = NULL;
    }
	
	if (bWriteState != TRUE) {
		if (GetLastError() == ERROR_IO_PENDING) {
			iRet = WaitForSingleObject(overlapped.hEvent,BOX_WAIT_TIME);  //等待信号
			switch( iRet) {
			case WAIT_ABANDONED :
				iRet = CPrinterException::ERROR_Printer_WAIT_ABANDONED ;
				break;
			case WAIT_OBJECT_0 :    //成功
				iRet = 0;
				break;
			case WAIT_TIMEOUT : 
				iRet = CPrinterException::ERROR_Printer_WAIT_TIMEOUT;
				::CancelIo(m_hPrinter);
				break;
			case WAIT_FAILED :
				iRet =  CPrinterException::ERROR_Printer_WRITEFILE;
				break;
				}
		}
		else{
			iRet = CPrinterException::ERROR_Printer_WRITEFILE;
		}
	}
	else{
		iRet = CPrinterException::ERROR_Printer_WRITEFILE;
		return iRet;
	}
	return iRet;
}
