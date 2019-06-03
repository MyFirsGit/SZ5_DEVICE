#pragma once
#include "TVMForeService.h"

/**
@brief    设备模式切换
*/
class CDeviceModeSwitchSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CDeviceModeSwitchSvc();
	~CDeviceModeSwitchSvc();
private:
	typedef enum _dialog_group{
		DIALOG_GROUP_MODE_SWITCH = 1,                       
	} DIALOG_GROUP;
};