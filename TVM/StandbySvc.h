#pragma once
#include "TVMForeService.h"
#include "ParseXML.h"

/**
@brief ´ý»úÒµÎñService
*/
class CStandbySvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CStandbySvc();
	virtual ~CStandbySvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_STANDBY = 1,
	} DIALOG_GROUP;

	void NotifyDataModelChanged();	
private:
	void OnStart();
};