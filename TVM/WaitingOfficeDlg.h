#pragma once
#include "ReceptionDlg.h"
#include "PassengerForeSvc.h"
// CWaitingOfficeDlg �Ի���

/**
@brief      �ȴ�����
*/
class CWaitingOfficeDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CWaitingOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CWaitingOfficeDlg(CService* pService,CString strServiceName = _T(""));
	virtual ~CWaitingOfficeDlg();

    // �Ի�������
	enum { IDD = IDD_WAITING_DLG };
  
private:
	void OnDraw(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// ���ȡ����ť
	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();
private:
	CTVMForeService* pSvc;
    CString m_strServiceName;  // ���浱ǰҵ����������ʾ����
	CImage m_bkgImage;
};
