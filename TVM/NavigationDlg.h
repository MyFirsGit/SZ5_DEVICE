#pragma once
#include "BaseDlg.h"
#include "GroupBox.h"
#include "lable.h"
#include "OperationDlg.h"
#include "MenuMgr.h"

enum KEYBOARD
{
	KEY_NOBODY	= 0x0000, // 禁用维护键盘
	KEY_NUMBER	= 0x0001, // 数字键 0 ~ 9
	KEY_DOT		= 0x0002, // 圆点键 .
	KEY_CLEAR	= 0x0004, // 删除键 Backspace
	KEY_ENTER	= 0x0008, // 确认键 Enter
	KEY_ESC		= 0x0010, // 返回键 Esc
	KEY_F1		= 0x0020, // F1 键 未使用
	KEY_F2		= 0x0040, // F2 键 部分业务中的最终保存
	KEY_F3		= 0x0080, // F3 键 返回上一级
	KEY_F4		= 0x0100, // F4 键 未使用
	KEY_F5		= 0x0200, // F5 键 上一页
	KEY_F6		= 0x0400, // F6 键 下一页

	KEY_INPUT	= KEY_NUMBER | KEY_CLEAR | KEY_ENTER,	// 常用组合 基本输入
	KEY_RETURN	= KEY_F3 | KEY_ESC,						// 常用组合 返回菜单
	KEY_PAGE	= KEY_F5 | KEY_F6,						// 常用组合 翻页
};				

/**
 @ brief 操作员菜单导航栏对话框
*/
class CNavigationDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CNavigationDlg)
    DECLARE_MESSAGE_MAP()

public:
	CNavigationDlg(CService* pService);
	virtual ~CNavigationDlg();

	enum { IDD = IDD_00301_CHARGE_RECEPT_DLG };

	void SetKeyboard(DWORD keys);						// 启用维护键盘指定按键
	void DiableKeyboard(DWORD keys);					// 禁用维护键盘指定按键
	void ShowLevel(SERVICE_ID& serviceID);				// 显示菜单级别
	void AppendLevel(CString level);					// 追加菜单级别

	void DoService(SERVICE_ID& serviceID);				// 启动业务
	Menu_Item* SearchService(SERVICE_ID& serviceID);	// 查找id对应的菜单

protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDraw(CDC* pDC);
	void OnMenuKindChanged(CMenuMgr::MENU_KIND&, CMenuMgr::MENU_KIND&);
	void OnMenuItemStatusChanged(Menu_Item*);
	void OnStartingService(CService* service);
	void OnStartedService(CService* service);

	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
	
private:
	CRect m_navi_rect;				// 导航栏内容显示区域
	CString m_auto_level;			// 自动获取的菜单级别
	CString m_manual_level;			// 手动追加的菜单级别

	static DWORD m_hook_keys;		// 启用指定按键，枚举 KeyBoard 按位与。
	static HHOOK m_keyboard_hook;	// 局部键盘钩子
};
