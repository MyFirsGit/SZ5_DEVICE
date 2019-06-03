#include "stdafx.h"
#include "CoinShutterCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCSGetStatus::CCSGetStatus()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCSGetStatus::~CCSGetStatus()
{

}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSGetStatus::ExecuteCommand()
{
	long lRet = 1;
	lRet = GetStatusFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取命令反馈

 @param		LPCS_SPEC1_RSP pRes 指向获取反馈信息的对象

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSGetStatus::GetResponse(tShutterStatus* pSensorStatus)
{
	memcpy(pSensorStatus, &m_sSensorStatus, sizeof(tShutterStatus));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		读取状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSGetStatus::GetStatusFunction()
{
	//CString sLogString(_T(""));
	//SYNC(CS,_T("GET_STATUS"));

	//if (m_bConnected == false){
	//	return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	//}

	//sLogString.Format(_T("iGetDevStatus: "));
	//theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//long errCode = E_NO_CS_ERROR;

	//tDevReturn sDevReturn[8];
	//errCode = m_ShutterDev.iGetDevStatus(&m_sSensorStatus,sDevReturn);
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	//else{
	//	//为状态接口赋值

	//}

	//sLogString.Format(_T("iGetDevStatus: errCode = %d"), errCode);
	//theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	//return errCode;
	return 0;
	CString sLogString(_T(""));
	SYNC(CS,_T("GetStatusFunction"));

	if (m_bConnected == false){
		return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("iInit: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;;
	// 串口打印机的OVERLAPPED 结构
	OVERLAPPED writeOverlapped={0};   
	writeOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if (writeOverlapped.hEvent == NULL || writeOverlapped.hEvent == INVALID_HANDLE_VALUE) {
		return CCoinShutterException::SET_PARAM_ERR;
	}
	// 命令
	char cCmd[]={0x1b,0x49,0x03};
	// 输出到打印机
	DWORD dwBytesWritten = 0;
	BOOL bWriteState = ::WriteFile(m_hCoinShutter,cCmd,sizeof(cCmd),&dwBytesWritten,&writeOverlapped);  
	int iWriteResult =  0;
	// 写串口失败
	if(bWriteState != TRUE){
		if(GetLastError() !=  ERROR_IO_PENDING){
			iWriteResult =  CCoinShutterException::ERROR_CS_CMD;
		}
		else{
			DWORD dwWaitResult = WaitForSingleObject(writeOverlapped.hEvent,15000);  //等待信号
			switch( dwWaitResult) {
			case WAIT_TIMEOUT: 
				iWriteResult =  CCoinShutterException::ERROR_CS_CMD;
				break;
			case WAIT_FAILED:
			default:				
				iWriteResult = CCoinShutterException::ERROR_CS_SEND;
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
		return CCoinShutterException::SET_PARAM_ERR;
	}
	char cGetStatusResponse[15] = {0};
	DWORD dwBytesRead = 0;
	// 读取打印机反馈
	BOOL  bReadState = ::ReadFile(m_hCoinShutter,cGetStatusResponse,3,&dwBytesRead,&readOverlapped);
	int iReadResult = 0;
	// 读串口失败
	if(bReadState != TRUE){
		if(GetLastError()!= ERROR_IO_PENDING){
			iReadResult = CCoinShutterException::ERROR_CS_RECEIVE;
		}
		else{
		}
	}
	if(dwBytesRead == 0){
		iReadResult =  CCoinShutterException::ERROR_CS_RECEIVE;
	}
	CloseHandle(readOverlapped.hEvent);
	if(iReadResult != 0){
		return cGetStatusResponse[2];
	}
	//tDevReturn sDevReturn[8];//接口定义说明有可能会返回多维状态 王峰

	//errCode = m_ShutterDev.iInit(sDevReturn); 
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	errCode = iReadResult;
	sLogString.Format(_T("iInit: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
