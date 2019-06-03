#include "stdafx.h"
#include "MaintenanceSvc.h"
#include "MaintenanceBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CMaintenanceSvc,CTVMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceSvc::CMaintenanceSvc()
	:CTVMForeService(MAINTENANCE_SVC)
{
    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_20000_MAINTENANCE_BASE_DLG, new CMaintenanceBaseDlg(this));

    // 设置乘客画面

    // 设置画面分组
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F6_ESC2));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CMaintenanceBaseDlg::IDD);

    // 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	theAPP_SESSION.SetMaxInputLength(2);
	m_isReturn = false;
	InitData();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceSvc::~CMaintenanceSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      启动业务后设置初始界面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceSvc::OnStart(){
	m_isSecondary = false;//add
	if (m_isSecondary && 
		(theAPP_SESSION.GetMaintainMenu() != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC &&
		theAPP_SESSION.GetReturnSeviceID() != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC))
	{
		theAPP_SESSION.SetUserLoginStatus(true);
		// 设置初始画面组
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(2);

// 		theAPP_SESSION.ReturnMaintainMenu();
		InitData();

		__super::OnStart();
		m_isSecondary = false;

		return;
	}
	m_isSecondary = false;
	// 登出后再次登录，需要设置第一个画面组
	if(!theAPP_SESSION.HasUserLogin()){
		theAPP_SESSION.SetUserLoginStatus(true);
		// 设置初始画面组
		m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
		theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(2);
		// 每次登陆都需要重新显示主菜单
		m_DataModel.secondlist = NULL;
	}	
	// 如果不是F3退出，直接显示主界面
	if(!theAPP_SESSION.IsReturnMaintainMenu()){
		m_DataModel.secondlist = NULL;
	}
	InitData();
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceSvc::Model& CMaintenanceSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMaintenanceSvc::OnKeyboardEnter(WPARAM wParam, LPARAM lParam)
{
	__super::OnKeyboardEnter(wParam, lParam);

	theAPP_SESSION.SetKeyboard(KEY_NOBODY);
	ExecuteService();
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMaintenanceSvc::OnKeyboardF3(WPARAM wParam, LPARAM lParam)
{
	__super::OnKeyboardF3(wParam, lParam);

	// 二级菜单返回一级菜单
	if (m_DataModel.secondlist != NULL)
	{
		m_DataModel.secondlist = NULL;
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F6_ESC2));
		NotifyDataModelChanged();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [ESC] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMaintenanceSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam){
	// 二级菜单ESC返回主菜单
	if(m_DataModel.secondlist != NULL){
		m_DataModel.secondlist = NULL;
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F6_ESC2));
		NotifyDataModelChanged();
	}
	else{// 一级菜单登出
		__super::OnKeyboardEsc(wParam,lParam);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化翻页相关数据

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceSvc::InitData()
{
	// 获取菜单列表
	m_DataModel.firstlist = theMENU_MGR.GetMaintainMenuItems();
	// 获取返回菜单编号
	SERVICE_ID svcid = theAPP_SESSION.ResetReturnSeviceID();
	if (m_isReturn)
	{
		// 获取二级菜单
		if (svcid != UNKNOWN_SVC)
		{
			CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
			CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
			Menu_Item* menu = dlg->SearchService(svcid);
			if (menu->pParent != NULL)
				m_DataModel.secondlist = menu->pParent;
			else
				m_DataModel.secondlist = NULL;
		}
		m_isReturn = false;
		return;
	}
	if (m_isSecondary && svcid != SERVICE_ID::ONE_KEY_SETTLEMENT_SVC)
	{
		svcid = theAPP_SESSION.GetMaintainMenu();
		vector<Menu_Item*>& menulist = m_DataModel.firstlist;

		// 遍历当前菜单
		int index = 0;
		Menu_Item* menu = NULL;
		vector<Menu_Item*>::iterator iter = menulist.begin();
		for (; iter != menulist.end(); ++iter)
		{
			// 隐藏菜单项，不参与排序（与TVM/AVM功能有关）
			if (!(*iter)->isVisible || !(*iter)->isEnable)
				continue;
			menu = *iter;
			// 累加菜单编号，判断菜单编号是否相等
			if (svcid == menu->serviceID)
			{
				// 判断当前项是否开启（与权限相关）
				if (!(*iter)->isEnable)
				{
					theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ACCOUNT_ERROR_SHORT));
					return;
				}
				else
				{
					m_DataModel.secondlist = menu;
					break;
				}
			}
		}
		return;
	}
	theAPP_SESSION.SetMaintainMenu(MAINTENANCE_SVC);
	// 获取二级菜单
	if (svcid != UNKNOWN_SVC)
	{
		CMainSvc* pSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		CNavigationDlg* dlg =  (CNavigationDlg*)(pSvc->GetDialogFlow()->GetDialog(CNavigationDlg::IDD));
		Menu_Item* menu = dlg->SearchService(svcid);
		if (menu->pParent != NULL)
			m_DataModel.secondlist = menu->pParent;
		else
			m_DataModel.secondlist = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行处理

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceSvc::ExecuteService()
{
	// 获取并清除输入内容
	int menucode = theAPP_SESSION.GetInputToInt();
	theAPP_SESSION.DeleteAllInput();

	// 判断当前是一级菜单还是二级菜单
	vector<Menu_Item*>& menulist = (m_DataModel.secondlist == NULL) ?
		m_DataModel.firstlist : m_DataModel.secondlist->children;

	// 遍历当前菜单
	int index = 0;
	Menu_Item* menu = NULL;
	vector<Menu_Item*>::iterator iter = menulist.begin();
	for (; iter != menulist.end(); ++iter)
	{
		// 隐藏菜单项，不参与排序（与TVM/AVM功能有关）
		if (!(*iter)->isVisible || !(*iter)->isEnable)
			continue;

		// 累加菜单编号，判断菜单编号是否相等
		if (menucode == ++index)
		{
			// 判断当前项是否开启（与权限相关）
			if (!(*iter)->isEnable)
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ACCOUNT_ERROR_SHORT));
				return;
			}
			else
			{
				menu = *iter;
				break;
			}
		}
	}

	// 超过最大菜单编号
	if (menu == NULL)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	m_isSecondary = true;//add
	if (!m_isSecondary && 
		(menu->serviceID == BANKNOTE_BOX_MANAGE_SVC || 
		menu->serviceID == COIN_BOX_MANAGE_SVC ||
		menu->serviceID == ONE_KEY_SETTLEMENT_SVC))
	{
		m_isSecondary = true;
		theSERVICE_MGR.StartService(LOGIN_SVC);
		theAPP_SESSION.SetMaintainMenu(menu->serviceID);
		return;
	}
	// 判断是否存在子菜单
	if (menu->children.empty())
	{
		theSERVICE_MGR.StartService(menu->serviceID);
	}
	else
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
		m_DataModel.secondlist = menu;
		NotifyDataModelChanged();
	}
}
