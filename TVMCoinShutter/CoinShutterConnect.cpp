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

 @param		(i)int nPort 串口端口号
 @param		(i)int nBaud 串口波特率
 @param		(i)int nSize 字长

 @retval	无

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCSConnect::CCSConnect(int nPort, int nBaud)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCSConnect::~CCSConnect()
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
long CCSConnect::ExecuteCommand()
{
	long lRet = 1;
	lRet = ConnectFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		打开串口，并设置相关参数

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSConnect::ConnectFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CONNECT"));

	long iRet = E_NO_CS_ERROR;

	sLogString.Format(_T("iSetCommPara:port[%d] baud[%d] "),this->m_nPort,this->m_nBaud);
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	TCHAR szPort[20];
	//int iRet = 1;
	CString strport = _T("");
	strport.Format(_T("COM%d"),this->m_nPort);
	memset(szPort,'\0',40);
	::swprintf_s(szPort,20,strport);

	if(m_hCoinShutter != NULL){
		if (!CloseHandle(m_hCoinShutter)) {
			iRet = CCoinShutterException::ERROR_CS_CLOSE_FAILED;
			m_hCoinShutter = NULL;
			return iRet;
		}
	}
	//打开串口
	m_hCoinShutter = ::CreateFile(szPort,GENERIC_READ | GENERIC_WRITE ,
		0,NULL,OPEN_EXISTING,/*FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED*/NULL,NULL);

	if(m_hCoinShutter == NULL){
		iRet = CCoinShutterException::ERROR_CS_OPEN_FAILED;
		return iRet;
	}

	if(m_hCoinShutter == INVALID_HANDLE_VALUE){
		iRet = CCoinShutterException::ERROR_CS_OPEN_FAILED;
		m_hCoinShutter = NULL;
		return iRet;
	}

	COMMTIMEOUTS CommTimeOuts;
	if(!::GetCommTimeouts(m_hCoinShutter, &CommTimeOuts))
	{
		iRet = CCoinShutterException::ERROR_CS_OTHER;
		CloseHandle(m_hCoinShutter);
		m_hCoinShutter = NULL;
		return iRet;
	}
	//设置超时
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;

	CommTimeOuts.WriteTotalTimeoutConstant = 500;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 100;

	if(!SetCommTimeouts(m_hCoinShutter,&CommTimeOuts)){
		iRet = CCoinShutterException::ERROR_CS_OTHER;
		CloseHandle(m_hCoinShutter);
		m_hCoinShutter = NULL;
		return iRet;
	}
	DCB dcb;
	::GetCommState(m_hCoinShutter,&dcb);
	dcb.BaudRate = this->m_nBaud;
	dcb.ByteSize = 8;
	dcb.StopBits = TWOSTOPBITS;
	dcb.Parity = NOPARITY;
	::SetCommState(m_hCoinShutter,&dcb);
	//清空串口缓冲区
	if(!::PurgeComm(m_hCoinShutter, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
	{
		iRet = CCoinShutterException::ERROR_CS_OTHER;
		CloseHandle(m_hCoinShutter);
		m_hCoinShutter = NULL;
		return iRet;
	}
	//return iRet;
	//tDevReturn sDevReturn[8];//接口定义说明有可能会返回多维状态 王峰
	//errCode = m_ShutterDev.iSetCommPara(sDevReturn);
	//if (errCode != E_NO_CS_ERROR){
	//	m_bConnected = false;
	//	errCode = sDevReturn[0].iLogicCode;
	//}
	//else{
	//	m_bConnected = true;
	//}
	m_bConnected = true;
	sLogString.Format(_T("iSetCommPara: errCode = %d"), iRet);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return iRet;
}
