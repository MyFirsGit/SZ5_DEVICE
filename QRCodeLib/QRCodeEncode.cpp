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
@brief    　构造函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeEncode::CQRCodeEncode( int nLevel, int nVersion, int nMaskingNo,int nMagnification, CString strSource,CString strFileName)
{
	m_nLevel = nLevel;						// 纠错级别		
	m_nVersion = nVersion;					// 版本
	m_nMaskingNo = nMaskingNo;				// 掩模
	m_nMagnification = nMagnification;		// 图片放大倍数
	m_strSource = strSource;				// 数据内容
	m_strFileName = strFileName;			// 文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CQRCodeEncode::~CQRCodeEncode()
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
int CQRCodeEncode::EncodeFunction()
{
	USES_CONVERSION;
	return  CreateQrCodeBmp(m_nLevel, m_nVersion, m_nMaskingNo,m_nMagnification, T2A((LPTSTR)m_strSource.GetString()),T2A((LPTSTR)m_strFileName.GetString()));
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
long CQRCodeEncode::ExecuteCommand()
{
	long iRet = 1;
	iRet = EncodeFunction();
	this->OnComplete(iRet);
	return iRet;
}