#pragma once
#include "ReceptionDlg.h"
#include "AnalyzeSvc.h"
#include "resource.h"

// CBaseUlInfoDlg dialog

class CBaseUlInfoDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CBaseUlInfoDlg)

public:
	CBaseUlInfoDlg(CService* pServic);   // standard constructor
	virtual ~CBaseUlInfoDlg();

// Dialog Data
	enum { IDD = IDD_10804_ANALYZE_BASEUL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateUI();

	DECLARE_MESSAGE_MAP()

private:
	CAnalyzeSvc* m_pSvc;
	LABEL_GROUP m_UlInfoLable[UD_MAX_ANAROWS];
public:
	void updataBaseUlInfo();
	void createFunBtns();
};
