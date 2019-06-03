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
CBHReset::CBHReset()
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
CBHReset::~CBHReset()
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
long CBHReset::ExecuteCommand()
{
	long lRet = 1;
	lRet = ResetFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��λ

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHReset::ResetFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// ��������
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_RESET), (BYTE*)CMD_BH_RESET);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	Sleep(200);
	// 2.��ѯ�жϸ�λ�����Ƿ�ɹ�
	isBHAbort = false;
	while (!isBHAbort)
	{
		// 2.1.��ȡ״̬
		BH_STS sts;
		CBHGetSts stsCmd;
		long errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);
		done_phase += DONE_PHASE_ACT_GET_STATUS;

		// 2.2.�������޴���
		if (sts.error.empty())
		{
			// 2.2.1.��λ�����ɹ�
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
		// 2.3.�쳣
		else
		{
			// 2.3.1.��λ�����ɹ�
			if (sts.action == ACT_BH_IDLE)
			{
				done_phase |= DONE_PHASE_ACTION;
			}

			// 2.3.2.���ش������
			return sts.error.at(0);
		}
	}

	// 3.�ֶ���ֹ��ѯ
	done_phase |= DONE_PHASE_ACT_ABORT;
	return 0;
}
