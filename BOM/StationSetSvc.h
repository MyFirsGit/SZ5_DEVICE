#pragma once
#include "BOMForeService.h"

/**
@brief    保守业务 - 车站设置业务
*/
class CStationSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CStationSetSvc();
	~CStationSetSvc();
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_STATION_SET	= 1,                       
	} DIALOG_GROUP;
};