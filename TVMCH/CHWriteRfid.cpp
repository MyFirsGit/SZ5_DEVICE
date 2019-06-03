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
CCHWriteRfid::CCHWriteRfid(int boxNo, BYTE* rfidData)
{
	m_boxNo = boxNo;
	memset(m_RfidData, 0, sizeof(m_RfidData));
	memcpy(m_RfidData, rfidData, 8);
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHWriteRfid::~CCHWriteRfid()
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
long CCHWriteRfid::ExecuteCommand()
{
	long lRet = 1;
	lRet = WriteFunction();
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
long CCHWriteRfid::WriteFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("WRITE_RFID"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iSetCoinBoxSN: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	long errCode = E_NO_CH_ERROR;
	tCHADevReturn DevStatus ={0};
	unsigned char RFIDInfo[36]={0};
	errCode = CHA_WriteRFIDInfo(0,0,RFIDInfo,&DevStatus);
	/*
	tCAS002_RFIDInfo sRfInfo;
	tCAS002_Data sData;
	tDevReturn sDevReturn[8];
	memset(&sData, 0, sizeof(sData));
	memcpy(sData.acData, &m_RfidData, sizeof(m_RfidData));
	errCode = gOCAS002Dev.iSetCoinBoxSN(m_boxNo,&sData,sDevReturn);
	*/
	if (errCode != E_NO_CH_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	}
	else{
		//memcpy(&m_RfidData, sData.acData, sizeof(m_RfidData));
	}

	sLogString.Format(_T("iSetCoinBoxSN: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}