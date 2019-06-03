#pragma once
#include "SCAPI.h"
#include "SCExp.h"
#include "Control.h"

/**
 @brief      ´òÓ¡»ú
*/
class SC_API  CSCControl : public CControl
{
public:
	CSCControl(void);
	virtual ~CSCControl(void);
	long Initialize();
	long Shutdown(void);
};