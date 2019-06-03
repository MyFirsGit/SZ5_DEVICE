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
CCHGetStatus::CCHGetStatus()
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
CCHGetStatus::~CCHGetStatus()
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
long CCHGetStatus::ExecuteCommand()
{
	long lRet = 1;
	lRet = GetStatusFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ�����

 @param		LPCH_SYSTEM_STATUS_RSP pRes ָ���ȡ������Ϣ�Ķ���

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHGetStatus::GetResponse(tCHADevStatus* pRes)
{
	memcpy(pRes, &m_status, sizeof(tCHADevStatus));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ȡ״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHGetStatus::GetStatusFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("GET_STATUS"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	sLogString.Format(_T("iGetDevStatus: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);
	long errCode = E_NO_CH_ERROR;
	tCHADevReturn DevStatus ={0};
	errCode = CHA_GetDevStatus(&DevStatus);
	if (errCode != E_NO_CH_ERROR){
		sLogString.Format(_T("iGetDevStatus: errCode = %d"), errCode);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
		return errCode;

	}
	else{
		errCode = CHA_GetCoinBoxInfo(&m_status,&DevStatus);
	}
	//WORD wHopperACnt;			//Aѭ����������Ӳ������
	//WORD wHopperBCnt;			//Bѭ����������Ӳ������
	//WORD wPreparationACnt;		//A��������Ӳ������
	//WORD wPreparationBCnt;		//B��������Ӳ������
	//WORD wRecoverCnt;			//��������Ӳ������(������ʵ��5��Ӳ�һ�����ʹ�ã����Դ˴��������ִ�������ֻ��1ԪӲ��ʹ��)

	sLogString.Format(_T("iGetDevStatus: wHopperACnt = %d, wHopperBCnt = %d, wPreparationACnt = %d,wPreparationBCnt = %d, wRecoverCnt = %d"), 
		m_status.wHopperACnt, m_status.wHopperBCnt, m_status.wPreparationACnt, m_status.wPreparationBCnt, m_status.wRecoverCnt);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	sLogString.Format(_T("iGetDevStatus: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
