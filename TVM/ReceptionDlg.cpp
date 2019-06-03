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

//@brief      ���캯��

//@param      (i)UINT ID             �Ի���ID
//@param      (i)CService* pService  �Ի���ʹ�õ�Service

//@retval     ��

//@exception  ��

//////////////////////////////////////////////////////////////////////////
CReceptionDlg::CReceptionDlg(UINT ID, CService* pService,CRect rect,bool needFunctionArea)
	: CBaseDlg(ID, theAPP_SESSION.GetChildViewRec(), rect, GRAY, pService,false)
{
	this->m_needFunctionArea = needFunctionArea;
	Initialize();
}

//////////////////////////////////////////////////////////////////////////

//@brief      ���캯��
//
//@param      (i)UINT ID             �Ի���ID
//@param      (i)CRect position       λ�ô�С
//@param      (i)COLORREF bkColor     ����ɫ
//@param      (i)CService* pService   �Ի���ʹ�õ�Service
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CReceptionDlg::CReceptionDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea)
	: CBaseDlg(ID, theAPP_SESSION.GetChildViewRec(), position, bkColor, pService,false)
{
	this->m_needFunctionArea = needFunctionArea;
	Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ��������

 @param      ��

 @retval     ��

 @exception  ��
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
	// ������Ϣ���ֽ���Ϣ����
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

	// ɾ����ť��
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

//@brief      ���ڻ�
//
//@param      ��
//
//@retval     ��
//
//@exception  ��

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
	//������Ϣ
	m_baseInfo = new GSTATIC_INFO;
	m_baseInfo->point = BASEINFO_AREA_POINT;
	m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
	m_baseInfo->titleInfo.color = ORANGE;
	m_baseInfo->detailInfo.color = BLACK;
	m_baseInfo->detailInfo.labelGroup = m_TipsInfoGroup;
	m_baseInfo->detailInfo.row = 13;
	m_baseInfo->detailInfo.editGroup.cnt = 0;                   // �༭�����
	m_baseInfo->detailInfo.editGroup.isLeftAlign = true;
	//�ֽ��ֽ���Ϣ
	m_cashInfo = new GSTATIC_INFO;
	m_cashInfo->point = CASHINFO_AREA_POINT;
	m_cashInfo->stytle = PASSAGE_NARROW_STYLE;
	m_cashInfo->titleInfo.titleName = _tl(TXT_CASH_INFO);
	m_cashInfo->titleInfo.color = ORANGE;
	m_cashInfo->detailInfo.color = BLACK;
	m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup/*m_cashLabelGroup*/;
	m_cashInfo->detailInfo.row = 4;
	m_cashInfo->detailInfo.editGroup.cnt = 0;                   // �༭�����
	m_cashInfo->detailInfo.editGroup.isLeftAlign = true;

	m_cashLabelGroup[0].nameLeft = add_hsp(_tl(TXT_PAYMENT_METHOD)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);  // ֧����ʽ
	m_cashLabelGroup[0].nameRight = add_hsp(_tl(TXT_UNDEFINE),MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[1].nameLeft = add_hsp(_tl(TXT_TOTAL)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN);  // Ӧ�����
	m_cashLabelGroup[1].nameRight = add_hsp(_tl(MONEY_UNIT),MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[2].nameLeft = add_hsp(_tl(TXT_INCOME)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN); // ʵ�ս��
	m_cashLabelGroup[2].nameRight = add_hsp(_tl(MONEY_UNIT),MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[3].nameLeft = add_hsp(_tl(TXT_CHANGE)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN); // ����
	m_cashLabelGroup[3].nameRight = add_hsp(_tl(MONEY_UNIT),MAX_CASH_RIGHT_LEN);
	
	//��ʾ��Ϣ����
	CString szPath = _T("");
	GetAppPath(szPath);
	//��ť�鱳��ͼ
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

	//������ť
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
	// ��ť�鶨��
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
	// ��·ѡ��
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
	// ����ѡ��ť��
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
	// ����ѡ��ť��
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
 @brief      ���洴������

 @param      (i)LPCREATESTRUCT lpCreateStruct  

 @retval     int    \n
			 0:�����ɹ�    -1:����ʧ��

 @exception  ��
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

		// ��Ŧ��
		if(m_needFunctionArea){
			// ���ܰ�Ŧ��
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
 @brief      ������ƺ���

 @param      (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnDraw(CDC* pDC)
{
}

void CReceptionDlg::preShow()
{
	// ��ʾ�˿ͻ����������ҵ��Ŧ����
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_SHOW);
	// ���ء���ӭ���١�����
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
	// �����л���ʱ���������������ʽ
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
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  ��
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
@brief      ��ʾ���

@param      (i)ulong ulCompomentFlag           �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ���

@param      (i)CWnd *pCompoment                �������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i)ulong ulCompomentFlag           �����־

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������

@param      (i)CWnd *pCompoment                �������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ/�������

@param      (i)ulong ulCompomentFlag           �����־
@param      (i)int nCmdShow                    ����(SW_SHOW/SW_HIDE)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
	// ȷ�ϰ�ť
	if (ulCompomentFlag & BTN_OK) {
		ShowHideCompoment(m_btnOK, nCmdShow);
	}
	// ȷ�ϰ�ť
	if (ulCompomentFlag & BTN_OK_AND_CANCEL1) {
		ShowHideCompoment(m_btnOKAndCancel1, nCmdShow);
	}
	// ȡ����ť
	if (ulCompomentFlag & BTN_OK_AND_CANCEL2) {
		ShowHideCompoment(m_btnOKAndCancel2, nCmdShow);
	}

	// ��ӡ��ť
	if (ulCompomentFlag & BTN_PRINT) {
		ShowHideCompoment(m_btnPrint, nCmdShow);
	}
	// ��ӡ��ť
	if (ulCompomentFlag & BTN_NOPRINT) {
		ShowHideCompoment(m_btnNOPrint, nCmdShow);
	}
	
	// ������������
	if (ulCompomentFlag & BASE_AREA) {
		ShowHideCompoment(m_baseInfoArea, nCmdShow);
	}
	// �ֽ���������
	if (ulCompomentFlag & CASH_AREA) {
		ShowHideCompoment(m_cashInfoArea, nCmdShow);
	}
	// ��ʾ��������
	if (ulCompomentFlag & GUIDE_AREA) {
		ShowHideCompoment(m_guideInfoArea, nCmdShow);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ/�������

@param      (i)CWnd *pCompoment                �������
@param      (i)int nCmdShow                    ����(SW_SHOW/SW_HIDE)

@retval     ��

@exception  ��
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
@brief      ȡ��������ڵ�����

@param      (i)CWnd *pCompoment                �������
@param      (o)CRect* lpRect                   ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);

    // ת��������ڵ�ǰOperationDlg��λ��
    ScreenToClient(lpRect);

    // ���ϱ߿�Ĵ�С
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ж�ok��ȡ����ť�Ƿ����

@param      bool true : OK��ť����״̬ 
				 false����ӡ��ť״̬

@retval     bool true ���ɼ� 
				 false�����ɼ�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CReceptionDlg::IsButtonEnable(bool  bIsOkOrPrint){
	return bIsOkOrPrint ? m_btnOK->IsVisible():m_btnPrint->IsVisible();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����������ʾ�İ�ť

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ʼ���Ҳ๦������ť��

@param      (i)long lComponents  �Զ��������Ϣ

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::InitializeFunctionBtns(long lComponents)
{
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	CPassengerForeSvc* pSvc = (CPassengerForeSvc*)theSERVICE_MGR.GetCurService();
	// ��·ѡ��ť��
	if(lComponents & LINE_SELECT_BUTTON){
		m_vec_Top_1_BTN_INFO.clear();

		BTN_INFO btnInfo;
		btnInfo.btnMsg.Format(_T("%d"), 0);
		btnInfo.text = _T("��ͼ")/*_tl(TXT_ISSUE_HOME)*/;	
		m_vec_Top_1_BTN_INFO.push_back(btnInfo);

		btnInfo.btnMsg.Format(_T("%d"), 1);
		btnInfo.text = _T("2����")/*_tl(TXT_ISSUE_HOME)*/;	
		m_vec_Top_1_BTN_INFO.push_back(btnInfo);
		/*
		vector<int> vec_Lines; 
		theSTATION_MAP_XML.GetLineIds(vec_Lines);
		for (int i=0;i<vec_Lines.size();i++){
		BTN_INFO btnInfo;
		btnInfo.btnMsg.Format(_T("%d"), vec_Lines[i]);		
		if(language==LANGUAGE_ID_CHINESE){
		btnInfo.text.Format(_T("%d����"), vec_Lines[i]);		
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
		title = _T("ѡ��^��·");

		m_Top_1_Group->ShowButtons(true);
		m_Top_1_Group->SetButtonTitle(title);

		m_Top_1_Group->SetButtonInfos(m_vec_Top_1_BTN_INFO);

		//m_Top_1_Group->SetButtonTitle(title);

		//UIINFO uiInfo=m_Top_1_Group->getUIInfo();
		//uiInfo.m_Font=theGUI_INFO.GetBtnFont();
	}

	// Ʊ�۰�ť��
	if(lComponents & PRICE_BUTTON)
	{
		m_vec_Right_1_BTN_INFO.clear();
		vector<int> vecFareList;
		vecFareList = pSvc->GetTicketAmountTable();
		for (vector<int>::iterator iter = vecFareList.begin(); iter != vecFareList.end(); ++iter)
		{
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"), *iter);										// ��ť֪ͨ��Ϣ��Ʊ��
			btnInfo.text = _T("��") + ComFormatAmount(*iter);							// ��ť��ʾ�ı���Ʊ��

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

	// ��Ʊ������
	if(lComponents & NUMBER_BUTTON)
	{
		//// ���ݲ����ѿ�������������÷�������
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
		// ���÷���������Ϣ
		m_vec_Left_1_BTN_INFO.clear();
		vector<int> vecCountList;
		vecCountList = pSvc->GetTicketCountTable();
		for (vector<int>::iterator iter = vecCountList.begin(); iter != vecCountList.end(); ++iter)
		{
			if(1/**iter <= nAvaliableCountA + nAvaliableCountB*/){
				BTN_INFO btnInfo;
				btnInfo.btnMsg.Format(_T("%d"), *iter);	
				if(language==LANGUAGE_ID_CHINESE){
					btnInfo.text.Format(_T("%d��"), *iter);		
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
@brief      ��ʼ����·ͼ����

@param      (i)DWORD areaLayoutID ����ҳ����

@retval     true: ��ʼ���ɹ�  false:ҳ�治����

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

	if (layoutID==0){//��ͼ
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
			m_LineAreaUIInfo.m_nParentType			= 1;//��ͼ����
			if (language==LANGUAGE_ID_CHINESE){
				m_LineAreaUIInfo.m_strBackgroundImage = globalInfo.ImageName_CN;
				m_LineAreaUIInfo.m_pBackgroundImage		= globalInfo.pImage_CN;	
			}
			else{
				m_LineAreaUIInfo.m_strBackgroundImage = globalInfo.ImageName_EN;
				m_LineAreaUIInfo.m_pBackgroundImage		= globalInfo.pImage_EN;	
			}

			// վ�㰴ť��Ϣ
			// ע�⣺��ǰ��·ͼ��ʾ�����СREC_LINE_MAP_RECT, ��ʵ����·ͼ��СΪ740*590����˰�ť��С��������Ҫ������
			float a_x = (float)REC_LINE_MAP_RECT.Width();
			float b_x = (float)740; 
			float radio_x = a_x/b_x;
			float a_y = (float)REC_LINE_MAP_RECT.Height();
			float b_y = (float)590; 
			float radio_y = a_y/b_y;
			radio_x = 1.0;//0.7796 + 0.4;//��ͼ����ķŴ�����ű�����
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

				// ������ť��Ϣ
				BTN_INFO btnInfo;
				btnInfo.btnID = _ttoi(StationInfo.StationId);
				btnInfo.btnMsg.Format(_T("%d"), 0);
				btnInfo.text = _T("");
				btnInfo.isEnable = (StationInfo.IsUse==_T("1"));
				btnInfo.type = BUTTON_TYPE_LINE_AREA;
				m_vec_buttonInfos.push_back(btnInfo);

				// ������Ϣ
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
			// ͼƬ��ʾ��Ϣ
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
			m_LineAreaUIInfo.m_nParentType			= 0;//����ͼ����
			if (language==LANGUAGE_ID_CHINESE){
				m_LineAreaUIInfo.m_strBackgroundImage = lineInfo.ImageName_CN;
				m_LineAreaUIInfo.m_pBackgroundImage		= lineInfo.pImage_CN;	
			}
			else{
				m_LineAreaUIInfo.m_strBackgroundImage = lineInfo.ImageName_EN;
				m_LineAreaUIInfo.m_pBackgroundImage		= lineInfo.pImage_EN;	
			}

			// վ�㰴ť��Ϣ
			// ע�⣺��ǰ��·ͼ��ʾ�����СREC_LINE_MAP_RECT, ��ʵ����·ͼ��СΪ740*590����˰�ť��С��������Ҫ������
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

				// ������ť��Ϣ
				BTN_INFO btnInfo;
				btnInfo.btnID = _ttoi(StationInfo.StationId);
				btnInfo.btnMsg.Format(_T("%d"), 0);
				btnInfo.text = _T("");
				btnInfo.isEnable = (StationInfo.IsUse==_T("1"));
				btnInfo.type = BUTTON_TYPE_LINE_AREA;
				m_vec_buttonInfos.push_back(btnInfo);

				// ������Ϣ
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
@brief      ������ʾ�������ʾ��Ϣ

@param      ��

@retval     ��

@exception  ��
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
		m_baseInfo->detailInfo.editGroup.cnt = 0;                   // �༭�����
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
		m_cashInfo->detailInfo.editGroup.cnt = 0;                   // �༭�����
		m_cashInfo->detailInfo.editGroup.isLeftAlign = true;
		m_cashInfoArea = new CGStatic(this, m_cashInfo, false,m_pImageBack_center); 
		
	}
	else{

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ�������ʾ��Ϣ

@param      ��

@retval     ��

@exception  ��
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


