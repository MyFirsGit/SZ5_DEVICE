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
@brief      获取二维码

@retval		(i)nLevel;				纠错级别		
@retval		(i)nVersion;			版本
@retval		(i)nMaskingNo;			掩模
@retval		(i)nMagnification;		图片放大倍数
@retval		(i)strSource;			数据内容
@retval		(i)strFileName;			文件名

@retval     0 成功
			other 失败

@exception  无
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
@brief      删除本地二维码

@retval		(i)strFileName;			文件名

@retval     无

@exception  无
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