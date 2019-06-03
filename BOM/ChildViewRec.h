#pragma once
// ChildViewRec.h : CChildViewRec ��Ľӿ�
//
#include "OutOfReceptionDlg.h"
#include "GuideReceptionDlg.h"
#include "ReceptionHeadDLG.h"

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
    CGuideReceptionDlg* m_pReceptionGuideDlg;       // �˿���Dialog

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL PreCreateWindow(CREATESTRUCT& cs);
};

