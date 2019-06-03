#pragma once
#include "BHExp.h"
#include "Control.h"

/**
 @brief		Ö½±Ò½ÓÊÕÄ£¿é
*/
class BH_API CBHControl : public CControl
{
public:
	CBHControl();
	virtual ~CBHControl();
	long Initialize();
	long Shutdown();
};