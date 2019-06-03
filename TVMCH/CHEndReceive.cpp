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
CCHEndReceive::CCHEndReceive()
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
CCHEndReceive::~CCHEndReceive()
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
long CCHEndReceive::ExecuteCommand()
{
	long lRet = 1;
	lRet = EndFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������Ӳ��

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHEndReceive::EndFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("END_RECEIVE"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("iStopRevCoin: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	//tModuleStatus sModuleStatus;
	//memset(&sModuleStatus,0,sizeof(tModuleStatus));
	//tDevReturn sDevReturn[8];
	tCHADevReturn DevStatus;

    errCode = CHA_StopReceiveCoin(&DevStatus);
	//errCode = gOCAS002Dev.iStopRevCoin(&sModuleStatus,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	}

	sLogString.Format(_T("iStopRevCoin: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;

}
