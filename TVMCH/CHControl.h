#pragma once
#include "CHExp.h"
#include "Control.h"

/**
 @brief		Ó²±Ò½ÓÊÕÄ£¿é
*/
class CH_API CCHControl : public CControl
{
public:
	CCHControl();
	virtual ~CCHControl();
	long Initialize();
	long Shutdown();
};