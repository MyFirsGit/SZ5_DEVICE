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
CCSOpenShutter::CCSOpenShutter()
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
CCSOpenShutter::~CCSOpenShutter()
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
long CCSOpenShutter::ExecuteCommand()
{
	long lRet = 0;
	lRet = OpenShutterFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		初始化

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSOpenShutter::OpenShutterFunction()
{
	//CString sLogString(_T(""));
	//SYNC(CS,_T("OPEN_SHUTTER"));

	//if (m_bConnected == false){
	//	return CCoinShutterException::ERROR_CS_NOT_OPEN;
	//}

	//sLogString.Format(_T("iOpen: "));
	//theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	//long errCode = E_NO_CS_ERROR;;
	//tDevReturn sDevReturn[8];//接口定义说明有可能会返回多维状态 王峰

	//errCode = m_ShutterDev.iOpen(sDevReturn); 
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}

	//sLogString.Format(_T("iOpen: errCode = %d"), errCode);
	//theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	//return errCode;

	CString sLogString(_T(""));
	SYNC(CS,_T("OpenShutterFunction"));

	if (m_bConnected == false){
		return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("OpenShutterFunction: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;;
	// 命令
	char cCmd[]={0x1b,0x4F,0x03};
	// 输出到打印机
	DWORD dwBytesWritten = 0;
	BOOL bWriteState = ::WriteFile(m_hCoinShutter,cCmd,sizeof(cCmd),&dwBytesWritten,NULL);  
	int iWriteResult =  0;
	// 写串口失败
	if(bWriteState != TRUE){
		if(GetLastError() !=  ERROR_IO_PENDING){
			iWriteResult =  CCoinShutterException::ERROR_CS_CMD;
		}
		else{
		}
	}
	if(iWriteResult!=0){
		sLogString.Format(_T("OpenShutterFunction: errCode = %d"), iWriteResult);
		theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		return iWriteResult;
	}

	char cGetStatusResponse[15] = {0};
	DWORD dwBytesRead = 0;
	// 读取打印机反馈
	BOOL  bReadState = ::ReadFile(m_hCoinShutter,cGetStatusResponse,3,&dwBytesRead,NULL);
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
	//清空串口缓冲区
	if(!::PurgeComm(m_hCoinShutter, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
	{
		//iReadResult = CCoinShutterException::ERROR_CS_OTHER;
		//CloseHandle(m_hCoinShutter);
		//m_hCoinShutter = NULL;
		//return iRet;
	}

	if(cGetStatusResponse[1] != 's'){
		sLogString.Format(_T("OpenShutterFunction: errCode = %d"), (cGetStatusResponse[2]==0?iReadResult:cGetStatusResponse[2]));
		theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

		return cGetStatusResponse[2];
	}
	//tDevReturn sDevReturn[8];//接口定义说明有可能会返回多维状态 王峰

	//errCode = m_ShutterDev.iInit(sDevReturn); 
	//if (errCode != E_NO_CS_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	errCode = iReadResult;
	sLogString.Format(_T("OpenShutterFunction: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
