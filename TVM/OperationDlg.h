#pragma once
#include "basedlg.h"
#include "service.h"
#include "CompositGroupBox.h"
#include "ButtonGroup.h"
#include "gedit.h"
#include "SysInfo.h"
#include "MaintenanceInfo.h"
#include "ExceptionMgr.h"
#include "MessageIDDefinition.h"
#include "UIDefinition.h"
#include <vector>
#include "Keyboard.h"
using namespace std;

const static  TCHAR* const BUTTON_LAST_STATUS_KEY = _T("BUTTON_LAST_STATUS_KEY");

/**
@brief  COperationDlg 维护业务对话框父类
*/
class COperationDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(COperationDlg)
    DECLARE_MESSAGE_MAP()

public:
    // 共同屏幕元素（按钮、数字键盘）定义
	static const ULONG GUIDE_AREA		= 0X0001; // 向导框
	static const ULONG BASE_AREA		= 0X0002; // 基本数据区域
	static const ULONG EDIT_BOX			= 0x0004; // 编辑框

	COperationDlg(UINT ID, CService* pService, CRect rect = OPERATOR_BASE_RECT);
    COperationDlg(UINT ID, CRect rect, COLORREF bkColor, CService* pService);
    ~COperationDlg(void);
	
	virtual void OnServiceBusy(bool busy);
    void ShowCompoment(ULONG ulCompomentFlag);						// 显示组件
    void HideCompoment(ULONG ulCompomentFlag);						// 隐藏组件
	
	PGSTATIC_INFO GetBaseInfo();									// 获取基本数据						
	CGStatic* GetBaseArea();										// 获取基本数据区域控件
	CString GetEditText(int index);									// 获取编辑框显示的文本
	int GetEditFocusID();											// 获取当前焦点编辑框ID

protected:
	virtual void OnDraw(CDC* pDC);														// 画面绘制
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag = 0x00);  // 画面创建
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);				// 窗口激活

	LRESULT OnEditSetFocus(WPARAM,LPARAM);							// 编辑框得到焦点的消息响应
	void SetEditFocus(int index);									// 设置编辑框焦点
	void SetEditText(int index, CString text);						// 设置编辑框文本
	CGEdit* GetEdit(int index);										// 获取编辑框对象

	CGStatic* m_baseInfoArea;										// 基本数据区域
	PGSTATIC_INFO m_baseInfo;										// 基本数据

private:
    void Initialize();												// 初期化
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);	// 显示或隐藏组件
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);			// 显示或隐藏组件
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);			// 获取组件的区域

    vector<CGEdit*> m_editArray;									// 编辑框列表
    int m_curEditIndex;												// 当前编辑框编号
};
