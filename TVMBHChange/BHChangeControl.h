#pragma once
#include "BHExp.h"
#include "Control.h"

/**
 @brief		ֽ�ҽ���ģ��
*/
class BH_API CBHChangeControl : public CControl
{
public:
	CBHChangeControl();
	virtual ~CBHChangeControl();
	long Initialize();
	long Shutdown();
};