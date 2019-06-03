#pragma once
#include "BHExp.h"
#include "Control.h"

/**
 @brief		Ö½±Ò½ÓÊÕÄ£¿é
*/
class BH_API CBHChangeControl : public CControl
{
public:
	CBHChangeControl();
	virtual ~CBHChangeControl();
	long Initialize();
	long Shutdown();
};