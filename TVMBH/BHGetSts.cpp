#include "stdafx.h"
#include "BHCommand.h"
#include "BHException.h"
//#include "bh_proto.h"

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
CBHGetSts::CBHGetSts()
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
CBHGetSts::~CBHGetSts()
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
long CBHGetSts::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetStsFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		取得执行结果

 @param		(o)BH_STS* rsp	状态结构体

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetSts::GetResponse(BH_STS* rsp)
{
	try {
		*rsp = m_bhSts;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		向纸币接收模块请求状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetSts::GetStsFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 发送命令
	BYTE recvbuf[RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(strlen(CMD_BH_GET_STATUS), (BYTE*)CMD_BH_GET_STATUS, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// 解析反馈信息
	if (recvlen > 0){
		m_bhSts.Deserialize(recvbuf);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
