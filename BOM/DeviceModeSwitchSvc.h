#pragma once
#include "BOMForeService.h"

/**
@brief    设备模式切换
*/
class CDeviceModeSwitchSvc : public CBOMForeService
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