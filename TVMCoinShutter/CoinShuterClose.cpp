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
CCSClose::CCSClose()
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
CCSClose::~CCSClose()
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
long CCSClose::ExecuteCommand()
{
	long lRet = 1;
	lRet = CloseFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		关闭串口

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSClose::CloseFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CLOSE"));

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("close: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	int iRet = 1;
	if ( m_hCoinShutter != NULL && m_hCoinShutter != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(m_hCoinShutter)) {
			iRet = CCoinShutterException::ERROR_CS_CLOSE_FAILED;
			return iRet;
		}
		else{
			iRet = 0;
			m_hCoinShutter = NULL;
		}
	}
	else {
		iRet = 0;//CPrinterException::ERROR_Printer_NOTOPEN ;
	}

	errCode = iRet;
	sLogString.Format(_T("close: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
