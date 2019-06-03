#pragma once
#include "IOExp.h"
#include "Control.h"

/**
 @brief     IO 通信控制类
*/
class IO_API  CIOControl : public CControl
{
public:
	CIOControl(void);
	virtual ~CIOControl(void);
	long Initialize();
	long Shutdown(void);
};