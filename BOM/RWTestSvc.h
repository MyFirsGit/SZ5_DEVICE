#pragma once
#include "BOMForeService.h"

/**
@brief    ��д������
*/
class CRWTestSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRWTestSvc();
	~CRWTestSvc();

private:
	typedef enum _dialog_group {
		DIALOG_GROUP_CARD_READWRITE_MACHINE = 1,
	} DIALOG_GROUP;

	void OnReadCard(UINT,CRWReadCard*);
};