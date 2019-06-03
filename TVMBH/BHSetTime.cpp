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
CBHSetTime::CBHSetTime(_DATE_TIME current)
{
	m_current_datetime = current;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHSetTime::~CBHSetTime()
{
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)_DATE_TIME* rsp	ʱ��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHSetTime::GetResponse(_DATE_TIME* rsp)
{
	try {
		*rsp = m_bh_datetime;
	}
	catch (...){
		return CBHException::OTHER_ERR;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ִ������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHSetTime::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = SetTimeFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ֽ�ҽ���ģ������״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHSetTime::SetTimeFunction()
{
	if (m_hBH == NULL || (HANDLE)m_hBH == INVALID_HANDLE_VALUE){
		return CBHException::ERROR_BH_NOT_OPEN;
	}

	// ƴ������
	CStringA cmd;
	cmd.Format("%s%02x%02x%02x%02x%02x%02x", CMD_BH_SET_TIME, 
		m_current_datetime.wYear-2000,m_current_datetime.biMonth, m_current_datetime.biDay,
		m_current_datetime.biHour, m_current_datetime.biMinute, m_current_datetime.biSecond);
	cmd.MakeUpper();

	// ��������
	DWORD recvlen = 0;
	BYTE recvbuf[RES_MAX] = { 0 };
	BHTRS_RESULT result = BHTRS_RES_OK;
	result = SendOneCommand(cmd.GetLength(), (BYTE*)cmd.GetBuffer(cmd.GetLength()), recvbuf, recvlen);
	cmd.ReleaseBuffer();
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// ��֤��������������
	if (recvlen > 0){
		if (recvbuf[0] != 'M' || recvbuf[1] != 'A' || recvbuf[2] != '0')
			return CBHException::ERROR_BH_DESERIALIZE;
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	// ��ȡʱ��
	recvlen = 0;
	memset(recvbuf, 0, RES_MAX);
	result = SendOneCommand(strlen(CMD_BH_GET_TIME), (BYTE*)CMD_BH_GET_TIME, recvbuf, recvlen);
	cmd.ReleaseBuffer();
	if (result != BHTRS_RES_OK){
		return CBHException::ERROR_BH_SEND;
	}

	// ��֤��������������
	if (recvlen > 0){
		if (recvbuf[0] != 'M' || recvbuf[1] != 'A')
		{
			return CBHException::ERROR_BH_RECEIVE;
		}
		LPSTR junk = NULL;
		char waitconv[3];
		// ��
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+2, 2);
		m_bh_datetime.wYear = 2000 + strtoul(waitconv, &junk, 16);
		// ��
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+4, 2);
		m_bh_datetime.biMonth = strtoul(waitconv, &junk, 16);
		// ��
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+6, 2);
		m_bh_datetime.biDay = strtoul(waitconv, &junk, 16);
		// ʱ
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+8, 2);
		m_bh_datetime.biHour = strtoul(waitconv, &junk, 16);
		// ��
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+10, 2);
		m_bh_datetime.biMinute = strtoul(waitconv, &junk, 16);
		// ��
		memset(waitconv, 0, sizeof(waitconv));
		memcpy(waitconv, recvbuf+12, 2);
		m_bh_datetime.biSecond = strtoul(waitconv, &junk, 16);
	}
	else{
		return CBHException::ERROR_BH_RECEIVE;
	}

	return 0;
}
