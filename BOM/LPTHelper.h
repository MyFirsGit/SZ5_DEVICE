#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "LPT.h"


/**
@brief  LPT控制类
*/

class CLPTHelper : public CServiceHelper
{
public:

	CLPTHelper(CService& service);                                                    // 构造函数
	~CLPTHelper();                                                                    // 析构函数
	
	// LPT共有接口 
	long  LPTOpenLPT();								                                   // 打开并口
	long  LPTReadLPT(BYTE&);											               // 读并口
	long  LPTCloseLPT();															   // 关闭并口
	DOORSTATUS GetTicketboxDoorStatus();
};
