#pragma once
#include "operationdlg.h"
#include "VersionInfoSvc.h"
class CVersionInfoDlg :
	public COperationDlg
{
	DECLARE_DYNAMIC(CVersionInfoDlg)
	DECLARE_MESSAGE_MAP()
public:
	CVersionInfoDlg(CService* pService);
	~CVersionInfoDlg(void);
	enum { IDD = IDD_20121_DATAINPUT_SELECT_DLG };

public:
	void UpdateUI();

protected:
	void InitDataInputUI();
	void UpdateDataInputUI();
	void Clear();
private:
	LABEL_GROUP_THREE  m_LableThree[MAX_ROW];
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);                     //画面创建函数
	afx_msg void  OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);   //画面激活函数 
};


