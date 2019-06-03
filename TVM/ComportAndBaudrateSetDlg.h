#pragma once
#include "OperationDlg.h"

const int const MAX_LEN = 15;		// 行数最大值

/**
@brief      通信参数输设置画面
*/
class CComportAndBaudrateSetDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CComportAndBaudrateSetDlg)
	DECLARE_MESSAGE_MAP()

public:
	CComportAndBaudrateSetDlg(CService* pService);
	~CComportAndBaudrateSetDlg();

	enum { IDD = IDD_20601_OUT_DEVICE_MANEGE_DLG };

	
protected:
	virtual void UpdateUI();														// 更新显示完成回调函数
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);							// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// 画面激活

	LABEL_GROUP_THREE m_labelGroupThree[MAX_LEN];										// 画面显示的通信参数信息

	void ShowComportAndBaudRate();													// 显示具体内容

	void ClearLableData();															// 清理显示数据
};
