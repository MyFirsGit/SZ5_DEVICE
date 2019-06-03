#include "stdafx.h"
#include "CHCommand.h"


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
CCHStartReceive::CCHStartReceive()
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
CCHStartReceive::~CCHStartReceive()
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
long CCHStartReceive::ExecuteCommand()
{
	long lRet = 1;
	lRet = StartReceiveFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ʼ����Ӳ��

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHStartReceive::StartReceiveFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("RECEIVE"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iStartRevCoin: "));
	theCH_TRACE->WriteData(_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;
	tCHADevReturn DevStatus = {0};
    errCode =CHA_StartReceiveCoin(&DevStatus);
	//tModuleStatus sModuleStatus;
	//tDevReturn sDevReturn[8];
	//errCode = gOCAS002Dev.iStartRevCoin(&sModuleStatus,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
		//errCode = sDevReturn[0].iLogicCode;
	}

	sLogString.Format(_T("iStartRevCoin: errCode = %d"), errCode);
	theCH_TRACE->WriteData(_T("<")+sLogString,NULL,0);

	return errCode;
}
