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
CBHGetBoxID::CBHGetBoxID()
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
CBHGetBoxID::~CBHGetBoxID()
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
long CBHGetBoxID::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetBoxIDFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		取得执行结果

 @param		(o)BH_BOX_ID* rsp	钱箱ID

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetBoxID::GetResponse(BH_BOX_ID* rsp)
{
	try {
		*rsp = m_box_id;
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
long CBHGetBoxID::GetBoxIDFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 选择顾客ID
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_USER_ID), (BYTE*)CMD_BH_USER_ID);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// 读取顾客ID
	DWORD recvlen = 0;
	BYTE recvbuf[RES_MAX] = {0}; 
	result = SendOneCommand(strlen(CMD_BH_READ), (BYTE*)CMD_BH_READ, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		memcpy(m_box_id.user_id, recvbuf, BOX_ID_LEN);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// 选择制造商ID
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_BOX_ID), (BYTE*)CMD_BH_BOX_ID);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// 读取制造商ID
	recvlen = 0;
	memset(recvbuf, 0, RES_MAX);
	result = SendOneCommand(strlen(CMD_BH_READ), (BYTE*)CMD_BH_READ, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		memcpy(m_box_id.manu_id, recvbuf, BOX_ID_LEN);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
