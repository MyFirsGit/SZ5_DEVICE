#pragma once
#include "BRHExp.h"
#include "Control.h"

/**
 @brief		ֽ�ҽ���ģ��
*/
class BRH_API CBRHControl : public CControl
{
public:
	CBRHControl();
	virtual ~CBRHControl();
	long Initialize();
	long Shutdown();
};