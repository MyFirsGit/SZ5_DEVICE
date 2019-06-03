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

 @param		void

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCSReset::CCSReset()
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
CCSReset::~CCSReset()
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
long CCSReset::ExecuteCommand()
{
	long lRet = 1;
	lRet = ResetFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		复位

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCSReset::ResetFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("RESET"));

	if (m_bConnected == false){
		return CCoinShutterHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("reset: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("reset: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
