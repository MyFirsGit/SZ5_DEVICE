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

 @param		(i)bool isCheckShutter 是否检测退钞口有钞票

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHReturnOne::CBHReturnOne(bool isCheckShutter /* = false */)
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
CBHReturnOne::~CBHReturnOne()
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
long CBHReturnOne::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReturnOneFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		退回缓冲区的纸币（缓冲区->出口）

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHReturnOne::ReturnOneFunction()
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

	// 2.判断缓存是否有纸币,是否属于拒收
	if (!sts1.IsValidFaceValue() && sts1.reject.empty())
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.发送命令
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_RETURN_ONE), (BYTE*)CMD_BH_RETURN_ONE);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_RETURN_ONE;

	// 4.轮询判断退钞动作是否成功
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

		// 4.2.正常（无错误，或仅有退钞口有纸币错误）
		if (sts.error.empty() || (sts.error.size() == 1 && sts.error.at(0) == ERR_BH_RETURN_SHUTTER))
		{
			// 4.2.1.退钞动作成功
			if (sts.action == ACT_BH_RETURN_BUFF)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			else
			{
				Sleep(1000);
				continue;
			}

			// 4.2.2.直接返回，或等待乘客取走钞票后返回
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
			if (sts.action == ACT_BH_RETURN_BUFF)
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
	return done_phase;
}
