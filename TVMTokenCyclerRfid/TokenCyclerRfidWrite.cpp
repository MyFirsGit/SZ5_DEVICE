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

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidWrite::CTCRfidWrite(BYTE* rfidData)
{
	memset(m_rfidData, 0, sizeof(m_rfidData));
	memcpy(m_rfidData, rfidData, 8);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CTCRfidWrite::~CTCRfidWrite()
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
long CTCRfidWrite::ExecuteCommand()
{
	long lRet = 1;
	lRet = WriteFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidWrite::WriteFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("WRITE"));

	if (m_bTokenCyclerRfidConnected == false){
		return CTokenCyclerRfidHardwareException::CS_ERR_NOT_INIT;
	}

	sLogString.Format(_T("zlg500B_write: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CS_ERROR;

	/*
		addr:���(0x00--0x3f) 
		data:������ ���� 16 ���ֽ� 
		�������� �˺��������Ƕ�ָ���Ŀ����д���� ����ֵ 0 ��ʾ�ɹ� ���򷵻ش�����
	*/
	unsigned char addr = 1;
	errCode = zlg500B_write(addr, m_rfidData);

	sLogString.Format(_T("zlg500B_write: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
