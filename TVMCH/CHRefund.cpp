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
CCHRefund::CCHRefund(UINT coinnum)
{
  m_returncoinnum = coinnum;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHRefund::~CCHRefund()
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
long CCHRefund::ExecuteCommand()
{
	long lRet = 1;
	lRet = RefundFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		保留返还

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHRefund::RefundFunction()
{
	tCHADevReturn DevStatus;
	CString sLogString(_T(""));
	SYNC(CH,_T("REFUND"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iEjectCoin: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;

	errCode = CHA_EjectCoin(&m_returncoinnum , &DevStatus);
	//tModuleStatus sModuleStatus;
	//tDevReturn sDevReturn[8];
	//errCode = gOCAS002Dev.iEjectCoin(&sModuleStatus,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
		//errCode = sDevReturn[0].iLogicCode;
	}

	sLogString.Format(_T("iEjectCoin: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
