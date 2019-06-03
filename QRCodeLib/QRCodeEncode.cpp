#include "stdafx.h"
#include "QRCodeCommand.h"
#include "QRCodeException.h"
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
CQRCodeEncode::CQRCodeEncode( int nLevel, int nVersion, int nMaskingNo,int nMagnification, CString strSource,CString strFileName)
{
	m_nLevel = nLevel;						// ������		
	m_nVersion = nVersion;					// �汾
	m_nMaskingNo = nMaskingNo;				// ��ģ
	m_nMagnification = nMagnification;		// ͼƬ�Ŵ���
	m_strSource = strSource;				// ��������
	m_strFileName = strFileName;			// �ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeEncode::~CQRCodeEncode()
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
int CQRCodeEncode::EncodeFunction()
{
	USES_CONVERSION;
	return  CreateQrCodeBmp(m_nLevel, m_nVersion, m_nMaskingNo,m_nMagnification, T2A((LPTSTR)m_strSource.GetString()),T2A((LPTSTR)m_strFileName.GetString()));
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
long CQRCodeEncode::ExecuteCommand()
{
	long iRet = 1;
	iRet = EncodeFunction();
	this->OnComplete(iRet);
	return iRet;
}