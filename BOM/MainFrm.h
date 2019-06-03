#pragma once
#include "ChildView.h"

/**
@brief	Õ¾Ô±Ö÷¿ò¼Ü´°¿Ú
*/
class CMainFrm : public CFrameWnd
{
    DECLARE_DYNAMIC(CMainFrm)
    DECLARE_MESSAGE_MAP()

public:

	CMainFrm();
	virtual ~CMainFrm();


public:  // control bar embedded members
	CChildView  m_wndView;
	

// Generated message map functions
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	LRESULT OnServiceMessage(WPARAM,LPARAM);
    LONG OnProgress(WPARAM wParam, LPARAM lParam);
    LONG OnStartService(WPARAM wParam, LPARAM lParam);
    LONG OnStopService(WPARAM wParam, LPARAM lParam);
    LONG OnException(WPARAM wParam, LPARAM lParam);
    LONG OnModeChange(WPARAM wParam, LPARAM lParam);

};

/////////////////////////////////////////////////////////////////////////////
