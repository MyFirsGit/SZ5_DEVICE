#pragma once

#include "QRCodeExp.h"
#include "Command.h"
#include "QrCodeCodecAPI.h"

/**
 @brief  ��������
*/
class QRCode_API CQRCodeEncode : public CCommand
{
public:
	CQRCodeEncode(int nLevel, int nVersion, int nMaskingNo,int nMagnification, CString strSource,CString strFileName);
	~CQRCodeEncode();
public:
	long ExecuteCommand(void);								// ִ������

private:
	int EncodeFunction();									// ��

private:
	int m_nLevel;				// ������		
	int m_nVersion;				// �汾
	int m_nMaskingNo;			// ��ģ
	int m_nMagnification;		// ͼƬ�Ŵ���
	CString m_strSource;		// ��������
	CString m_strFileName;		// �ļ���
};

/**
@brief  ��������
*/
class QRCode_API CQRCodeDecode : public CCommand
{
public:
	CQRCodeDecode(CString trFileName);
	~CQRCodeDecode();
public:
	long ExecuteCommand(void);								// ִ������
	CString GetPRResponse();
private:
	int DecodeFunction();									// ��

private:
	int m_nLevel;				// ������		
	int m_nVersion;				// �汾
	int m_nMaskingNo;			// ��ģ
	int m_nSize;				// ͼƬ��С
	CString m_strSource;		// ��������
	CString m_strFileName;		// �ļ���
};
