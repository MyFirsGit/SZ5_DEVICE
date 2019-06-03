//#include "stdafx.h"
//#include "CTokenManageSvc.h"
//#include "OperationDlg.h"
///*
//*@brief TOKEN箱管理界面
//**/
//
//#define MAX_ROW		15					// 定义最大行数
//#define MAX_COL		 5					// 定义最大列数
//
//class CTokenBoxInfoDlg : public COperationDlg
//{
//	DECLARE_DYNAMIC(CTokenBoxInfoDlg)
//	DECLARE_MESSAGE_MAP()
//
//public:
//	CTokenBoxInfoDlg(CService* pService);
//	~CTokenBoxInfoDlg();
//
//	enum{	IDD = IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG};
//
//	virtual void UpdateUI();													// 界面更新
//protected:
//	// 初始化界面
//	void InitMainDlg();															// 初始化Main界面
//	void InitInstallDlg();														// 初始化安装界面
//	void InitInstalledDlg();													// 初始化安装完成界面
//	void InitUninstallDlg();													// 初始化卸载界面
//	void InitUninstalledDlg();													// 初始化卸载完成界面
//	void InitRecycleDlg();														// 初始化回收界面
//	void InitRecycledDlg();														// 初始化回收完成界面
//	// 显示界面
//	void ShowUpMainDlg();														// 显示Main界面
//	void ShowUpInstallDlg();													// 显示安装界面
//	void ShowUpInstalledDlg();													// 显示安装完成界面
//	void ShowUpUninstallDlg();													// 显示卸载界面
//	void ShowUpUninstalledDlg();												// 显示卸载完成界面
//	void ShowUpRecycleDlg();													// 显示回收界面
//	void ShowUpRecycledDlg();													// 显示回收完成界面
//
//	// 其他
//	void ClearLabelData();														// 清空显示区的数据
//	CString TransforBoxID(const CString strID);									// 转换ID显示
//	CString TransforStatus(const CTokenManageSvc::TOKEN_BOX_STATUS status);		// 转换箱子当前状态
//	inline CString TransforCount(const int nCount){								// 转换数量为CString类型
//		CString strCount;
//		strCount.Format(_T("%d"),nCount);
//		return strCount;
//	}
//private:
//	CTokenManageSvc*	pService;												// 服务类指针对象
//	CTokenManageSvc::Model model;												// 工作空间
//	LABEL_GROUP_FIVE	m_labelGroupFive[MAX_ROW];								// 定义Label容器
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);						// 窗口创建消息
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);		// 窗口激活消息
//};