#pragma once
#include <map>
#include <vector>
#include "stdafx.h"
#include "UIDefinition.h"

using namespace std;

#define theMENU_MGR CMenuMgr::GetInstance()

class CMenuMgr
{
public:
	static CMenuMgr& GetInstance()
	{
		return instance;
	}
	// 菜单种类
	typedef enum _menu_kind {
		MENU_UNKNOWN = -1,          // 未知
		MENU_COMMON,					// 主业务
		MENU_MAINTAIN,					// 维护业务
	} MENU_KIND;

	void Initialize(/*CMenuDlg* menuDlg*/);
	void ShutDown();

	void SetWorkMode(BYTE workMode );
	void SetUserPermission(DWORD userID);
	void SetMenuKind(MENU_KIND menuKind);         // 设置主菜单
	
	delegate<void(MENU_KIND&,MENU_KIND&)> MenuKindChanged;
	delegate<void(Menu_Item*)> MenuItemStatusChanged;
	delegate<void(Menu_Item*)> MenuRecpItemChanged;
	Menu_Item* GetMainMenuItemByID(MENU_ITEM_ID menu_Item_ID);
	Menu_Item* GetSecondMenuItemByID(MENU_ITEM_ID mainMenuID,MENU_ITEM_ID secondMenuID);
	vector<Menu_Item*> & GetCurrentMenuItems();
	vector<Menu_Item*> & GetCommonMenuItems();
	vector<Menu_Item*> & GetMaintainMenuItems();
	MENU_KIND GetMenuKind();                                                // 获取主菜单类别
	
	~CMenuMgr();
private:
	// 构造,析构函数
	CMenuMgr();
	void OnTHModeChanged(int&);
	void OnExceptionNotify(CSysException& ,bool);
	void RefreshMenuItemStatus(Menu_Item* menuItem);
	void SetItemExceptionEnable();
	void SetItemAspectEnable(Menu_Item* menuItem,const TCHAR* aspectName,bool enable,bool applyToChild=true);
	void OnRecpLanguageModeChanged(LANGUAGETYPE_T&);
	void OnOperatorLanguageModeChanged(LANGUAGETYPE_T&);
	Menu_Item* BuildMenuItem(UINT serviceItemID,CString serviceItemText,int shortcutKey = 0,SERVICE_ID serviceID=UNKNOWN_SVC,Menu_Item* parentMenuItem=NULL);

	MENU_KIND m_menuKind;                                                   // 菜单类别
	int Device_Type;														// 设备类型

	vector<Menu_Item*> m_CommonMenus;//主业务
	vector<Menu_Item*> m_MaintainMenus;//维护业务
	
	Menu_Item* FindMenuItem(vector<Menu_Item*>& menus, MENU_ITEM_ID ID);

	static CMenuMgr instance;
};