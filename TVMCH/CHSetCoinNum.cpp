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
CCHSetCoinNum::CCHSetCoinNum(tCoinBoxInfo numInfo)
{
	memcpy(&m_numInfo,&numInfo,sizeof(tCoinBoxInfo));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHSetCoinNum::~CCHSetCoinNum()
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
long CCHSetCoinNum::ExecuteCommand()
{
	long lRet = 1;
	lRet = SetNumFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		设置各箱子硬币计数

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHSetCoinNum::SetNumFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("SET_COIN_NUM"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iWriteCoinNum: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;

	tCHADevReturn DevStatus;
	errCode = CHA_SetCoinBoxInfo(&m_numInfo,&DevStatus);
	if (errCode != E_NO_CH_ERROR){
		//errCode = sDevReturn[0].iLogicCode;
	}

	sLogString.Format(_T("iWriteCoinNum: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
