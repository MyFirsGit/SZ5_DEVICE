#pragma once
#include "resource.h"
#include "UIDefinition.h"

typedef struct _msg_show_up{
	CString strCN;
	CString strEN;
	CString strPN;
	_msg_show_up(){
		strCN = _T("系统正在处理中，请稍候...");
		strEN = _T("Processing,Please waiting...");
		strPN = _T("Processing,por favor,esperando...");
	}
}MSG_SHOW_UP; 

class CProcessDlg : public CDialog
{
	// Construction
public:
	CProcessDlg(CWnd* pParent = NULL);	// standard constructor
	enum { IDD = IDD_DIALOG1 };

	
protected:
	
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();            // 画面绘制函数
//	LRESULT CloseWindow(WPARAM wParam,LPARAM lParam);
	MSG_SHOW_UP m_show_msg;
};

class CProcessDlgHelper{

private:
	static CProcessDlg dlg;
	static int refernceCount;
	

public: 
	CProcessDlgHelper();
	~CProcessDlgHelper();

	static void HideProcessDlg(bool isDestroyNow = false);
	static void WaitForProcessWindowClose();
};

#ifndef _DEBUG
#define NOTIFY_PROCESSING_WINDOW() CProcessDlgHelper cProcessDlgHelper
#define NOTIFY_PROCESSING_WINDOW_CLOSE() CProcessDlgHelper::HideProcessDlg(true)
#define WAIT_PROCESSING_WINDOW_CLOSE() CProcessDlgHelper::WaitForProcessWindowClose()
#else
#define NOTIFY_PROCESSING_WINDOW() CProcessDlgHelper cProcessDlgHelper
#define NOTIFY_PROCESSING_WINDOW_CLOSE() CProcessDlgHelper::HideProcessDlg(true)
#define WAIT_PROCESSING_WINDOW_CLOSE() CProcessDlgHelper::WaitForProcessWindowClose()

#endif

