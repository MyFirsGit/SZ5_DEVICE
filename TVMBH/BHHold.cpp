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
CBHHold::CBHHold()
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
CBHHold::~CBHHold()
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
long CBHHold::ExecuteCommand()
{
	long lRet = 1;
	lRet = HoldFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压暂存（缓冲区->暂存器）

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHHold::HoldFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 1.读取状态
	BH_STS sts1;
	CBHGetSts sts1Cmd;
	long errCode = sts1Cmd.ExecuteCommand();
	if (errCode != SP_SUCCESS)
	{
		return errCode;
	}
	sts1Cmd.GetResponse(&sts1);
	done_phase |= DONE_PHASE_ACT_GET_STATUS;

	// 2.判断缓存是否有纸币
	if (!sts1.IsValidFaceValue())
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.发送压暂存命令
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_HOLD), (BYTE*)CMD_BH_HOLD);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_HOLD;

	// 4.轮询判断压暂存作是否成功
	//isBHAbort = false;
	while (!isBHAbort)
	{
		// 4.1.获取状态
		BH_STS sts2;
		CBHGetSts sts2Cmd;
		long errCode = sts2Cmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		sts2Cmd.GetResponse(&sts2);
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 4.2.正常（无错误）
		if (sts2.error.empty())
		{
			// 4.2.1.压暂存动作成功
			if (sts2.action == ACT_BH_HOLD)
			{
				done_phase |= DONE_PHASE_ACTION;
				done_phase |= DONE_PHASE_SUCCESS;
				return 0;
			}
			else
			{
				Sleep(50);
				continue;
			}
		}
		// 4.3.异常
		else
		{
			if (sts2.action == ACT_BH_HOLD)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			return sts2.error.at(0);
		}
	}

	// 5.手动中止轮询
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}
