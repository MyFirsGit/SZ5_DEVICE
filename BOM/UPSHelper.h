#pragma once

#include "ServiceHelper.h"
#include "UPSCommand.h"

/**
@brief  UPS控制类
*/

class CUPSHelper :
	public CServiceHelper
{
public:

	CUPSHelper(CService& service);
	~CUPSHelper();

	// 打印机相关 
	long UPSOpen();															// 连接UPS
	long UPSRead(_tagUPSBatteryInfo &info);									// 读取UPS
	long UPSClose();                                                        // 关闭UPS
	DWORD GetUPSBatteryCapacity();
	DWORD GetUPSLeftStandbyTime();
};