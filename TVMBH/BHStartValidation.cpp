#include "stdafx.h"
#include "BHCommand.h"
#include "BHException.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHStartValidation::CBHStartValidation()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHStartValidation::~CBHStartValidation()
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
long CBHStartValidation::ExecuteCommand()
{
	long lRet = 1;
	lRet = StartValidationFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ����ֽ�ң�����ʶ��ģʽ����ֽ�ҿڣ�

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHStartValidation::StartValidationFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// ��������
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_START), (BYTE*)CMD_BH_START);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	return 0;
}
