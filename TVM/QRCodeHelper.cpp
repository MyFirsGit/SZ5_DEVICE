#include "StdAfx.h"
#include "QRCodeHelper.h"


CQRCodeHelper::CQRCodeHelper(void)
{
}


CQRCodeHelper::~CQRCodeHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��ά��

@retval		(i)nLevel;				������		
@retval		(i)nVersion;			�汾
@retval		(i)nMaskingNo;			��ģ
@retval		(i)nMagnification;		ͼƬ�Ŵ���
@retval		(i)strSource;			��������
@retval		(i)strFileName;			�ļ���

@retval     0 �ɹ�
			other ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CQRCodeHelper::QRCodeEncode(int nLevel, int nVersion, int nMaskingNo,int nMagnification, CString strSource,CString strFileName)
{
	long lRet = 0;
	CQRCodeEncode QRcodeEncode(nLevel,nVersion,nMaskingNo,nMagnification,strSource,strFileName);
	lRet = QRcodeEncode.ExecuteCommand();
	return lRet;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ɾ�����ض�ά��

@retval		(i)strFileName;			�ļ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CQRCodeHelper::DeleteQRCode(CString strFileName)
{
	CString strAppPath;
	GetAppPath(strAppPath);
	if (strFileName.GetBuffer() == NULL || strFileName.GetLength() ==0)
	{
		strFileName = _T("");
	}
	else
		strFileName = strAppPath + strFileName;
	DeleteFile(strFileName);

}