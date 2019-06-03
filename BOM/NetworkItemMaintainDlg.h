#pragma once
#include "OperationDlg.h"
#include "GStatic.h"
#include "NetworkTestSvc.h"
#include "FTPCommand.h"

/**
@brief      网络测试画面
*/
class CNetworkItemMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CNetworkItemMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CNetworkItemMaintainDlg(CService* pService);				// 标准构造函数
	~CNetworkItemMaintainDlg();

	// 对话框数据
	enum { IDD = IDD_20551_NETWORK_ITEM_DLG };

private:
	CString	m_sNetworkItem;										// 具体选中的网络测试项目对应的MSG
	bool    m_bIsShowPingResult;								// 是否显示Ping结果区域
	CString m_sPingResult;										// Ping结果

protected:
	void OnDraw(CDC* pDC);										// 画面绘制函数

public:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);						// 处理确认按钮消息
	afx_msg LRESULT OnNetworkItemClick(WPARAM,LPARAM);			// 按钮按下后的处理
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// 画面创建函数

};