#pragma once

#include "QRCodeExp.h"
#include "Command.h"
#include "QrCodeCodecAPI.h"

/**
 @brief  编码命令
*/
class QRCode_API CQRCodeEncode : public CCommand
{
public:
	CQRCodeEncode(int nLevel, int nVersion, int nMaskingNo,int nMagnification, CString strSource,CString strFileName);
	~CQRCodeEncode();
public:
	long ExecuteCommand(void);								// 执行命令

private:
	int EncodeFunction();									// 打开

private:
	int m_nLevel;				// 纠错级别		
	int m_nVersion;				// 版本
	int m_nMaskingNo;			// 掩模
	int m_nMagnification;		// 图片放大倍数
	CString m_strSource;		// 数据内容
	CString m_strFileName;		// 文件名
};

/**
@brief  编码命令
*/
class QRCode_API CQRCodeDecode : public CCommand
{
public:
	CQRCodeDecode(CString trFileName);
	~CQRCodeDecode();
public:
	long ExecuteCommand(void);								// 执行命令
	CString GetPRResponse();
private:
	int DecodeFunction();									// 打开

private:
	int m_nLevel;				// 纠错级别		
	int m_nVersion;				// 版本
	int m_nMaskingNo;			// 掩模
	int m_nSize;				// 图片大小
	CString m_strSource;		// 数据内容
	CString m_strFileName;		// 文件名
};
