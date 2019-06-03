#pragma once
#include "BaseDlg.h"
#include "util.h"

/**
@brief 操作员状态栏对话框
*/
class CStatusDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CStatusDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStatusDlg(CWnd* pParent = NULL);
	~CStatusDlg();

	enum { IDD = IDD_STATUS_DLG };

	void SetOperatorID(CString id);							// 设置操作员ID
	void SetNetWorkStatus(LANGUAGE_MSG networkStatus);		// 设置网络状态
	void SetOperatingMode(CString systemMode);				// 设置运行模式
	void ShowStatusMsg(LANGUAGE_MSG strMsg,bool bIsShowup = true);				// 显示状态信息
	
	void SetUserID(CString userID);// TODO:等待最后调试通过删除
	void SetVMIssueAmount(DWORD money);// TODO:等待最后调试通过删除
	void ReFlashStatusDlg();			// 刷新状态显示
private:
	void OnDraw(CDC* pDC);									// 画面绘制
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 画面创建
	afx_msg void OnDestroy();								// 画面销毁
	afx_msg void OnTimer(UINT nIDEvent);					// 系统时钟刷新控制

private:
	CTime m_system_clock_time;					// 系统时钟
	CString m_operator_id;						// 操作员ID
	LANGUAGE_MSG m_network_status;				// 网络状态
	CString m_operating_mode;					// 运行模式
	LANGUAGE_MSG m_status_msg;					// 状态消息

	CRect m_system_clock_rc;					// 系统时钟显示区域
	CRect m_operator_id_rc;						// 操作员ID显示区域
	CRect m_network_status_rc;					// 网络状态显示区域
	CRect m_operating_mode_rc;					// 运行模式显示区域

	COLORREF m_network_status_color;			// 网络状态颜色
	_DATE_TIME m_old_time;						// 上次刷新时间
	const static UINT TIMER_INTERVAL = 1000;	// 系统时钟刷新间隔（1秒）
	bool m_bIsShowMsg;
};
