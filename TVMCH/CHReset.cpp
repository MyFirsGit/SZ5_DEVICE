#include "stdafx.h"
#include "CHCommand.h"


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
CCHReset::CCHReset(BYTE XinCtl)
{
	m_Ctl = XinCtl;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHReset::~CCHReset()
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
long CCHReset::ExecuteCommand()
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
long CCHReset::ResetFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("RESET"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("reset: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;
    tCHADevReturn sDevReturn;
	errCode = CHA_Init(m_Ctl,&sDevReturn); 
	if (errCode != E_NO_CH_ERROR){
		//errCode = sDevReturn[0].uiErrorCode;
	}
	sLogString.Format(_T("reset: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
