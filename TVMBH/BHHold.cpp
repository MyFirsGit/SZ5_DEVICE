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
CBHHold::CBHHold()
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
CBHHold::~CBHHold()
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
long CBHHold::ExecuteCommand()
{
	long lRet = 1;
	lRet = HoldFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ѹ�ݴ棨������->�ݴ�����

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHHold::HoldFunction()
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
	done_phase |= DONE_PHASE_ACT_GET_STATUS;

	// 2.�жϻ����Ƿ���ֽ��
	if (!sts1.IsValidFaceValue())
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.����ѹ�ݴ�����
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_HOLD), (BYTE*)CMD_BH_HOLD);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_HOLD;

	// 4.��ѯ�ж�ѹ�ݴ����Ƿ�ɹ�
	//isBHAbort = false;
	while (!isBHAbort)
	{
		// 4.1.��ȡ״̬
		BH_STS sts2;
		CBHGetSts sts2Cmd;
		long errCode = sts2Cmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		sts2Cmd.GetResponse(&sts2);
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 4.2.�������޴���
		if (sts2.error.empty())
		{
			// 4.2.1.ѹ�ݴ涯���ɹ�
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
		// 4.3.�쳣
		else
		{
			if (sts2.action == ACT_BH_HOLD)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			return sts2.error.at(0);
		}
	}

	// 5.�ֶ���ֹ��ѯ
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}
