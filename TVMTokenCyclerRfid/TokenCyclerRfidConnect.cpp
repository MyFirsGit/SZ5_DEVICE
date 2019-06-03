#include "stdafx.h"
#include "TokenCyclerRfidCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool m_bTokenCyclerRfidConnected = false;

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
CTCRfidConnect::CTCRfidConnect(int nPort, int nBaud)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidConnect::~CTCRfidConnect()
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
long CTCRfidConnect::ExecuteCommand()
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
long CTCRfidConnect::ConnectFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CONNECT"));

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("zlg500B_init: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);


	errCode = zlg500B_init(m_nPort, m_nBaud);
	if (errCode != E_NO_CS_ERROR){
		m_bTokenCyclerRfidConnected = false;
	}
	else{
		m_bTokenCyclerRfidConnected = true;
	}

	sLogString.Format(_T("zlg500B_init: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
