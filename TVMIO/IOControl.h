#pragma once
#include "IOExp.h"
#include "Control.h"

/**
 @brief     IO ͨ�ſ�����
*/
class IO_API  CIOControl : public CControl
{
public:
	CIOControl(void);
	virtual ~CIOControl(void);
	long Initialize();
	long Shutdown(void);
};