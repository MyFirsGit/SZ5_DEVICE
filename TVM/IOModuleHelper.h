#pragma once
#include "stdafx.h"
#include "ServiceHelper.h"
#include "DIOMessages.h"
#include "IOdef.h"

/**
@brief  IO控制类
*/

class CIOModuleHelper
{
public:

	CIOModuleHelper();                              // 构造函数
	~CIOModuleHelper();                                              // 析构函数

	void OpenIO();                                                   // 打开IO通信
	void CloseIO();                                                  // 关闭IO通信
	void InitializeIO();											// 初始化IO
	void GetIOStatus(IO_RSP_STATUS_INFO&);	                      	// 获取IO状态
	void OpenCHIndicator();											// 点亮投币口指示灯
	void CloseCHIndicator();										// 关闭投币口指示灯
	void OpenBHIndicator();											// 点亮纸币投币口指示灯
	void CloseBHIndicator();										// 关闭纸币投币口指示灯

	void OpenReturnCupIndicator();						            // 点亮取票找零指示灯
	void CloseReturnCupIndicator();			             			// 熄灭取票找零指示灯
	void OpenReturnLight();									        // 点亮取票找零照明灯
	void CloseReturnLight();									    // 熄灭取票找零照明灯
	void OpenAlarm();												// 打开蜂鸣报警
	void CloseAlarm();												// 关闭蜂鸣报警
	void OpenBKCardIndicator();										// 点亮储值卡指示灯
	void CloseBKCardIndicator();									// 关闭储值卡指示灯
	void OpenOperatorLight();									    // 点亮操作照明灯
	void CloseOperatorLight();									    // 熄灭操作照明灯

	void ReturnRechargeCard();										// 充值卡弹卡
	// 组合命令
	void Initialize();												// 打开并初始化IO
	void RepareIO();												// 修复IO异常
	void OpenAll();
	void CloseAll();


	//不用的接口
	void OpenCoinShutter();											// 打开投币口
	void CloseCoinShutter();										// 关闭投币口

	void OpenHelpButtonLight();										// 打开招援按钮灯
	void CloseHelpButtonLight();									// 关闭招援按钮等
	void OpenReturnCupIndicatorFlash();								// 点亮取票找零指示灯已闪烁方式

	void AbortCheckCardStandby(){}
	void CheckCardStandby(CService*,int){};
	void CloseChargeIndicator(){};
	void OpenChargeIndicator(){}
};
