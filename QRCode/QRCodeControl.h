#pragma once
#include "QRCodeExp.h"
#include "Control.h"

/**
 @brief      ��ӡ��
*/
class QRCode_API  CQRCodeControl : public CControl
{
public:
	CQRCodeControl(void);
	virtual ~CQRCodeControl(void);
	long Initialize();
	long Shutdown(void);
};