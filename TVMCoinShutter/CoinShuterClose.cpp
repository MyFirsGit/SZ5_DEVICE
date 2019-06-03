#include "stdafx.h"
#include "CoinShutterCommand.h"


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
CCSClose::CCSClose()
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
CCSClose::~CCSClose()
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
long CCSClose::ExecuteCommand()
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
long CCSClose::CloseFunction()
{
	CString sLogString(_T(""));
	SYNC(CS,_T("CLOSE"));

	long errCode = E_NO_CS_ERROR;

	sLogString.Format(_T("close: "));
	theCoinShutter_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	int iRet = 1;
	if ( m_hCoinShutter != NULL && m_hCoinShutter != INVALID_HANDLE_VALUE) {
		if (!CloseHandle(m_hCoinShutter)) {
			iRet = CCoinShutterException::ERROR_CS_CLOSE_FAILED;
			return iRet;
		}
		else{
			iRet = 0;
			m_hCoinShutter = NULL;
		}
	}
	else {
		iRet = 0;//CPrinterException::ERROR_Printer_NOTOPEN ;
	}

	errCode = iRet;
	sLogString.Format(_T("close: errCode = %d"), errCode);
	theCoinShutter_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
