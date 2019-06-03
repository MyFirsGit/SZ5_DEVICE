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
CBHTransCollect::CBHTransCollect()
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
CBHTransCollect::~CBHTransCollect()
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
long CBHTransCollect::ExecuteCommand()
{
	long lRet = 1;
	lRet = TransCollectFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		传送区->钱箱

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHTransCollect::TransCollectFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 发送命令
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_TRANS_COLLECT), (BYTE*)CMD_BH_TRANS_COLLECT);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	return 0;
}
