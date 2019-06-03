#include "stdafx.h"
#include "PrinterCommand.h"
#include "PrinterException.h"
#include "DebugLog.h"

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
CPrinterGetInfo::CPrinterGetInfo(void)
{
	memset(m_printerId,0,sizeof(m_printerId));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterGetInfo::~CPrinterGetInfo(void){
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
long CPrinterGetInfo::ExecuteCommand(void)
{
	long iRet = 1;
	iRet = GetPrinterId();
	this->OnComplete(iRet);
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得执行结果

@param      (o)BYTE* printerId 打印机ID

@retval     none

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterGetInfo::GetRWResponse(BYTE* printerId)
{
	if(printerId == NULL){
		throw CPrinterException(CPrinterException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
	}
	memcpy(printerId,m_printerId,sizeof(m_printerId));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取打印机ID

@param      none

@retval     long   0 : 成功  非0 : 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterGetInfo::GetPrinterId()
{
	if (m_hPrinter == NULL || m_hPrinter == INVALID_HANDLE_VALUE){
		return CPrinterException::ERROR_Printer_NOTOPEN;
	}

	// 串口打印机的OVERLAPPED 结构
	OVERLAPPED writeOverlapped={0};   
	writeOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (writeOverlapped.hEvent == NULL || writeOverlapped.hEvent == INVALID_HANDLE_VALUE) {
		return CPrinterException::ERROR_Printer_OVERLAPPED_EVENT;
	}
	// 取状态命令
	char cCmdGetStatus[]={0x1D,0x49,0x44};
	// 输出到打印机
	DWORD dwBytesWritten = 0;
	BOOL bWriteState = ::WriteFile(m_hPrinter,cCmdGetStatus,sizeof(cCmdGetStatus),&dwBytesWritten,&writeOverlapped);  
	int iWriteResult =  0;
	// 写串口失败
	if(bWriteState != TRUE){
		if(GetLastError() !=  ERROR_IO_PENDING){
			iWriteResult =  CPrinterException::ERROR_Printer_WRITEFILE;
		}
		else{
			DWORD dwWaitResult = WaitForSingleObject(writeOverlapped.hEvent,Printer_WAIT_TIME);  //等待信号
			switch( dwWaitResult) {
			case WAIT_ABANDONED:
				iWriteResult = CPrinterException::ERROR_Printer_WAIT_ABANDONED;
				break;
			case WAIT_OBJECT_0: 
				break;
			case WAIT_TIMEOUT: 
				::CancelIo(m_hPrinter);
				iWriteResult =  CPrinterException::ERROR_Printer_WAIT_TIMEOUT;
				break;
			case WAIT_FAILED:
				iWriteResult =  CPrinterException::ERROR_Printer_WRITEFILE;
				break;
			default:
				iWriteResult =  CPrinterException::ERROR_Printer_WRITEFILE;
				break;
			}
		}
	}
	CloseHandle(writeOverlapped.hEvent);
	if(iWriteResult!=0){
		return iWriteResult;
	}

	// 串口打印机的OVERLAPPED 结构
	OVERLAPPED readOverlapped={0};   
	readOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (readOverlapped.hEvent == NULL || readOverlapped.hEvent == INVALID_HANDLE_VALUE) {
		return CPrinterException::ERROR_Printer_OVERLAPPED_EVENT;
	}
	char cGetStatusResponse[15] = {0};
	DWORD dwBytesRead = 0;
	// 读取打印机反馈
	BOOL  bReadState = ::ReadFile(m_hPrinter,cGetStatusResponse,15,&dwBytesRead,&readOverlapped);
	int iReadResult = 0;
	// 读串口失败
	if(bReadState != TRUE){
		if(GetLastError()!= ERROR_IO_PENDING){
			iReadResult = CPrinterException::ERROR_Printer_WRITEFILE;
		}
		else{
			DWORD dwWaitResult = WaitForSingleObject(readOverlapped.hEvent,Printer_WAIT_TIME);  //等待信号
			switch( dwWaitResult){
			case WAIT_ABANDONED :
				iReadResult =   CPrinterException::ERROR_Printer_WAIT_ABANDONED ;
				break;
			case WAIT_OBJECT_0 :
				::GetOverlappedResult(m_hPrinter, &readOverlapped, &dwBytesRead,false);
				break;
			case WAIT_TIMEOUT : 
				::CancelIo(m_hPrinter);
				iReadResult =  CPrinterException::ERROR_Printer_WAIT_TIMEOUT;
				break;
			case WAIT_FAILED :
				iReadResult =  CPrinterException::ERROR_Printer_WRITEFILE;
				break;
			default:
				iReadResult =  CPrinterException::ERROR_Printer_WRITEFILE;
				break;
			}
		}
	}
	if(dwBytesRead == 0){
		iReadResult =  CPrinterException::ERROR_Printer_WRITEFILE;
	}
	CloseHandle(readOverlapped.hEvent);
	if(iReadResult != 0){
		return iReadResult;
	}
	memcpy(m_printerId,cGetStatusResponse+5,sizeof(m_printerId));
	CString printID = (TCHAR*)m_printerId;
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("printerId = %.6s"),m_printerId);
	return 0;
}
