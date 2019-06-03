#pragma once
#include "OperationDlg.h"
#include "XButton.h"
#include "uidefinition.h"
#include "servicedefinition.h"
#include "ButtonGroup.h"
#include "menumgr.h"


class CMainSvc;

class CMenuDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CMenuDlg)
    DECLARE_MESSAGE_MAP()

public:
	CMenuDlg(CService* pService);                           // 构造函数
	virtual ~CMenuDlg();                                    // 析构函数
    enum { IDD = IDD_MENU_LOGIN_DLG };                        // 对话框编号
	void SetMainMenuItems(vector<Menu_Item*>& vecMenuItem);
	void SetSecondMenuItems(vector<Menu_Item*>& vecMenuItem);
	CButtonGroup* GetMainGroup();
	CButtonGroup* GetSecondGroup();
protected:
	virtual void  OnDraw(CDC* pDC);
	virtual void OnServiceBusy(bool busy);
private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);    // 画面创建函数
    LRESULT MainMenuClick(WPARAM wParam, LPARAM lParam);      // 主菜单按下后的处理
	LRESULT SecondMenuClick(WPARAM wParam, LPARAM lParam);      // 二级菜单按下后的处理

	void OnMenuKindChanged(CMenuMgr::MENU_KIND&,CMenuMgr::MENU_KIND&);
	void OnMenuItemStatusChanged(Menu_Item*);

	void  OnStartingService(CService* service);
	void OnStartedService(CService* service);
	void  OnButtonClicking(CXButton*,bool*);
	bool ConfirmDoService(SERVICE_ID& serviceID);
	void  DoService(SERVICE_ID& serviceID);
    const static int MENU_BUTTON_COUNT = 9;                 // 按钮个数
	CButtonGroup* m_MenuGroup;
	CButtonGroup* m_SecondMenuGroup;
	BTN_INFO* m_CurrentMainMenuInfo ;
};
