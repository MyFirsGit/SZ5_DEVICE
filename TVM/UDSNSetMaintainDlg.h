#pragma once
#include "OperationDlg.h"
#include "GStatic.h"

/**
@brief      UDSN信息输入画面
*/
class CUDSNSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CUDSNSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CUDSNSetMaintainDlg(CService* pService);
	~CUDSNSetMaintainDlg();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// 处理确认按钮消息
	afx_msg LRESULT OnReset(WPARAM,LPARAM);								// 处理重置按钮消息
	enum { IDD = IDD_20119_UDSN_SET_DLG };								// 对话框数据

private:
	typedef enum _udsninfo_edit_group {									 // 流水号对应的EDITGROUP
		UDSNINFO_EDIT_GROUP_ACCUDSN = 0,
		UDSNINFO_EDIT_GROUP_AFCUDSN = 1,
	} UDSNINFO_EDIT_GROUP;

	LABEL_GROUP m_labelUDSNinfo[LABEL_GROUP_EDIT_ROWS_3];				// 画面显示的流水号信息

	void InitLabelUDSNInfo();											// 初始化BASE区域LABEL里面的数据
	void InitEditTextUDSNInfo();										// 初始化BASE区域EDIT框里面的数据
	void SetUDSNInfoToSvc();											// 把EDIT框里面数据的保存到SVC里面
	bool CheckInputText();												// 检查EDIT框里的数据是否合法

};
