#pragma once
#include "SysException.h"
#include "BaseDlg.h"
#include "util.h"

// CStatusDlg 对话框
class CStatusDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CStatusDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStatusDlg(CWnd* pParent = NULL);           // 构造函数
	~CStatusDlg();                              // 析构函数

	enum { IDD = IDD_STATUS_DLG };

	void SetUserID(CString userID);             // 设置管理员ID
	void SetNetWorkStatus(CString networkStatus);	//网络状态
	void SetPrinterStatus(CString printerStatus);	//设置打印机状态
	void SetOutRWStatus(CString outRWStatus);	//设置外部读写器状态
	void SetTWStatus(CString twStatus);					//设置内部读写器状态
	void SetTHStatus(CString thStatus);

private:
	const static UINT TIMER_INTERVAL = 1000;    // 时间刷新间隔(1 second)

	void OnDraw(CDC* pDC);                      // 画面绘制函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);    // 画面创建函数
	afx_msg void OnTimer(UINT nIDEvent);        // 时间控制
	afx_msg void OnDestroy();                   // 画面销毁函数
	void OnExceptionNotify(CSysException& ,bool);
	void OnTHModeChanged(int&);
	void RefreshHardwareStatus();
private:
	CString m_userID;                           // 操作员ID
	CString m_networkStatus;						//网络状态
	CString m_printerStatus;							//打印机状态
	CString m_outRWStatus;							//读写器状态
	CString m_TWStatus;								//内部读写器状态
	CString m_THStatus;								//出票机状态

	CRect m_rectUserID;
	CRect m_rectNetworkStatus;
	CRect m_rectPrinterStatus;
	CRect m_rectOutRWStatus;
	CRect m_rectTWStatus;
	CRect m_rectTHStatus;
	CRect m_rectTime;

	_DATE_TIME m_preTime;                       // 前次刷新时间
	_DATE    m_busiDay;                         // 前次运营日期
};
