#pragma once
#include "bomforeservice.h"
#include "UIDefinition.h"

#include "SvcDef.h"		//rhd
//#include "CPSCOMLIBAPI.h"

/**
@brief    保守业务 - 硬件自检业务
*/
class CSystemSetSvc :
	public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CSystemSetSvc();
	~CSystemSetSvc();

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_SYSTEM_SET                     =  1,                          // 设备自检选择
	} DIALOG_GROUP;



};