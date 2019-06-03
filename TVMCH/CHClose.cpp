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
CCHClose::CCHClose()
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
CCHClose::~CCHClose()
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
long CCHClose::ExecuteCommand()
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
long CCHClose::CloseFunction()
{
	tCHADevReturn DevStatus;
	CString sLogString(_T(""));
	SYNC(CH,_T("CLOSE"));

	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("close: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	memset(&DevStatus,0,sizeof(DevStatus));
	errCode = CHA_CloseComm(&DevStatus);
	sLogString.Format(_T("close: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
