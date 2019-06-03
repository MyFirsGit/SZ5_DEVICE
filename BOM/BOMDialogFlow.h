#pragma once

#include "DialogFlow.h"
#include "OperationDlg.h"

class CBOMDialogFlow:public CDialogFlow{
public:
	CBOMDialogFlow(CService& service);                                 // 构造函数
	~CBOMDialogFlow();                                                 // 析构函数

	void AddDialog(UINT dlgID, CBaseDlg* pDlg);                     // 添加Dialog
	CBaseDlg* GetDialog(UINT dlgID);
						
	virtual void ShowDialogGroup(UINT dlgGroupID);                          // 显示对话框组
	virtual void HideDialogGroup(UINT dlgGroupID);                          // 隐藏对话框组
	COperationDlg* GetCurOperationDlg();
	void NotifyCurrentDialogGroupUpdate();										    //通知当前画面组的窗体更新
	bool HasReceptionDlg(UINT dlgGroupID);
	bool HasWelcomeReceptionDlg(UINT dlgGroupID);

protected:
	virtual void ShowDialog(UINT dlgID);                                    // 显示Dialog
	virtual void HideDialog(UINT dlgID);                                    // 隐藏Dialog
private:
	void OnServiceStateChanged(SERVICE_STATE oldState,SERVICE_STATE newState);
};