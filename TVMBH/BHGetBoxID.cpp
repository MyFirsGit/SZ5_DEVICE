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
CBHGetBoxID::CBHGetBoxID()
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
CBHGetBoxID::~CBHGetBoxID()
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
long CBHGetBoxID::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetBoxIDFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)BH_BOX_ID* rsp	Ǯ��ID

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetBoxID::GetResponse(BH_BOX_ID* rsp)
{
	try {
		*rsp = m_box_id;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ֽ�ҽ���ģ������״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetBoxID::GetBoxIDFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// ѡ��˿�ID
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_USER_ID), (BYTE*)CMD_BH_USER_ID);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// ��ȡ�˿�ID
	DWORD recvlen = 0;
	BYTE recvbuf[RES_MAX] = {0}; 
	result = SendOneCommand(strlen(CMD_BH_READ), (BYTE*)CMD_BH_READ, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		memcpy(m_box_id.user_id, recvbuf, BOX_ID_LEN);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// ѡ��������ID
	result = SendOneCommand_NoRrcv(strlen(CMD_BH_BOX_ID), (BYTE*)CMD_BH_BOX_ID);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// ��ȡ������ID
	recvlen = 0;
	memset(recvbuf, 0, RES_MAX);
	result = SendOneCommand(strlen(CMD_BH_READ), (BYTE*)CMD_BH_READ, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		memcpy(m_box_id.manu_id, recvbuf, BOX_ID_LEN);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
