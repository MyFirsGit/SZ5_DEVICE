#include "stdafx.h"
#include "TokenCyclerRfidCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		void

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidRead::CTCRfidRead()
{
	memset(m_rfidData, 0, sizeof(m_rfidData));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidRead::~CTCRfidRead()
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
long CTCRfidRead::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReadFunction();
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
long CTCRfidRead::ReadFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("READ"));

	if (m_bTokenCyclerRfidConnected == false){
		return CTokenCyclerRfidHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("zlg500B_read: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	/*
		addr:���(0x00--0x3f) 
		data:������ ���� 16 ���ֽ� 
		�������� �˺��������Ƕ�ȡ����һ��������� ����ֵ 0 ��ʾ�ɹ� ���򷵻ش�����
	*/
	long errCode = E_NO_CS_ERROR;
	unsigned char addr = 1*4 + 1;//��1����1��
	errCode = zlg500B_read(addr, m_rfidData);

	sLogString.Format(_T("zlg500B_read: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}


long CTCRfidRead::GetResponse(BYTE* rfidData)
{
	memcpy(rfidData, m_rfidData, 8);
	return 0;
}