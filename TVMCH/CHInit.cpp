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

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHInit::CCHInit(BYTE ctl)
{
	m_ctl = ctl;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHInit::~CCHInit()
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
long CCHInit::ExecuteCommand()
{
	long lRet = 0;
	lRet = InitFunction();
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
long CCHInit::InitFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("INIT"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iInit: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;
	tCHADevReturn sDevReturn;//接口定义说明有可能会返回多维状态 王峰
	errCode = CHA_Init(m_ctl,&sDevReturn); 
	if (errCode != E_NO_CH_ERROR){
		errCode = sDevReturn.uiType;
	}

	sLogString.Format(_T("iInit: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
