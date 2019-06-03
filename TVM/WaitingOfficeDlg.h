#pragma once
#include "ReceptionDlg.h"
#include "PassengerForeSvc.h"
// CWaitingOfficeDlg 对话框

/**
@brief      等待画面
*/
class CWaitingOfficeDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CWaitingOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CWaitingOfficeDlg(CService* pService,CString strServiceName = _T(""));
	virtual ~CWaitingOfficeDlg();

    // 对话框数据
	enum { IDD = IDD_WAITING_DLG };
  
private:
	void OnDraw(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// 点击取消按钮
	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();
private:
	CTVMForeService* pSvc;
    CString m_strServiceName;  // 下面当前业务名区域显示内容
	CImage m_bkgImage;
};
