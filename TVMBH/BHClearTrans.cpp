#include "stdafx.h"
#include "BHCommand.h"
#include "BHException.h"
#include "BHHardwareException.h"
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
CBHClearTrans::CBHClearTrans(BH_CLEAN_DEST dest /* = CLEAN_TO_BOX */)
{
	m_clean_dest = dest;
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
CBHClearTrans::~CBHClearTrans()
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
long CBHClearTrans::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = ClearTransFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		清空传送系统

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHClearTrans::ClearTransFunction()
{
	long errCode = 0;

	// 1.清钞到暂存器/钱箱/退钞口
	if (m_clean_dest == CLEAN_TO_BOX)
	{
		CBHTransCollect collectCmd;
		errCode = collectCmd.ExecuteCommand();

	}
	else if (m_clean_dest == CLEAN_TO_RETURN_SHUTTER)
	{
		CBHTransReturn returnCmd;
		errCode = returnCmd.ExecuteCommand();
	}
	else if (m_clean_dest == CLEAN_TO_REGISTER)
	{
		CBHTransHold holdCmd;
		errCode = holdCmd.ExecuteCommand();
	}
	else
	{
		return BH_FUNC_PARAM_ERR;
	}
	if (errCode != SP_SUCCESS)
	{
		return errCode;
	}
	done_phase |= DONE_PHASE_ACT_CLEAN;

	// 2.轮询判断是否完成
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
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 2.2.清钞成功
		if (sts.action == ACT_BH_CLEAR_TRANS_OK || sts.action == ACT_BH_IDLE)
		{
			done_phase |= DONE_PHASE_ACTION;
			done_phase |= DONE_PHASE_SUCCESS;
			return 0;
		}

		// 2.3.清钞正在进行
		if (sts.action == ACT_BH_CLEARING_TRANS)
		{
			Sleep(1000);
			continue;
		}

		// 2.4.清钞失败
		if (sts.action == ACT_BH_CLEAR_TRANS_NG && !sts.error.empty())
		{
			done_phase |= DONE_PHASE_ACTION;
			return sts.error.at(0);
		}
		
		// 2.5.清钞异常
		return CBHHardwareException::ERROR_BH_CLEAR_MONEY_FAILED;
	}

	// 3.动作被中止
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}
