#pragma once
#include "BOMForeService.h"

/**
@brief    保守业务 - TH模式设定业务
*/
class CTHModeSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTHModeSetSvc();
	~CTHModeSetSvc();	
	int GetCurrentTHMode();			//获取当前TH模式
	void SetTHMode(int);			//设定TH模式

private:
	typedef enum _dialog_group
	{
		DIALOG_GROUP_TH_MODE_SET = 1,                       
	} DIALOG_GROUP;
};