#pragma once
#include "OperationDlg.h"
#include "RemoveExceptionSvc.h"

/**
@brief     异常解除画面
*/
class CRemoveExceptionDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRemoveExceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionDlg(CService* pService);				// 标准构造函数
	~CRemoveExceptionDlg();

	// 对话框数据
	enum { IDD = IDD_20701_REMOVE_EXCEPTION_DLG };

public:
	void UpdateUI();                        //刷新界面（外部接口）
protected:
	void InitDialogUI();                    //初始界面

	void UpdateDialogData();                //更新界面数据
	void ClearLabelData();                  //清空数据
private:
	LABEL_GROUP_FOUR  m_LableGroupFour[_MAX_ROW];

	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // 画面激活
};
