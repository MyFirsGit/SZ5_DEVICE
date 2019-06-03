#include "stdafx.h"
#include "MenuDlg.h"
#include "MainSvc.h"
#include "ServiceMgr.h"
#include "bitmapmanager.h"
#include "exceptionmgr.h"
#include "InnerException.h"
#include "appsession.h"
#include "MessageIDDefinition.h"
#include "tvmforeservice.h"
#include "logoffhelper.h"
#include "MessageBox.h"
#include "IssueSvc.h"
#include "ChargeSvc.h"
#include "ProductIssueSvc.h"
#include <..\src\mfc\afximpl.h>
#include "menumgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief      菜单画面
*/

IMPLEMENT_DYNAMIC(CMenuDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CMenuDlg, CBaseDlg)
    ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_BTN_BACK_CLICK(OnCancel)
	ON_BTN_CHARGE_CLICK(ChargeMenuClick)
	ON_BTN_CASH_PAY_CLICK(CashPayMenuClisk)
	ON_BTN_NET_PAY_CLICK(NetPayMenuClish)
	ON_BTN_SELECT_LANGUAGE_CLICK(LanguageMenuClick)
	ON_GROUP_BUTTON_CLICK(MENU_GROUP_ID,MainMenuClick)
	ON_GROUP_BUTTON_CLICK(SECOND_MENU_GROUP_ID,LanguageMenuClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService  Dialog对应的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMenuDlg::CMenuDlg()
	:CBaseDlg(CMenuDlg::IDD,NULL, REC_MENU_MAIN_RECT , RECEPTION_HEAD_BK_COLOR,NULL,false)
{
	CString bmppath;
	CString apppath;
	GetAppPath(apppath);
	// 背景图片
	bmppath = apppath + TVM_IMAGE_STATUS_BKG;
	m_bkg.Load(bmppath); 
	//if(!m_bkg.IsNull()) ConvertPng(&m_bkg);
	m_imgBtnBkg.Load(apppath+_T(".\\res\\TVM_btn_Group_Blue_Normal.png"));
	m_imgBtnCancel.Load(apppath+_T(".\\res\\TVM_btn_Cancel_Default.png"));
	bmppath = apppath + TVM_IMAGE_MODULE_STATUS_RED;
	m_moduleStatusRed.Load(bmppath); 
	bmppath = apppath + TVM_IMAGE_MODULE_STATUS_GREEN;
	m_moduleStatusGreen.Load(bmppath); 
	bmppath = apppath + TVM_IMAGE_MODULE_STATUS_YELLOW;
	m_moduleStatusYellow.Load(bmppath); 
	bmppath = apppath + TVM_IMAGE_MODULE_STATUS_GRAY;
	m_moduleStatusGray.Load(bmppath); 
	
	m_MenuGroup = NULL;
	m_LanguageGroup = NULL;
	m_CancelBtn = NULL;
	m_btnLanguage = NULL;
	m_btnCharge = NULL;
	m_btnCashPay = NULL;
	m_btnNetPay = NULL;

	UIINFO menuGroupUIInfo;
	menuGroupUIInfo.m_pParentWnd = this;
	menuGroupUIInfo.m_DockStyle = DOCKSTYLE::NONE;
	menuGroupUIInfo.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;   
	menuGroupUIInfo.m_Size.cx = 0;//DEFAULT_SIZE_MENU_BUTTON.cx*SERVICE_MENU_BUTTON_COUNT;   
	menuGroupUIInfo.m_Size.cy = 0;//DEFAULT_SIZE_MENU_BUTTON.cy;
	menuGroupUIInfo.m_BorderWidth = 0;
	menuGroupUIInfo.m_BKMode = BKMODE_LAST;
	menuGroupUIInfo.m_BackColor = OPERATOR_HEAD_BK_COLOR;
	m_MenuGroup = new CButtonGroup(menuGroupUIInfo,1,SERVICE_MENU_BUTTON_COUNT,MENU_GROUP_ID,CButtonGroup::SIGLE,BUTTON_TYPE_SERVICE);

	UIINFO languageGroupUIInfo;
	languageGroupUIInfo.m_pParentWnd = this;
	languageGroupUIInfo.m_DockStyle = DOCKSTYLE::NONE;
	languageGroupUIInfo.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;   
	languageGroupUIInfo.m_Location.x = 10;//REC_MENU_MAIN_RECT.Width()-DEFAULT_SIZE_MENU_BUTTON.cx*LANGUAGE_MENU_BUTTON_COUNT;   
	languageGroupUIInfo.m_Size.cx = DEFAULT_SIZE_MENU_BUTTON.cx*LANGUAGE_MENU_BUTTON_COUNT;   
	languageGroupUIInfo.m_Size.cy = DEFAULT_SIZE_MENU_BUTTON.cy;
	languageGroupUIInfo.m_BorderWidth = 0;
	languageGroupUIInfo.m_BKMode = BKMODE_LAST;
	languageGroupUIInfo.m_BackColor = RECEPTION_HEAD_BK_COLOR;
	m_LanguageGroup = new CButtonGroup(languageGroupUIInfo,1,LANGUAGE_MENU_BUTTON_COUNT,SECOND_MENU_GROUP_ID,CButtonGroup::SIGLE,BUTTON_TYPE_LANGUAGE);


	theSERVICE_MGR.StartingService.AddHandler(this,&CMenuDlg::OnStartingService);
	theSERVICE_MGR.StartedService.AddHandler(this,&CMenuDlg::OnStartedService);
	theMENU_MGR.MenuKindChanged.AddHandler(this,&CMenuDlg::OnMenuKindChanged);
	theMENU_MGR.MenuRecpItemChanged.AddHandler(this,&CMenuDlg::OnMenuItemChanged);
	CXButton::Clicking.AddHandler(this,&CMenuDlg::OnButtonClicking);
	SetNotifyContent();
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
	theMENU_MGR.MenuRecpItemChanged.RemoveHandler(this,&CMenuDlg::OnMenuItemChanged);
	CXButton::Clicking.RemoveHandler(this,&CMenuDlg::OnButtonClicking);

    delete m_MenuGroup;
    m_MenuGroup = NULL;
	delete m_LanguageGroup;
	m_LanguageGroup = NULL;

	delete m_CancelBtn;
	m_CancelBtn = NULL;
	if (m_btnLanguage)
	{
		delete m_btnCharge;
		m_btnCharge = NULL;
	}
	if (m_btnLanguage)
	{
		delete m_btnLanguage;
		m_btnLanguage = NULL;
	}
	if (m_btnCashPay)
	{
		delete m_btnCashPay;
		m_btnCashPay = NULL;
	}
	if (m_btnNetPay)
	{
		delete m_btnNetPay;
		m_btnNetPay = NULL;
	}
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

		//// 语言选择按钮
		//UIINFO uilan;
		//uilan.m_pParentWnd = this;
		//uilan.m_DockStyle = DOCKSTYLE::NONE;
		//uilan.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;
		//uilan.m_Location.x = 40;
		//uilan.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		//uilan.m_Visible = true;
		//uilan.m_pBackgroundImage = &m_imgBtnBkg;
		//BTN_INFO btnInfo;
		//btnInfo.text = STR_CHINESE;
		//btnInfo.btnMsg = WM_BTN_SELECT_LANGUAGE;
		//btnInfo.isVisible = false;
		//btnInfo.type = BUTTON_TYPE_LANGUAGE;
		//m_btnLanguage = new CXButton(uilan,btnInfo);

		// 充值按钮
		UIINFO uiCharge;
		uiCharge.m_pParentWnd = this;
		uiCharge.m_DockStyle = DOCKSTYLE::NONE;
		uiCharge.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;
		uiCharge.m_Location.x = DEFAULT_SIZE_MENU_BUTTON.cx*LANGUAGE_MENU_BUTTON_COUNT+10;
		uiCharge.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		uiCharge.m_Visible = true;
		uiCharge.m_pBackgroundImage = &m_imgBtnBkg;
		BTN_INFO btnInfo;
		btnInfo.initialize();
		btnInfo.text = _tl(TXT_ID_CHARGE_SVC);;
		btnInfo.btnMsg = WM_BTN_CHARGE;;
		btnInfo.isVisible = false;
		btnInfo.type = BUTTON_TYPE_CHARGE;
		m_btnCharge = new CXButton(uiCharge,btnInfo);

		// 现金支付按钮
		UIINFO uiCash;
		uiCash.m_pParentWnd = this;
		uiCash.m_DockStyle = DOCKSTYLE::NONE;
		uiCash.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;
		uiCash.m_Location.x = 40;
		uiCash.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		uiCash.m_Visible = true;
		uiCash.m_pBackgroundImage = &m_imgBtnBkg;
		btnInfo.initialize();
		btnInfo.text = _T("现金支付");
		btnInfo.btnMsg = WM_BTN_CASH_PAY;
		btnInfo.isVisible = false;
		btnInfo.type = BUTTON_TYPE_NORMAL;
		m_btnCashPay = new CXButton(uiCash,btnInfo);

		// 扫码支付按钮
		UIINFO uiNet;
		uiNet.m_pParentWnd = this;
		uiNet.m_DockStyle = DOCKSTYLE::NONE;
		uiNet.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;
		uiNet.m_Location.x = 40 + DEFAULT_SIZE_MENU_BUTTON.cx+7;
		uiNet.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		uiNet.m_Visible = true;
		uiNet.m_pBackgroundImage = &m_imgBtnBkg;
		btnInfo.initialize();
		btnInfo.text = _T("扫码支付");
		btnInfo.btnMsg = WM_BTN_NET_PAY;
		btnInfo.isVisible = false;
		btnInfo.type = BUTTON_TYPE_NORMAL;
		m_btnNetPay = new CXButton(uiNet,btnInfo);

		// 取消按纽
		UIINFO ui;
		ui.m_pParentWnd = this;
		ui.m_DockStyle = DOCKSTYLE::NONE;
		ui.m_Location.y = (REC_MENU_MAIN_RECT.Height()-DEFAULT_SIZE_MENU_BUTTON.cy)/2;
		ui.m_Location.x = REC_MENU_MAIN_RECT.Width()-DEFAULT_SIZE_MENU_BUTTON.cx-45;
		ui.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		ui.m_Visible = true;
		ui.m_pBackgroundImage = &m_imgBtnCancel;
		btnInfo.initialize();
		btnInfo.text = _tl(STR_CANCEL);
		btnInfo.btnMsg = WM_BTN_BACK;
		btnInfo.isVisible = false;
		btnInfo.type = BUTTON_TYPE_OK;
		m_CancelBtn = new CXButton(ui,btnInfo);

		//m_MenuGroup->Create();
		m_LanguageGroup->Create();
		m_CancelBtn->Create();
		//m_btnLanguage->Create();
		m_btnCharge->Create();
		m_btnCashPay->Create();
		m_btnNetPay->Create();
		ShowServiceButtonGroup(false);
		ShowLanguageButtonGroup(false);
		ShowCancleButton(false);
		ShowChargeButton(false);
		ShowPayMethodButton(false,false);
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
	CRect rcClient;
	this->GetClientRect(&rcClient);
	// 绘制提示背景图片
	if (!m_bkg.IsNull()){
		m_bkg.Draw(pDC->m_hDC,rcClient);
	}

	if (m_nVerFrontSize == VERSION_FONT_SIZE)
	{
		rcClient.top  += 60;
		rcClient.left += 815;

	}
	else
	{
		rcClient.top  += 30;
		rcClient.left += 800;

	}
	//CString strVersion = _T("");
	//strVersion.Format(_T("%s\r\nTVM:V%d TPU:V%d ID:%s SC:%s"),theEXCEPTION_MGR.HasException() ? theEXCEPTION_MGR.GetDescription(theEXCEPTION_MGR.GetLastException()) : _T(""), 
	//	theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),
	//	theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CARD_RW_APP),
	//	theMAINTENANCE_INFO.GetCurrentDevice().ToString(),
	//	(theAPP_SESSION.IsSCConnected()?_T("已连接"):_T("未连接"))
	//	);
	DrawText(pDC,rcClient,m_strVersion,_T("黑体"),m_nVerFrontSize/*VERSION_FONT_SIZE*/,FW_NORMAL,WHITE,DT_LEFT|DT_VCENTER|DT_WORDBREAK);

}
void CMenuDlg::SetNotifyContent(CString szCont)
{
	if (szCont == _T(""))
	{
		m_strVersion.Format(_T("%s\r\nTVM:V%d TPU:V%d ID:%s SC:%s"),theEXCEPTION_MGR.HasException() ? theEXCEPTION_MGR.GetDescription(theEXCEPTION_MGR.GetLastException()) : _T(""), 
			theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),
			theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CARD_RW_APP),
			theMAINTENANCE_INFO.GetCurrentDevice().ToString(),
			(theAPP_SESSION.IsSCConnected()?_T("已连接"):_T("未连接"))
			);
		m_nVerFrontSize = VERSION_FONT_SIZE;
	}
	else
	{
		m_strVersion = szCont;
		m_nVerFrontSize = VERSION_FONT_SIZE+20;
	}
	this->Invalidate();
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

	//m_LanguageGroup->EnableWindow(busy?FALSE:TRUE);
	//m_LanguageGroup->Invalidate(FALSE);
	
	m_CancelBtn->EnableWindow(busy?FALSE:TRUE);
	m_CancelBtn->Invalidate(FALSE);

	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		this->UpdateWindow();
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
	if(newMenuKind == CMenuMgr::MENU_COMMON){
		// 初始化隐藏按钮，在启动默认服务时显示按钮。
		//ShowServiceButtonGroup(true);
		//ShowLanguageButtonGroup(true);
		//ShowCancleButton(false);
		//m_MenuGroup->ClickFirstAvailButton();		
	}
	else{
		ShowServiceButtonGroup(false);
		ShowLanguageButtonGroup(false);
		ShowCancleButton(false);
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
void CMenuDlg::OnMenuItemChanged(Menu_Item* menuItem)
{
	vector<BTN_INFO>& vecBtnInfo = m_MenuGroup->GetBtnInfo();

	for(vector<BTN_INFO>::iterator it = vecBtnInfo.begin();it!=vecBtnInfo.end();it++){
		if(it->btnID == menuItem->btnID && it->btnMsg == menuItem->btnMsg){
			it->isEnable = menuItem->isEnable;
			it->isVisible = menuItem->isVisible;
			it->text = menuItem->text;				
			if (m_CurrentMainMenuInfo != NULL&&menuItem->btnMsg == m_CurrentMainMenuInfo->btnMsg){
				m_MenuGroup->ClickButton(m_CurrentMainMenuInfo->btnMsg);
			}			
		}
	}
	if (m_MenuGroup->GetBtnInfo().size()>0){
		ShowServiceButtonGroup(true);
		ShowLanguageButtonGroup(true);
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
		m_CurrentMainMenuInfo = pBtnInfo;
		SERVICE_ID serviceID=pMenuItem->serviceID;	
		// 判断当前如果是充值业务，是否支持充值业务？
		if (CHARGE_SVC == serviceID){
			// 显示当前不支持充值业务
			if (!IsSupportRecharge()){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_RECHARGE_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				// 设置按钮为未选中状态
				m_MenuGroup->ClearSelection();
				return TRUE;
			}
			if (theTVM_STATUS_MGR.GetPrintMode() != 0){
				INT_PTR res = CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_NOPRINT_CONTINUE),NULL,CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);

				if(IDCANCEL == res || IDABORT == res){
					theSERVICE_MGR.SetState(READY);
					// 设置按钮为未选中状态
					m_MenuGroup->ClearSelection();
					return TRUE;
				}		
			}
		}
		if ( serviceID == ANALYZE_SVC)			// 查询业务是否可用
		{
			if(!IsAnalyzeSupported()){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_PSG_ATTENTION)*/,_tl(TXT_PSG_NO_ANALYZE_SVC),NULL,CMessagePictureBox::BTN_OK, DT_CENTER ,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				m_MenuGroup->ClearSelection();
				return TRUE;
			}			
			// 盲人购票中....
			if(theAPP_SESSION.IsBlindManOperation()){
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->OnClickButtonWithoutPinPad(true);
			}
		}
		if (serviceID == ISSUE_PRODUCT_SVC)// 添加产品是否可用
		{
			if(!IsIssueProductSupported()){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_PSG_ATTENTION)*/,_tl(TXT_PSG_NO_ISSU_PRODUCT_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				m_MenuGroup->ClearSelection();
				return TRUE;
			}
			if (theTVM_STATUS_MGR.GetPrintMode() != 0){
				INT_PTR res = CMessagePictureBox::Show(_tl(TXT_PSG_INFO_LOADED_NEW_PRODUCT),_tl(TXT_NOPRINT_CONTINUE),_T(""),CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);
				if(IDCANCEL == res || IDABORT == res){
					theSERVICE_MGR.SetState(READY);
					// 设置按钮为未选中状态
					m_MenuGroup->ClearSelection();
					return TRUE;
				}		
			}
			// 盲人购票中....
			if(theAPP_SESSION.IsBlindManOperation()){
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->OnClickButtonWithoutPinPad(true);
			}
		}
		DoService(serviceID);
	}
    return TRUE;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      语言菜单按下后的响应函数

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMenuDlg::LanguageMenuClick(WPARAM wParam,LPARAM lParam)
{
 	BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
	LANGUAGETYPE_T selectLanguage = (LANGUAGETYPE_T)pBtnInfo->btnID;
// 	if (theMAINTENANCE_INFO.GetLanguageMode() == LANGUAGETYPE_T::ENGLISH)
// 	{
// 		selectLanguage = LANGUAGETYPE_T::CHINESE;
// 	}
// 	else
// 	{
// 		selectLanguage = LANGUAGETYPE_T::ENGLISH;
// 	}
	// 保存设置到session
	theAPP_SESSION.SetCurrentClientLanguageType(selectLanguage);

	return TRUE;
}
// 充值按钮按下
LRESULT CMenuDlg::ChargeMenuClick(WPARAM wParam, LPARAM lParam)
{
	//// 显示当前不支持充值业务
	//if (!theAPP_SESSION.IsRechargeServerEnabled()
	//	|| theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
	//	|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
	//	|| (theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
	//		CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_RECHARGE_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
	//		theSERVICE_MGR.SetState(READY);
	//		// 设置按钮为未选中状态
	//		m_btnCharge->SetSelect(false);
	//		return TRUE;
	//}
	//if (theTVM_STATUS_MGR.GetPrintMode() != 0){
	//	INT_PTR res = CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_NOPRINT_CONTINUE),NULL,CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);
	//	if(IDCANCEL == res || IDABORT == res){
	//		theSERVICE_MGR.SetState(READY);
	//		// 设置按钮为未选中状态
	//		m_btnCharge->SetSelect(false);
	//		return TRUE;
	//	}		
	//}
	theSERVICE_MGR.StartService(SERVICE_ID::CHARGE_SVC);
	return TRUE;
}
LRESULT CMenuDlg::CashPayMenuClisk(WPARAM wParam, LPARAM lParam)
{
	m_btnNetPay->EnableWindow(TRUE);
	m_btnCashPay->EnableWindow(FALSE);
	theSERVICE_MGR.GetService<CIssueSvc>(ISSUE_SVC)->StartReceiveMoney();
	return TRUE;
}
LRESULT CMenuDlg::NetPayMenuClish(WPARAM wParam, LPARAM lParam)
{
	m_btnCashPay->EnableWindow(TRUE);
	m_btnNetPay->EnableWindow(FALSE);
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
	// 启动Service
	if(serviceID!=UNKNOWN_SVC){
		bool cancel = false;
		 theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartingForeService(serviceID,&cancel);
		if(!cancel)
		{
			theSERVICE_MGR.StartService(serviceID);
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
		//??
		CIssueSvc* pIssueSvc = dynamic_cast<CIssueSvc*>(pSvc);
		int issueTicketCategory = 0;
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
	const	vector<BTN_INFO>& selectedBtnInfo=m_MenuGroup->GetSelectedBtnInfo();
	if(selectedBtnInfo.size() > 0)
	{
		const BTN_INFO* firstAvailBtnInfo = m_MenuGroup->GetFirstAvailBtnInfo();
		if(firstAvailBtnInfo != NULL)
		{
			const BTN_INFO& btnInfo = selectedBtnInfo[0];
			if(btnInfo.btnID == firstAvailBtnInfo->btnID && btnInfo.btnMsg == firstAvailBtnInfo->btnMsg)
			{
				theSERVICE_MGR.SetState(READY);
				// 恢复语言设置为系统默认配置
				// ??theAPP_SESSION.SetCurrentClientLanguageType(theMAINTENANCE_INFO.GetLanguageMode());
			}
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
void  CMenuDlg::OnButtonClicking(CXButton* pButton,bool* cancel)
{
	//
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
@brief      设置语言菜单项

@param      bool isShow

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::ShowLanguageButtonGroup(bool isShow)
{
	LANGUAGETYPE_T currentClientLanguagetype = theAPP_SESSION.GetCurrentClientLanguageType();
	vector<BTN_INFO> vec_Btn_info;
	BTN_INFO btnInfo;
	if (isShow){
		if(currentClientLanguagetype == LANGUAGE_ID_ENGLISH){
			BTN_INFO btnInfo;
			btnInfo.btnID = LANGUAGE_ID_PORTUGUESE;
			btnInfo.text = STR_PORTUGUESE;
			btnInfo.btnMsg = STR_PORTUGUESE;;
			btnInfo.isVisible = true;
			btnInfo.type = BUTTON_TYPE_LANGUAGE;
			vec_Btn_info.push_back(btnInfo);

			btnInfo.initialize();
			btnInfo.btnID = LANGUAGE_ID_CHINESE;
			btnInfo.text = STR_CHINESE;
			btnInfo.btnMsg = STR_CHINESE;
			btnInfo.isVisible = true;
			btnInfo.type = BUTTON_TYPE_LANGUAGE;
			vec_Btn_info.push_back(btnInfo);

		}
		else if(currentClientLanguagetype == LANGUAGE_ID_PORTUGUESE){
			BTN_INFO btnInfo;
			btnInfo.btnID = LANGUAGE_ID_CHINESE;
			btnInfo.text = STR_CHINESE;
			btnInfo.btnMsg = STR_CHINESE;
			btnInfo.isVisible = true;
			btnInfo.type = BUTTON_TYPE_LANGUAGE;
			vec_Btn_info.push_back(btnInfo);
			btnInfo.initialize();
			btnInfo.btnID = LANGUAGE_ID_ENGLISH;
			btnInfo.text = STR_ENGLISH;
			btnInfo.btnMsg = STR_ENGLISH;
			btnInfo.isVisible = true;
			btnInfo.type = BUTTON_TYPE_LANGUAGE;
			vec_Btn_info.push_back(btnInfo);
		}
		else{		
			BTN_INFO btnInfo;
			btnInfo.btnID = LANGUAGE_ID_PORTUGUESE;
			btnInfo.text = STR_PORTUGUESE;
			btnInfo.btnMsg = STR_PORTUGUESE;;
			btnInfo.isVisible = true;
			btnInfo.type = BUTTON_TYPE_LANGUAGE;
			vec_Btn_info.push_back(btnInfo);

			btnInfo.initialize();
			btnInfo.btnID = LANGUAGE_ID_ENGLISH;
			btnInfo.text = STR_ENGLISH;
			btnInfo.btnMsg = STR_ENGLISH;
			btnInfo.isVisible = true;
			btnInfo.type = BUTTON_TYPE_LANGUAGE;
			vec_Btn_info.push_back(btnInfo);

		}
	}
	m_LanguageGroup->SetButtonInfos(vec_Btn_info);
	//if (isShow){
	//	if(currentClientLanguagetype == LANGUAGE_ID_ENGLISH){
	//		m_btnLanguage->SetText(_T("中文"));
	//	}
	//	else{		
	//		m_btnLanguage->SetText(_T("English"));
	//	}
	//}
	//m_btnLanguage->SetVisible(isShow);
	//m_btnLanguage->EnableWindow(isShow);

	this->Invalidate();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      控制取消按纽的显示属性

@param      (i)

@retval     LRESULT  0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void  CMenuDlg::ShowCancleButton(bool isShow)
{
	m_CancelBtn->SetVisible(isShow);
	m_CancelBtn->EnableWindow(isShow);
	this->Invalidate();
}
void CMenuDlg::ShowPayMethodButton(bool showNet,bool showCash)
{
	m_btnCashPay->SetVisible(showCash);
	m_btnCashPay->EnableWindow(showCash);
	m_btnNetPay->SetVisible(showNet);
	m_btnNetPay->EnableWindow(showNet);
	this->Invalidate();
}

void CMenuDlg::SetCancleButtonEnable(bool isEnable)
{
	//m_CancelBtn->SetVisible(isShow);
	m_CancelBtn->EnableWindow(isEnable);
	this->Invalidate();

}

void CMenuDlg::ShowChargeButton(bool isShow)
{
	m_btnCharge->SetVisible(isShow);
	m_btnCharge->EnableWindow(isShow);
	this->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      控制取消按纽的显示属性

@param      (i)

@retval     LRESULT  0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void  CMenuDlg::ShowServiceButtonGroup(bool isShow)
{
	isShow = false;
	if (isShow){
		SetMainMenuItems(theMENU_MGR.GetCommonMenuItems());
	}
	else{
		vector<Menu_Item*> emptySubMenuItem;
		SetMainMenuItems(emptySubMenuItem);
	}
	this->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理取消按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CMenuDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	CTVMForeService* currentForeService = (CTVMForeService*)theSERVICE_MGR.GetCurService();
	if(currentForeService!=NULL){
		CReceptionDlg* currentreceptionDlg = currentForeService->GetCurReceptionDlg();
		if(currentreceptionDlg!=NULL){
			return currentreceptionDlg->CallWndProc(WM_XBUTTON,wParam,lParam);
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		判断当前业务是否支持充值  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CMenuDlg::IsSupportRecharge(){
	// 无异常，且当前模式支持充值
	if (!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID) && 
		!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)){
		if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		判断是否支持分析（查询）业务  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CMenuDlg::IsAnalyzeSupported()
{
	if (!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID))
	{
			/*if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_NO_COIN_QUERY) == DEVICE_CONTRL_CODE_NO_COIN_QUERY)
			{
				return true;
			}*/
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		判断是否支持售产品业务

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CMenuDlg::IsIssueProductSupported()
{
	if (!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID) && !theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID))
	{
		/*if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_PRODUCT) == DEVICE_CONTRL_CODE_BUSINESS_PRODUCT)
		{
			return true;
		}*/
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		刷新取消按钮的文字

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::RefreshCancelButton()
{
	m_CancelBtn->SetText(_tl(STR_CANCEL));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取消按钮是否可用

@param      无

@retval     bool true：可用 false：不可用

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMenuDlg::IsCancleButtonEnable(){
	return /*m_CancelBtn->IsEnabled()*/ m_CancelBtn->IsVisible();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      透明处理

@param      CImage* image

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMenuDlg::ConvertPng(CImage* image)
{
	for(int i = 0; i < image->GetWidth(); i++){  
		for(int j = 0; j < image->GetHeight(); j++){  
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(image->GetPixelAddress(i , j));  
			pucColor[0] = pucColor[0] * pucColor[3] / 255;  
			pucColor[1] = pucColor[1] * pucColor[3] / 255;  
			pucColor[2] = pucColor[2] * pucColor[3] / 255;  
		}  
	}  
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置背景颜色(禁止绘制底色)
//
//@param      (i)CDC* pDC
//
//@retval     BOOL    TRUE:禁止绘制底色; FALSE:允许绘制底色
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
BOOL CMenuDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bret=TRUE;;
	COLORREF brush_color = BLUE;

	if (theTVM_STATUS_MGR.GetServiceStatus()==IN_SERVICE){
		brush_color = WHITE;
	}

	CBrush bkBrush(brush_color);      // 定义背景色刷子
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &bkBrush); // 用背景色画一个矩形

	return TRUE;
}