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
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	LRESULT OnServiceMessage(WPARAM,LPARAM);
    LONG OnProgress(WPARAM wParam, LPARAM lParam);
    LONG OnStartService(WPARAM wParam, LPARAM lParam);
    LONG OnStopService(WPARAM wParam, LPARAM lParam);
    LONG OnException(WPARAM wParam, LPARAM lParam);
};


