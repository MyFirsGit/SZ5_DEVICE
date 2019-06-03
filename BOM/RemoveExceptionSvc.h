#pragma once
#include "BOMForeService.h"

/**
@brief      异常解除业务类
*/
class CRemoveExceptionSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionSvc();
	~CRemoveExceptionSvc();
private:
	typedef enum _dialog_group {
		DIALOG_GROUP_REMOVE_EXCEPTION = 1
	} DIALOG_GROUP;
};
