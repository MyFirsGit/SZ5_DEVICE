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
@brief    　构造函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeDecode::CQRCodeDecode( CString strFileName)
{
	m_strFileName = strFileName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeDecode::~CQRCodeDecode()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      打开摄像头

@param      (i)无

@retval     int   \n
0:成功  非0:失败

@exception  无
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
@brief      执行命令

@param      (i)无

@retval     long   \n 
0 : 成功   非0 : 失败 

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
@brief      f获取执行结果

@param      (i)无

@retval     CString 图片解析内容

@exception   
*/
//////////////////////////////////////////////////////////////////////////
CString CQRCodeDecode::GetPRResponse()
{
	return m_strSource;
}
