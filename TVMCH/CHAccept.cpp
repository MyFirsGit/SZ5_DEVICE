#include "stdafx.h"
#include "CHCommand.h"
#include "CHException.h"
#include "CHHardwareException.h"


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
CCHAccept::CCHAccept()
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
CCHAccept::~CCHAccept()
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
long CCHAccept::ExecuteCommand()
{
	long lRet = 1;
	lRet = AcceptFunction();
	this->OnComplete(lRet);
	return lRet;
}


long CCHAccept::GetResponse(tCHACoinValueInfo* rsp)
{
	memcpy(rsp, &m_rsp, sizeof(tCHACoinValueInfo));
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		复位

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHAccept::AcceptFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("ACCEPT"));
	tCHADevReturn DevStatus ={0};

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iAcceptCoinEx: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;

	errCode = CHA_AcceptCoin (&m_rsp,&DevStatus);
//	errCode = gOCAS002Dev.iAcceptCoinEx(&sModuleStatus, nCoin1, nCoin2, sDevReturn, acceptPos);
	if (errCode != E_NO_CH_ERROR){
		//errCode = sDevReturn[0].iLogicCode;
	}


	sLogString.Format(_T("iAcceptCoinEx: 回收1元数量 = %d, 5角数量 = %d"), m_rsp.stCoinBoxValueInfo[0].uiCoinNum, m_rsp.stCoinBoxValueInfo[1].uiCoinNum);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	sLogString.Format(_T("iAcceptCoinEx: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;

}
