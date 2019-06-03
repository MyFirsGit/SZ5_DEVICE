#pragma once 
#include "OperationDlg.h"

/**
@brief  数据输出-输出内容选择
*/
class  CDataOutputSelectMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CDataOutputSelectMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDataOutputSelectMaintainDlg(CService* pService) ;
	~CDataOutputSelectMaintainDlg() ;
	enum { IDD = IDD_20131_DATAOUTPUT_SELECT_DLG };

private:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);						// 处理确认按钮消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// 窗体创建函数
};