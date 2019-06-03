#pragma once

#include "ForeService.h"
#include "CSaleStaticBaseOfficeDlg.h"

class CSaleStaticSvc : public CBOMForeService
{
public:
	CSaleStaticSvc();
	~CSaleStaticSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_WAIT = 1,
		DIALOG_GROUP_BASE,
		DIALOG_GROUP_FINISH,
		DIALOG_GROUP_CHARGE_NOT
	} DIALOG_GROUP;
	
	PLABEL_GROUP_THREE GetAuditInfoGroup();
	UINT InitDetailInfo();
	UINT CSaleStaticSvc::GetPurcherDataSize();

private:

	UINT m_PurcherDataSize;
	LABEL_GROUP_THREE m_auditGUIInfo[LABEL_GROUP_EDIT_ROWS_8]; // 基本画面信息内容

};