#pragma once
#include "OperationDlg.h"


#define _MAX_ROW 15
#define _MAX_COL 4

class COneKeySettlementDlg : public COperationDlg
{
	DECLARE_DYNAMIC(COneKeySettlementDlg)
	DECLARE_MESSAGE_MAP()

public:
	COneKeySettlementDlg(CService* pService);				// 标准构造函数
	~COneKeySettlementDlg();

	// 对话框数据
	enum { IDD = IDD_20701_REMOVE_EXCEPTION_DLG };

public:
	void UpdateUI();                        //刷新界面（外部接口）
	void UpdateLabelData();					// 强制显示界面
protected:
	void InitDialogUI();                    //初始界面
private:
	LABEL_GROUP_FIVE  m_LableGroupFour[_MAX_ROW];

	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // 画面激活
};