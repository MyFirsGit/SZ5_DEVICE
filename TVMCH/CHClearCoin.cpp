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

 @param		void

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHClearCoin::CCHClearCoin()
{
	memset(&CoinBoxInfo,0,sizeof(tCoinBoxInfo));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHClearCoin::~CCHClearCoin()
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
long CCHClearCoin::ExecuteCommand()
{
	long lRet = 1;
	lRet = ClearFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		����

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHClearCoin::ClearFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("CLEAR"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iClearCoin: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;

	tCHADevReturn DevStatus={0};
    errCode = CHA_ClearCoin (0,0,0,0,&CoinBoxInfo,0,&DevStatus);
	if (errCode != E_NO_CH_ERROR){
	}


	sLogString.Format(_T("iClearCoin: hopperA = %d, hopperB = %d changeBoxA = %d changeBoxB = %d"), 
		CoinBoxInfo.OneY_inHopper,CoinBoxInfo.HalfY_inHopper,CoinBoxInfo.OneY_preparation,CoinBoxInfo.HalfY_preparation);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	sLogString.Format(_T("iClearCoin: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;

}


long CCHClearCoin::GetResponse(tCoinBoxInfo* boxinfo)
{
	if (boxinfo != NULL)
	{
		memcpy(boxinfo,&CoinBoxInfo,sizeof(tCoinBoxInfo));
	}
	return 0;
}
