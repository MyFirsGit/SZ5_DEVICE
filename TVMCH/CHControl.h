#pragma once
#include "CHExp.h"
#include "Control.h"

/**
 @brief		Ӳ�ҽ���ģ��
*/
class CH_API CCHControl : public CControl
{
public:
	CCHControl();
	virtual ~CCHControl();
	long Initialize();
	long Shutdown();
};