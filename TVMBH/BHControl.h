#pragma once
#include "BHExp.h"
#include "Control.h"

/**
 @brief		ֽ�ҽ���ģ��
*/
class BH_API CBHControl : public CControl
{
public:
	CBHControl();
	virtual ~CBHControl();
	long Initialize();
	long Shutdown();
};