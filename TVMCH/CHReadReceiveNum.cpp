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
CCHReadCoinNum::CCHReadCoinNum()
{
	memset(&m_res, 0x00, sizeof(m_res));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHReadCoinNum::~CCHReadCoinNum()
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
long CCHReadCoinNum::ExecuteCommand()
{
	long lRet = 1;
	lRet = ReadCoinNumFunction();
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
long CCHReadCoinNum::GetResponse(tCHACoinValueInfo* pRes)
{
	memcpy(pRes,&m_res, sizeof(m_res));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�رմ���

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHReadCoinNum::ReadCoinNumFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("Read_Coin"));

	if (m_hCH == NULL || (HANDLE)m_hCH == INVALID_HANDLE_VALUE){
		return CCHException::ERROR_CH_NOT_OPEN;
	}

	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("iReadCoinNum: "));
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	//tDevReturn sDevReturn[8];
    tCHADevReturn DevStatus  ={0};
	errCode = CHA_GetReceiveNumber(&m_res, &DevStatus);
	//errCode = gOCAS002Dev.iReadCoinNum(m_res,sDevReturn);
	if (errCode != E_NO_CH_ERROR){
	//	errCode = sDevReturn[0].iLogicCode;
	}
	
	for(int i=0;i<2;i++){
		sLogString.Format(_T("iReadCoinNum: ���յ�Ӳ����Ϣ����%d��ö����%d��"), m_res.stCoinBoxValueInfo[i].uiCoinValue,m_res.stCoinBoxValueInfo[i].uiCoinNum);
		theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);
	}

	sLogString.Format(_T("iReadCoinNum: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
