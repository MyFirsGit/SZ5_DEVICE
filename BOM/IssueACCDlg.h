#pragma once
#include "OperationDlg.h"
#include "IssueSvc.h"
#include "ButtonGroup.h"


/**
@brief      售票初期画面
*/

class CIssueSvc;

class CIssueACCDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CIssueACCDlg)
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _tagIssueComponents
	{
		LINE_BUTTON = 0x0001,
		STATION_BUTTON = 0x0002,
		PRICE_BUTTON = 0x0004,
		PIECE_BUTTON = 0x0008,
		PAYTYPE_BUTTON = 0x0010,
		CHARGEVALUE_BUTTON = 0x0020,
	};
	CIssueACCDlg(CService*);
	~CIssueACCDlg();

	// 对话框数据
	enum { IDD = IDD_00101_ISSUE_INIT_DLG };
	enum {MAX_TICKET_INFO_ROWS = 10};
private:
	CIssueSvc* pSvc;
	void InitializeComponents(long lComponents);
	void UpdateUI();
	bool ValidateInput();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnLineClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnStationClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPriceClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPieceClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnEditChange(WPARAM wParam,LPARAM lParam);
	LRESULT OnOK(WPARAM wParam,LPARAM lParam);

	LABEL_GROUP m_TicketInfoGroup[MAX_TICKET_INFO_ROWS];                          // 车票信息区域数据
	long m_lComponents;
};