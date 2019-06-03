#pragma once
#include "ReceptionDlg.h"
#include "PassengerForeSvc.h"

/**
@brief      乘客充值结束错误画面
*/
class CPassengerResultDlg:public CReceptionDlg
{
	DECLARE_DYNAMIC(CPassengerResultDlg)
	DECLARE_MESSAGE_MAP()

public:

	CPassengerResultDlg(CService*);
	virtual ~CPassengerResultDlg();

	// 对话框数据
	enum { IDD = IDD_00103_ISSUE_RESULT_DLG };

private:

	CPassengerForeSvc* pSvc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void OnDraw(CDC* pDC);
	void UpdateUI();

	void InitializeGuideArea();
	void ShowGuideArea();

	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);
};