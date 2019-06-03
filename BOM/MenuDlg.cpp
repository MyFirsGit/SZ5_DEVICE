#include "stdafx.h"
#include "MenuDlg.h"
#include "MainSvc.h"
////#include "LogoffHelper.h"
#include "ServiceMgr.h"
#include "bitmapmanager.h"
#include "exceptionmgr.h"
#include "InnerException.h"
#include "appsession.h"
#include "MessageIDDefinition.h"
#include "bomforeservice.h"
#include "logoffhelper.h"
#include "MessageBox.h"
#include "IssueSvc.h"
#include "ChargeSvc.h"
#include "AdjustSvc.h"
#include "menumgr.h"
#include "RefundApplySvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/**
@brief      菜单画面
*/

IMPLEMENT_DYNAMIC(CMenuDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CMenuDlg, COperationDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(MENU_GROUP_ID,MainMenuClick)
	ON_GROUP_BUTTON_CLICK(SECOND_MENU_GROUP_ID,SecondMenuClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService  Dialog对应的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMenuDlg::CMenuDlg(CService* pService)
	:COperationDlg(CMenuDlg::IDD, MENU_MAIN_RECT , RECEPTION_HEAD_BK_COLOR, pService,false)
{
	UIINFO menuGroupUIInfo;
	menuGroupUIInfo.m_pParentWnd = this;
	menuGroupUIInfo.m_DockStyle = DOCKSTYLE::TOP;
	menuGroupUIInfo.m_Size.cy = MENU_MAIN_RECT.Height()/2;
	menuGroupUIInfo.m_BorderWidth = 0;
	menuGroupUIInfo.m_BKMode = BKMODE_LAST;
	menuGroupUIInfo.m_BackColor = RECEPTION_HEAD_BK_COLOR;
	m_MenuGroup = new CButtonGroup(menuGroupUIInfo,1,MENU_BUTTON_COUNT,MENU_GROUP_ID,CButtonGroup::SIGLE);

	UIINFO secondMenuGroupUIInfo;
	secondMenuGroupUIInfo.m_pParentWnd = this;
	secondMenuGroupUIInfo.m_DockStyle = DOCKSTYLE::BOTTOM;
	secondMenuGroupUIInfo.m_Size.cy = MENU_MAIN_RECT.Height()/2;
	secondMenuGroupUIInfo.m_BorderWidth = 0;
	secondMenuGroupUIInfo.m_BKMode = BKMODE_LAST;
	secondMenuGroupUIInfo.m_BackColor = RECEPTION_HEAD_BK_COLOR;
	m_SecondMenuGroup = new CButtonGroup(secondMenuGroupUIInfo,1,MENU_BUTTON_COUNT,SECOND_MENU_GROUP_ID,CButtonGroup::SIGLE);

	theSERVICE_MGR.StartingService.AddHandler(this,&CMenuDlg::OnStartingService);
	theSERVICE_MGR.StartedService.AddHandler(this,&CMenuDlg::OnStartedService);
	theMENU_MGR.MenuKindChanged.AddHandler(this,&CMenuDlg::OnMenuKindChanged);
	theMENU_MGR.MenuItemStatusChanged.AddHandler(this,&CMenuDlg::OnMenuItemStatusChanged);
	CXButton::Clicking.AddHandler(this,&CMenuDlg::OnButtonClicking);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMenuDlg::~CMenuDlg()
{
	theSERVICE_MGR.StartingService.RemoveHandler(this,&CMenuDlg::OnStartingService);
	theSERVICE_MGR.StartedService.RemoveHandler(this,&CMenuDlg::OnStartedService);
	theMENU_MGR.MenuKindChanged.RemoveHandler(this,&CMenuDlg::OnMenuKindChanged);
	theMENU_MGR.MenuItemStatusChanged.RemoveHandler(this,&CMenuDlg::OnMenuItemStatusChanged);
	CXButton::Clicking.RemoveHandler(this,&CMenuDlg::OnButtonClicking);

    delete m_MenuGroup;
    m_MenuGroup = NULL;
	delete m_SecondMenuGroup;
	m_SecondMenuGroup = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CMenuDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try {
        if (__super::OnCreate(lpCreateStruct) == -1) {
            return -1;
        }
		m_MenuGroup->Create();
		m_SecondMenuGroup->Create();
    }
    catch (CSysException& e) {
		
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CMenuDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::OnServiceBusy(bool busy)
{
	m_MenuGroup->EnableWindow(busy?FALSE:TRUE);
	m_MenuGroup->Invalidate(FALSE);
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		m_MenuGroup->UpdateWindow();
	}
	m_SecondMenuGroup->EnableWindow(busy?FALSE:TRUE);
	m_SecondMenuGroup->Invalidate(FALSE);
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		m_SecondMenuGroup->UpdateWindow();
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
void CMenuDlg::OnMenuKindChanged(CMenuMgr::MENU_KIND& oldMenuKind,CMenuMgr::MENU_KIND& newMenuKind)
{
	SetMainMenuItems(theMENU_MGR.GetCurrentMenuItems());
	if(newMenuKind == CMenuMgr::MENU_LOGIN || newMenuKind == CMenuMgr::MENU_INIT_ABNORMAL)
	{
		vector<Menu_Item*> emptySubMenuItem;
		SetSecondMenuItems(emptySubMenuItem);
	}
	else
	{
		m_MenuGroup->ClickFirstAvailButton();
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
void CMenuDlg::OnMenuItemStatusChanged(Menu_Item* menuItem)
{
	vector<BTN_INFO>& vecBtnInfo = menuItem->pParent == NULL ? m_MenuGroup->GetBtnInfo() : m_SecondMenuGroup->GetBtnInfo();
	for(vector<BTN_INFO>::iterator it = vecBtnInfo.begin();it!=vecBtnInfo.end();it++)
	{
		if(it->btnID == menuItem->btnID && it->btnMsg == menuItem->btnMsg)
		{
			it->isEnable = menuItem->isEnable;
			it->isVisible = menuItem->isVisible;
			if(menuItem->pParent == NULL)
			{
				m_MenuGroup->Invalidate(TRUE);
			}
			else
			{
				m_SecondMenuGroup->Invalidate(TRUE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得主菜单按钮组

@param      

@retval     CButtonGroup* 主菜单按钮组

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CButtonGroup* CMenuDlg::GetMainGroup()
{
	return m_MenuGroup;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得二级菜单按钮组

@param      

@retval     CButtonGroup* 二级菜单按钮组

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CButtonGroup* CMenuDlg::GetSecondGroup()
{
	return m_SecondMenuGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      主菜单按下后的响应函数

 @param      (i)WPARAM wParam  消息信息
 @param      (i)LPARAM lParam  消息信息

 @retval     LRESULT  \n
             TRUE:处理成功  FALSE:处理失败

 @exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMenuDlg::MainMenuClick(WPARAM wParam,LPARAM lParam)
{
    BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
	Menu_Item* pMenuItem=theMENU_MGR.GetMainMenuItemByID((MENU_ITEM_ID)pBtnInfo->btnID);
	if(pMenuItem!=NULL)
	{
		SERVICE_ID serviceID=pMenuItem->serviceID;
		DoService(serviceID);
		m_CurrentMainMenuInfo = pBtnInfo;
		SetSecondMenuItems(pMenuItem->children);
	}
    return TRUE;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      二级菜单按下后的响应函数

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMenuDlg::SecondMenuClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
	Menu_Item* pSecondeMenuItem=theMENU_MGR.GetSecondMenuItemByID((MENU_ITEM_ID)m_CurrentMainMenuInfo->btnID,(MENU_ITEM_ID)pBtnInfo->btnID);
	if(pSecondeMenuItem!=NULL)
	{
		SERVICE_ID serviceID=pSecondeMenuItem->serviceID;
		DoService(serviceID);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CMenuDlg::DoService(SERVICE_ID& serviceID)
{
	CMainSvc* pSvc = GetService<CMainSvc>();
	switch (serviceID) {
		case LOGOFF_SVC:    // 登出
			{
				CLogoffHelper logoffHelper(*pSvc);
				logoffHelper.Logoff();
			}
			break;
		case END_SVC:       // 业务结束
			pSvc->CloseOperation();
			break;            

		case SHUTDOWN_SVC:  // 关机
			pSvc->Shutdown();
			break;

		case REMOVE_INIT_EXCEPTION_SVC:               // 初期化异常解除
			pSvc->RepairInitException();
			break;
		case EXIT_BOM_SVC:
			pSvc->ExitBOM();
			break;
		case RECEIPT_PRINT_SVC://打印最后一笔充值单据
			{
				pSvc->GetPRTHelper()->PrintLastChargeReceipt();
			}
			break;
		default:
			// 启动Service
			if(serviceID!=UNKNOWN_SVC){
				bool cancel = false;
				pSvc->StartingForeService(serviceID,&cancel);
				if(!cancel)
				{
					theSERVICE_MGR.StartService(serviceID);
				}
			}
	}
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      启动服务前的响应函数

@param      CService* pSvc 将要启动的服务

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::OnStartingService(CService* pSvc)
{
	// 售票
	if(pSvc->GetServiceID() == ISSUE_SVC)
	{
		const vector<BTN_INFO>& selectedBtnInfo=m_SecondMenuGroup->GetSelectedBtnInfo();
		CIssueSvc* pIssueSvc = dynamic_cast<CIssueSvc*>(pSvc);
		TicketType_t ticketType = selectedBtnInfo[0].btnID;
		if(selectedBtnInfo[0].text== TXT_PRESELL)
		{
			ticketType = TICKET_SINGLE_RIDE;
			pIssueSvc->SetIsPreSell(true);
		}
		pIssueSvc->SetTicketType(ticketType);
	}
	else if(pSvc->GetServiceID()==ADJUST_SVC)
	{
		const vector<BTN_INFO>& selectedBtnInfo=m_SecondMenuGroup->GetSelectedBtnInfo();
		CAdjustSvc* pAdjustSvc = dynamic_cast<CAdjustSvc*>(pSvc);
		pAdjustSvc->SetIsPaidArea(selectedBtnInfo[0].btnID== ADJUST_PAIDAREA_ID);
	}
	else if(pSvc->GetServiceID() == REFUND_APPLY_SVC)
	{
		const vector<BTN_INFO>& selectedBtnInfo=m_SecondMenuGroup->GetSelectedBtnInfo();
		CRefundApplySvc* pAdjustSvc = dynamic_cast<CRefundApplySvc*>(pSvc);
		pAdjustSvc->SetIsQuery(selectedBtnInfo[0].btnID== REFUND_QUERY_SVC_ID);
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
void CMenuDlg::OnStartedService(CService* pSvc)
{
	SERVICE_STATE currentState = theSERVICE_MGR.GetState();
	if(currentState == INIT || currentState == OUT_OF || currentState == MAINTENANCE)
	{
		return;
	}
	const	vector<BTN_INFO>& secondSelectedBtnInfo=m_SecondMenuGroup->GetSelectedBtnInfo();
	const	vector<BTN_INFO>& mainSelectedBtnInfo=m_MenuGroup->GetSelectedBtnInfo();
	bool isMainMenuClickDefautlButton = false;
	bool isStartDefaultSevices = false;
	if(mainSelectedBtnInfo.size() > 0)
	{
		const BTN_INFO* firstAvailBtnInfo = m_MenuGroup->GetFirstAvailBtnInfo();
		if(firstAvailBtnInfo != NULL)
		{
			const BTN_INFO& btnInfo = mainSelectedBtnInfo[0];
			if(btnInfo.btnID == firstAvailBtnInfo->btnID && btnInfo.btnMsg == firstAvailBtnInfo->btnMsg)
			{
				//theSERVICE_MGR.SetState(READY);
				isMainMenuClickDefautlButton = true;
			}
		}
	}
	if(isMainMenuClickDefautlButton)
	{
		if(secondSelectedBtnInfo.size() > 0)
		{
			const BTN_INFO* firstAvailBtnInfo = m_SecondMenuGroup->GetFirstAvailBtnInfo();
			if(firstAvailBtnInfo != NULL)
			{
				const BTN_INFO& btnInfo = secondSelectedBtnInfo[0];
				if(btnInfo.btnID == firstAvailBtnInfo->btnID && btnInfo.btnMsg == firstAvailBtnInfo->btnMsg)
				{
					isStartDefaultSevices = true;
				}
			}
		}
		else
		{
			isStartDefaultSevices = true;
		}
	}
	if(isStartDefaultSevices)
	{
		theSERVICE_MGR.SetState(READY);
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
void  CMenuDlg::OnButtonClicking(CXButton* pButton,bool* cancel)
{

	CWnd* pButtonParent =  pButton->GetParent() ;
	while(pButtonParent != NULL && (pButtonParent!=this))
	{
		pButtonParent = pButtonParent->GetParent();
	}
	if(pButtonParent == NULL)
	{
		return;
	}
	MENU_ITEM_ID menuItemID =  (MENU_ITEM_ID)pButton->GetID();
	MENU_ITEM_ID confirmMenuItems[] = {LOGOFF_SVC_ID,END_SVC_ID,SHUTDOWN_SVC_ID,EXIT_BOM_SVC_ID};
	if(!CValidation::IsInRange(menuItemID,confirmMenuItems))
	{
		return;
	}
	SERVICE_ID serviceID = UNKNOWN_SVC;
	if(menuItemID == LOGOFF_SVC_ID) serviceID = LOGOFF_SVC;
	if(menuItemID == END_SVC_ID) serviceID = END_SVC;
	if(menuItemID == SHUTDOWN_SVC_ID) serviceID = SHUTDOWN_SVC;
	if(menuItemID == EXIT_BOM_SVC_ID) serviceID = EXIT_BOM_SVC;
	CString sTital = theAPP_SESSION.GetServiceNameCH(serviceID); 
	CString message = sTital + _T(",确认?");
	CWnd* current = GetFocus();
	INT_PTR ret = CMessageBox::Show(sTital, message, CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30,CMessageBox::BTN_OK);
	if (IDCANCEL == ret) {
		*cancel = true;
		if(current!=NULL)
		{
			current->SetFocus();
		}
	}
		
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     设置主菜单项内容 

@param     vector<Menu_Item*>& vecMenuItem 主菜单项内容 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::SetMainMenuItems(vector<Menu_Item*>& vecMenuItem)
{
	m_CurrentMainMenuInfo = NULL;
	vector<BTN_INFO> vecButtonInfo;
	for(int i=0;i<vecMenuItem.size();i++)
	{
		if(vecMenuItem[i]->isVisible)
		{
			vecButtonInfo.push_back(*(vecMenuItem[i]));
		}
	}
	m_MenuGroup->SetButtonInfos(vecButtonInfo);
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置二级菜单项

@param      vector<Menu_Item*>& vecMenuItem 二级菜单项

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::SetSecondMenuItems(vector<Menu_Item*>& vecMenuItem)
{
	vector<BTN_INFO> vecButtonInfo;
	for(int i=0;i<vecMenuItem.size();i++)
	{
		if(vecMenuItem[i]->isVisible)
		{
			vecButtonInfo.push_back(*(vecMenuItem[i]));
		}
	}
	m_SecondMenuGroup->SetButtonInfos(vecButtonInfo);
	m_SecondMenuGroup->ClickFirstAvailButton();
}



