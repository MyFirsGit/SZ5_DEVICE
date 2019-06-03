#pragma once
#include "BOMForeService.h"
#include "WaitingOfficeDlg.h"
#include "ReceptionDlg.h"

static const TCHAR* const WM_TO_BASE_DLG            = _T("WM_TO_BASE_DLG");              // 基本画面
static const TCHAR* const WM_TO_REFRESH_NOT_DLG     = _T("WM_TO_REFRESH_NOT_DLG");       // 激活不可画面
static const TCHAR* const WM_TO_REFRESH_FINISH_DLG  = _T("WM_TO_REFRESH_FINISH_DLG");    // 激活结束画面
static const TCHAR* const WM_TO_REFRESH_FAILURE_DLG = _T("WM_TO_REFRESH_FAILURE_DLG");   // 激活失败画面

/**
@brief      激活业务Service
*/
class CRefreshSvc :	public CBOMForeService
{
    DECLARE_MESSAGE_MAP()

public:
    typedef enum _dialog_group {
        DIALOG_GROUP_WAIT = 1,
        DIALOG_GROUP_REFRESH_NOT,
        DIALOG_GROUP_BASE,
        DIALOG_GROUP_FINISH
    } DIALOG_GROUP;
	typedef struct _tagDataModel
	{
		RW_READ_CARD_RESPONSE readCardResponse;
		TicketType_t ticketType;
		CAccTicketParam::TICKET_TYPE ticketTypeDetail;
		ProductCategory_t cardCategory;
		int balance;
		_DATE_TIME orignalValidate;
		_DATE_TIME newValidate;
		_tagDataModel()
		{
			ticketType = 0;
		}
	} DataModel;

	CRefreshSvc();
	~CRefreshSvc();
	long DoRefresh();
	DataModel& GetDataModel();
protected:
	void OnReadCard(UINT,CRWReadCard*);
private:
	DataModel m_DataModel;
};
