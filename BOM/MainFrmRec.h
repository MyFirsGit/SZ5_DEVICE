#pragma once

#include "ChildViewRec.h"

/**
@brief	乘客主框架窗口
*/

class CMainFrmRec : public CFrameWnd
{
    DECLARE_DYNAMIC(CMainFrmRec)
    DECLARE_MESSAGE_MAP()

public:
	CMainFrmRec();
    ~CMainFrmRec();

// 属性
public:
    CChildViewRec    m_wndView;

// 操作
public:

// 重写
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


