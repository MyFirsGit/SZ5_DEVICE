#pragma once
#include "BOMForeService.h"

/**
@brief    ʱ���趨
*/
class CTimeSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTimeSetSvc();
	~CTimeSetSvc();
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_TIMESET = 1,                       
	} DIALOG_GROUP;
};