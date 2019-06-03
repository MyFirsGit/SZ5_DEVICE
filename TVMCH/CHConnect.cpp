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

 @param		(i)int nPort ���ڶ˿ں�
 @param		(i)int nBaud ���ڲ�����
 @param		(i)int nSize �ֳ�

 @retval	��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHConnect::CCHConnect(int nPort, int nBaud, int nSize)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
	this->m_nSize = nSize;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CCHConnect::~CCHConnect()
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
long CCHConnect::ExecuteCommand()
{
	long lRet = 1;
	lRet = ConnectFunction();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�򿪴��ڣ���������ز���

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CCHConnect::ConnectFunction()
{
	CString sLogString(_T(""));
	SYNC(CH,_T("CONNECT"));
	tCHADevReturn DevStatus;
	long errCode = E_NO_CH_ERROR;

	sLogString.Format(_T("iSetCommPara:comport %d baud %d "),m_nPort,m_nBaud);
	theCH_TRACE->WriteData(__LINE__,_T(">")+sLogString,NULL,0);

	//tDevReturn sDevReturn[8];//�ӿڶ���˵���п��ܻ᷵�ض�ά״̬ ����
	errCode = CHA_CommOpen (m_nPort, m_nBaud, &DevStatus);
	//errCode = gOCAS002Dev.iSetCommPara(sDevReturn);
	if (errCode != E_NO_CH_ERROR){
		m_hCH = NULL;
		//errCode = sDevReturn[0].iLogicCode;
	}
	else{
		m_hCH = 1;
	}

	sLogString.Format(_T("iSetCommPara: errCode = %d"), errCode);
	theCH_TRACE->WriteData(__LINE__,_T("<")+sLogString,NULL,0);

	return errCode;
}
