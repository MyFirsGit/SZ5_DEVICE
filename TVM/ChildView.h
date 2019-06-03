#pragma once
#include "StatusDlg.h"
#include "GuideDlg.h"
#include "NavigationDlg.h"
#include "OperationDlg.h"

// ǰ������
class CFunctionDlg;

/**
@brief  CChildView ���沼����ͼ
*/
class CChildView : public CWnd
{
	DECLARE_MESSAGE_MAP()

public:
	CChildView();
	virtual ~CChildView();

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);						// ��� Owner Draw ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// ���洴��
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	LRESULT CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam);	// ������Ϣ����

public:
	CStatusDlg* m_pStatusDlg;		// ״̬��
	vector<CBaseDlg*> children;
};
