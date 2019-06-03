#pragma once
#include "BOMForeService.h"

class CSignCardReportLossSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	typedef enum _dialog_group{
		DIALOG_GROUP_BASE	= 1,
		DIALOG_GROUP_OVER
	} DIALOG_GROUP;

	CSignCardReportLossSvc();
	~CSignCardReportLossSvc();
};