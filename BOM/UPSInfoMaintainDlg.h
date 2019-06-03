#pragma once
#include "OperationDlg.h"

/**
@brief      UPS信息显示画面
*/
class CUPSInfoMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CUPSInfoMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CUPSInfoMaintainDlg(CService* pService);
	~CUPSInfoMaintainDlg();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 画面创建函数

	enum { IDD = IDD_UPS_INFO_DLG };								// 对话框数据

private:
	LABEL_GROUP m_labelUPSInfo[LABEL_GROUP_EDIT_ROWS_2];				// 画面显示的UPS信息

	void InitLabelUPSInfo();											// 初始化BASE区域LABEL里面的数据
};
