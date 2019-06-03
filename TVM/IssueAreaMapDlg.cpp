#include "stdafx.h"
#include "IssueAreaMapDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CIssueAreaMapDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueAreaMapDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_XLINEGROUP, XLineGroupButtonClick)			// 线路图按钮
	ON_GROUP_BUTTON_CLICK(TOP_1_GROUP_ID,OnLineGroupClick)		// 线路选择按钮
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPriceTypeClick)	// 票价按钮
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnTicketNumClick)	// 购票数量按钮
	ON_GROUP_BUTTON_CLICK(RIGHT_3_GROUP_ID,OnTopStationClick)	// 票价按钮
	ON_GROUP_BUTTON_CLICK(RIGHT_4_GROUP_ID,OnFunctionClick)		// 票价按钮	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueAreaMapDlg::CIssueAreaMapDlg(CService* pService)
: CReceptionDlg(CIssueAreaMapDlg::IDD, pService)
{
	m_pTipsDlg = new CStatic;
	m_pTitleDlg = new CStatic;

	m_pAllLineDlg = new CAllLineShowBox(CAllLineShowBox::IDD, pService, CRect(0,0,REC_LINE_MAP_RECT.Width(),REC_LINE_MAP_RECT.Height()), false);
	pSvc = (CIssueSvc*)pService;
	m_lineGroupArea = NULL;	//初始化设置为NULL

	// 线路选择按钮
	m_BTN_Top_1_Group_Info.rows = 1;
	m_BTN_Top_1_Group_Info.cols = 6;
	m_BTN_Left_1_Group_Info.rows = 9;
	m_BTN_Left_1_Group_Info.cols = 1;
	m_BTN_Left_1_Group_Info.title = _T("");
	//m_BTN_Left_2_Group_Info.rows = 4;
	//m_BTN_Left_2_Group_Info.cols = 2;

	// 右侧票价按钮
	m_BTN_Right_1_Group_Info.rows = 7;
	m_BTN_Right_1_Group_Info.cols = 1;
	m_BTN_Right_1_Group_Info.title = _T("");

	m_dwMaxTicketAmount = theFunction_INFO.GetTicketMaxAmount();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CIssueAreaMapDlg::~CIssueAreaMapDlg()
{
	if (m_lineGroupArea != NULL)
	{
		delete m_lineGroupArea;
		m_lineGroupArea = NULL;
	}

	if (m_pAllLineDlg != NULL)
	{
		delete m_pAllLineDlg;
		m_pAllLineDlg = NULL;
	}

	if (m_pTipsDlg != NULL)
	{
		delete m_pTipsDlg;
		m_pTipsDlg = NULL;
	}

	if (m_pTitleDlg != NULL)
	{
		delete m_pTitleDlg;
		m_pTitleDlg = NULL;
	}	
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:创建成功    -1:创建失败

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
int CIssueAreaMapDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct,CASH_COUNT_AREA | LINE_AREA | PRICES_AREA | NUMBER_AREA | TOP_STATION_AREA | FUNC_AREA /*|BASE_AREA*/ | CASH_AREA | BTN_OK | BTN_OK_AND_CANCEL1 | BTN_OK_AND_CANCEL2) == -1) {
		return -1;
	}

	m_pTipsDlg->Create(_T(""), WS_CHILD, CRect(1140+50,850+30,1350+50,890+30), this, ID_TIPS_DLG);
	m_pTitleDlg->Create(_T(""), WS_CHILD, CRect(690,40,1000,100), this, ID_TITLE_DLG);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(true);
	m_bShowBaseArea = false;
	m_strNotifyInfo = _T("");
	m_strTipInfo = _T("");
	//HideCompoment(BASE_AREA);
	HideCompoment(CASH_AREA);
	HideCompoment(BTN_OK);
	HideCompoment(BTN_OK_AND_CANCEL1);
	HideCompoment(BTN_OK_AND_CANCEL2);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);

	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_AREA_MAP){
		theSERVICE_MGR.SetState(READY);
		// 隐藏信息界面
		m_bShowBaseArea = false;
		m_pTipsDlg->ShowWindow(SW_HIDE);
		m_pTitleDlg->ShowWindow(SW_HIDE);
		//HideCompoment(BASE_AREA);
		HideCompoment(CASH_AREA);
		HideCompoment(BTN_OK);
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);
		if(m_lineGroupArea!=NULL) m_lineGroupArea->ShowWindow(SW_SHOW);
		// 启用按钮组
		m_Left_1_Group->EnableWindow(TRUE);
		m_Right_1_Group->EnableWindow(TRUE);
		m_Left_1_Group->ClearSelection();
		m_Right_1_Group->ClearSelection();
		//m_Left_1_Group->Invalidate(TRUE);
		//m_Left_1_Group->UpdateWindow();
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(true);
		m_cashInInfo->SetContent(_T("0.0元"));
		CString szCont = _T("请选择目的车站或票价购票\r\n充值请点击左侧[充值]按钮");
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(szCont);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowPayMethodButton(false,false);

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示之前

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::preShow(){
	__super::preShow();

	LOG("CIssueAreaMapDlg::preShow--- m_bServiceNewStart = %d", pSvc->m_bServiceNewStart?1:0);
	m_pAllLineDlg->End(0);
	if (pSvc->m_bServiceNewStart){
		pSvc->m_bServiceNewStart = false;
		InitializeLineGroup();
		// 加载左、右侧功能区，主要判断是否支持发售单程票
		long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
		InitializeFunctionBtns(component);
		if (m_Top_1_Group != NULL)
		{
			m_Top_1_Group->ClickFirstAvailButton();
		}
		//if(m_Left_1_Group != NULL){
		//	m_Left_1_Group->ClickFirstAvailButton();
		//}

		theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_NORMAL);
		////theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_HIDE);
		//// 显示功能区按钮，语言选择按钮，隐藏取消按钮
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
		//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面显示后

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::showComplete(){
	__super::showComplete();
	this->RedrawWindow();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		语言选择按钮变化时响应函数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::OnClientLanguageModeChanged(LANGUAGETYPE_T& langue)
{	
	// 设置为等待中...
	theSERVICE_MGR.SetState(READY);
	// 初始化区域按钮
	InitializeLineGroup();

	// 加载左、右侧功能区，主要判断是否支持发售单程票
	long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
	m_Left_1_Group->ShowButtons(false);//这个很重要
	InitializeFunctionBtns(component);

	//if(m_Left_1_Group != NULL){
	//	m_Left_1_Group->ClickFirstAvailButton();
	//}
	if (m_Top_1_Group != NULL)
	{
		m_Top_1_Group->ClickFirstAvailButton();
	}

	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_NORMAL);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      热点车站按钮点击响应事件

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)  按钮通知消息为车站代码

@retval     

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnTopStationClick(WPARAM wParam, LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	// 当前站不响应
	if(_ttoi(btnInfo->btnMsg) == theMAINTENANCE_INFO.GetStationCodeWithLine()){
		theSERVICE_MGR.SetState(READY);
		return TRUE;
	}	

	m_wCurrClickStation = _ttoi(btnInfo->btnMsg);//注意，这里的站点编码与别的地方不同
	// 判断是否支持发售单程票
	if(!IsSupportIssueCard())
	{
		return FALSE;
	}

	pSvc->GetDataModel().productInfo.dtValidateDateTime = ComGetCurTime();
	pSvc->SetProductType(SJT);					// 产品类型：一定是单程票
	pSvc->SetSubProductType(0);					// 产品子类型：SJT
	pSvc->SetBeginStation();						// 起始车站：一定是当前车站
	pSvc->SetEndStation(_ttoi(btnInfo->btnMsg));	// 目的车站：

	if(pSvc->CalculatePrice()==RW_RSP_NOT_AVALIABLE_STATION){// 车票单价：
		CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_NOT_SALE_TO_THIS_STATION),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		return FALSE;
	}
	pSvc->SetTicketPiece(1);						// 默认设置发售一张	
	pSvc->DoneToNextDialog();						// 转到支付界面

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      线路面板按钮点击响应事件

@param      WPARAM wParam groupID

@retval     LPARAM lParam(BTN_INFO*)  按钮通知消息：跳转页ID或车站代码LocationCode

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::XLineGroupButtonClick(WPARAM wParam, LPARAM lParam)
{	// 设置乘客操作中
	theSERVICE_MGR.SetState(RECEPT);
	//pLocationDlg->ShowWindow(SW_HIDE);
	// 执行正常发售操作
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD layoutID = _ttoi(btnInfo->btnMsg);
	// 当前站不响应
	if(btnInfo->btnID == theMAINTENANCE_INFO.GetStationCodeWithLine()){
		theSERVICE_MGR.SetState(READY);
		return TRUE;
	}	
	
	// 车站按钮
	if (layoutID == 0)
	{
		m_wCurrClickStation = btnInfo->btnID;
		// 判断是否支持售票
		if(!IsSupportIssueCard())
		{
			//m_Left_1_Group->ClickFirstAvailButton();
			if (m_Top_1_Group != NULL)
			{
				m_Top_1_Group->ClickFirstAvailButton();
			}

			return FALSE;
		}

		DWORD locationNum = 0;
			
		locationNum = btnInfo->btnID;
		pSvc->GetDataModel().productInfo.dtValidateDateTime = ComGetCurTime();
		pSvc->SetProductType(SJT);										// 产品类型：一定是单程票
		pSvc->SetSubProductType(0);										// 产品子类型：设为默认0xFF
		pSvc->SetBeginStation();											// 起始车站：一定是当前车站
		pSvc->SetEndStation(locationNum);									// 目的车站：
		if(pSvc->CalculatePrice()==RW_RSP_NOT_AVALIABLE_STATION){// 车票单价：
			CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_NOT_SALE_TO_THIS_STATION),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
			return FALSE;
		}
		pSvc->SetTicketPiece(1);											// 默认设置发售一张	
		pSvc->DoneToNextDialog();											// 转到支付界面
		
	}
	// 跳转按钮
	else
	{
		if (m_currentLayoutID == layoutID || !InitializeLineGroup(layoutID))
		{
			return TRUE;
		}

		//InitializeFunctionBtns(TOP_STATION_BUTTON);lichao:屏蔽热点车站

		// 保留旧的布局 -> 创建新的布局 -> 赋值新的布局 -> 删除旧的布局
		if(NULL != m_lineGroupArea){
			CLineGroup* oldLayout = m_lineGroupArea;
			CLineGroup* newLayout = new CLineGroup(m_LineAreaUIInfo);
			if(NULL != newLayout){
				newLayout->BuildButton(m_vec_BtnUIInfo,m_vec_buttonInfos,m_vec_textInfos);
				m_lineGroupArea = newLayout;
				delete oldLayout;
				oldLayout = NULL;
			}
		}
		else{
			m_lineGroupArea =new CLineGroup(m_LineAreaUIInfo);
			m_lineGroupArea->BuildButton(m_vec_BtnUIInfo,m_vec_buttonInfos,m_vec_textInfos);
		}
		
		if(m_currentLayoutID == m_dwCurPageLayout){
			// 使窗口无效，需要重新绘制
			m_Right_1_Group->Invalidate();
			m_Left_1_Group->Invalidate();
			//m_Right_2_Group->UpdateWindow();
			theSERVICE_MGR.SetState(READY);
			theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_NORMAL);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
			// 显示功能区按钮，语言选择按钮，隐藏取消按钮

			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
			//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);//lichao: 暂时屏蔽语言切换按钮
		}
		else{
			m_Right_1_Group->Invalidate();
			m_Left_1_Group->Invalidate();
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		判断是否支持售Token  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueAreaMapDlg::IsSupportIssueCard(){
	// 1.判断当前模式是否支持售卡？
	WORD device_service_mode = theTVM_STATUS_MGR.GetWorkMode();
	// 1.1判断是否支持售卡
	if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) != DEVICE_CONTRL_CODE_BUSINESS_CARD){		
		// 1.2提示没有卡可售
		CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_SORRY_NO_CARD_AVALIABLE),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		// 1.2清空点击的热点车站选择状态
		m_Right_1_Group->ClearSelection();
		m_Left_1_Group->ClearSelection();
		// 1.3这时界面不跳转，还是处于默认业务，设置状态为等待状态
		theSERVICE_MGR.SetState(READY);
		return false;
	}
	// 2.售票限制命令
	if(theISSUE_PERMIT.IsPermitIssueStation(m_wCurrClickStation) && m_wCurrClickStation != 0){
		// 1.2提示没有卡可售
		CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_NOT_SALE_TO_THIS_STATION),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		// 1.2清空点击的热点车站选择状态
		m_Right_1_Group->ClearSelection();
		m_Left_1_Group->ClearSelection();
		// 1.3这时界面不跳转，还是处于默认业务，设置状态为等待状态
		theSERVICE_MGR.SetState(READY);
		return false;
	}

	// 2.判断是否支持售Token
	/*if (!pSvc->SupportSJT()){
	return false;
	}*/
	// 3.判断Token数量是否支持
	//CTTokenCountInfo::TOKEN_BOX_INFO Token_Box_A = theTOKEN_COUNT.GetHopperAInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO Token_Box_B = theTOKEN_COUNT.GetHopperBInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO Token_Box_Wast = theTOKEN_COUNT.GetTokenboxWastedInfo();

	//CAfcTVMOperationParam::MAGAZINE_PARAM magazine_Box;
	//theAFC_TVM_OPERATION.GetMagazineParam(magazine_Box);
	//if (Token_Box_A.ulCurCount < magazine_Box.TokenAEmptyValue && Token_Box_B.ulCurCount < magazine_Box.TokenBEmptyValue )
	//{
	//	if(!theAPP_SESSION.IsBlindManOperation()){
	//		// 3.1提示没有token可售
	//		CMessagePictureBox::Show(_T("")/*_tl(TXT_ISSUE_INFO)*/,_tl(TXT_GUIDE_SORRY_NO_TOKEN_AVALIABLE),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
	//		// 3.2清空点击的热点车站选择状态
	//		m_Right_2_Group->ClearSelection();
	//		// 3.3这时界面不跳转，还是处于默认业务，设置状态为等待状态
	//		theSERVICE_MGR.SetState(READY);
	//	}
	//	else{
	//		// 当前不支持此票种
	//		pSvc->PlayAudioInDlg(AUDIO_COMMON_MSG_NOT_SUPPORT_TICKET_TYPE);
	//	}
	//	return false;
	//}
	// 4.提示是否支持打印？
	if(false/*theTVM_STATUS_MGR.GetPrintMode() != 0*/){
		if(!theAPP_SESSION.IsBlindManOperation()){
			INT_PTR res = CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_NOPRINT_CONTINUE)/*_T("当前不支持打印，是否继续？")*/,NULL,CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);

			if(IDCANCEL == res || IDABORT == res){
				m_Right_1_Group->ClearSelection();
				m_Left_1_Group->ClearSelection();
				theSERVICE_MGR.SetState(READY);
				return false;
			}
		}			
	}

	// 5.售票限制命令
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      票价选择按钮

@param      WPARAM wParam;
			LPARAM lParam;

@retval     LRESULT TRUE,FALSE

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnPriceTypeClick(WPARAM wParam,LPARAM lParam){

	// 判断当前是否支持单程票
	BTN_INFO* btnInfo = (BTN_INFO*)lParam; 
	// 判断是否支持售票
	if(!IsSupportIssueCard())
	{
		return FALSE;
	}
	pSvc->GetDataModel().productInfo.dtValidateDateTime = ComGetCurTime();
	pSvc->SetProductType(SJT);						// 产品类型：一定是单程票
	pSvc->SetSubProductType(0);						// 产品子类型：SJT
	pSvc->SetBeginStation();						// 起始车站：一定是当前车站
	//pSvc->SetEndStation(_ttoi(btnInfo->btnMsg));	// 目的车站
	pSvc->SetTicketAmount(_ttoi(btnInfo->btnMsg));	// 设置单价
	pSvc->SetTicketPiece(pSvc->GetDataModel().productInfo.nProductPiece >0?pSvc->GetDataModel().productInfo.nProductPiece:1);						// 默认设置发售一张
	pSvc->DoneToNextDialog();						// 转到支付界面

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      线路选择按钮点击处理

@param      WPARAM wParam;
			LPARAM lParam;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnLineGroupClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btn = (BTN_INFO*)lParam;
	// 检查按钮点击事件
	DWORD dwLineId = 0;
	if(NULL != btn){
		dwLineId = _ttoi(btn->btnMsg);
		if (dwLineId==0){//地图还原
			theSERVICE_MGR.SetState(READY);
			long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
			InitializeFunctionBtns(component);
			//m_Left_1_Group->Invalidate(TRUE);
			//m_Left_1_Group->UpdateWindow();
		}
		else if(dwLineId==255){//全部线路
			m_Left_1_Group->EnableWindow(FALSE);
			m_Right_1_Group->EnableWindow(FALSE);
			Invalidate(TRUE);
			INT_PTR retValue = m_pAllLineDlg->Show();
			m_Left_1_Group->EnableWindow(TRUE);
			m_Right_1_Group->EnableWindow(TRUE);
			m_Left_1_Group->Invalidate(TRUE);
			m_Left_1_Group->UpdateWindow();
			Invalidate(TRUE);
			if(retValue==0){
				return TRUE;			
			}
			dwLineId = (DWORD)retValue;
			// 选中对应线路按钮
			CString strLineId;
			strLineId.Format(_T("%d"), dwLineId);
			m_Left_1_Group->ClearSelection();
			m_Left_1_Group->SetSelection(strLineId);
		}

		// 适配到线路
		if(InitializeLineGroup(dwLineId)){
			m_dwCurPageLayout = dwLineId;
			CLineGroup* oldLayout = m_lineGroupArea;
			CLineGroup* newLayout = new CLineGroup(m_LineAreaUIInfo);
			if(NULL != newLayout){
				newLayout->BuildButton(m_vec_BtnUIInfo,m_vec_buttonInfos,m_vec_textInfos);
				m_lineGroupArea = newLayout;
				m_lineGroupArea->Create(CLineGroup::IDD,this);
				m_lineGroupArea->ShowWindow(SW_SHOW);
			}
			if (NULL != oldLayout){
				delete oldLayout;
				oldLayout = NULL;
			}
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      无

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnFunctionClick(WPARAM wParam,LPARAM lParam){

	BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
	Menu_Item* pMenuItem=theMENU_MGR.GetMainMenuItemByID((MENU_ITEM_ID)pBtnInfo->btnID);
	if(pMenuItem!=NULL)
	{
		SERVICE_ID serviceID=pMenuItem->serviceID;	
		// 判断当前如果是充值业务，是否支持充值业务？
		if (CHARGE_SVC == serviceID){
			// 显示当前不支持充值业务
			if (!theAPP_SESSION.IsRechargeServerEnabled()
				|| theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
				|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
				|| (theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_RECHARGE_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				// 设置按钮为未选中状态
				return TRUE;
			}
			if (theTVM_STATUS_MGR.GetPrintMode() != 0){
				INT_PTR res = CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_NOPRINT_CONTINUE),NULL,CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);
				if(IDCANCEL == res || IDABORT == res){
					theSERVICE_MGR.SetState(READY);
					// 设置按钮为未选中状态
					return TRUE;
				}		
			}
		}
		else if (ANALYZE_SVC == serviceID){
			// 显示当前不支持业务
			if (theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
				|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
				|| (theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_QUERY) != DEVICE_CONTRL_CODE_BUSINESS_QUERY){
					CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_ANALYZE_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
					theSERVICE_MGR.SetState(READY);
					// 设置按钮为未选中状态
					return TRUE;
			}
		}
		else if ( serviceID == ISSUE_USE_SVT_SVC){			// 查询储值卡购票业务是否可用
			// 判断当前是否支持充值业务
			if (theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
				|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
				|| (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_SVT) != DEVICE_CONTRL_CODE_PAYMENT_SVT){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_SVT_ISSUE)/*_tl(TXT_PSG_NO_RECHARGE_SVC)*/,NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				// 设置按钮为未选中状态
				//m_Right_4_Group->ClearSelection();
			}
			else{
				if(pSvc->m_ePayType != CIssueSvc::PAY_WITH_SVT){
					pSvc->m_ePayType = CIssueSvc::PAY_WITH_SVT;
					//m_Right_2_Group->ClickFirstAvailButton();
				}
				else{
					pSvc->m_ePayType = CIssueSvc::PAY_WITH_NORMAL;
					// 设置按钮为未选中状态
					//m_Right_4_Group->ClearSelection();
				}
			}

			return TRUE;
		}
		else if ( serviceID == LANGUAGE_SWITH_SVC){//语言切换
			LANGUAGETYPE_T curLanguageId = theAPP_SESSION.GetCurrentClientLanguageType();
			if (curLanguageId==LANGUAGE_ID_CHINESE){
				// 保存设置到session
				theAPP_SESSION.SetCurrentClientLanguageType(LANGUAGE_ID_ENGLISH);
			}
			else{
				// 保存设置到session
				theAPP_SESSION.SetCurrentClientLanguageType(LANGUAGE_ID_CHINESE);
			}
			// 设置按钮为未选中状态
			//m_Right_4_Group->ClearSelection();
			return TRUE;
		}

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
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      车票数量选择按钮

@param      WPARAM wParam;
			LPARAM lParam;

@retval     LRESULT TRUE,FALSE

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnTicketNumClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	pSvc->SetTicketPiece(_ttoi(btnInfo->btnMsg));
	pSvc->NotifyDataModelChanged();
	//// 重置定时器
	//pSvc->ResetMoneyNeedTimer();

	return TRUE;
}


//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      语言切换
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//LRESULT CIssueAreaMapDlg::OnLanguageClick(WPARAM wParam,LPARAM lParam){
//
//	BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
//	LANGUAGETYPE_T selectLanguage = (LANGUAGETYPE_T)pBtnInfo->btnID;
//
//	// 保存设置到session
//	theAPP_SESSION.SetCurrentClientLanguageType(selectLanguage);
//	return TRUE;
//}
