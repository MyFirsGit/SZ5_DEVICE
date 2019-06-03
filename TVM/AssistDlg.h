#pragma once
#include "resource.h"
#include "AssistSvc.h"
#include "receptiondlg.h"

class CAssistDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CAssistDlg)
	DECLARE_MESSAGE_MAP()
public:
	CAssistDlg(CService* pServic);               //Constructor with SVC
	~CAssistDlg(void);

	enum { IDD = IDD_10805_ANALYZE_BASEM1_DLG };

	CRect REC_ASSIST_TIP;                        //The Rect of Tip-Dlg
	CXButton* m_btnCancel;              // È¡Ïû°´Å¥

	void InitUI();
private:
	void UpdateUI();
	VOID DrawRoundRect();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint(); 
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);

};

