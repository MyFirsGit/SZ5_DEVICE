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
CCHReadRfid::CCHReadRfid(int boxNo)
{
	m_boxNo = boxNo;
	memset(&m_RfidData, 0, sizeof(m_RfidData));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHReadRfid::~CCHReadRfid()
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
long CCHReadRfid::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReadFunction();
	this->OnComplete(lRet);
	return lRet;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		���ø�����Ӳ�Ҽ���

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadRfid::ReadFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("READ_RFID"));
	tCHADevReturn DevStatus ={0};
	unsigned char RFIDInfo[36]={0};
	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iGetCoinBoxSN: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;
	errCode =CHA_ReadRFIDInfo(0,0,RFIDInfo,&DevStatus);
	//tCAS002_Data sData;
	//tDevReturn sDevReturn[8];
	//memset(&sData, 0, sizeof(sData));
	//errCode = gOCAS002Dev.iGetCoinBoxSN(m_boxNo,&sData,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	}
	else{
	//	memcpy(&m_RfidData, sData.acData, sizeof(m_RfidData));
	}

	sLogString.Format(_T("iGetCoinBoxSN: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ�����

 @param		LPCH_SYSTEM_STATUS_RSP pRes ָ���ȡ������Ϣ�Ķ���

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadRfid::GetResponse(BYTE* pRes)
{
	memcpy(pRes, m_RfidData, 8);
	return 0;
}