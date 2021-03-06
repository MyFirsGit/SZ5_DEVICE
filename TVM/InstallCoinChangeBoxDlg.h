#pragma once
#include "OperationDlg.h"

class CInstallCoinChangeBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CInstallCoinChangeBoxDlg) 
	DECLARE_MESSAGE_MAP()

public:
	CInstallCoinChangeBoxDlg(CService* pService);
	~CInstallCoinChangeBoxDlg(void);
	
	enum { IDD = IDD_20115_COMMUNICATION_SET_END_DLG };

	void InitAttachCoinCollectionBoxInfo();
	void ShowAttachCoinCollectionBoxInfo();					// 显示硬币回收箱状态
	void ShowDoAttachCoinCollectionBoxInfo();				// 显示硬币回收箱安装状态
	void SetColor(int row,CString status);					// 设置状态显示的颜色
protected:
	virtual void UpdateUI();								// 更新显示完成回调函数

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_FOUR m_lableGroup[15];
};

