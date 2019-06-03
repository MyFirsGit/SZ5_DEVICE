#include "stdafx.h"
#include "MenuMgr.h"
#include "ExceptionMgr.h"
#include "ServiceMgr.h"
#include "functioninfo.h"
#include "TicketHelper.h"
#include "setting.h"

const static TCHAR* MENU_ITEM_EXCEPTION_ENABLE_KEY =_T("MENU_ITEM_EXCEPTION_ENABLE_KEY");
const static TCHAR* MENU_ITEM_FUNCTION_ENABLE_KEY =_T("MENU_ITEM_FUNCTION_ENABLE_KEY");
const static TCHAR* MENU_ITEM_PERMISSION_ENABLE_KEY = _T("MENU_ITEM_PERMISSION_ENABLE_KEY");
const static TCHAR* MENU_ITEM_CONFIG_ENABLE_KEY = _T("MENU_ITEM_CONFIG_ENABLE_KEY");
CMenuMgr CMenuMgr::instance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMenuMgr::CMenuMgr()
{	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMenuMgr::~CMenuMgr(void)
{	
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      ����һ���˵���

@param      UINT serviceItemID			����ť���
@param      CString serviceItemText	����ť��ʾ����
@param      SERVICE_ID serviceID	������
@param      Menu_Item* parentMenuItem	���˵���

@retval     Menu_Item* �˵���

@exception  ��
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
	bool isConfigAllow = theFunction_INFO.IsMenuEnabled(menuItem->btnID,menuItem->text);
	menuItem->bag->SetValue(MENU_ITEM_CONFIG_ENABLE_KEY,isConfigAllow);
	menuItem->isEnable = isConfigAllow;
	menuItem->isVisible = theFunction_INFO.IsMenuVisibled(menuItem->btnID,menuItem->text);
	if(parentMenuItem!=NULL)
	{
		menuItem->pParent = parentMenuItem;
		parentMenuItem->children.push_back(menuItem);
	}
	return menuItem;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ���˵���Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::Initialize(/*CMenuDlg* menuDlg*/)
{
	
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CMenuMgr::OnExceptionNotify);
	theSETTING.THModeChanged.AddHandler(this,&CMenuMgr::OnTHModeChanged);
	m_LoginMenus.push_back(BuildMenuItem(END_SVC_ID,TXT_END_SVC,VK_F1,END_SVC));
	m_LoginMenus.push_back(BuildMenuItem(SHUTDOWN_SVC_ID,TXT_SHUTDOWN_SVC,VK_F2,SHUTDOWN_SVC));
	m_LoginMenus.push_back(BuildMenuItem(REMOVE_INIT_EXCEPTION_SVC_ID,TXT_REMOVE_INIT_EXCEPTION_SVC,VK_F3,REMOVE_INIT_EXCEPTION_SVC));

	m_InitAbnormalMenus.push_back(BuildMenuItem(END_SVC_ID,TXT_END_SVC,VK_F1,END_SVC));
	m_InitAbnormalMenus.push_back(BuildMenuItem(SHUTDOWN_SVC_ID,TXT_SHUTDOWN_SVC,VK_F2,SHUTDOWN_SVC));
	m_InitAbnormalMenus.push_back(BuildMenuItem(REMOVE_INIT_EXCEPTION_SVC_ID,TXT_REMOVE_INIT_EXCEPTION_SVC,VK_F3,REMOVE_INIT_EXCEPTION_SVC));
	m_InitAbnormalMenus.push_back(BuildMenuItem(LOGIN_SVC_ID,TXT_LOGIN_SVC,VK_F4,LOGIN_SVC));
	//��Ʊ
	Menu_Item* adjustMenu = BuildMenuItem(ADJUST_MAIN_ID,TXT_ADJUST_SVC,VK_F2);
	BuildMenuItem(ADJUST_PAIDAREA_ID,TXT_ADJUST_SVC_PAIDAREA,0,ADJUST_SVC,adjustMenu);
	BuildMenuItem(ADJUST_NOPAIDAREA_ID,TXT_ADJUST_SVC_NOPAIDAREA,0,ADJUST_SVC,adjustMenu);
	m_CommonMenus.push_back(adjustMenu);
	//��ֵ
	m_CommonMenus.push_back(BuildMenuItem(RECHARGE_MAIN_ID,TXT_CHARGE_SVC,VK_F3,CHARGE_SVC));
	//�˿�
	Menu_Item* refundMenu = BuildMenuItem(REFUND_MAIN_ID,TXT_REFUND_MAIN,VK_F4);
	BuildMenuItem(REFUND_BASE_SVC_ID,TXT_REFUND_BASE_SVC,0,REFUND_BASE_SVC,refundMenu);
	BuildMenuItem(REFUND_APPLY_SVC_ID,TXT_REFUND_APPLY_SVC,0,REFUND_APPLY_SVC,refundMenu);
	BuildMenuItem(REFUND_QUERY_SVC_ID,TXT_REFUND_QUERY_SVC,0,REFUND_APPLY_SVC,refundMenu);
	m_CommonMenus.push_back(refundMenu);
	//�滻
	m_CommonMenus.push_back(BuildMenuItem(EXCHANGE_SVC_ID,TXT_EXCHANGE_SVC,0,EXCHANGE_SVC));
	//����
	//Menu_Item* refreshMainMenu = BuildMenuItem(REFRESH_MAIN_ID,TXT_DEFER_SVC,0);
	//BuildMenuItem(REFRESH_SVC_ID,TXT_REFRESH_SVC,0,REFRESH_SVC,refreshMainMenu);
	//BuildMenuItem(DEFER_SVC_ID,TXT_DEFER_SVC,0,DEFER_SVC,refreshMainMenu);
	//m_CommonMenus.push_back(refreshMainMenu);
	// ����
	m_CommonMenus.push_back(BuildMenuItem(DEFER_SVC_ID,TXT_DEFER_SVC,VK_F5,DEFER_SVC));
	//��ѯ
	m_CommonMenus.push_back(BuildMenuItem(ANALYZE_SVC_ID,TXT_ANALYZE_SVC,VK_F6,ANALYZE_SVC));
	//Ԥ�۵�����δ����
	Menu_Item* preSellCancelTicketMenu = BuildMenuItem(PRESELL_CANCEL_TICKET_MAIN_ID,TXT_PRESELL_CANCEL_TICKET);
	BuildMenuItem(PRESELL_TICKET_ID,TXT_PRESELL,0,ISSUE_SVC,preSellCancelTicketMenu);
	BuildMenuItem(COUNTERACT_SVC_ID,TXT_CANCEL_READ_TICKET_SVC,0,COUNTERACT_SVC,preSellCancelTicketMenu);
	m_CommonMenus.push_back(preSellCancelTicketMenu);
	//����
	Menu_Item* otherMenu = BuildMenuItem(OTHER_MAIN_ID,TXT_OTHERS_SVC,VK_F7);
	//TVM�����˿�
	BuildMenuItem(TVMFAILURE_REFUND_SVC_ID,TXT_TVMFAILURE_REFUND_SVC,0,TVMFAILURE_REFUND_SVC,otherMenu);
	//����������
	BuildMenuItem(SIGNCARD_APPLY_SVC_ID,TXT_SIGNCARD_APPLY_SVC,0,SIGNCARD_APPLY_SVC,otherMenu);
	//��ʧ���
	BuildMenuItem(CARD_SUSPEND_AND_RESUME_SVC_ID,TXT_CARD_SUSPEND_AND_RESUME_SVC,0,CARD_SUSPEND_AND_RESUME_SVC,otherMenu);
	//��������(��Ӫҵ��֧)
	BuildMenuItem(NOBUSINESS_INCOME_SVC_ID,TXT_ADMINISTRATION_PROCESS_SVC,0,NOBUSINESS_INCOME_SVC,otherMenu);
	//�˻�����
	BuildMenuItem(LOCK_SVC_ID,TXT_LOCK_SVC,0,LOCK_SVC,otherMenu);
	//����ͳ��
	BuildMenuItem(SALE_STATIC_SVC_ID,TXT_SALE_STATIC_SVC,0,SALE_STATIC_SVC,otherMenu);
	//��ӡ����
	BuildMenuItem(RECEIPT_PRINT_SVC_ID,TXT_RECEIPT_PRINT_SVC,0,RECEIPT_PRINT_SVC,otherMenu);
	m_CommonMenus.push_back(otherMenu);

	Menu_Item* assistMenu =BuildMenuItem(ASSIST_MAIN_ID,TXT_ASSIST_MAIN,VK_F8);
	//�쳣���
	BuildMenuItem(REMOVE_EXCEPTION_SVC_ID,TXT_REMOVE_EXCEPTION_SVC,0,REMOVE_EXCEPTION_SVC,assistMenu);
	//��Ʊģʽ�趨
	BuildMenuItem(TH_MODE_SVC_ID,TXT_TH_MODE_SET_SVC,0,TH_MODE_SET_SVC,assistMenu);
	//Ʊ�����
	BuildMenuItem(TICKETBOX_EXCHANGE_SVC_ID,TXT_TICKETBOX_EXCHANGE_SVC,0,TICKETBOX_EXCHANGE_SVC,assistMenu);
	//�ǳ�
	BuildMenuItem(LOGOFF_SVC_ID,TXT_LOGOFF_SVC,0,LOGOFF_SVC,assistMenu);
	m_CommonMenus.push_back(assistMenu);

	Menu_Item* systemSetMenu = BuildMenuItem(SYSTEM_SET_MAIN_ID,TXT_SYSTEM_SET_SVC,0);
	BuildMenuItem(STATION_SET_SVC_ID,TXT_STATION_SET_SVC,0,STATION_SET_SVC,systemSetMenu);
	BuildMenuItem(COMMUNICATION_SET_SVC_ID,TXT_COMMUNICATION_PARAMETER_SET_SVC,0,COMMUNICATION_SET_SVC,systemSetMenu);
	BuildMenuItem(MODE_SWITCH_SVC_ID,TXT_MODE_SWITCH_SVC,0,MODE_SWITCH_SVC,systemSetMenu);
	BuildMenuItem(SYSTEM_TIME_SET_SVC_ID,TXT_SYSTEM_TIME_SET_SVC,0,SYSTEM_TIME_SET_SVC,systemSetMenu);
	BuildMenuItem(RFID_MODE_SVC_ID,TXT_RFID_MODE_SET_SVC,0,RFID_MODE_SET_SVC,systemSetMenu);
	BuildMenuItem(UDSN_SET_SVC_ID,TXT_UDSN_SET_SVC,0,UDSN_SET_SVC,systemSetMenu);
	m_MaintainMenus.push_back(systemSetMenu);


	Menu_Item* dataManageMenu = BuildMenuItem(DATA_MANAGE_MAIN_ID,TXT_DATA_MANAGE_SVC,0);
	BuildMenuItem(DATA_INPUT_SVC_ID,TXT_DATA_INPUT_SVC,0,DATA_INPUT_SVC,dataManageMenu);
	BuildMenuItem(DATA_OUTPUT_SVC_ID,TXT_DATA_OUTPUT_SVC,0,DATA_OUTPUT_SVC,dataManageMenu);
	m_MaintainMenus.push_back(dataManageMenu);


	Menu_Item* deviceTestMenu = BuildMenuItem(DEVICE_TEST_SVC_ID,TXT_DEVICE_TEST_SVC,0);
	BuildMenuItem(OUTRW_TEST_SVC_ID,TXT_OUTRW_TEST_SVC,0,OUTRW_TEST_SVC,deviceTestMenu);
	BuildMenuItem(PRINTER_TEST_SVC_ID,TXT_PRINTER_TEST_SVC,0,PRINTER_TEST_SVC,deviceTestMenu);
	BuildMenuItem(NETWORK_TEST_SVC_ID,TXT_NETWORK_TEST_SVC,0,NETWORK_TEST_SVC,deviceTestMenu);
	BuildMenuItem(MONEYBOX_TEST_SVC_ID,TXT_MONEYBOX_TEST_SVC,0,MONEYBOX_TEST_SVC,deviceTestMenu);
	BuildMenuItem(UPS_INFO_SVC_ID,TXT_UPS_INFO_SVC,0,UPS_INFO_SVC,deviceTestMenu);
	BuildMenuItem(TH_TEST_SVC_ID,TXT_TH_TEST_SVC,0,TH_TEST_SVC,deviceTestMenu);
	m_MaintainMenus.push_back(deviceTestMenu);

	m_MaintainMenus.push_back(BuildMenuItem(REMOVE_EXCEPTION_SVC_ID,TXT_REMOVE_EXCEPTION_SVC,0,REMOVE_EXCEPTION_SVC));
	m_MaintainMenus.push_back(BuildMenuItem(LOGOFF_SVC_ID,TXT_LOGOFF_SVC,0,LOGOFF_SVC));
	m_MaintainMenus.push_back(BuildMenuItem(EXIT_BOM_SVC_ID,TXT_EXIT_BOM_SVC,0,EXIT_BOM_SVC));

	SetItemExceptionEnable();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ�����۲˵�

@param		none    

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::InitializeIssueMenu()
{
	Menu_Item* issueMenu = BuildMenuItem(ISSUE_MAIN_ID,TXT_ISSUE_SVC,VK_F1);
	m_CommonMenus.insert(m_CommonMenus.begin(), issueMenu);

	// ����ƱĬ�ϵ�һ��Ʊ�֣����ҳ�������˵�	
	typedef CAccTicketParam::TICKET_TYPE TICKET_TYPE_t;
	TICKET_TYPE_t ticketTypeTmp;
	vector<TICKET_TYPE_t>  vecTicketTypes =  theACC_TICKET.GetTicketTypeListParam();
	// ��������Ʊ��
	vector<TICKET_TYPE_t>::const_iterator it;
	for (it = vecTicketTypes.begin(); it != vecTicketTypes.end(); it++){
		ticketTypeTmp = *it;
		// Ʊ�����Ƹ�ʽ��
		CString ticketTypeName = ticketTypeTmp.ticketTypeName_cn;
		// �ҵ�����Ʊ
		if(ticketTypeTmp.ticketTypeCode == TICKET_SINGLE_RIDE){
			// ���ÿ���
			if(theFunction_INFO.IsTicketIssueEnabled(ticketTypeTmp.ticketTypeCode)){
				BuildMenuItem(ticketTypeTmp.ticketTypeCode,ticketTypeName,0,ISSUE_SVC,issueMenu);
			}
			// ɾ������Ʊ
			vecTicketTypes.erase(it);
			break;
		}
	}

	// ����Ʊ��
	for (unsigned int i = 0;i < vecTicketTypes.size(); i++){
		ticketTypeTmp = vecTicketTypes[i];
		// Ʊ�����Ƹ�ʽ��
		CString ticketTypeName = ticketTypeTmp.ticketTypeName_cn;
		//if(ticketTypeTmp.ticketTypeCode == TICKET_YKT_STORE_VALUE){
		//	ticketTypeName.Insert(6,'\n');
		//}
		if(theFunction_INFO.IsTicketIssueEnabled(ticketTypeTmp.ticketTypeCode)){
			BuildMenuItem(ticketTypeTmp.ticketTypeCode,ticketTypeName,0,ISSUE_SVC,issueMenu);
		}		
	}
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
	theSETTING.THModeChanged.RemoveHandler(this,&CMenuMgr::OnTHModeChanged);
	for(int i=0;i<m_LoginMenus.size();i++)
	{
		delete m_LoginMenus[i];
	}
	for(int i=0;i<m_InitAbnormalMenus.size();i++)
	{
		delete m_InitAbnormalMenus[i];
	}
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
@brief      �������˵�

@param      (i)MENU_KIND menuKind   ����
@param      (i)bool startDefault    �Ƿ�����Ĭ��ҵ��

@retval     ��

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
@brief      ��ȡ���˵����

@param      ��

@retval     MENU_KIND   �˵����

@exception  ��
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
	case MENU_INIT_ABNORMAL:
		return m_InitAbnormalMenus;
	case MENU_LOGIN:
		return m_LoginMenus;
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
	}
	//ASSERT(false);
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ò˵������쳣����»��Ƿ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CMenuMgr::SetItemExceptionEnable()
{
	//�Ƿ����쳣
	bool hasException = theEXCEPTION_MGR.HasException();

	//�Ƿ�ֻ��Ӳ���쳣
	bool onlyHasHardWardException = theEXCEPTION_MGR.OnlyHasHardwareException();

	vector<long> dependOnInnerModuleIDs;
	dependOnInnerModuleIDs.push_back(CTHHardwareException::MODULE_ID);
	dependOnInnerModuleIDs.push_back(CTHException::MODULE_ID);
	dependOnInnerModuleIDs.push_back(CTWHardwareException::MODULE_ID);
	dependOnInnerModuleIDs.push_back(CTWException::MODULE_ID);

	vector<long> dependOnOuterModuleIDs;
	dependOnOuterModuleIDs.push_back(CRWHardwareException::MODULE_ID);
	dependOnOuterModuleIDs.push_back(CRWException::MODULE_ID);

	//�ܷ�ʹ���ڲ���д����ҵ��
	bool canUseInnerTPU = true;//(theSETTING.GetTHMode() == TH_MODE_NORMAL) && ((!hasException) || (onlyHasHardWardException && !theEXCEPTION_MGR.HasException(dependOnInnerModuleIDs)));
	
	//�ܷ�ʹ���ⲿ��д����ҵ��
	bool canUseOuterTPU = true;//(!hasException) || (onlyHasHardWardException && !theEXCEPTION_MGR.HasException(dependOnOuterModuleIDs));

	Menu_Item* issueMenuItem = FindMenuItem(m_CommonMenus,ISSUE_MAIN_ID);
	if(issueMenuItem!=NULL)
	{
		for(vector<Menu_Item*>::iterator it = issueMenuItem->children.begin();it!=issueMenuItem->children.end();it++)
		{
			TicketType_t ticketType = (*it)->btnID;
			CTicketHelper::IssueWay issueWay = theTICKET_HELPER.GetIssueWay(ticketType);
			if(issueWay == CTicketHelper::BY_INNER_TPU){
				//�ڲ����۵�Ʊʹ���ڲ���д��
				SetItemAspectEnable(*it,MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseInnerTPU,false);
			}
			else if(issueWay == CTicketHelper::BY_OUTER_TPU || issueWay == CTicketHelper::BY_INNER_CONVERTTO_OUTER_TPU){
				//�ⲿ���۵�Ʊ���ڲ�ת�ⲿʹ���ⲿ��д��
				SetItemAspectEnable(*it,MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU,false);
			}
		}
		bool isExceptionAllowIssue = true;
		for(vector<Menu_Item*>::iterator it = issueMenuItem->children.begin();it!=issueMenuItem->children.end();it++)
		{
			(*it)->bag->GetValue(MENU_ITEM_EXCEPTION_ENABLE_KEY,isExceptionAllowIssue);
			if(isExceptionAllowIssue){
				break;
			}
		}
		SetItemAspectEnable(issueMenuItem,MENU_ITEM_EXCEPTION_ENABLE_KEY,true,false);
	}
	//��Ʊʹ���ⲿ��д��
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ADJUST_MAIN_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);	
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,RECHARGE_MAIN_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,REFUND_MAIN_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,EXCHANGE_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,REFRESH_MAIN_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,DEFER_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ANALYZE_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,OTHER_MAIN_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,true);
	Menu_Item* preIssueCancelMenuItem = FindMenuItem(m_CommonMenus,PRESELL_CANCEL_TICKET_MAIN_ID);
	SetItemAspectEnable(preIssueCancelMenuItem,MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseInnerTPU || canUseOuterTPU,false);
	if(preIssueCancelMenuItem!=NULL)
	{
		//Ԥ��ʹ���ڲ���д������
		SetItemAspectEnable(FindMenuItem(preIssueCancelMenuItem->children,PRESELL_TICKET_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseInnerTPU);
		//����ʹ���ⲿ��д����ҵ��
		SetItemAspectEnable(FindMenuItem(preIssueCancelMenuItem->children,COUNTERACT_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canUseOuterTPU);
	}

	Menu_Item* assistMenu = FindMenuItem(m_CommonMenus,ASSIST_MAIN_ID);
	bool canExchangeTicketBox = true;//theSETTING.GetTHMode() == TH_MODE_NORMAL && (!theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID));
	//Ʊ�����ʹ�õ�TH
	SetItemAspectEnable(FindMenuItem(assistMenu->children,TICKETBOX_EXCHANGE_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,canExchangeTicketBox);
	
	SetItemAspectEnable(FindMenuItem(m_InitAbnormalMenus,REMOVE_INIT_EXCEPTION_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,hasException);
	SetItemAspectEnable(FindMenuItem(m_LoginMenus,REMOVE_INIT_EXCEPTION_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,hasException);
	Menu_Item* assistantMenuItem = FindMenuItem(m_CommonMenus,ASSIST_MAIN_ID);
	SetItemAspectEnable(FindMenuItem(assistantMenuItem->children,REMOVE_INIT_EXCEPTION_SVC_ID),MENU_ITEM_EXCEPTION_ENABLE_KEY,hasException);
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
void CMenuMgr::SetWorkMode(WORK_MODE workMode )
{
	bool canIssue = workMode == ISSUE_MODE || workMode == ISSUE_ADJUST_MODE;
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ISSUE_MAIN_ID),MENU_ITEM_FUNCTION_ENABLE_KEY,canIssue);

	bool canAdjust = workMode == ADJUST_MODE || workMode == ISSUE_ADJUST_MODE;
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ADJUST_MAIN_ID),MENU_ITEM_FUNCTION_ENABLE_KEY,canAdjust);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����TH����ģʽ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuMgr::OnTHModeChanged(int&)
{
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
void CMenuMgr::SetUserPermission(DWORD userID)
{
	bool superUser = theBOM_INFO.IsFounderStaffID(userID);
	bool canIssue = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_ISSUE);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ISSUE_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canIssue);
	

	bool canCharge = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_CHARGE);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,RECHARGE_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canCharge);

	bool canAdjust = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_ADJUST);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ADJUST_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canAdjust);

	bool canRefund = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_REFUND);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,REFUND_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canRefund);

	bool canExchange = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_EXCHANGE);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,EXCHANGE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canExchange);

	bool canRefresh =  superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_REFRESH);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,REFRESH_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canRefresh);

	bool canAnalyze = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_ANALYZE);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,ANALYZE_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canAnalyze);

	bool canDoOther = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_OTHER);
	SetItemAspectEnable(FindMenuItem(m_CommonMenus,OTHER_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canDoOther);

	bool canSystemSet = superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_SYSTEM_SET);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,SYSTEM_SET_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canSystemSet);

	bool canDataManage= superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_DATA_MANAGE);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,DATA_MANAGE_MAIN_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canDataManage);

	bool canDeviceTest= superUser || theAFC_ACCESS_LEVEL.IsStaffHasRight(userID,SYS_FUNCTION_HARDWARE_TEST);
	SetItemAspectEnable(FindMenuItem(m_MaintainMenus,DEVICE_TEST_SVC_ID),MENU_ITEM_PERMISSION_ENABLE_KEY,canDeviceTest);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief   �쳣��Ϣ�б仯ʱ   

@param      ��

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CMenuMgr::OnExceptionNotify(CSysException& ,bool)
{
		SetItemExceptionEnable();
}
