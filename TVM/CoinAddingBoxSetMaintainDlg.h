#pragma once
#include "OperationDlg.h"
#include "CoinAddingBoxSetSvc.h"

/**
@brief      补充箱币种设置画面
*/
class CCoinAddingBoxMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinAddingBoxMaintainDlg)
	DECLARE_MESSAGE_MAP()
public:
	CCoinAddingBoxMaintainDlg(CService* pService);
	~CCoinAddingBoxMaintainDlg();
	
	enum{IDD = IDD_20112_STATION_SET_DLG};
	
	void InitlabelAddingBoxInfo();											//初始化基本区域的label信息
	void SetFromReadIniText();												//显示从TVM.INI读取的配置文件在对应项上
	void ShowGuideDisplay();												// 向导栏显示提示数据
	void ShowCodeContent();													//基本信息区显示对应项设置的数据
protected:
	virtual void UpdateUI();												//更新显示完成回调函数
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// 画面激活
	LABEL_GROUP_THREE m_labelGroup[15];										// 画面显示的车站信息
};