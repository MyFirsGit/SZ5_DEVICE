#pragma once
#include "ReceptionDlg.h"
#include "AnalyzeSvc.h"
#include "resource.h"
#include "GridCtrl.h"
// CBaseM1InfoDlg dialog

class CBaseM1InfoDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CBaseM1InfoDlg)

public:
	CBaseM1InfoDlg(CService* pServic);   // standard constructor
	virtual ~CBaseM1InfoDlg();

// Dialog Data
	enum { IDD = IDD_10805_ANALYZE_BASEM1_DLG };
	enum { IDC_SELECTED_NO_GRID = 1000};
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void preShow();

	LRESULT OnServiceSelect(WPARAM wParam, LPARAM lParam); //Right1 button
	LRESULT OnProductSelect(WPARAM wParam, LPARAM lParam); //Riggt2 button
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateUI();
	void ShowBaseInfo(CardType_t cardType, CACCCard::ACCCARD_PRODUCT_INFO& productInfo); //基本信息界面
	void UpdatePrivateInfo();//个人信息界面
	void UpdateHistoryInfo();//历史记录界面

	DECLARE_MESSAGE_MAP()

private:
	CAnalyzeSvc* m_pSvc;
	LABEL_GROUP m_BaseInfoGroup[UD_MAX_WIDEROWS];
	LABEL_GROUP m_HistoryInfoGroup[UD_MAX_WIDEROWS];
	void ClearBaseInfo();
	bool isHistoryBtnClicked;
	
public:
	CGridCtrl *m_pTable;//Table
	CGroupBox* m_pGroup;//GroupBox	
	void InitButtonGroup();//初始化按钮信息
	CGStatic* m_historyInfoArea;
	PGSTATIC_INFO HistoryInfo; 

};
