#pragma once
#include "BRHExp.h"
#include "Control.h"

/**
 @brief		Ö½±Ò½ÓÊÕÄ£¿é
*/
class BRH_API CBRHControl : public CControl
{
public:
	CBRHControl();
	virtual ~CBRHControl();
	long Initialize();
	long Shutdown();
};