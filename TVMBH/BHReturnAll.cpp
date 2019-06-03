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
CBHReturnAll::CBHReturnAll(bool isCheckShutter /* = false */)
{
	done_phase = DONE_PHASE_READY;
	m_isCheckShutter = isCheckShutter;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHReturnAll::~CBHReturnAll()
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
long CBHReturnAll::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReturnAllFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回全部纸币（缓冲区+暂存器->出口）

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHReturnAll::ReturnAllFunction()
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
	if (!sts1.IsValidFaceValue() && sts1.cnt == 0 && sts1.reject.empty())
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.发送退钞命令
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_RETURN_ALL), (BYTE*)CMD_BH_RETURN_ALL);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_RETURN_ALL;

	// 4.轮询判断退钞是否成功
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
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 4.2.正常（无错误，或仅有退钞口有钞票错误）
		if (sts.error.empty() || (sts.error.size() == 1 && sts.error.at(0) == ERR_BH_RETURN_SHUTTER))
		{
			// 4.2.1.退钞动作成功
			if (sts.action == ACT_BH_RETURN_BUFF || sts.action == ACT_BH_RETURN_STACK)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			else
			{
				Sleep(1200);
				continue;
			}

			// 4.2.2.直接返回，或乘客取走钞票后返回
			if (!m_isCheckShutter || sts.error.empty())
			{
				done_phase |= DONE_PHASE_SUCCESS;
				return 0;
			}

			Sleep(1000);
			continue;
		}
		// 4.3.异常
		else
		{
			// 4.3.1.退钞动作成功
			if (sts.action == ACT_BH_RETURN_BUFF || sts.action == ACT_BH_RETURN_STACK)
			{
				done_phase |= DONE_PHASE_ACTION;
				return done_phase;
			}

			// 4.3.2.返回错误代码
			return sts.error.at(0);
		}
	}

	// 5.手动中止轮询
	done_phase |= DONE_PHASE_ACT_ABORT;
	return 0;
}
