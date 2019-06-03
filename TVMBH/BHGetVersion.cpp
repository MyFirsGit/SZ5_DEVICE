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
CBHGetVersion::CBHGetVersion()
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
CBHGetVersion::~CBHGetVersion()
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
long CBHGetVersion::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = GetVersionFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)BH_VERSION* rsp	�汾��Ϣ

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetVersion::GetResponse(BH_VERSION* rsp)
{
	try {
		*rsp = m_bhVer;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ֽ�ҽ���ģ���ȡ�汾

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHGetVersion::GetVersionFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}
	
	// RS0 : ��ȡ�汾���ϲ�����ϵͳ�汾
	DWORD recvlen = 0;
	BYTE recvbuf[RES_MAX] = {0}; 
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(strlen(CMD_BH_VER_TRANS_TOP), (BYTE*)CMD_BH_VER_TRANS_TOP, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.trans_top);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}
	
	// RS1 : ��ȡ�汾��ʶ����OS�汾
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_TRANS_TOP), (BYTE*)CMD_BH_VER_TRANS_TOP, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.os);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// RS2 : ��ȡ�汾��ʶ�����㷨�汾
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_ALGORITHM), (BYTE*)CMD_BH_VER_ALGORITHM, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.algorithm);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// RS3 : ��ȡ�汾���²�����ϵͳ�汾
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_TRANS_BOTTOM), (BYTE*)CMD_BH_VER_TRANS_BOTTOM, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.trans_bottom);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// RS5 : ��ȡ�汾��ODS�̼��汾
	recvlen = 0;
	memset(recvbuf, 0, sizeof(recvbuf));
	result = SendOneCommand(strlen(CMD_BH_VER_ODS), (BYTE*)CMD_BH_VER_ODS, recvbuf, recvlen);
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}
	if (recvlen > 0){
		m_bhVer.Deserialize(recvbuf, m_bhVer.ods);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
