#pragma once
#include "StatusDisplayExp.h"
#include "Control.h"

/**
 @brief      ´òÓ¡»ú
*/
class STATUS_DISPLAY_API  CStatusDisplayControl : public CControl
{
public:
	CStatusDisplayControl(void);
	virtual ~CStatusDisplayControl(void);
	long Initialize();
	long Shutdown(void);
};