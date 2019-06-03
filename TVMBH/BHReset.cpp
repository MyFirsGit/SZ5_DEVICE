#include "stdafx.h"
#include "BHCommand.h"
#include "BHException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool isBHAbort;
//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHReset::CBHReset()
{
	done_phase = DONE_PHASE_READY;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHReset::~CBHReset()
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
long CBHReset::ExecuteCommand()
{
	long lRet = 1;
	lRet = ResetFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		复位

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHReset::ResetFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 发送命令
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_RESET), (BYTE*)CMD_BH_RESET);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	Sleep(200);
	// 2.轮询判断复位动作是否成功
	isBHAbort = false;
	while (!isBHAbort)
	{
		// 2.1.获取状态
		BH_STS sts;
		CBHGetSts stsCmd;
		long errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);
		done_phase += DONE_PHASE_ACT_GET_STATUS;

		// 2.2.正常（无错误）
		if (sts.error.empty())
		{
			// 2.2.1.复位动作成功
			if (sts.action == ACT_BH_IDLE)
			{
				done_phase |= DONE_PHASE_ACTION;
				done_phase |= DONE_PHASE_SUCCESS;
				return 0;
			}
			else
			{
				Sleep(300);
				continue;
			}
		}
		// 2.3.异常
		else
		{
			// 2.3.1.复位动作成功
			if (sts.action == ACT_BH_IDLE)
			{
				done_phase |= DONE_PHASE_ACTION;
			}

			// 2.3.2.返回错误代码
			return sts.error.at(0);
		}
	}

	// 3.手动中止轮询
	done_phase |= DONE_PHASE_ACT_ABORT;
	return 0;
}
