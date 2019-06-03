#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"
#include "Sync.h"

class CIssueSvc;

/**
@brief 售票基本信息画面
*/
class CIssueACCIniDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CIssueACCIniDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueACCIniDlg(CService*);
	~CIssueACCIniDlg();

	enum { IDD = IDD_10101_ISSUE_DLG };

	// 界面更新区域
	enum {
		BASE_NONE	   = 0,
		BASE_INFO_ARAE = 1,
		BASE_CASH_ARAE = 2,
	};
	typedef enum _tagIssueComponents
	{
		PASSAGETYPE = 0x0001,
		PAYTYPE = 0x0002,
		PIECE_BUTTON = 0x0004,
	};

	enum {MAX_TICKET_INFO_ROWS = 23};
public:
	void ClearLabelData();
	void ShowCoinMaxCount();	 // 显示硬币数量达到最大数量
	void ShowBanknoteMaxCount(); // 显示纸币数量达到最大数量
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);
	LRESULT OnPassagerTypeClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPieceClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPayTypeClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnPrint(WPARAM wParam,LPARAM lParam);

	void InitializeBaseInfo();
	void InitializeFunctionBtns(long lComponents);
	void InitializeGuideArea();
	bool ValidateInput();
	void ShowGuideMsgOfPayInfo();					// 显示支付信息  
	bool m_bIsShowCoinMaxCount;		// 是否显示硬币最大数量
	bool m_bIsShowBanknoteMaxCount;// 是否显示纸币最大数量
	bool m_bIsShowFinshedInsertMoney;// 是否完成投钱

	bool m_bShowOkButtonAndPayMented;// 是否显示确认按钮及支付方式？
private:
	CIssueSvc* pSvc;
	LABEL_GROUP m_TicketInfoGroup[MAX_TICKET_INFO_ROWS];
	bool m_bConfirmPayment;
};