#pragma once
#include "BaseDlg.h"
#include "service.h"
#include "receptionstaticinfo.h"
#include "SysException.h"
#include "InnerException.h"
#include "ExceptionMgr.h"


/**
@brief      乘客父类对话框
*/

class CReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:

	static const ULONG BUSINESS_AREA = 0x0001;					   //业务区域

    CReceptionStaticInfo* m_BusinessArea;
	PMSG_INFO m_BusinessInfo;                                      // 业务数据

	CReceptionDlg(UINT ID, CService* pService);                     // 构造函数
	~CReceptionDlg(void);                                           // 析构函数

    void ShowCompoment(ULONG ulCompomentFlag);                      // 显示组件
    void HideCompoment(ULONG ulCompomentFlag);                      // 隐藏组件
	void ShowCompoment(CWnd *pCompoment);                           // 显示组件
	void HideCompoment(CWnd *pCompoment);                           // 隐藏组件

protected:
	virtual void OnDraw(CDC* pDC);                                  // 画面绘制函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulAreaFlag = 0x00);   // 画面创建函数
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // 画面激活后的处理


private:
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);    // 显示或隐藏组件
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);         // 显示或隐藏组件
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);         // 取得组件的区域

};
