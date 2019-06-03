#pragma once
#include "BOMForeService.h"

/**
@brief    通信参数设定
*/
class CCommunicationSetSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationSetSvc();
	~CCommunicationSetSvc();
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_COMMUNICATION_SET = 1,                       
	} DIALOG_GROUP;
};