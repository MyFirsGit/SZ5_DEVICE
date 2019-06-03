#pragma once
#include "OperationDlg.h"
#include "MessageBox.h"

// CWaitingOfficeDlg �Ի���

/**
@brief      �ȴ�����
*/
class CWaitingOfficeDlg : public COperationDlg
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

private:
    CString m_strServiceName;  // ���浱ǰҵ����������ʾ����
};
