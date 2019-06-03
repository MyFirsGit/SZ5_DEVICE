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

 @param		BH_FACEVALUE_TABLE& bhTable

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHOpenAndValidate::CBHOpenAndValidate(BH_FACE_ACEPTED_TABLE& bhTable)
{
	done_phase = DONE_PHASE_READY;
	m_bhTable  = bhTable;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHOpenAndValidate::CBHOpenAndValidate()
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
CBHOpenAndValidate::~CBHOpenAndValidate()
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
long CBHOpenAndValidate::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = OpenAndValidateFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)BH_STATUS *res	ֽ�ҽ���ģ��״̬�����ṹ

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
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
 @brief		��ֽ�ҽ���ģ������״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHOpenAndValidate::OpenAndValidateFunction()
{
	long errCode = 0;
	// 0.�ж��Ƿ��Ѿ�ȡ�����ң�
	if(isBHAbort){
		done_phase |= DONE_PHASE_ACT_ABORT;
		return done_phase;
	}
	// 1.��ʼ����ֽ��
	CBHStartValidation startCmd;
	errCode = startCmd.ExecuteCommand();
	if (errCode != SP_SUCCESS)
	{
		return errCode;
	}
	done_phase |= DONE_PHASE_ACT_START_ACCEPT;

	// 2.��ѯ����״̬
	//isBHAbort = false;
	while (!isBHAbort)
	{
		// 2.1.��ȡ״̬
		BH_STS sts;
		CBHGetSts stsCmd;
		errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);
		done_phase |= DONE_PHASE_ACT_GET_STATUS;

		// 2.2.�쳣����
		if (!sts.error.empty())
		{
			if (sts.action == ACT_BH_INSERT_OK)
			{
				done_phase |= DONE_PHASE_ACTION;
			}
			// �˳��ڡ���������ֽ�Һ��Դ˾��棬����ֽ��Ͷ����
			if(sts.error.size() == 1 && (sts.error.at(0) == ERR_BH_RETURN_SHUTTER || ERR_BH_INSERT_SHUTTER))
			{
				//Do nothing...
			}
			else{
				return sts.error.at(0);
			}			
		}

		// 2.3.ֽ��Ͷ��ɹ�
		if (sts.action == ACT_BH_INSERT_OK)
		{
			done_phase |= DONE_PHASE_ACTION;
			return GetFace();
		}

		// 2.4.Ͷ��ʧ�ܣ�������ѯ
		if (sts.action == ACT_BH_INSERT_NG)
		{
			Sleep(400);
			continue;
		}

		// 2.5.��δͶ��
		Sleep(1200);
	}

	// 3.�ֶ���ֹ��ѯ
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ��ֵ

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHOpenAndValidate::GetFace()
{
	long errCode = 0;

	// 1.��ѯ��ֵ
	//isBHAbort = false;
	while (!isBHAbort)
	{
		// 1.1.��ȡ״̬
		BH_STS sts;
		CBHGetSts stsCmd;
		errCode = stsCmd.ExecuteCommand();
		if (errCode != SP_SUCCESS)
		{
			return errCode;
		}
		stsCmd.GetResponse(&sts);

		// 1.2.����ԭ��
		bool need_return = false;
		if (!sts.reject.empty())
		{
			need_return = true;
		}

		// 1.3.��ȡ��ֵ
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
				// �ж��Ƿ���������յ�ֽ�����
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

		// 1.4.������ѯ
		if (!need_return && (!sts.IsValidFaceValue() || m_bhStatus.bh_faceValue.faceValue == VALUE_UNKNOWN))
		{
			Sleep(1200);
			continue;
		}

		// 1.5.�˳��ж�
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

		// 1.6.�ɹ�������״̬
		done_phase |= DONE_PHASE_SUCCESS;
		m_bhStatus.bh_sts = sts;
		return 0;
	}

	// 2.�ֶ���ֹ��ѯ
	done_phase |= DONE_PHASE_ACT_ABORT;
	return done_phase;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��ǿɽ��ܵ���ֵ

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