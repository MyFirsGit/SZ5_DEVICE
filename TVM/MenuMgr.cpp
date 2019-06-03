#include "stdafx.h"
#include "MenuMgr.h"
#include "ExceptionMgr.h"
#include "ServiceMgr.h"
#include "functioninfo.h"
#include "TicketHelper.h"
#include "tvmsetting.h"
//#include "TOKENException.h"
//#include "TOKENHardwareException.h"

const static TCHAR* MENU_ITEM_EXCEPTION_ENABLE_KEY =_T("MENU_ITEM_EXCEPTION_ENABLE_KEY");
const static TCHAR* MENU_ITEM_FUNCTION_ENABLE_KEY =_T("MENU_ITEM_FUNCTION_ENABLE_KEY");
const static TCHAR* MENU_ITEM_PERMISSION_ENABLE_KEY = _T("MENU_ITEM_PERMISSION_ENABLE_KEY");
const static TCHAR* MENU_ITEM_CONFIG_ENABLE_KEY = _T("MENU_ITEM_CONFIG_ENABLE_KEY");
CMenuMgr CMenuMgr::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMenuMgr::CMenuMgr()
{	
	Device_Type = theMAINTENANCE_INFO.GetDeviceType();										// 获取设备类型
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMenuMgr::~CMenuMgr(void)
{	
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      构建一个菜单项

@param      UINT serviceItemID			服务按钮序号
@param      CString serviceItemText	服务按钮显示内容
@param      SERVICE_ID serviceID	服务编号
@param      Menu_Item* parentMenuItem	父菜单项

@retval     Menu_Item* 菜单项

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
Menu_Item* CMenuMgr::BuildMenuItem(UINT serviceItemID,CString serviceItemText,int shortcutKey,SERVICE_ID serviceID,Menu_Item* parentMenuItem)
{
	Menu_Item* menuItem = new Menu_Item;
	menuItem->btnID = serviceItemID;
	menuItem->btnMsg.Format(_T("%d"),serviceItemID);
	menuItem->text = serviceItemText;
	menuItem->serviceID = serviceID;
	menuItem->shortCutKey = shortcutKey;
	bool isConfigAllow = theFunction_INFO.IsMenuEnabled(menuItem->btnID);
	menuItem->bag->SetValue(MENU_ITEM_CONFIG_ENABLE_KEY,isConfigAllow);
	menuItem->isEnable = isConfigAllow;
	menuItem->isVisible = theFunction_INFO.IsMenuVisibled(menuItem->btnID);
	if(parentMenuItem!=NULL)
	{
		menuItem->pParent = parentMenuItem;
		parentMenuItem->children.push_back(menuItem);
	}
	return menuItem;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化菜单信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::Initialize(/*CMenuDlg* menuDlg*/)
{
	theAPP_SESSION.ClientLanguageModeChanged.AddHandler(this,&CMenuMgr::OnRecpLanguageModeChanged);
	theAPP_SESSION.OperatorLanguageModeChanged.AddHandler(this,&CMenuMgr::OnOperatorLanguageModeChanged);
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CMenuMgr::OnExceptionNotify);


	// ----------------------------------乘客界面菜单--------------------------------------------
	// 售票
	m_CommonMenus.push_back(BuildMenuItem(ISSUE_ID,_tl(TXT_ID_ISSUE_SVC),VK_F1,ISSUE_SVC));
	// 充值
	m_CommonMenus.push_back(BuildMenuItem(RECHARGE_ID,_tl(TXT_ID_CHARGE_SVC),VK_F2,CHARGE_SVC));
	// 添加产品
	//m_CommonMenus.push_back(BuildMenuItem(ADDPRODUCT_ID,_tl(TXT_ID_ADD_PRODUCT),VK_F3,ISSUE_PRODUCT_SVC));
	// 查询
	m_CommonMenus.push_back(BuildMenuItem(ANALYZE_SVC_ID,_tl(TXT_ID_ANALYZE_SVC),VK_F6,ANALYZE_SVC));


	// ---------------------------------维护界面菜单---------------------------------------------

	// 交易一览
	m_MaintainMenus.push_back(BuildMenuItem(TRANSACTION_HISTORY_ID, _opl(TXT_ID_TVM_TRANSACTION_HISTORY_SVC) , 0, TRANSACTION_HISTORY_SVC));

	// 设备状态
	m_MaintainMenus.push_back(BuildMenuItem(DEVICE_STATUS_ID, _opl(TXT_ID_TVM_DEVICE_STATUS_SVC) , 0, DEVICE_STATUS_SVC));

	// 销售统计
	m_MaintainMenus.push_back(BuildMenuItem(SALE_STATISTIC_SVC_ID, _opl(TXT_ID_TVM_SALE_STATISTIC_SVC) , 0, SALE_STATIC_SVC));

	// Card箱管理
	m_MaintainMenus.push_back(BuildMenuItem(CARD_BOX_MANAGE_SVC_ID, _opl(TXT_ID_TVM_CARD_BOX_MANAGE_SVC), 0, CARD_BOX_MANAGE_SVC));

	// 纸币箱管理
	Menu_Item* banknote_box_mng = BuildMenuItem(BANKNOTE_BOX_MANAGE_SVC_ID, _opl(TXT_ID_TVM_BANKNOTE_BOX_MANAGE_SVC), 0,BANKNOTE_BOX_MANAGE_SVC);
	BuildMenuItem(INSTALL_BANKNOTE_COLLECTION_BOX_SVC_ID, _opl(TXT_ID_TVM_BANKNOTE_COLLECTION_BOX_INSTALL), 0, INSTALL_BANKNOTE_COLLECTION_BOX_SVC, banknote_box_mng);
	BuildMenuItem(REMOVE_BANKNOTE_COLLECTION_BOX_SVC_ID, _opl(TXT_ID_TVM_BANKNOTE_COLLECTION_BOX_UNINSTALL), 0, REMOVE_BANKNOTE_COLLECTION_BOX_SVC, banknote_box_mng);
	BuildMenuItem(INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC_ID, _opl(TXT_ID_TVM_BANKNOTE_CHANGE_INVALID_BOX_INSTALL), 0, INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC, banknote_box_mng);
	BuildMenuItem(REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC_ID, _opl(TXT_ID_TVM_BANKNOTE_CHANGE_INVALID_BOX_UNINSTALL), 0, REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC, banknote_box_mng);
	BuildMenuItem(BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC_ID,_T("纸币回收"),0,BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC,banknote_box_mng);
	m_MaintainMenus.push_back(banknote_box_mng);

	// 硬币箱管理
	Menu_Item* coinbox_management = BuildMenuItem(COIN_BOX_MANAGE_SVC_ID, _opl(TXT_ID_TVM_COIN_BOX_MANAGE_SVC), 0,COIN_BOX_MANAGE_SVC);
	BuildMenuItem(ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC_ID, _opl(TXT_COINMGR_ADDCOIN_DEDICATED_CHANGE_BOX), 0, ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC, coinbox_management);
	BuildMenuItem(INSTALL_COIN_CHANGE_BOX_SVC_ID, _opl(TXT_COINMGR_MENU_INSTALL_CHANGE_BOX), 0, INSTALL_COIN_CHANGE_BOX_SVC, coinbox_management);
	BuildMenuItem(REMOVE_COIN_CHANGE_BOX_SVC_ID, _opl(TXT_COINMGR_MENU_UNINSTALL_CHANGE_BOX), 0, REMOVE_COIN_CHANGE_BOX_SVC, coinbox_management);
	BuildMenuItem(ATTACH_COLLECTION_COIN_BOX_SVC_ID, _opl(TXT_COINMGR_MENU_ATTACH), 0, ATTACH_COLLECTION_COIN_BOX_SVC, coinbox_management);
	BuildMenuItem(DETACH_COLLECTION_COIN_BOX_SVC_ID, _opl(TXT_COINMGR_MENU_DETACH), 0, DETACH_COLLECTION_COIN_BOX_SVC, coinbox_management);
	BuildMenuItem(COLLECTION_COIN_SVC_ID, _opl(TXT_COINMGR_COLLECTION_COIN), 0, COLLECTION_COIN_SVC, coinbox_management);
	m_MaintainMenus.push_back(coinbox_management);


	// 业务结束
	m_MaintainMenus.push_back(BuildMenuItem(OPERATION_CLOSE_SVC_ID, _opl(TXT_ID_TVM_OPERATION_CLOSE_SVC) , 0, OPERATION_CLOSE_SVC));

	// 一键收机
	m_MaintainMenus.push_back(BuildMenuItem(ONE_KEY_SETTLEMENT_SVC_ID,_opl(TXT_ID_TVM_MENU_ONE_KEY_CLOSE),0,ONE_KEY_SETTLEMENT_SVC));

	// 运行模式设定
	m_MaintainMenus.push_back(BuildMenuItem(RUN_MODE_SETTING_SVC_ID, _opl(TXT_ID_TVM_RUN_MODE_SETTING_SVC) , 0, RUN_MODE_SETTING_SVC));

	// 凭证补打
	m_MaintainMenus.push_back(BuildMenuItem(RECEIPT_REPRINT_SVC_ID, _opl(TXT_ID_TVM_RECEIPT_REPRINT_SVC),0, RECEIPT_REPRINT_SVC));

	// 系统设定
	Menu_Item* system_setting = BuildMenuItem(SYSTEM_SET_SVC_ID, _opl(TXT_ID_TVM_SYSTEM_SET_SVC),0, SYSTEM_SET_SVC);
	BuildMenuItem(STATION_SET_SVC_ID, _opl(TXT_ID_TVM_STATION_SET_SVC), 0, STATION_SET_SVC, system_setting);
	BuildMenuItem(COMMUNICATION_SET_SVC_ID, _opl(TXT_ID_TVM_COMMUNICATION_SET_SVC), 0, COMMUNICATION_SET_SVC, system_setting);
	BuildMenuItem(SYSTEM_TIME_SET_SVC_ID, _opl(TXT_ID_TVM_SYSTEM_TIME_SET_SVC), 0, SYSTEM_TIME_SET_SVC, system_setting);
	BuildMenuItem(SWITCH_OFF_SVC_ID, _opl(TXT_ID_TVM_SWITCH_OFF_SVC), 0, SWITCH_OFF_SVC, system_setting);
	BuildMenuItem(DEVICE_COMPORT_SET_ID,_T("串口号与波特率设置"),0,DEVICE_COMPORT_SET_SVC,system_setting);
	m_MaintainMenus.push_back(system_setting);

	// 数据管理
	Menu_Item* data_management = BuildMenuItem(DATA_MANAGE_SVC_ID, _opl(TXT_ID_TVM_DATA_MANAGE_SVC),0, DATA_MANAGE_SVC);
	BuildMenuItem(DATA_INPUT_SVC_ID, _opl(TXT_ID_TVM_DATA_INPUT_SVC), 0, DATA_INPUT_SVC, data_management);
	BuildMenuItem(DATA_OUTPUT_SVC_ID, _opl(TXT_ID_TVM_DATA_OUTPUT_SVC), 0, DATA_OUTPUT_SVC, data_management);
	BuildMenuItem(VERSION_INFO_SVC_ID,_opl(TXT_ID_TVM_VERSION_INFO_SVC), 0, VERSION_INFO_SVC, data_management);
	m_MaintainMenus.push_back(data_management);

	// 硬件自检
	Menu_Item* device_test = BuildMenuItem(DEVICE_TEST_SVC_ID, _opl(TXT_ID_TVM_DEVICE_TEST_SVC),0, DEVICE_TEST_SVC);
	BuildMenuItem(RW_TEST_SVC_ID, _opl(TXT_ID_TVM_RW_TEST_SVC), 0, RW_TEST_SVC, device_test);
	BuildMenuItem(MONEYBOX_TEST_SVC_ID, _opl(TXT_ID_TVM_MONEYBOX_TEST_SVC), 0, COIN_MODULE_TEST_SVC, device_test);
	BuildMenuItem(BANKNOTE_ACCEPT_MODULE_TEST_SVC_ID, _T("纸币循环模块")/*opl(TXT_ID_TVM_BANKNOTE_ACCEPT_MODULE_TEST_SVC)*/, 0, BANKNOTE_ACCEPT_MODULE_TEST_SVC, device_test);
	//BuildMenuItem(BANKNOTE_CHANGE_MODULE_TEST_SVC_ID, _opl(TXT_ID_TVM_BANKNOTE_CHANGE_MODULE_TEST_SVC), 0, BANKNOTE_CHANGE_MODULE_TEST_SVC, device_test);
	BuildMenuItem(CARD_ISSUE_MODULE_TEST_SVC_ID, _opl(TXT_ID_TVM_CARD_ISSUE_MODULE_TEST_SVC), 0, CARD_MODULE_TEST_SVC, device_test);
	BuildMenuItem(RECEIPT_RINTER_TEST_SVC_ID, _opl(TXT_ID_TVM_RECEIPT_RINTER_TEST_SVC), 0, RECEIPT_RINTER_TEST_SVC, device_test);
	BuildMenuItem(IO_MODULE_TEST_SVC_ID, _opl(TXT_ID_TVM_IO_MODULE_TEST_SVC), 0, IO_MODULE_TEST_SVC, device_test);
	BuildMenuItem(NETWORK_TEST_SVC_ID, _opl(TXT_ID_TVM_NETWORK_TEST_SVC), 0, NETWORK_TEST_SVC, device_test);
	BuildMenuItem(ALL_DEVICE_STATUS_ID,_opl(TXT_ID_ALL_DEVICE_STATUS),0,ALL_DEVICE_STATUS_SVC,device_test);
	m_MaintainMenus.push_back(device_test);

	// 异常解除
	m_MaintainMenus.push_back(BuildMenuItem(REMOVE_EXCEPTION_SVC_ID,_opl(TXT_ID_TVM_REMOVE_EXCEPTION_SVC),0,REMOVE_EXCEPTION_SVC));

	// 关机重启
	Menu_Item * shutdown_restart = BuildMenuItem(SHUTDOWN_ID, _opl(TXT_ID_TVM_SHUTDOWN_SVC), 0,SHUTDOWN_RESTART_SVC); 
	BuildMenuItem(SHUTDOWN_COMPUTER_ID, _opl(TXT_ID_SHUTDOWN_COMPUTER), 0,SHUTDOWN_SVC,shutdown_restart);
	BuildMenuItem(RESTART_COMPUTTER_ID, _opl(TXT_ID_RESTART_COMPUTER), 0, RESTART_SVC, shutdown_restart);
	BuildMenuItem(EXIT_PROGRAM_ID, _T("退出程序"), 0, EXIT_SVC, shutdown_restart);
	m_MaintainMenus.push_back(shutdown_restart);

	SetItemExceptionEnable();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::ShutDown()
{
	theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CMenuMgr::OnExceptionNotify);
	theAPP_SESSION.ClientLanguageModeChanged.RemoveHandler(this,&CMenuMgr::OnRecpLanguageModeChanged);
	theAPP_SESSION.OperatorLanguageModeChanged.RemoveHandler(this,&CMenuMgr::OnOperatorLanguageModeChanged);
	
	for(int i=0;i<m_CommonMenus.size();i++)
	{
		delete m_CommonMenus[i];
	}
	for(int i=0;i<m_MaintainMenus.size();i++)
	{
		delete m_MaintainMenus[i];
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置主菜单

@param      (i)MENU_KIND menuKind   种类
@param      (i)bool startDefault    是否启动默认业务

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::SetMenuKind(MENU_KIND menuKind)
{
	MENU_KIND oldMenuKind = m_menuKind;
	m_menuKind = menuKind;
	MenuKindChanged.Invoke(oldMenuKind,m_menuKind);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取主菜单类别

@param      无

@retval     MENU_KIND   菜单类别

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMenuMgr::MENU_KIND CMenuMgr::GetMenuKind()
{
	return m_menuKind;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
 vector<Menu_Item*> & CMenuMgr::GetCurrentMenuItems()
{
	switch(m_menuKind)
	{
	case MENU_COMMON:
		return m_CommonMenus;
	case MENU_MAINTAIN:
		return m_MaintainMenus;
	default:
		ASSERT(false);
	}
}

 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      

 @param      

 @retval     

 @exception  
 */
 //////////////////////////////////////////////////////////////////////////
 vector<Menu_Item*> & CMenuMgr::GetCommonMenuItems()
 {
	 return m_CommonMenus;	
 }

 //////////////////////////////////////////////////////////////////////////
 /*
 @brief      获取维护业务菜单 

 @param      无

 @retval     无

 @exception  无
 */
 //////////////////////////////////////////////////////////////////////////
 vector<Menu_Item*>& CMenuMgr::GetMaintainMenuItems()
 {
	 return m_MaintainMenus;	
 }


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Menu_Item* CMenuMgr::GetMainMenuItemByID(MENU_ITEM_ID menu_Item_ID)
{
	vector<Menu_Item*> & vecMenuItem = GetCurrentMenuItems();
	for(int i=0;i<vecMenuItem.size();i++)
	{
		if(vecMenuItem[i]->btnID == menu_Item_ID)
		{
			return vecMenuItem[i];
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::RefreshMenuItemStatus(Menu_Item* menuItem)
{
	bool isFunctionAllow = true;
	bool isExceptionAllow = true;
	bool isPermissionAllow = true;
	bool isConfigAllow = true;
	menuItem->bag->GetValue(MENU_ITEM_FUNCTION_ENABLE_KEY,isFunctionAllow);
	menuItem->bag->GetValue(MENU_ITEM_EXCEPTION_ENABLE_KEY,isExceptionAllow);
	menuItem->bag->GetValue(MENU_ITEM_PERMISSION_ENABLE_KEY,isPermissionAllow);
	menuItem->bag->GetValue(MENU_ITEM_CONFIG_ENABLE_KEY,isConfigAllow);

	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("MenuItem:%s IsPermissionAllow=%d,IsFunctionAllow=%d,IsExceptionAllow = %d,IsConfigAllow=%d"),menuItem->text,isPermissionAllow,isFunctionAllow ,isExceptionAllow,isConfigAllow);
	bool oldEnableStatus = menuItem->isEnable;
	menuItem->isEnable = isPermissionAllow&&isFunctionAllow && isExceptionAllow &&isConfigAllow;
	if(oldEnableStatus!=menuItem->isEnable)
	{
		MenuItemStatusChanged.Invoke(menuItem);
		MenuRecpItemChanged.Invoke(menuItem);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Menu_Item* CMenuMgr::GetSecondMenuItemByID(MENU_ITEM_ID mainMenuID,MENU_ITEM_ID secondMenuID)
{
	Menu_Item* mainMenuItem = GetMainMenuItemByID(mainMenuID);
	if(mainMenuItem == NULL)
	{
		return NULL;
	}
	const vector<Menu_Item*> vecSecondMenuItem = mainMenuItem->children;
	for (int i=0;i<vecSecondMenuItem.size();i++)
	{
		if(vecSecondMenuItem[i]->btnID == secondMenuID)
		{
			return vecSecondMenuItem[i];
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Menu_Item* CMenuMgr::FindMenuItem(vector<Menu_Item*>& menus, MENU_ITEM_ID ID)
{
	for (int i=0;i<menus.size();i++)
	{
		Menu_Item* menu_item = menus[i];
		if(menu_item->btnID == ID)
		{
			return menu_item;
		}
		else if(menu_item->children.size() > 0)
		{
			Menu_Item* childMenuItem = FindMenuItem(menu_item->children,ID);
			if(childMenuItem!=NULL)
			{
				return childMenuItem;
			}
		}
	}
	//ASSERT(false);
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      设置菜单项在异常情况下还是否可用

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CMenuMgr::SetItemExceptionEnable()
{
	//是否有异常
	bool hasException = theEXCEPTION_MGR.HasException();

	//是否只有硬件异常
	bool onlyHasHardWardException = theEXCEPTION_MGR.OnlyHasHardwareException();
	
	vector<long> dependOnTOKENModuleIDs;
	/*dependOnTOKENModuleIDs.push_back(CTOKENHardwareException::MODULE_ID);
	dependOnTOKENModuleIDs.push_back(CTOKENException::MODULE_ID);*/
	dependOnTOKENModuleIDs.push_back(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID);
	dependOnTOKENModuleIDs.push_back(CRWException::TVM_TOKEN_RW_MODULE_ID);
	dependOnTOKENModuleIDs.push_back(CCHException::MODULE_ID);
	dependOnTOKENModuleIDs.push_back(CBRHException::MODULE_ID);

	vector<long> dependOnCARDModuleIDs;
	dependOnCARDModuleIDs.push_back(CTHHardwareException::MODULE_ID);
	dependOnCARDModuleIDs.push_back(CTHException::MODULE_ID);
	dependOnCARDModuleIDs.push_back(CRWHardwareException::TVM_CARD_RW_MODULE_ID);
	dependOnCARDModuleIDs.push_back(CRWException::TVM_CARD_RW_MODULE_ID);
	dependOnCARDModuleIDs.push_back(CCHException::MODULE_ID);
	dependOnCARDModuleIDs.push_back(CBRHException::MODULE_ID);

	vector<long> dependOnCHARGEModuleIDs;
	dependOnCHARGEModuleIDs.push_back(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID);
	dependOnCHARGEModuleIDs.push_back(CRWException::TVM_RECHARGE_RW_MODULE_ID);
	dependOnCHARGEModuleIDs.push_back(CCHException::MODULE_ID);
	dependOnCHARGEModuleIDs.push_back(CBRHException::MODULE_ID);

	vector<long> dependOnAnalyzeModuleIDs;
	dependOnAnalyzeModuleIDs.push_back(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID);
	dependOnAnalyzeModuleIDs.push_back(CRWException::TVM_RECHARGE_RW_MODULE_ID);
	dependOnAnalyzeModuleIDs.push_back(CCHException::MODULE_ID);
	dependOnAnalyzeModuleIDs.push_back(CBRHException::MODULE_ID);

	
	//能否发售TOKEN
	bool canUseTOKEN = (!hasException) || (onlyHasHardWardException && !theEXCEPTION_MGR.HasException(dependOnTOKENModuleIDs));
	//能否发售CARD
	bool canUseCARD = (!hasException) || (onlyHasHardWardException && !theEXCEPTION_MGR.HasException(dependOnCARDModuleIDs));
	//能否充值加产品
	bool canUseCHARGE = (!hasException) || (onlyHasHardWardException && !theEXCEPTION_MGR.HasException(dependOnCHARGEModuleIDs));
	//能否使用外部读写器做业务
	bool canUseCHARGETPU = (!hasException) || (onlyHasHardWardException && !theEXCEPTION_MGR.HasException(dependOnAnalyzeModuleIDs));

	//SetItemAspectEnable(FindMenuItem(m_CommonMenus,ISSUE_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseTOKEN||canUseCARD);
	//SetItemAspectEnable(FindMenuItem(m_CommonMenus,ADDPRODUCT_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseCHARGETPU);
	//SetItemAspectEnable(FindMenuItem(m_CommonMenus,RECHARGE_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseCHARGETPU);
	//SetItemAspectEnable(FindMenuItem(m_CommonMenus,ANALYZE_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseCHARGETPU);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::SetItemAspectEnable(Menu_Item* menuItem,const TCHAR* aspectName,bool enable,bool applyToChild)
{
	if(menuItem == NULL)
	{
		return;
	}
	menuItem->bag->SetValue(aspectName,enable);
	RefreshMenuItemStatus(menuItem);
	if(applyToChild)
	{
		for (vector<_Menu_Item*>::iterator childIter = menuItem->children.begin(); childIter != menuItem->children.end();childIter++)
		{
			SetItemAspectEnable(*childIter,aspectName,enable,applyToChild);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::SetWorkMode(BYTE workMode )
{
	/*bool canIssue = workMode & WORK_MODE_ISSUE_TOKEN || workMode & WORK_MODE_ISSUE_CARD;
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ISSUE_ID),MENU_ITEM_FUNCTION_ENABLE_KEY,canIssue);

	bool canCharge = workMode & WORK_MODE_RECHARGE ;
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,RECHARGE_ID),MENU_ITEM_FUNCTION_ENABLE_KEY,canCharge);

	bool canAddProduct = workMode & WORK_MODE_ISSUE_PRODUCT ;
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ADDPRODUCT_ID),MENU_ITEM_FUNCTION_ENABLE_KEY,canAddProduct);

	bool canQuerry = workMode & WORK_MODE_QUERY ;
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ANALYZE_SVC_ID),MENU_ITEM_FUNCTION_ENABLE_KEY,canQuerry);*/

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::SetUserPermission(DWORD userID)
{
	bool superUser = theFunction_INFO.IsFounderStaffID(userID);
	bool isHavePermission = false;

	// 交易一览
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_CASH_SALE_INFO);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,TRANSACTION_HISTORY_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 设备状态
	isHavePermission= superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_CASH_SALE_INFO);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,DEVICE_STATUS_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 销售统计
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_CASH_SALE_INFO);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,SALE_STATISTIC_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	if(DEVICE_TYPE_TVM == Device_Type)
	{
		// 票箱管理
		isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_CARD_BOX_MANAGE);
		SetItemAspectEnable(FindMenuItem(m_MaintainMenus,TOKEN_BOX_MANAGE_SVC_ID/*CARD_BOX_MANAGE_SVC_ID*/),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

		// TOKEN箱管理
		/*isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_TOKEN_BOX_MANAGE);
		SetItemAspectEnable(FindMenuItem(m_MaintainMenus,TOKEN_BOX_MANAGE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);*/
	}
	

	// 纸币箱管理
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,BANKNOTE_BOX_MANAGE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	if(DEVICE_TYPE_TVM == Device_Type)
	{
		// 硬币箱管理
		isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_BH_CH_BOX_MANAGE);
		SetItemAspectEnable(FindMenuItem(m_MaintainMenus,COIN_BOX_MANAGE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);
	}
	
	// 模式设定
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_MODE_SET);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,RUN_MODE_SETTING_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 结算处理
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_BALANCE_MANAGE);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,OPERATION_CLOSE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 一键收机
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_BALANCE_MANAGE);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,ONE_KEY_SETTLEMENT_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 凭证补打
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_BALANCE_MANAGE);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,RECEIPT_REPRINT_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 系统设定
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_SYSTEM_HARDWARE_TEST);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,SYSTEM_SET_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 数据管理
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_DATA_MANAGE);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,DATA_MANAGE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 硬件自检
	isHavePermission = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_TVM_FUNCTION_SYSTEM_HARDWARE_TEST);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,DEVICE_TEST_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,isHavePermission);

	// 异常解除
	FindMenuItem(m_MaintainMenus,REMOVE_EXCEPTION_SVC_ID)->isVisible = true;
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,REMOVE_EXCEPTION_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,true);
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   异常信息有变化时   

@param      无

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CMenuMgr::OnExceptionNotify(CSysException& ,bool)
{
		SetItemExceptionEnable();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   当语言模式发生变化时，需要更新菜单中的文言。   

@param      无

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CMenuMgr::OnRecpLanguageModeChanged(LANGUAGETYPE_T&)
{
	// 普通业务
	FindMenuItem(m_CommonMenus,ISSUE_ID)->text = _tl(TXT_ID_ISSUE_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_CommonMenus,ISSUE_ID));
	FindMenuItem(m_CommonMenus,RECHARGE_ID)->text = _tl(TXT_ID_CHARGE_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_CommonMenus,RECHARGE_ID));
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief   当语言模式发生变化时，需要更新菜单中的文言。   

@param      无

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CMenuMgr::OnOperatorLanguageModeChanged(LANGUAGETYPE_T&)
{
	
	// 交易一览
	FindMenuItem(m_MaintainMenus,TRANSACTION_HISTORY_ID)->text = _opl(TXT_ID_TVM_TRANSACTION_HISTORY_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,TRANSACTION_HISTORY_ID));
	// 设备状态
	FindMenuItem(m_MaintainMenus,DEVICE_STATUS_ID)->text = _opl(TXT_ID_TVM_DEVICE_STATUS_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DEVICE_STATUS_ID));
	// 销售统计
	FindMenuItem(m_MaintainMenus,SALE_STATISTIC_SVC_ID)->text = _opl(TXT_ID_TVM_SALE_STATISTIC_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SALE_STATISTIC_SVC_ID));
	if(DEVICE_TYPE_TVM == Device_Type)
	{
		//// Smart-Card箱管理
		//FindMenuItem(m_MaintainMenus,CARD_BOX_MANAGE_SVC_ID)->text = _opl(TXT_ID_TVM_CARD_BOX_MANAGE_SVC);
		//MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,CARD_BOX_MANAGE_SVC_ID));
		// Token箱管理
		FindMenuItem(m_MaintainMenus,TOKEN_BOX_MANAGE_SVC_ID)->text = _opl(TXT_ID_TVM_TOKEN_BOX_MANAGE_SVC);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,TOKEN_BOX_MANAGE_SVC_ID));
	}
	
	// 销售统计
	FindMenuItem(m_MaintainMenus,BANKNOTE_BOX_MANAGE_SVC_ID)->text = _opl(TXT_ID_TVM_BANKNOTE_BOX_MANAGE_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,BANKNOTE_BOX_MANAGE_SVC_ID));
	if(DEVICE_TYPE_TVM == Device_Type)
	{
		// 硬币箱管理
		FindMenuItem(m_MaintainMenus,COIN_BOX_MANAGE_SVC_ID)->text = _opl(TXT_ID_TVM_COIN_BOX_MANAGE_SVC);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,COIN_BOX_MANAGE_SVC_ID));
		FindMenuItem(m_MaintainMenus,ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC_ID)->text = _opl(TXT_COINMGR_ADDCOIN_DEDICATED_CHANGE_BOX);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC_ID));
		/*FindMenuItem(m_MaintainMenus,ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC_ID)->text = _opl(TXT_COINMGR_ADDCOIN_CYCLE_CHANGE_BOX);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC_ID));*/
		FindMenuItem(m_MaintainMenus,ATTACH_COLLECTION_COIN_BOX_SVC_ID)->text = _opl(TXT_COINMGR_MENU_ATTACH);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,ATTACH_COLLECTION_COIN_BOX_SVC_ID));
		FindMenuItem(m_MaintainMenus,DETACH_COLLECTION_COIN_BOX_SVC_ID)->text = _opl(TXT_COINMGR_MENU_DETACH);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DETACH_COLLECTION_COIN_BOX_SVC_ID));
		//FindMenuItem(m_MaintainMenus,DETACH_ADDING_COIN_BOX_SVC_ID)->text = _opl(TXT_COINMGR_DETACH_ADDING_BOX);
		//MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DETACH_ADDING_COIN_BOX_SVC_ID));
		FindMenuItem(m_MaintainMenus,COLLECTION_COIN_SVC_ID)->text = _opl(TXT_COINMGR_COLLECTION_COIN);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,COLLECTION_COIN_SVC_ID));
	}
	
	// 業務結束
	FindMenuItem(m_MaintainMenus,OPERATION_CLOSE_SVC_ID)->text = _opl(TXT_ID_TVM_OPERATION_CLOSE_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,OPERATION_CLOSE_SVC_ID));
	// 运行模式设定
	FindMenuItem(m_MaintainMenus,RUN_MODE_SETTING_SVC_ID)->text = _opl(TXT_ID_TVM_RUN_MODE_SETTING_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,RUN_MODE_SETTING_SVC_ID));
	/*FindMenuItem(m_MaintainMenus,SERVICE_MODE_SETTING_SVC_ID)->text = _opl(TXT_ID_TVM_SERVICE_MODE_SETTING_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SERVICE_MODE_SETTING_SVC_ID));
	FindMenuItem(m_MaintainMenus,PAYMENT_MODE_SETTING_SVC_ID)->text = _opl(TXT_ID_TVM_PAYMENT_MODE_SETTING_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,PAYMENT_MODE_SETTING_SVC_ID));
	FindMenuItem(m_MaintainMenus,CHANGE_MODE_SETTING_SVC_ID)->text = _opl(TXT_ID_TVM_CHANGE_MODE_SETTING_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,CHANGE_MODE_SETTING_SVC_ID));
	FindMenuItem(m_MaintainMenus,PRINT_MODE_SETTING_SVC_ID)->text = _opl(TXT_ID_TVM_PRINT_MODE_SETTING_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,PRINT_MODE_SETTING_SVC_ID));*/
	// 凭证补打
	FindMenuItem(m_MaintainMenus,RECEIPT_REPRINT_SVC_ID)->text = _opl(TXT_ID_TVM_RECEIPT_REPRINT_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,RECEIPT_REPRINT_SVC_ID));
	// 系统设定
	FindMenuItem(m_MaintainMenus,SYSTEM_SET_SVC_ID)->text = _opl(TXT_ID_TVM_SYSTEM_SET_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SYSTEM_SET_SVC_ID));
	FindMenuItem(m_MaintainMenus,STATION_SET_SVC_ID)->text = _opl(TXT_ID_TVM_STATION_SET_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,STATION_SET_SVC_ID));
	FindMenuItem(m_MaintainMenus,COMMUNICATION_SET_SVC_ID)->text = _opl(TXT_ID_TVM_COMMUNICATION_SET_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,COMMUNICATION_SET_SVC_ID));
	FindMenuItem(m_MaintainMenus,SYSTEM_TIME_SET_SVC_ID)->text = _opl(TXT_ID_TVM_SYSTEM_TIME_SET_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SYSTEM_TIME_SET_SVC_ID));
	//FindMenuItem(m_MaintainMenus,SWITCH_OFF_SVC_ID)->text = _opl(TXT_ID_TVM_SWITCH_OFF_SVC);
	//MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SWITCH_OFF_SVC_ID));
	//FindMenuItem(m_MaintainMenus,SCREEN_BRIGHTNESS_SVC_ID)->text = _opl(TXT_ID_TVM_SCREEN_BRIGHTNESS_SVC);
	//MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SCREEN_BRIGHTNESS_SVC_ID));
	// 数据管理
	FindMenuItem(m_MaintainMenus,DATA_MANAGE_SVC_ID)->text = _opl(TXT_ID_TVM_DATA_MANAGE_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DATA_MANAGE_SVC_ID));
	FindMenuItem(m_MaintainMenus,DATA_INPUT_SVC_ID)->text = _opl(TXT_ID_TVM_DATA_INPUT_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DATA_INPUT_SVC_ID));
	FindMenuItem(m_MaintainMenus,DATA_OUTPUT_SVC_ID)->text = _opl(TXT_ID_TVM_DATA_OUTPUT_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DATA_OUTPUT_SVC_ID));
	FindMenuItem(m_MaintainMenus,VERSION_INFO_SVC_ID)->text = _opl(TXT_ID_TVM_VERSION_INFO_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,VERSION_INFO_SVC_ID));
	// 硬件自检
	FindMenuItem(m_MaintainMenus,DEVICE_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_DEVICE_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,DEVICE_TEST_SVC_ID));
	FindMenuItem(m_MaintainMenus,RW_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_RW_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,RW_TEST_SVC_ID));
	if(DEVICE_TYPE_TVM == Device_Type)
	{
		FindMenuItem(m_MaintainMenus,MONEYBOX_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_MONEYBOX_TEST_SVC);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,MONEYBOX_TEST_SVC_ID));
	}
	FindMenuItem(m_MaintainMenus,ALL_DEVICE_STATUS_ID)->text = _opl(TXT_ID_ALL_DEVICE_STATUS);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,ALL_DEVICE_STATUS_ID));
	FindMenuItem(m_MaintainMenus,BANKNOTE_ACCEPT_MODULE_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_BANKNOTE_ACCEPT_MODULE_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,BANKNOTE_ACCEPT_MODULE_TEST_SVC_ID));
	if(DEVICE_TYPE_TVM == Device_Type)
	{
		FindMenuItem(m_MaintainMenus,CARD_ISSUE_MODULE_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_CARD_ISSUE_MODULE_TEST_SVC);
		MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,CARD_ISSUE_MODULE_TEST_SVC_ID));
		//FindMenuItem(m_MaintainMenus,TOKEN_MODULE_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_TOKEN_MODULE_TEST_SVC);
		//MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,TOKEN_MODULE_TEST_SVC_ID));
	}
	FindMenuItem(m_MaintainMenus,RECEIPT_RINTER_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_RECEIPT_RINTER_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,RECEIPT_RINTER_TEST_SVC_ID));
	/*FindMenuItem(m_MaintainMenus,PIN_PAD_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_PIN_PAD_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,PIN_PAD_TEST_SVC_ID));
	FindMenuItem(m_MaintainMenus,BANKCARD_MODULE_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_BANKCARD_MODULE_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,BANKCARD_MODULE_TEST_SVC_ID));*/
	FindMenuItem(m_MaintainMenus,IO_MODULE_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_IO_MODULE_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,IO_MODULE_TEST_SVC_ID));
	FindMenuItem(m_MaintainMenus,NETWORK_TEST_SVC_ID)->text = _opl(TXT_ID_TVM_NETWORK_TEST_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,NETWORK_TEST_SVC_ID));
	// 异常解除
	FindMenuItem(m_MaintainMenus,REMOVE_EXCEPTION_SVC_ID)->text = _opl(TXT_ID_TVM_REMOVE_EXCEPTION_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,REMOVE_EXCEPTION_SVC_ID));
	
	// 关机重启
	FindMenuItem(m_MaintainMenus,SHUTDOWN_ID)->text = _opl(TXT_ID_TVM_SHUTDOWN_SVC);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SHUTDOWN_ID));
	FindMenuItem(m_MaintainMenus,SHUTDOWN_COMPUTER_ID)->text = _opl(TXT_ID_SHUTDOWN_COMPUTER);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,SHUTDOWN_COMPUTER_ID));
	FindMenuItem(m_MaintainMenus,RESTART_COMPUTTER_ID)->text = _opl(TXT_ID_RESTART_COMPUTER);
	MenuRecpItemChanged.Invoke(FindMenuItem(m_MaintainMenus,RESTART_COMPUTTER_ID));

	// 刷新状态栏
	theAPP_SESSION.GetChildView()->m_pStatusDlg->ReFlashStatusDlg();
}
