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
CCHSetRejectType::CCHSetRejectType(COIN_ACCEPTABLE& acceptTable)
{
	memcpy(&m_acceptTable, &acceptTable, sizeof(m_acceptTable));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CCHSetRejectType::~CCHSetRejectType()
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
long CCHSetRejectType::ExecuteCommand()
{
	long lRet = 1;
	lRet = SetFunction();
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
long CCHSetRejectType::SetFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("SET_REJECT"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("iSetRejectType: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
#if 0
	tModuleStatus sModuleStatus;
	memset(&sModuleStatus,0,sizeof(tModuleStatus));
	tDevReturn sDevReturn[8];
	tRejectInfo l_sRejectInfo;
	memset(&l_sRejectInfo,0x00,sizeof(tRejectInfo));
	// 只拒收5角
	if (m_acceptTable.CH_COIN_ALLOW_05==false && m_acceptTable.CH_COIN_ALLOW_1==true){
		l_sRejectInfo.sRejectData[0].byType = 0;
		l_sRejectInfo.sRejectData[0].iDenomination = 0x01;
	}
	// 只拒收1元
	else if (m_acceptTable.CH_COIN_ALLOW_05==true && m_acceptTable.CH_COIN_ALLOW_1==false){
		l_sRejectInfo.sRejectData[0].byType = 0;
		l_sRejectInfo.sRejectData[0].iDenomination = 0x02;
	}
	// 拒收5角和1元
	else if (m_acceptTable.CH_COIN_ALLOW_05==false && m_acceptTable.CH_COIN_ALLOW_1==false){
		l_sRejectInfo.sRejectData[0].byType = 0;
		l_sRejectInfo.sRejectData[0].iDenomination = 0x03;
	}
	// 接收5角和1元
	else if (m_acceptTable.CH_COIN_ALLOW_05==true && m_acceptTable.CH_COIN_ALLOW_1==true){
		l_sRejectInfo.sRejectData[0].byType = 0;
		l_sRejectInfo.sRejectData[0].iDenomination = 0x00;
	}

	//errCode = gOCAS002Dev.iSetRejectType(&l_sRejectInfo,&sModuleStatus,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	}
#endif 
	sLogString.Format(_T("iSetRejectType: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
