#pragma once
#include "ReceptionDlg.h"
#include "ProductIssueSvc.h"

/**
@brief      乘客充值业务画面
*/
class CProductIssueReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CProductIssueReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:

	typedef enum _tagIssueComponents
	{
		PRODUCTTYPE = 0x0001,
		PAYTYPE = 0x0002,
		SUBPRODUCTTYPE = 0x0004,
		BEGINLOCATION = 0x0008,
		ENDLOCATION = 0x0010,
	};

	CProductIssueReceptionDlg(CService* pService);
	virtual ~CProductIssueReceptionDlg();

	enum { IDD = IDD_00301_CHARGE_RECEPT_DLG };
	enum { MAX_ISSUE_PRODUCT_INFO_ROWS = 13 };

protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LRESULT On1GroupBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT On2GroupBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT On3GroupBtnClick(WPARAM wParam, LPARAM lParam);

	LRESULT OnProductTypeBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnProductSubTypeBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnProductBeginLocationBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnProductEndLocationBtnClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnPayTypeClick(WPARAM wParam,LPARAM lParam);

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnPrint(WPARAM wParam,LPARAM lParam);
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);

private:

	CProductIssueSvc* pSvc;
	LANGUAGETYPE_T language_mode;

	void InitializeBaseInfo();
	void InitializeFunctionBtns(long lComponents);
	void InitializeGuideArea();											// 初始化基本信息区域
	void UpdateUI();

	LABEL_GROUP m_ProductIssueInfoGroup[MAX_ISSUE_PRODUCT_INFO_ROWS];	// 车票信息区域数据
	void showPaymethodGuide();
	CString BanknoteShowword();
	CString CoinShowword();
	UINT m_CurDlg;

	virtual void preShow();
	virtual void showComplete();
};
