#pragma once

#include "ReceptionDlg.h"

// CRefreshReceptionDlg 对话框

/**
@brief      乘客激活业务画面
*/
class CDeferReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CDeferReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDeferReceptionDlg(CService* pService);           // 标准构造函数
	virtual ~CDeferReceptionDlg();
	// 对话框数据
	enum { IDD = IDD_11701_DEFER_DLG };     
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	void UpdateUI();
	void OnDraw(CDC* pDC);
};
