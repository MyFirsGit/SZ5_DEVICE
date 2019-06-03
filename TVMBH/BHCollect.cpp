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
CBHCollect::CBHCollect()
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
CBHCollect::~CBHCollect()
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
long CBHCollect::ExecuteCommand()
{
	long lRet = 1;
	lRet = CollectFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		压钱箱（缓冲区+暂存器->钱箱）

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHCollect::CollectFunction()
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
	done_phase += DONE_PHASE_ACT_GET_STATUS;

	// 2.判断缓存是否有纸币
	if (!sts1.IsValidFaceValue() && sts1.cnt == 0)
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.发送压钞命令
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_COLLECT), (BYTE*)CMD_BH_COLLECT);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_COLLECT;

	// 4.轮询判断压钞动作是否成功
	isBHAbort = false;
	while (!isBHAbort)
	{
		// 4.1.获取状态
		BH_STS sts;
		CBHGetSts stsCmd;
		long errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);
		done_phase += DONE_PHASE_ACT_GET_STATUS;

		// 4.2.正常（无错误，或仅有将满、已满错误）
		if (sts.error.empty() || (sts.error.size() == 1 &&
			(sts.error.at(0) == ERR_BH_NEARLY_FULL || sts.error.at(0) == ERR_BH_FULL)))
		{
			// 4.2.1.压钞动作成功
			if (sts.action == ACT_BH_COLLECT)
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
		// 4.3.异常
		else
		{
			// 4.3.1.压钞动作成功
			if (sts.action == ACT_BH_COLLECT)
			{
				done_phase |= DONE_PHASE_ACTION;
			}

			// 4.3.2.返回错误代码
			return sts.error.at(0);
		}
	}

	// 5.手动中止轮询
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}
