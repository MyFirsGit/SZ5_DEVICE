#pragma once
#include "OperationDlg.h"

/**
  @brief  TVM故障退款-基本画面
*/

static const char        EDIT_COUNT                 = 3;				// 编辑框的个数

class  CTVMFailureRefundBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CTVMFailureRefundBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
   CTVMFailureRefundBaseOfficeDlg(CService* pService);
   ~CTVMFailureRefundBaseOfficeDlg();

   bool CheckValid();													// 检查数据有效性

   enum { IDD = IDD_01601_TVMFAILURE_REFUND_BASE_DLG };

   CString m_strTransNum;												// 交易流水号 
   CString m_strDeviceCode;												// 设备编码

private:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 对话框初始化
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);
public:
	virtual BOOL OnInitDialog();
	

};