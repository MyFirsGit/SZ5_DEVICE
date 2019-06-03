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
CBHGetSts::CBHGetSts()
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
CBHGetSts::~CBHGetSts()
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
long CBHGetSts::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetStsFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)BH_STS* rsp	״̬�ṹ��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetSts::GetResponse(BH_STS* rsp)
{
	try {
		*rsp = m_bhSts;
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
long CBHGetSts::GetStsFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// ��������
	BYTE recvbuf[RES_MAX] = { 0 }; 
	DWORD recvlen = 0;
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(strlen(CMD_BH_GET_STATUS), (BYTE*)CMD_BH_GET_STATUS, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// ����������Ϣ
	if (recvlen > 0){
		m_bhSts.Deserialize(recvbuf);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
