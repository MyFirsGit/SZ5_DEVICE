#pragma once
#include "PrinterExp.h"
#include "Control.h"

/**
 @brief      ´òÓ¡»ú
*/
class PRINTER_API  CPrinterControl : public CControl
{
public:
	CPrinterControl(void);
	virtual ~CPrinterControl(void);
	long Initialize();
	long Shutdown(void);
};