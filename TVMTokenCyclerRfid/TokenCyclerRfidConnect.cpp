#include "stdafx.h"
#include "TokenCyclerRfidCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool m_bTokenCyclerRfidConnected = false;

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
CTCRfidConnect::CTCRfidConnect(int nPort, int nBaud)
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
CTCRfidConnect::~CTCRfidConnect()
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
long CTCRfidConnect::ExecuteCommand()
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
long CTCRfidConnect::ConnectFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CONNECT"));

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("zlg500B_init: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);


	errCode = zlg500B_init(m_nPort, m_nBaud);
	if (errCode != E_NO_CS_ERROR){
		m_bTokenCyclerRfidConnected = false;
	}
	else{
		m_bTokenCyclerRfidConnected = true;
	}

	sLogString.Format(_T("zlg500B_init: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
