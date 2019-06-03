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

 @param		BH_FACEVALUE_TABLE& bhTable

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHOpenAndValidate::CBHOpenAndValidate(BH_FACE_ACEPTED_TABLE& bhTable)
{
	done_phase = DONE_PHASE_READY;
	m_bhTable  = bhTable;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBHOpenAndValidate::CBHOpenAndValidate()
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
CBHOpenAndValidate::~CBHOpenAndValidate()
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
long CBHOpenAndValidate::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = OpenAndValidateFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		取得执行结果

 @param		(o)BH_STATUS *res	纸币接收模块状态解析结构

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHOpenAndValidate::GetResponse(BH_STATUS* rsp)
{
	try {
		*rsp = m_bhStatus;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

void  BH_EnableInsert()
{
	isBHAbort=false;
}

void  BH_DisableInsert()
{
	isBHAbort=true;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		向纸币接收模块请求状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHOpenAndValidate::OpenAndValidateFunction()
{
	long errCode = 0;
	// 0.判断是否已经取消进币？
	if(isBHAbort){
		done_phase |= DONE_PHASE_ACT_ABORT;
		return done_phase;
	}
	// 1.开始接收纸币
	CBHStartValidation startCmd;
	errCode = startCmd.ExecuteCommand();
	if (errCode != SP_SUCCESS)
	{
		return errCode;
	}
	done_phase |= DONE_PHASE_ACT_START_ACCEPT;

	// 2.轮询接收状态
	//isBHAbort = false;
	while (!isBHAbort)
	{
		// 2.1.获取状态
		BH_STS sts;
		CBHGetSts stsCmd;
		errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 2.2.异常错误
		if (!sts.error.empty())
		{
			if (sts.action == ACT_BH_INSERT_OK)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			// 退钞口、进钞口有纸币忽略此警告，继续纸币投入检查
			if(sts.error.size() == 1 && (sts.error.at(0) == ERR_BH_RETURN_SHUTTER || ERR_BH_INSERT_SHUTTER))
			{
				//Do nothing...
			}
			else{
				return sts.error.at(0);
			}			
		}

		// 2.3.纸币投入成功
		if (sts.action == ACT_BH_INSERT_OK)
		{
			done_phase |= DONE_PHASE_ACTION;
			return GetFace();
		}

		// 2.4.投入失败，继续轮询
		if (sts.action == ACT_BH_INSERT_NG)
		{
			Sleep(400);
			continue;
		}

		// 2.5.尚未投入
		Sleep(1200);
	}

	// 3.手动中止轮询
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		获取面值

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBHOpenAndValidate::GetFace()
{
	long errCode = 0;

	// 1.轮询面值
	//isBHAbort = false;
	while (!isBHAbort)
	{
		// 1.1.获取状态
		BH_STS sts;
		CBHGetSts stsCmd;
		errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);

		// 1.2.拒收原因
		bool need_return = false;
		if (!sts.reject.empty())
		{
			need_return = true;
		}

		// 1.3.获取面值
		if (!need_return && sts.IsValidFaceValue())
		{
			CBHGetFaceValue faceCmd(sts.valueNo);
			errCode = faceCmd.ExecuteCommand();
			if (errCode != SP_SUCCESS)
			{
				need_return = true;
			}
			else
			{
				faceCmd.GetResponse(&m_bhStatus.bh_faceValue);
				// 判断是否是允许接收的纸币面额
				if (IsAcceptFaceValue(m_bhStatus.bh_faceValue.faceValue)){
					done_phase |= DONE_PHASE_ACT_GET_FACE;
				}
				else
				{
					need_return = true;
					sts.reject.push_back(BH_REJECT::REJECT_BH_NO_PERMISSION);
				}				
			}
		}

		// 1.4.继续轮询
		if (!need_return && (!sts.IsValidFaceValue() || m_bhStatus.bh_faceValue.faceValue == VALUE_UNKNOWN))
		{
			Sleep(1200);
			continue;
		}

		// 1.5.退钞判断
		if (need_return)
		{
			CBHReturnOne returnCmd(true);
			errCode = returnCmd.ExecuteCommand();
			if (errCode != SP_SUCCESS)
			{
				return errCode;
			}
			done_phase |= DONE_PHASE_ACT_RETURN_ONE;
		}

		// 1.6.成功，返回状态
		done_phase |= DONE_PHASE_SUCCESS;
		m_bhStatus.bh_sts = sts;
		return 0;
	}

	// 2.手动中止轮询
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否是可接受的面值

@param      BankNoteAndCoinType_t& thisFaceValue

@retval     bool

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CBHOpenAndValidate::IsAcceptFaceValue(BankNoteAndCoinType_t& thisFaceValue){
	switch (thisFaceValue){
	case Banknote1Yuan:
		return m_bhTable.bIsBanknote1Yuan;
	case Banknote5Yuan:
		return m_bhTable.bIsBanknote5Yuan;
	case Banknote10Yuan:
		return m_bhTable.bIsBanknote10Yuan;
	case Banknote20Yuan:
		return m_bhTable.bIsBanknote20Yuan;
	case Banknote50Yuan:
		return m_bhTable.bIsBanknote50Yuan;
	case Banknote100Yuan:
		return m_bhTable.bIsBanknote100Yuan;
	default:
		return false;
	}
}