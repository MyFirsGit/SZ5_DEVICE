#include "StdAfx.h"
#include "ReceptionDlg.h"
#include "appsession.h"
#include "InnerException.h"
#include "exceptionmgr.h"
#include "BitmapManager.h"
#include "ButtonGroup.h"
#include <windef.h>
#include "ParseStationMap.h"
#include "PassengerForeSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CReceptionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CReceptionDlg, CBaseDlg)
	ON_WM_ACTIVATE()	
	//ON_MESSAGE(WM_XBUTTON, XButtonClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//@brief      构造函数

//@param      (i)UINT ID             对话框ID
//@param      (i)CService* pService  对话框使用的Service

//@retval     无

//@exception  无

//////////////////////////////////////////////////////////////////////////
CReceptionDlg::CReceptionDlg(UINT ID, CService* pService,CRect rect,bool needFunctionArea)
	: CBaseDlg(ID, theAPP_SESSION.GetChildViewRec(), rect, GRAY, pService,false)
{
	this->m_needFunctionArea = needFunctionArea;
	Initialize();
}

//////////////////////////////////////////////////////////////////////////

//@brief      构造函数
//
//@param      (i)UINT ID             对话框ID
//@param      (i)CRect position       位置大小
//@param      (i)COLORREF bkColor     背景色
//@param      (i)CService* pService   对话框使用的Service
//
//@retval     无
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
CReceptionDlg::CReceptionDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea)
	: CBaseDlg(ID, theAPP_SESSION.GetChildViewRec(), position, bkColor, pService,false)
{
	this->m_needFunctionArea = needFunctionArea;
	Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      析构函数

 @param      无

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg::~CReceptionDlg(void)
{
	if (NULL != m_pImageBack_top)
	{
		delete m_pImageBack_top;
		m_pImageBack_top = NULL;
	}
	if (NULL != m_pImageBack_left)
	{
		delete m_pImageBack_left;
		m_pImageBack_left = NULL;
	}
	if (NULL != m_pImageBack_right)
	{
		delete m_pImageBack_right;
		m_pImageBack_right = NULL;
	}
	if (NULL!=m_btnOK){
		delete m_btnOK;
		m_btnOK = NULL;
	}
	if (NULL!=m_btnOKAndCancel1){
		delete m_btnOKAndCancel1;
		m_btnOKAndCancel1 = NULL;
	}
	if (NULL!=m_btnOKAndCancel2){
		delete m_btnOKAndCancel2;
		m_btnOKAndCancel2 = NULL;
	}
	if (NULL!=m_btnPrint){
		delete m_btnPrint;
		m_btnPrint = NULL;
	}
	if (NULL!=m_btnNOPrint){
		delete m_btnNOPrint;
		m_btnNOPrint = NULL;
	}
	// 基本信息与现金信息对象
	if (NULL!=m_baseInfoArea){
		delete m_baseInfoArea;
		m_baseInfoArea = NULL;
	}
	if (NULL!=m_cashInfoArea){
		delete m_cashInfoArea;
		m_cashInfoArea = NULL;
	}
	if (NULL!=m_guideInfoArea){
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
	if (NULL!=m_titleInfoArea){
		delete m_titleInfoArea;
		m_titleInfoArea = NULL;
	}

	if (NULL!=m_baseInfo){
		delete m_baseInfo;
		m_baseInfo = NULL;
	}
	if (NULL!=m_cashInfo){
		delete m_cashInfo;
		m_cashInfo = NULL;
	}
	if (NULL!=m_guideInfo){
		delete m_guideInfo;
		m_guideInfo = NULL;
	}
	if (NULL!=m_titleInfo){
		delete m_titleInfo;
		m_titleInfo = NULL;
	}

	// 删除按钮组
	if (m_Top_1_Group != NULL)
	{
		delete m_Top_1_Group;
		m_Top_1_Group = NULL;
	}
	if(m_Left_1_Group != NULL){
		delete m_Left_1_Group;
		m_Left_1_Group = NULL;
	}

	if (m_Right_1_Group != NULL){
		delete m_Right_1_Group;
		m_Right_1_Group = NULL;
	}
	
	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.RemoveHandler(this,&CReceptionDlg::HideAllCompoment);
}

//////////////////////////////////////////////////////////////////////////

//@brief      初期化
//
//@param      无
//
//@retval     无
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::Initialize()
{
	m_baseInfoArea = NULL;
	m_cashInfoArea = NULL;
	m_guideInfoArea = NULL;
	m_titleInfoArea = NULL;
	m_cashInInfo = NULL;
	m_Right_1_Group = NULL;
	m_Top_1_Group = NULL;
	m_Left_1_Group = NULL;
	//基本信息
	m_baseInfo = new GSTATIC_INFO;
	m_baseInfo->point = BASEINFO_AREA_POINT;
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->titleInfo.color = ORANGE;
	m_baseInfo->detailInfo.color = BLACK;
	m_baseInfo->detailInfo.labelGroup = m_TipsInfoGroup;
	m_baseInfo->detailInfo.row = 13;
	m_baseInfo->detailInfo.editGroup.cnt = 0;                   // 编辑框个数
	m_baseInfo->detailInfo.editGroup.isLeftAlign = true;
	//现金现金信息
	m_cashInfo = new GSTATIC_INFO;
	m_cashInfo->point = CASHINFO_AREA_POINT;
	m_cashInfo->stytle = PASSAGE_NARROW_STYLE;
	m_cashInfo->titleInfo.titleName = _tl(TXT_CASH_INFO);
	m_cashInfo->titleInfo.color = ORANGE;
	m_cashInfo->detailInfo.color = BLACK;
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup/*m_cashLabelGroup*/;
	m_cashInfo->detailInfo.row = 4;
	m_cashInfo->detailInfo.editGroup.cnt = 0;                   // 编辑框个数
	m_cashInfo->detailInfo.editGroup.isLeftAlign = true;

	m_cashLabelGroup[0].nameLeft = add_hsp(_tl(TXT_PAYMENT_METHOD)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);  // 支付方式
	m_cashLabelGroup[0].nameRight = add_hsp(_tl(TXT_UNDEFINE),MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[1].nameLeft = add_hsp(_tl(TXT_TOTAL)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);  // 应付金额
	m_cashLabelGroup[1].nameRight = add_hsp(_tl(MONEY_UNIT),MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[2].nameLeft = add_hsp(_tl(TXT_INCOME)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN); // 实收金额
	m_cashLabelGroup[2].nameRight = add_hsp(_tl(MONEY_UNIT),MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[3].nameLeft = add_hsp(_tl(TXT_CHANGE)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN); // 找零
	m_cashLabelGroup[3].nameRight = add_hsp(_tl(MONEY_UNIT),MAX_CASH_RIGHT_LEN);
	
	//提示信息区域
	CString szPath = _T("");
	GetAppPath(szPath);
	//按钮组背景图
	m_pImageBack_left = NULL;
	m_pImageBack_left = new CImage();
	m_pImageBack_left->Load(szPath +_T(".\\res\\TVM_left_back.png"));

	m_pImageBack_right = NULL;
	m_pImageBack_right = new CImage();
	m_pImageBack_right->Load(szPath + _T(".\\res\\TVM_right_back.png"));
	
	m_pImageBack_top = NULL;
	m_pImageBack_top = new CImage;
	m_pImageBack_top->Load(szPath + _T(".\\res\\TVM_top_back.png"));

	m_guideInfo = new IMAGE_GROUP_INFO;

	//操作按钮
	m_btnOK = BuildOperationButton(REC_BTN_OK_RECT, _tl(STR_OK),WM_BTN_OK,BUTTON_TYPE_OK);
	m_btnOKAndCancel1 = BuildOperationButton(REC_BTN_OkAndCancel1_RECT, _tl(STR_OK),WM_BTN_OkAndCancel1,BUTTON_TYPE_OK);
	m_btnOKAndCancel2 = BuildOperationButton(REC_BTN_OkAndCancel2_RECT, _tl(STR_OK),WM_BTN_OkAndCancel2,BUTTON_TYPE_OK);

	m_btnPrint = BuildOperationButton(REC_BTN_PRINT_RECT,_tl(STR_PRINT),WM_BTN_PRINT,BUTTON_TYPE_PRINT);
	m_btnNOPrint = BuildOperationButton(REC_BTN_CANCELPRINT_RECT,_tl(STR_PRINT),WM_BTN_NOPRINT,BUTTON_TYPE_PRINT);

	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.AddHandler(this,&CReceptionDlg::HideAllCompoment);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::BuideButtonGroup(ULONG ulCompomentFlag)
{	
	// 按钮组定义
	UIINFO groupUI;
	groupUI.m_pParentWnd = this;
	groupUI.m_DockStyle = DOCKSTYLE::NONE;
	groupUI.m_BKMode = BKMODE_LAST;
	groupUI.m_BorderWidth = 0;
	groupUI.m_Margins=DEFAULT_MARGINS;
	groupUI.m_Paddings = DEFAULT_PADDINGS;
	groupUI.m_BackColor = OPERATOR_FUNC_BK_COLOR/*RECEPTION_HEAD_BK_COLOR*/;
	UIINFO btnUIInfo=UIINFO();

	int rows = 0,cols = 0;

	SIZE size;
	size.cx = 0;
	size.cy = REC_FRMAE_RECT.top;

	CString apppath;
	GetAppPath(apppath);
	// 线路选择
	if(ulCompomentFlag & LINE_AREA){
		rows = m_BTN_Top_1_Group_Info.rows;
		cols = m_BTN_Top_1_Group_Info.cols;
		groupUI.m_Location = REC_AREA_TOP_RECT.TopLeft() - size;
		groupUI.m_Size	   = REC_AREA_TOP_RECT.Size();
		groupUI.m_Visible  = m_BTN_Top_1_Group_Info.bVisible;
		MARGINS buttonMagins={0,0,0,0};
		groupUI.m_Margins =  buttonMagins;
		groupUI.m_pBackgroundImage = m_pImageBack_top;
		UIINFO btnLine = btnUIInfo;
		btnLine.m_DockStyle = TOPBOTTOM;
		btnLine.m_Location = CPoint(btnLine.m_Location.x,btnLine.m_Location.y+25);
		//btnLine.m_Size = CSize(btnLine.m_Size.cx- 170,btnLine.m_Size.cy);
		m_Top_1_Group = new CButtonGroup(groupUI,rows,cols,TOP_1_GROUP_ID,CButtonGroup::SIGLE,BUTTON_TYPE_BUTTONGROUP_BLUE_SPEC,btnLine,m_BTN_Top_1_Group_Info.title,0,170);

	}
	// 张数选择按钮组
	if(ulCompomentFlag & NUMBER_AREA){
		rows = m_BTN_Left_1_Group_Info.rows;
		cols = m_BTN_Left_1_Group_Info.cols;
		CSize groupSize = left_number_size;

		groupUI.m_Location= REC_LEFT_NUMBER_RECT.TopLeft() - size;
		groupUI.m_Size = groupSize;//REC_LEFT_LINE_RECT.Size();
		groupUI.m_Visible	= m_BTN_Left_1_Group_Info.bVisible;
		MARGINS buttonMagins={0,0,0,0};
		groupUI.m_Margins =  buttonMagins;
		groupUI.m_pBackgroundImage = m_pImageBack_left;
		UIINFO btnNumber = btnUIInfo;
		btnNumber.m_DockStyle = LEFTRIGHT;
		btnNumber.m_Location = CPoint((left_number_size.cx-button_size.cx)/2,btnNumber.m_Location.y);
		btnNumber.m_Size = button_size;

		m_Left_1_Group = new CButtonGroup(groupUI,rows,cols,LEFT_1_GROUP_ID,CButtonGroup::SIGLE,BUTTON_TYPE_BUTTONGROUP_BLUE,btnNumber,m_BTN_Left_1_Group_Info.title);
	}
	// 单价选择按钮组
	if(ulCompomentFlag & PRICES_AREA){
		rows = m_BTN_Right_1_Group_Info.rows;
		cols = m_BTN_Right_1_Group_Info.cols;
		CSize groupSize = right_prices_size;

		groupUI.m_Location= REC_RIGHT_PRICES_RECT.TopLeft() - size;
		groupUI.m_Size = groupSize;
		groupUI.m_Visible	= m_BTN_Right_1_Group_Info.bVisible;
		MARGINS buttonMagins={0,0,0,0};
		groupUI.m_Margins =  buttonMagins;
		groupUI.m_pBackgroundImage = m_pImageBack_right;
		UIINFO btnNumber = btnUIInfo;
		btnNumber.m_DockStyle = LEFTRIGHT;
		btnNumber.m_Location = CPoint((right_prices_size.cx-button_size.cx)/2,btnNumber.m_Location.y);
		btnNumber.m_Size = button_size;
		m_Right_1_Group = new CButtonGroup(groupUI,rows,cols,RIGHT_1_GROUP_ID,CButtonGroup::SIGLE,BUTTON_TYPE_BUTTONGROUP_BROWN,btnNumber,m_BTN_Right_1_Group_Info.title);
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
CXButton* CReceptionDlg::BuildOperationButton(CRect rect,CString text,CString msg,int btnType)
{
	UIINFO ui;
	ui.m_pParentWnd = this;
	ui.m_DockStyle = DOCKSTYLE::NONE;
	ui.m_Location= rect.TopLeft();
	ui.m_Size = rect.Size();
	ui.m_Visible = true;
	BTN_INFO btnInfo;
	btnInfo.text = text;
	btnInfo.btnMsg = msg;
	btnInfo.isVisible = true;
	btnInfo.type = (BUTTON_TYPE)btnType;
	return new CXButton(ui,btnInfo);
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
int CReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag)
{
    try {
		if (__super::OnCreate(lpCreateStruct) == -1) {
			return -1;
		}
		if (ulCompomentFlag & CASH_COUNT_AREA)
		{
			m_cashInInfo = new CGStatic(this,REC_CASHIN_NOTICE_RECT);
		}
		if(ulCompomentFlag & BASE_AREA){
			m_baseInfoArea = new CGStatic(this, m_baseInfo);
		}
		if(ulCompomentFlag &CASH_AREA ){
			m_cashInfoArea = new CGStatic(this, m_cashInfo, false,m_pImageBack_center);
		}
		if(ulCompomentFlag &GUIDE_AREA ){
			m_guideInfoArea = new CReceptionGuideInfo(CWnd::FromHandle(m_cashInfoArea->GetSafeHwnd()), m_guideInfo,m_cashInfo->type);
			m_guideInfoArea->createWnd(CWnd::FromHandle(m_cashInfoArea->GetSafeHwnd()));
		}

		// 按纽区
		if(m_needFunctionArea){
			// 功能按纽区
			if (m_needFunctionArea){
				BuideButtonGroup(ulCompomentFlag);
			}
			if(m_Top_1_Group != NULL){
				m_Top_1_Group->Create();
				m_Top_1_Group->SetButtonInfos(m_vec_Top_1_BTN_INFO);
				m_Top_1_Group->SetButtonTitle(m_BTN_Top_1_Group_Info.title);
				m_Top_1_Group->ShowButtons(true);

			}

			if(m_Left_1_Group != NULL){
				m_Left_1_Group->Create();
				m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
				m_Left_1_Group->SetButtonTitle(m_BTN_Left_1_Group_Info.title);
				m_Left_1_Group->ShowButtons(true);
			}
			//if(m_Left_2_Group != NULL){
			//	m_Left_2_Group->Create();
			//	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
			//	m_Left_2_Group->SetButtonTitle(m_BTN_Left_2_Group_Info.title);
			//	m_Left_2_Group->ShowButtons(true);
			//}

			if (m_Right_1_Group!=NULL){			
				m_Right_1_Group->Create();
				//m_Right_1_Group->SetColWidth(165,0);
				m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
				m_Right_1_Group->SetButtonTitle(m_BTN_Right_1_Group_Info.title);
				m_Right_1_Group->ShowButtons(true);				
			}
 		}	  
		if (ulCompomentFlag & BTN_OK) m_btnOK->Create();
		if (ulCompomentFlag & BTN_OK_AND_CANCEL1) m_btnOKAndCancel1->Create();
		if (ulCompomentFlag & BTN_OK_AND_CANCEL2) m_btnOKAndCancel2->Create();

		if (ulCompomentFlag & BTN_PRINT) m_btnPrint->Create();
		if (ulCompomentFlag & BTN_NOPRINT) m_btnNOPrint->Create();	
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
/**
 @brief      画面绘制函数

 @param      (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnDraw(CDC* pDC)
{
}

void CReceptionDlg::preShow()
{
	// 显示乘客画面标题与主业务按纽界面
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_SHOW);
	// 隐藏“欢迎光临”画面
	theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->ShowWindow(SW_HIDE);
	if(m_needFunctionArea)
	{	
		/*m_Right_1_Group!=NULL?m_Right_1_Group->SetButtonTitle(m_BTN_GROUP_INFO[0].title):NULL;
		m_Right_1_Group!=NULL?m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO):NULL;
		m_Right_2_Group!=NULL?m_Right_2_Group->SetButtonTitle(m_BTN_GROUP_INFO[1].title):NULL;
		m_Right_2_Group!=NULL?m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO):NULL;
		m_Right_3_Group!=NULL?m_Right_3_Group->SetButtonTitle(m_BTN_GROUP_INFO[2].title):NULL;
		m_Right_3_Group!=NULL?m_Right_3_Group->SetButtonInfos(m_vec_Right_3_BTN_INFO):NULL;
		m_Right_4_Group!=NULL?m_Right_4_Group->SetButtonTitle(m_BTN_GROUP_INFO[3].title):NULL;
		m_Right_4_Group!=NULL?m_Right_4_Group->SetButtonInfos(m_vec_Right_4_BTN_INFO):NULL;*/
	}

	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->RefreshCancelButton();
	m_btnOK->SetText(_tl(STR_BACK));
	m_btnOKAndCancel1->SetText(_tl(STR_OK));
	m_btnOKAndCancel2->SetText(_tl(STR_CANCEL));
	m_btnPrint->SetText(_tl(STR_PRINT));// 
	// 界面切换的时候设置数据区域格式
	if(NULL != m_cashInfoArea){
		m_cashInfoArea->SetStyle();
	}
	if(NULL != m_baseInfoArea){
		m_baseInfoArea->SetStyle();
	}
}

void CReceptionDlg::showComplete()
{
	if(IsWindow(m_btnOK->m_hWnd))
	{
		m_btnOK->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnServiceBusy(bool busy)
{
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->OnServiceBusy(busy);
	if(busy)
	{
		bool printEnable = m_btnPrint->IsEnabled();
		m_btnPrint->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,printEnable);
		m_btnPrint->SetEnabled(false);

		bool NOprintEnable = m_btnNOPrint->IsEnabled();
		m_btnNOPrint->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,NOprintEnable);
		m_btnNOPrint->SetEnabled(false);

		bool okEnable = m_btnOK->IsEnabled();
		m_btnOK->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,okEnable);
		m_btnOK->SetEnabled(false);

		bool okAndCancel1Enable = m_btnOKAndCancel1->IsEnabled();
		m_btnOKAndCancel1->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,okAndCancel1Enable);
		m_btnOKAndCancel1->SetEnabled(false);
	
		bool okAndCancel2Enable = m_btnOKAndCancel2->IsEnabled();
		m_btnOKAndCancel2->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,okAndCancel2Enable);
		m_btnOKAndCancel2->SetEnabled(false);

	}
	else
	{
		bool printEnable = true;
		m_btnPrint->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,printEnable);
		m_btnPrint->SetEnabled(printEnable);
		bool NOprintEnable = true;
		m_btnNOPrint->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,NOprintEnable);
		m_btnNOPrint->SetEnabled(NOprintEnable);
		bool okEnable = true;
		m_btnOK->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,okEnable);
		m_btnOK->SetEnabled(okEnable);

		bool okAndCancel1Enable = true;
		m_btnOKAndCancel1->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,okAndCancel1Enable);
		m_btnOKAndCancel1->SetEnabled(okAndCancel1Enable);

		bool okAndCancel2Enable = true;
		m_btnOKAndCancel2->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,okAndCancel2Enable);
		m_btnOKAndCancel2->SetEnabled(okAndCancel2Enable);

	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      显示组件

@param      (i)ulong ulCompomentFlag           组件标志

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示组件

@param      (i)CWnd *pCompoment                组件对象

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏组件

@param      (i)ulong ulCompomentFlag           组件标志

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏组件

@param      (i)CWnd *pCompoment                组件对象

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示/隐藏组件

@param      (i)ulong ulCompomentFlag           组件标志
@param      (i)int nCmdShow                    命令(SW_SHOW/SW_HIDE)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
	// 确认按钮
	if (ulCompomentFlag & BTN_OK) {
		ShowHideCompoment(m_btnOK, nCmdShow);
	}
	// 确认按钮
	if (ulCompomentFlag & BTN_OK_AND_CANCEL1) {
		ShowHideCompoment(m_btnOKAndCancel1, nCmdShow);
	}
	// 取消按钮
	if (ulCompomentFlag & BTN_OK_AND_CANCEL2) {
		ShowHideCompoment(m_btnOKAndCancel2, nCmdShow);
	}

	// 打印按钮
	if (ulCompomentFlag & BTN_PRINT) {
		ShowHideCompoment(m_btnPrint, nCmdShow);
	}
	// 打印按钮
	if (ulCompomentFlag & BTN_NOPRINT) {
		ShowHideCompoment(m_btnNOPrint, nCmdShow);
	}
	
	// 基本数据区域
	if (ulCompomentFlag & BASE_AREA) {
		ShowHideCompoment(m_baseInfoArea, nCmdShow);
	}
	// 现金数据区域
	if (ulCompomentFlag & CASH_AREA) {
		ShowHideCompoment(m_cashInfoArea, nCmdShow);
	}
	// 提示数据区域
	if (ulCompomentFlag & GUIDE_AREA) {
		ShowHideCompoment(m_guideInfoArea, nCmdShow);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示/隐藏组件

@param      (i)CWnd *pCompoment                组件对象
@param      (i)int nCmdShow                    命令(SW_SHOW/SW_HIDE)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(CWnd *pCompoment, int nCmdShow)
{
	if (pCompoment == NULL) 
	{
		return;
	}
	CXButton* pButton = dynamic_cast<CXButton*>(pCompoment);
	if(pButton!=NULL)
	{
		pButton->SetVisible(nCmdShow == SW_SHOW ? true:false);
		return;
	}
	CRect rect;
	pCompoment->ShowWindow(nCmdShow);
	GetCompomentRect(pCompoment, &rect);
	InvalidateRect(&rect, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得组件所在的区域

@param      (i)CWnd *pCompoment                组件对象
@param      (o)CRect* lpRect                   区域

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);

    // 转换成相对于当前OperationDlg的位置
    ScreenToClient(lpRect);

    // 加上边框的大小
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断ok或取消按钮是否可用

@param      bool true : OK按钮可用状态 
				 false：打印按钮状态

@retval     bool true ：可见 
				 false：不可见

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CReceptionDlg::IsButtonEnable(bool  bIsOkOrPrint){
	return bIsOkOrPrint ? m_btnOK->IsVisible():m_btnPrint->IsVisible();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      隐藏所有显示的按钮

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideAllCompoment(){
	HideCompoment(BTN_OK|BTN_PRINT|BTN_OK_AND_CANCEL1|BTN_OK_AND_CANCEL2);
	if (NULL != m_Top_1_Group)
	{
		m_Top_1_Group->ShowButtons(false);
		m_Top_1_Group->UpdateWindow();
	}
	if(NULL != m_Left_1_Group){
		m_Left_1_Group->ShowButtons(false);
		m_Left_1_Group->UpdateWindow();
	}

	if(NULL != m_Right_1_Group){
		m_Right_1_Group->ShowButtons(false);
		m_Right_1_Group->UpdateWindow();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化右侧功能区按钮组

@param      (i)long lComponents  自定义组件信息

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::InitializeFunctionBtns(long lComponents)
{
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	CPassengerForeSvc* pSvc = (CPassengerForeSvc*)theSERVICE_MGR.GetCurService();
	// 线路选择按钮组
	if(lComponents & LINE_SELECT_BUTTON){
		m_vec_Top_1_BTN_INFO.clear();

		BTN_INFO btnInfo;
		btnInfo.btnMsg.Format(_T("%d"), 0);
		btnInfo.text = _T("总图")/*_tl(TXT_ISSUE_HOME)*/;	
		m_vec_Top_1_BTN_INFO.push_back(btnInfo);

		btnInfo.btnMsg.Format(_T("%d"), 1);
		btnInfo.text = _T("2号线")/*_tl(TXT_ISSUE_HOME)*/;	
		m_vec_Top_1_BTN_INFO.push_back(btnInfo);
		/*
		vector<int> vec_Lines; 
		theSTATION_MAP_XML.GetLineIds(vec_Lines);
		for (int i=0;i<vec_Lines.size();i++){
		BTN_INFO btnInfo;
		btnInfo.btnMsg.Format(_T("%d"), vec_Lines[i]);		
		if(language==LANGUAGE_ID_CHINESE){
		btnInfo.text.Format(_T("%d号线"), vec_Lines[i]);		
		}
		else{
		btnInfo.text.Format(_T("Line %d"), vec_Lines[i]);	
		}
		m_vec_Top_1_BTN_INFO.push_back(btnInfo);
		}	
		*/
		//btnInfo.btnMsg.Format(_T("%d"), 255);
		//btnInfo.text = _tl(TXT_ALL_LINES);		
		//m_vec_Top_1_BTN_INFO.push_back(btnInfo);
		CString title;
		title = _T("选择^线路");

		m_Top_1_Group->ShowButtons(true);
		m_Top_1_Group->SetButtonTitle(title);

		m_Top_1_Group->SetButtonInfos(m_vec_Top_1_BTN_INFO);

		//m_Top_1_Group->SetButtonTitle(title);

		//UIINFO uiInfo=m_Top_1_Group->getUIInfo();
		//uiInfo.m_Font=theGUI_INFO.GetBtnFont();
	}

	// 票价按钮组
	if(lComponents & PRICE_BUTTON)
	{
		m_vec_Right_1_BTN_INFO.clear();
		vector<int> vecFareList;
		vecFareList = pSvc->GetTicketAmountTable();
		for (vector<int>::iterator iter = vecFareList.begin(); iter != vecFareList.end(); ++iter)
		{
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"), *iter);										// 按钮通知消息：票价
			btnInfo.text = _T("￥") + ComFormatAmount(*iter);							// 按钮显示文本：票价

			if(m_dwMaxTicketAmount < *iter){
				continue;
			}
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
		}
		CString title;
		title = _tl(TXT_CHOOSE_PRICE);
		m_Right_1_Group->ShowButtons(true);
		m_Right_1_Group->SetButtonTitle(title);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	}

	// 售票张数组
	if(lComponents & NUMBER_BUTTON)
	{
		//// 根据参数已空条件，计算可用发售张数
		//CAfcTVMOperationParam::MAGAZINE_PARAM cardBoxParam;
		//theAFC_TVM_OPERATION.GetMagazineParam(cardBoxParam);
		//LONG nAvaliableCountA = theTOKEN_COUNT.GetHopperAInfo().ulCurCount - cardBoxParam.TicketEmptyValue;
		//LONG nAvaliableCountB = theTOKEN_COUNT.GetHopperBInfo().ulCurCount - cardBoxParam.TicketEmptyValue;
		//if (nAvaliableCountA<=0){
		//	nAvaliableCountA = 0;
		//}
		//if (nAvaliableCountB<=0){
		//	nAvaliableCountB = 0;
		//}
		// 设置发售张数信息
		m_vec_Left_1_BTN_INFO.clear();
		vector<int> vecCountList;
		vecCountList = pSvc->GetTicketCountTable();
		for (vector<int>::iterator iter = vecCountList.begin(); iter != vecCountList.end(); ++iter)
		{
			if(1/**iter <= nAvaliableCountA + nAvaliableCountB*/){
				BTN_INFO btnInfo;
				btnInfo.btnMsg.Format(_T("%d"), *iter);	
				if(language==LANGUAGE_ID_CHINESE){
					btnInfo.text.Format(_T("%d张"), *iter);		
				}
				else{
					btnInfo.text.Format(_T("%d"), *iter);		
				}		
				m_vec_Left_1_BTN_INFO.push_back(btnInfo);
			}
		}	

		CString title;
		title = _tl(TXT_CHOOSE_TICKETS);
		m_Left_1_Group->ShowButtons(true);
		m_Left_1_Group->SetButtonTitle(title);
		m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
	}
	m_Top_1_Group->EnableWindow(TRUE);
	m_Left_1_Group->EnableWindow(TRUE);
	m_Right_1_Group->EnableWindow(TRUE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化线路图区域

@param      (i)DWORD areaLayoutID 布局页编码

@retval     true: 初始化成功  false:页面不可用

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
bool CReceptionDlg::InitializeLineGroup(DWORD layoutID /* = DEFAULT_FIRST_LAYOUT */)
{
	m_vec_BtnUIInfo.clear();
	m_vec_buttonInfos.clear();
	m_vec_textInfos.clear();
	m_vec_hotLocation.clear();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	if (layoutID==0){//总图
		CParseStationMapXML::GlobalInfoStruct globalInfo;
		if (theSTATION_MAP_XML.GetGlobalStationInfo(globalInfo)){
			SIZE size;
			size.cx = 130;
			size.cy = REC_FRMAE_RECT.top;
			m_LineAreaUIInfo.m_pParentWnd			= this;
			m_LineAreaUIInfo.m_DockStyle			= DOCKSTYLE::NONE;
			m_LineAreaUIInfo.m_Location				= REC_LINE_MAP_RECT.TopLeft() - size;
			m_LineAreaUIInfo.m_Size					= REC_LINE_MAP_RECT.Size();
			m_LineAreaUIInfo.m_BKMode				= BKMODE_LAST;
			m_LineAreaUIInfo.m_BorderWidth			= 0;
			m_LineAreaUIInfo.m_Margins				= DEFAULT_GROUPBOX_MARGINS;
			m_LineAreaUIInfo.m_nParentType			= 1;//总图窗口
			if (language==LANGUAGE_ID_CHINESE){
				m_LineAreaUIInfo.m_strBackgroundImage = globalInfo.ImageName_CN;
				m_LineAreaUIInfo.m_pBackgroundImage		= globalInfo.pImage_CN;	
			}
			else{
				m_LineAreaUIInfo.m_strBackgroundImage = globalInfo.ImageName_EN;
				m_LineAreaUIInfo.m_pBackgroundImage		= globalInfo.pImage_EN;	
			}

			// 站点按钮信息
			// 注意：当前线路图显示区域大小REC_LINE_MAP_RECT, 而实际线路图大小为740*590，因此按钮大小和区域都需要调整。
			float a_x = (float)REC_LINE_MAP_RECT.Width();
			float b_x = (float)740; 
			float radio_x = a_x/b_x;
			float a_y = (float)REC_LINE_MAP_RECT.Height();
			float b_y = (float)590; 
			float radio_y = a_y/b_y;
			radio_x = 1.0;//0.7796 + 0.4;//总图区域的放大后缩放比利率
			radio_y = 1.0;//0.7796 + 0.4;
			for(int i=0;i<globalInfo.vec_StationsInfo.size();i++){
				CParseStationMapXML::StationInfoStruct &StationInfo = globalInfo.vec_StationsInfo[i];
				UIINFO uiInfo;
				uiInfo.m_pBackgroundImage	= NULL;
				uiInfo.m_pParentWnd			= this;
				uiInfo.m_DockStyle			= DOCKSTYLE::NONE;
				//uiInfo.m_Location.x			= (LONG)(_ttoi(StationInfo.Pos_x)*radio_x);
				//uiInfo.m_Location.y			= (LONG)(_ttoi(StationInfo.Pos_y)*radio_y);
				uiInfo.m_Location.x			= (LONG)((_ttoi(StationInfo.Pos_x)-_ttoi(StationInfo.Width)/2)*radio_x);
				uiInfo.m_Location.y			= (LONG)((_ttoi(StationInfo.Pos_y)-_ttoi(StationInfo.Height)/2)*radio_y);
				uiInfo.m_Size				= CSize((int)(_ttoi(StationInfo.Width)*radio_x), (int)(_ttoi(StationInfo.Height)*radio_y));
				uiInfo.m_BKMode				= BKMODE_LAST;
				uiInfo.m_BorderWidth		= 0;
				uiInfo.m_Margins			= DEFAULT_GROUPBOX_MARGINS;
				uiInfo.m_Visible			= true;
				uiInfo.m_Enabled			= (StationInfo.IsUse==_T("1"));
				uiInfo.m_Area				= CRect(uiInfo.m_Location.x,uiInfo.m_Location.y,uiInfo.m_Size.cx+uiInfo.m_Location.x,uiInfo.m_Size.cy+uiInfo.m_Location.y);
				m_vec_BtnUIInfo.push_back(uiInfo);

				// 创建按钮信息
				BTN_INFO btnInfo;
				btnInfo.btnID = _ttoi(StationInfo.StationId);
				btnInfo.btnMsg.Format(_T("%d"), 0);
				btnInfo.text = _T("");
				btnInfo.isEnable = (StationInfo.IsUse==_T("1"));
				btnInfo.type = BUTTON_TYPE_LINE_AREA;
				m_vec_buttonInfos.push_back(btnInfo);

				// 字体信息
				TEXT_INFO textinfo;
				m_vec_textInfos.push_back(textinfo);
			}

		}
		else{
			LOG("STATION_MAP_XML is inited failed!!!");
		}
	}
	else{
		CParseStationMapXML::SigleLineInfo lineInfo;
		if (theSTATION_MAP_XML.GetLineStationInfo(layoutID, lineInfo)){
			if (lineInfo.IsUse!=_T("1")){
				return false;
			}
			// 图片显示信息
			SIZE size;
			size.cx = 0;
			size.cy = REC_FRMAE_RECT.top;
			m_LineAreaUIInfo.m_pParentWnd			= this;
			m_LineAreaUIInfo.m_DockStyle			= DOCKSTYLE::NONE;
			m_LineAreaUIInfo.m_Location				= REC_LINE_MAP_RECT.TopLeft() - size;
			m_LineAreaUIInfo.m_Size					= REC_LINE_MAP_RECT.Size();
			m_LineAreaUIInfo.m_BKMode				= BKMODE_LAST;
			m_LineAreaUIInfo.m_BorderWidth			= 0;
			m_LineAreaUIInfo.m_Margins				= DEFAULT_GROUPBOX_MARGINS;
			m_LineAreaUIInfo.m_nParentType			= 0;//非总图窗口
			if (language==LANGUAGE_ID_CHINESE){
				m_LineAreaUIInfo.m_strBackgroundImage = lineInfo.ImageName_CN;
				m_LineAreaUIInfo.m_pBackgroundImage		= lineInfo.pImage_CN;	
			}
			else{
				m_LineAreaUIInfo.m_strBackgroundImage = lineInfo.ImageName_EN;
				m_LineAreaUIInfo.m_pBackgroundImage		= lineInfo.pImage_EN;	
			}

			// 站点按钮信息
			// 注意：当前线路图显示区域大小REC_LINE_MAP_RECT, 而实际线路图大小为740*590，因此按钮大小和区域都需要调整。
			float a_x = (float)REC_LINE_MAP_RECT.Width();
			float b_x = (float)900; 
			float radio_x = a_x/b_x;
			float a_y = (float)REC_LINE_MAP_RECT.Height();
			float b_y = (float)806; 
			float radio_y = a_y/b_y;
			for(int i=0;i<lineInfo.vec_StationsInfo.size();i++){
				CParseStationMapXML::StationInfoStruct &StationInfo = lineInfo.vec_StationsInfo[i];
				UIINFO uiInfo;
				uiInfo.m_pBackgroundImage	= NULL;
				uiInfo.m_pParentWnd			= this;
				uiInfo.m_DockStyle			= DOCKSTYLE::NONE;
				uiInfo.m_Location.x			= (LONG)((_ttoi(StationInfo.Pos_x)-_ttoi(StationInfo.Width)/2)*radio_x);
				uiInfo.m_Location.y			= (LONG)((_ttoi(StationInfo.Pos_y)-_ttoi(StationInfo.Height)/2)*radio_y);
				uiInfo.m_Size				= CSize((int)(_ttoi(StationInfo.Width)*radio_x), (int)(_ttoi(StationInfo.Height)*radio_y));
				uiInfo.m_BKMode				= BKMODE_LAST;
				uiInfo.m_BorderWidth		= 0;
				uiInfo.m_Margins			= DEFAULT_GROUPBOX_MARGINS;
				uiInfo.m_Visible			= true;
				uiInfo.m_Enabled			= (StationInfo.IsUse==_T("1"));
				uiInfo.m_Area				= CRect(uiInfo.m_Location.x,uiInfo.m_Location.y,uiInfo.m_Size.cx+uiInfo.m_Location.x,uiInfo.m_Size.cy+uiInfo.m_Location.y);
				m_vec_BtnUIInfo.push_back(uiInfo);

				// 创建按钮信息
				BTN_INFO btnInfo;
				btnInfo.btnID = _ttoi(StationInfo.StationId);
				btnInfo.btnMsg.Format(_T("%d"), 0);
				btnInfo.text = _T("");
				btnInfo.isEnable = (StationInfo.IsUse==_T("1"));
				btnInfo.type = BUTTON_TYPE_LINE_AREA;
				m_vec_buttonInfos.push_back(btnInfo);

				// 字体信息
				TEXT_INFO textinfo;
				m_vec_textInfos.push_back(textinfo);
			}
		}
		else{
			LOG("STATION_MAP_XML is inited failed!!!");
		}

	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      设置显示区域的显示信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::SetShowAreaInfo(ULONG showAreaID, PGSTATIC_INFO showAreaInfo)
{
	if (showAreaID==BASE_AREA){
		if (m_baseInfoArea){
			delete m_baseInfoArea;
			if (m_baseInfo){
				delete m_baseInfo;
				m_baseInfo = new GSTATIC_INFO;
			}
		}
		*m_baseInfo = *showAreaInfo;
		m_baseInfo->titleInfo.color = ORANGE;
		m_baseInfo->detailInfo.color = BLACK;
		m_baseInfo->detailInfo.editGroup.cnt = 0;                   // 编辑框个数
		m_baseInfo->detailInfo.editGroup.isLeftAlign = true;
		m_baseInfoArea = new CGStatic(this, m_baseInfo); 
		
	}
	else if (showAreaID==CASH_AREA){
		if (m_cashInfoArea){
			delete m_cashInfoArea;
			if (m_cashInfo){
				delete m_cashInfo;
				m_cashInfo = new GSTATIC_INFO;
			}
		}
		*m_cashInfo = *showAreaInfo;
		m_cashInfo->titleInfo.color = ORANGE;
		m_cashInfo->detailInfo.color = BLACK;
		m_cashInfo->detailInfo.editGroup.cnt = 0;                   // 编辑框个数
		m_cashInfo->detailInfo.editGroup.isLeftAlign = true;
		m_cashInfoArea = new CGStatic(this, m_cashInfo, false,m_pImageBack_center); 
		
	}
	else{

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置显示区域的显示信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::SetTipsShow(CString strTxt)
{
	return;
	LOG("SetTipsShow::");
	if(m_pTipsDlg!=NULL && IsWindow(m_pTipsDlg->GetSafeHwnd())){
		m_pTipsDlg->SetWindowText(strTxt);
		m_pTipsDlg->ShowWindow(SW_SHOW);
		m_pTipsDlg->Invalidate(FALSE);
	}
}


