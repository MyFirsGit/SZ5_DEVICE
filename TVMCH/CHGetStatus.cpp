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
CCHGetStatus::CCHGetStatus()
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
CCHGetStatus::~CCHGetStatus()
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
long CCHGetStatus::ExecuteCommand()
{
	long lRet = 1;
	lRet = GetStatusFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取命令反馈

 @param		LPCH_SYSTEM_STATUS_RSP pRes 指向获取反馈信息的对象

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHGetStatus::GetResponse(tCHADevStatus* pRes)
{
	memcpy(pRes, &m_status, sizeof(tCHADevStatus));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		读取状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CCHGetStatus::GetStatusFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("GET_STATUS"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iGetDevStatus: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	long errCode = E_NO_CH_ERROR;
	tCHADevReturn DevStatus ={0};
	errCode = CHA_GetDevStatus(&DevStatus);
	if (errCode != E_NO_CH_ERROR){
		sLogString.Format(_T("iGetDevStatus: errCode = %d"), errCode);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
		return errCode;

	}
	else{
		errCode = CHA_GetCoinBoxInfo(&m_status,&DevStatus);
	}
	//WORD wHopperACnt;			//A循环找零箱中硬币数量
	//WORD wHopperBCnt;			//B循环找零箱中硬币数量
	//WORD wPreparationACnt;		//A备用箱中硬币数量
	//WORD wPreparationBCnt;		//B备用箱中硬币数量
	//WORD wRecoverCnt;			//回收箱中硬币数量(由于现实中5角硬币基本不使用，所以此处不做区分处理，当做只有1元硬币使用)

	sLogString.Format(_T("iGetDevStatus: wHopperACnt = %d, wHopperBCnt = %d, wPreparationACnt = %d,wPreparationBCnt = %d, wRecoverCnt = %d"), 
		m_status.wHopperACnt, m_status.wHopperBCnt, m_status.wPreparationACnt, m_status.wPreparationBCnt, m_status.wRecoverCnt);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	sLogString.Format(_T("iGetDevStatus: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
