#pragma once
#include "ReceptionDlg.h"
#include "AnalyzeSvc.h"
#include "resource.h"

// CBaseUserInfoDlg dialog

class CBaseUserInfoDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CBaseUserInfoDlg)

public:
	CBaseUserInfoDlg(CService* pServic);   // standard constructor
	virtual ~CBaseUserInfoDlg();

// Dialog Data
	enum { IDD = IDD_10807_ANALYZE_BASEUSER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateUI();

	DECLARE_MESSAGE_MAP()
private:
	CAnalyzeSvc* m_pSvc;
	LABEL_GROUP m_UserInfoLable[UD_MAX_ANAROWS];
public:
	void updataBaseUserInfo();
	void createFunBtns();
};
