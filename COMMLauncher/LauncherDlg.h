#pragma once
#define WHITE  RGB(255,255,255)

// 启动界面
class CCTLauncherDlg : public CDialog
{

public:
	CCTLauncherDlg(CWnd* pParent = NULL);	

	void DrawText(CDC* pDC, CRect& rect, CString sText, CString sFont,int nFontSize, int nFontWeight = FW_BOLD, COLORREF crColor = WHITE, UINT nFormat = DT_CENTER);           // 在指定位置显示文字

	enum { IDD = IDD_CTLAUNCHER_DIALOG };

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
};
