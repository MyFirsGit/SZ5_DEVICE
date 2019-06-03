#pragma once
#include "OperationDlg.h"

/**
@brief  时间设定
*/
class  CTimeSetBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CTimeSetBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTimeSetBaseOfficeDlg(CService*pService) ;
	~CTimeSetBaseOfficeDlg() ;

	enum { IDD = IDD_01801_TIMESET_BASE_DLG };

private:
	LABEL_GROUP timeSetInfo[LABEL_GROUP_EDIT_ROWS_5];		// 事件信息框组
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);					// 处理确认按钮消息
	afx_msg LRESULT OnReset(WPARAM,LPARAM);					// 处理重置按钮消息
	void InitlabelTimeInfo();								// 初始化BASE区域LABEL里面的数据
	bool CheckInput();										// 检查输入信息是否合法
	void HideGUIModule(ULONG ulCompomentFlag);
};