#include "stdafx.h"
#include "TokenCyclerRfidCommand.h"


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
CTCRfidClose::CTCRfidClose()
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
CTCRfidClose::~CTCRfidClose()
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
long CTCRfidClose::ExecuteCommand()
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
long CTCRfidClose::CloseFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CLOSE"));

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("zlg500B_exit: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (m_bTokenCyclerRfidConnected){
		zlg500B_exit();
	}


	sLogString.Format(_T("zlg500B_exit: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
