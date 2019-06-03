#pragma once
// ChildViewRec.h : CChildViewRec ��Ľӿ�
//
#include "OutOfReceptionDlg.h"
#include "GuideReceptionDlg.h"
#include "ReceptionHeadDLG.h"
#include "MenuDlg.h"

// CChildViewRec ����

class CChildViewRec : public CWnd
{
    DECLARE_MESSAGE_MAP()

// ����
public:
	CChildViewRec();
    ~CChildViewRec();

// ʵ��
public:
    COutOfReceptionDlg* m_pOutOfReceptionDlg;       // ��ͣ����Ի���
    CReceptionHeadDlg* m_pReceptionHeadDlg;         // �ӿͻ������Ի���
	CMenuDlg* m_pMenuDlg;
	LRESULT CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

