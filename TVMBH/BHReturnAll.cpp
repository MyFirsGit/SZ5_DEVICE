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
CBHReturnAll::CBHReturnAll(bool isCheckShutter /* = false */)
{
	done_phase = DONE_PHASE_READY;
	m_isCheckShutter = isCheckShutter;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHReturnAll::~CBHReturnAll()
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
long CBHReturnAll::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReturnAllFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�˻�ȫ��ֽ�ң�������+�ݴ���->���ڣ�

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHReturnAll::ReturnAllFunction()
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
	if (!sts1.IsValidFaceValue() && sts1.cnt == 0 && sts1.reject.empty())
	{
		done_phase |= DONE_PHASE_SUCCESS;
		return 0;
	}

	// 3.�����˳�����
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_RETURN_ALL), (BYTE*)CMD_BH_RETURN_ALL);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	done_phase |= DONE_PHASE_ACT_RETURN_ALL;

	// 4.��ѯ�ж��˳��Ƿ�ɹ�
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
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 4.2.�������޴��󣬻�����˳����г�Ʊ����
		if (sts.error.empty() || (sts.error.size() == 1 && sts.error.at(0) == ERR_BH_RETURN_SHUTTER))
		{
			// 4.2.1.�˳������ɹ�
			if (sts.action == ACT_BH_RETURN_BUFF || sts.action == ACT_BH_RETURN_STACK)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			else
			{
				Sleep(1200);
				continue;
			}

			// 4.2.2.ֱ�ӷ��أ���˿�ȡ�߳�Ʊ�󷵻�
			if (!m_isCheckShutter || sts.error.empty())
			{
				done_phase |= DONE_PHASE_SUCCESS;
				return 0;
			}

			Sleep(1000);
			continue;
		}
		// 4.3.�쳣
		else
		{
			// 4.3.1.�˳������ɹ�
			if (sts.action == ACT_BH_RETURN_BUFF || sts.action == ACT_BH_RETURN_STACK)
			{
				done_phase |= DONE_PHASE_ACTION;
				return done_phase;
			}

			// 4.3.2.���ش������
			return sts.error.at(0);
		}
	}

	// 5.�ֶ���ֹ��ѯ
	done_phase |= DONE_PHASE_ACT_ABORT;
	return 0;
}
