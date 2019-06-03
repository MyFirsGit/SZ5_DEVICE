#pragma once
#include "QRCodeExp.h"
#include "Control.h"

/**
 @brief      ´òÓ¡»ú
*/
class QRCode_API  CQRCodeControl : public CControl
{
public:
	CQRCodeControl(void);
	virtual ~CQRCodeControl(void);
	long Initialize();
	long Shutdown(void);
};