#pragma once
#include "BOMForeService.h"

/**
@brief    ����ҵ�� - ��վ����ҵ��
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