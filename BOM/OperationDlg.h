#pragma once
#include "basedlg.h"
#include "service.h"
#include "CompositGroupBox.h"
#include "ButtonGroup.h"
#include "gedit.h"
#include "BomInfo.h"
#include "MaintenanceInfo.h"
#include "ExceptionMgr.h"
#include "MessageIDDefinition.h"
#include "UIDefinition.h"
#include <vector>
#include "Keyboard.h"
using namespace std;

const static  TCHAR* const BUTTON_LAST_STATUS_KEY = _T("BUTTON_LAST_STATUS_KEY");

/**
@brief      业务对话框父类
*/
class COperationDlg :
	public CBaseDlg
{
	DECLARE_DYNAMIC(COperationDlg)
    DECLARE_MESSAGE_MAP()

public:
    // 共同屏幕元素（按钮、数字键盘）定义
    static const ULONG BTN_OK               = 0X0001;                               // 确定按钮
    static const ULONG BTN_BACK             = 0X0002;                               // 返回按钮
    static const ULONG BTN_PRINT            = 0X0004;                               // 打印按钮
	static const ULONG BTN_RESET		    = 0X0008;								// 重置按钮
    static const ULONG BASE_AREA            = 0X0010;                               // 基本数据区域
    static const ULONG CASH_AREA            = 0X0020;                               // 现金数据区域
    static const ULONG EDIT_BOX             = 0X0040;                               // 编辑框
	static const ULONG GUIDE_AREA			= 0x0080;								// 向导框
	static const ULONG KEYBOARD_AREA	 = 0x0100;								//键盘

	COperationDlg(UINT ID, CService* pService,CRect rect = OPERATION_FRMAE_RECT,bool needFunctionArea = true);                                     // 构造函数
    COperationDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea = true);   // 构造函数
    ~COperationDlg(void);                                                           // 析构函数
	
	virtual void OnServiceBusy(bool busy);
    void ShowCompoment(ULONG ulCompomentFlag);                                      // 显示组件
    void HideCompoment(ULONG ulCompomentFlag);                                      // 隐藏组件
	
	PGSTATIC_INFO GetBaseInfo();
	PGSTATIC_INFO GetCashInfo();

	CGStatic* GetBaseArea();
	CGStatic* GetCashArea();
	CString GetEditText(int index);                                                 // 获取编辑框的显示文字
	
protected:
	virtual void OnDraw(CDC* pDC);                                                  // 画面绘制函数
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag = 0x00);  // 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // 按钮按下后的处理
    afx_msg LRESULT OnEditSetFocus(WPARAM,LPARAM);                                  // 编辑框得到焦点的消息响应函数

    void SetEditFocus(int index);                                                   // 设置输入焦点到编辑框
    void SetEditText(int index, CString text);                                      // 设置编辑框的显示文字
    CGEdit* GetEdit(int index);                                                     // 获取编辑框对象
	CXButton*       m_btnOK;                                                        // 确定按钮
	CXButton*       m_btnBack;                                                      // 返回按钮
	CXButton*       m_btnPrint;                                                     // 打印按钮
	CXButton*		m_btnReset;														// 重置按钮
    CGStatic*       m_baseInfoArea;                                                 // 基本数据区域
    CGStatic*       m_cashInfoArea;                                                 // 现金数据区域
    PGSTATIC_INFO   m_baseInfo;                                                     // 基本数据
    PGSTATIC_INFO   m_cashInfo;                                                     // 现金数据
	CButtonGroup*		m_Right_1_Group;
	CButtonGroup*		m_Right_2_Group;
	CButtonGroup*		m_Left_1_Group;
	CButtonGroup*		m_Left_2_Group;
	CKeyboard*           m_Keyboard;
	vector<BTN_INFO> m_vec_Left_1_BTN_INFO;
	vector<BTN_INFO> m_vec_Left_2_BTN_INFO;
	vector<BTN_INFO> m_vec_Right_1_BTN_INFO;
	vector<BTN_INFO> m_vec_Right_2_BTN_INFO;
	LABEL_GROUP	m_cashLabelGroup[LABEL_GROUP_EDIT_ROWS_3];
private:
	
    void Initialize();                                                              // 初期化
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);                    // 显示或隐藏组件
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);                         // 显示或隐藏组件
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);                         // 获取组件的区域
	CXButton* BuildOperationButton(CRect rect,CString text,CString msg);
	bool needFunctionArea ;
    vector<CGEdit*> m_editArray;                                                    // 编辑框列表
    int m_curEditIndex;                                                             // 当前编辑框编号
};
