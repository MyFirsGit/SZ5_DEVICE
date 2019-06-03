#pragma once
#include "OperationDlg.h"

/**
@brief      通信参数输设置画面
*/
class CCommunicationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCommunicationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationSetMaintainDlg(CService* pService);
	~CCommunicationSetMaintainDlg();

	enum { IDD = IDD_20114_COMMUNICATION_SET_DLG };

	void SetFromReadIniText();														// 读取Maintenance.INI配置文件并显示在对应项上
	void InitlabelCommunicationInfo();												// 初始化BASE区域LABEL里面的数据
	void ShowIPContent();															//基本信息区显示对应项设置的数据
protected:
	virtual void UpdateUI();														// 更新显示完成回调函数
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);							// 画面创建函数							
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// 画面激活

	LABEL_GROUP m_labelGroup[15];													// 画面显示的通信参数信息
};
