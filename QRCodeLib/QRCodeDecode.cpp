#include "stdafx.h"
#include "QRCodeCommand.h"
#include "QRCodeException.h"
#define  BUFFER_LENGTH  1024

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����캯��

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeDecode::CQRCodeDecode( CString strFileName)
{
	m_strFileName = strFileName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeDecode::~CQRCodeDecode()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ͷ

@param      (i)��

@retval     int   \n
0:�ɹ�  ��0:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CQRCodeDecode::DecodeFunction()
{
	char szContent[BUFFER_LENGTH] = {0};
	USES_CONVERSION;
	int iRet =  DecodeQrCodeBmp(T2A((LPTSTR)m_strFileName.GetString()),m_nLevel,m_nVersion,m_nSize,szContent,BUFFER_LENGTH);
	m_strSource = A2T(szContent);
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      (i)��

@retval     long   \n 
0 : �ɹ�   ��0 : ʧ�� 

@exception   
*/
//////////////////////////////////////////////////////////////////////////
long CQRCodeDecode::ExecuteCommand()
{
	long iRet = 1;
	iRet = DecodeFunction();
	this->OnComplete(iRet);
	return iRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      f��ȡִ�н��

@param      (i)��

@retval     CString ͼƬ��������

@exception   
*/
//////////////////////////////////////////////////////////////////////////
CString CQRCodeDecode::GetPRResponse()
{
	return m_strSource;
}
