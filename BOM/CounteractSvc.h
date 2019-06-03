#pragma once

#include "BOMForeService.h"

static const TCHAR* const WM_COUNTERACT_WAIT_TO_CONFIRM = _T("WM_COUNTERACT_WAIT_TO_CONFIRM");
static const TCHAR* const WM_COUNTERACT_WAIT_TO_ERROR = _T("WM_COUNTERACT_WAIT_TO_ERROR");
static const TCHAR* const WM_COUNTERACT_CONFIRM_TO_ERROR = _T("WM_COUNTERACT_CONFIRM_TO_ERROR");

class CCounteractSvc : public CBOMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _dialog_group{
		DIALOG_GROUP_WAIT	= 1,
		DIALOG_GROUP_CONFIRM=2,
		DIALOG_GROUP_ERROR = 3,
	} DIALOG_GROUP;

	CCounteractSvc();
	~CCounteractSvc();

	PLABEL_GROUP GetCounteractConfirmInfo();                            // 取得确认画面车票信息
	UINT GetCounteractConfirmInfoSize();
	void OnOK();
protected:
	void OnReadCard(UINT,CRWReadCard*);
	void OnStart();
private:
	void Counteract();
	void OnCardError(CString);
	void StartReadCard();
	void SaveBusinessData(LPRW_BUSSINESS_RESPONSE);
	UINT InitCounteractConfirmInfo();
	void UpdateCounteractConfirmInfo(CULCard&);
	UINT m_counteractConfirmInfoSize;
	LABEL_GROUP m_counteractConfirmInfo[LABEL_GROUP_EDIT_ROWS_4];		// 确认画面显示的车票信息
	CULCard m_ULCard;
	RW_READ_CARD_RESPONSE m_RWReadCardResponse;
};
