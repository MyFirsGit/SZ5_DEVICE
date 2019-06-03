#pragma once
#include "OperationDlg.h"

/**
@brief  屏幕亮度设定
*/
class  CScreenBrightnessSetDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CScreenBrightnessSetDlg)
	DECLARE_MESSAGE_MAP()

public:
	CScreenBrightnessSetDlg(CService*pService) ;
	~CScreenBrightnessSetDlg() ;

	enum { IDD = IDD_01801_TIMESET_BASE_DLG };

	void InitlabelScreenBrightnessInfo();					// 初始化BASE区域LABEL里面的数据
	void ShowCodeContent();									// 基本信息区显示对应项设置的数据
protected:
	virtual void UpdateUI();								// 更新显示完成回调函数
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // 画面激活
	LABEL_GROUP m_labelGroup[15];							// 画面显示的时间信息
};