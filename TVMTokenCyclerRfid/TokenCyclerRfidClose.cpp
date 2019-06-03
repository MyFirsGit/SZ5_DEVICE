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
CTCRfidClose::CTCRfidClose()
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
CTCRfidClose::~CTCRfidClose()
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
long CTCRfidClose::ExecuteCommand()
{
	long lRet = 1;
	lRet = CloseFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�رմ���

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CTCRfidClose::CloseFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CLOSE"));

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("zlg500B_exit: "));
	theTokenCyclerRfid_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	if (m_bTokenCyclerRfidConnected){
		zlg500B_exit();
	}


	sLogString.Format(_T("zlg500B_exit: errCode = %d"), errCode);
	theTokenCyclerRfid_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
