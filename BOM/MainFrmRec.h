#pragma once

#include "ChildViewRec.h"

/**
@brief	�˿�����ܴ���
*/

class CMainFrmRec : public CFrameWnd
{
    DECLARE_DYNAMIC(CMainFrmRec)
    DECLARE_MESSAGE_MAP()

public:
	CMainFrmRec();
    ~CMainFrmRec();

// ����
public:
    CChildViewRec    m_wndView;

// ����
public:

// ��д
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


