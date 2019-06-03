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
 @brief		���캯��

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHCollect::CBHCollect()
{
	done_phase = DONE_PHASE_READY;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHCollect::~CBHCollect()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ִ������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
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
 @brief		ѹǮ�䣨������+�ݴ���->Ǯ�䣩

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHCollect::CollectFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// 1.��ȡ״̬
	BH_STS sts1;
	CBHGetSts sts1Cmd;
	long errCode = sts1Cmd.ExecuteCommand();
	if (errCode != SP_SUCCESS)
	{
		return errCode;
	}
	sts1Cmd.GetResponse(&sts1);
	done_phase += DONE_PHASE_ACT_GET_STATUS;

	// 2.�жϻ����Ƿ���ֽ��
	if (!sts1.IsValidFaceValue() && sts1.cnt == 0)
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.����ѹ������
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_COLLECT), (BYTE*)CMD_BH_COLLECT);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_COLLECT;

	// 4.��ѯ�ж�ѹ�������Ƿ�ɹ�
	isBHAbort = false;
	while (!isBHAbort)
	{
		// 4.1.��ȡ״̬
		BH_STS sts;
		CBHGetSts stsCmd;
		long errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);
		done_phase += DONE_PHASE_ACT_GET_STATUS;

		// 4.2.�������޴��󣬻���н�������������
		if (sts.error.empty() || (sts.error.size() == 1 &&
			(sts.error.at(0) == ERR_BH_NEARLY_FULL || sts.error.at(0) == ERR_BH_FULL)))
		{
			// 4.2.1.ѹ�������ɹ�
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
		// 4.3.�쳣
		else
		{
			// 4.3.1.ѹ�������ɹ�
			if (sts.action == ACT_BH_COLLECT)
			{
				done_phase |= DONE_PHASE_ACTION;
			}

			// 4.3.2.���ش������
			return sts.error.at(0);
		}
	}

	// 5.�ֶ���ֹ��ѯ
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}
