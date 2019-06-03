#include "stdafx.h"
#include "BHChangeCommand.h"
//#include "bh_proto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		(i)int nPort ���ڶ˿ں�
 @param		(i)int nBaud ���ڲ�����
 @param		(i)int nSize �ֳ�

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeConnect::CBHChangeConnect(int nPort, int nBaud, int nSize)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
	this->m_nSize = nSize;
	this->m_connectParam.ComPort           = m_nPort;
	this->m_connectParam.Speed             = m_nBaud;
	this->m_connectParam.MaxRecvCnt        = MAX_SEND_ITEMS_COUNT;
	this->m_connectParam.MaxSendCnt        = MAX_RECEIVE_ITEMS_COUNT;
	this->m_connectParam.MaxRecvBuffSize   = MAX_BUFFER_SIZE;
	this->m_connectParam.MaxSendBuffSize   = MAX_BUFFER_SIZE;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBHChangeConnect::~CBHChangeConnect()
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
long CBHChangeConnect::ExecuteCommand()
{
	long lRet = 1;
	lRet = ConnectFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�򿪴��ڣ���������ز���

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBHChangeConnect::ConnectFunction()
{
	DWORD errCode = CBHChangeException::ERROR_BH_OPEN_FAILED;

	// ���򿪣�����Ҫ�ر�
	if (m_hBH != NULL){
		errCode = BHTrs_Close(m_hBH);
		m_hBH = NULL;
		if (errCode != BHTRS_RES_OK){
			return CBHChangeException::ERROR_BH_CLOSE_FAILED;
		}
	}

	// ��
	errCode = BHTrs_Open(&m_hBH, &m_connectParam);
	if (errCode != BHTRS_RES_OK){
		m_hBH = NULL;
		return CBHChangeException::ERROR_BH_OPEN_FAILED;
	}

	return 0;
}
