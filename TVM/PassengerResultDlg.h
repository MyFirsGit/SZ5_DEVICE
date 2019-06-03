#pragma once
#include "ReceptionDlg.h"
#include "PassengerForeSvc.h"

/**
@brief      �˿ͳ�ֵ����������
*/
class CPassengerResultDlg:public CReceptionDlg
{
	DECLARE_DYNAMIC(CPassengerResultDlg)
	DECLARE_MESSAGE_MAP()

public:

	CPassengerResultDlg(CService*);
	virtual ~CPassengerResultDlg();

	// �Ի�������
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