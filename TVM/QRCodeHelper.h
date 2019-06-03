#pragma once
#include "QRCodeCommand.h"

/*
@brief二维码模块控制类
*/
class CQRCodeHelper
{
public:
	CQRCodeHelper(void);
	~CQRCodeHelper(void);

	long QRCodeEncode(int nLevel, int nVersion, int nMaskingNo,int nMagnification, CString strSource,CString strFileName);
	void DeleteQRCode(CString strFileName);
};

