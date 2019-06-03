#include "stdafx.h"
#include "IssueAreaMapDlg.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CIssueAreaMapDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CIssueAreaMapDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_XLINEGROUP, XLineGroupButtonClick)			// ��·ͼ��ť
	ON_GROUP_BUTTON_CLICK(TOP_1_GROUP_ID,OnLineGroupClick)		// ��·ѡ��ť
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPriceTypeClick)	// Ʊ�۰�ť
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnTicketNumClick)	// ��Ʊ������ť
	ON_GROUP_BUTTON_CLICK(RIGHT_3_GROUP_ID,OnTopStationClick)	// Ʊ�۰�ť
	ON_GROUP_BUTTON_CLICK(RIGHT_4_GROUP_ID,OnFunctionClick)		// Ʊ�۰�ť	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

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
	m_lineGroupArea = NULL;	//��ʼ������ΪNULL

	// ��·ѡ��ť
	m_BTN_Top_1_Group_Info.rows = 1;
	m_BTN_Top_1_Group_Info.cols = 6;
	m_BTN_Left_1_Group_Info.rows = 9;
	m_BTN_Left_1_Group_Info.cols = 1;
	m_BTN_Left_1_Group_Info.title = _T("");
	//m_BTN_Left_2_Group_Info.rows = 4;
	//m_BTN_Left_2_Group_Info.cols = 2;

	// �Ҳ�Ʊ�۰�ť
	m_BTN_Right_1_Group_Info.rows = 7;
	m_BTN_Right_1_Group_Info.cols = 1;
	m_BTN_Right_1_Group_Info.title = _T("");

	m_dwMaxTicketAmount = theFunction_INFO.GetTicketMaxAmount();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:�����ɹ�    -1:����ʧ��

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
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);

	if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_AREA_MAP){
		theSERVICE_MGR.SetState(READY);
		// ������Ϣ����
		m_bShowBaseArea = false;
		m_pTipsDlg->ShowWindow(SW_HIDE);
		m_pTitleDlg->ShowWindow(SW_HIDE);
		//HideCompoment(BASE_AREA);
		HideCompoment(CASH_AREA);
		HideCompoment(BTN_OK);
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);
		if(m_lineGroupArea!=NULL) m_lineGroupArea->ShowWindow(SW_SHOW);
		// ���ð�ť��
		m_Left_1_Group->EnableWindow(TRUE);
		m_Right_1_Group->EnableWindow(TRUE);
		m_Left_1_Group->ClearSelection();
		m_Right_1_Group->ClearSelection();
		//m_Left_1_Group->Invalidate(TRUE);
		//m_Left_1_Group->UpdateWindow();
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(true);
		m_cashInInfo->SetContent(_T("0.0Ԫ"));
		CString szCont = _T("��ѡ��Ŀ�ĳ�վ��Ʊ�۹�Ʊ\r\n��ֵ�������[��ֵ]��ť");
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(szCont);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowPayMethodButton(false,false);

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ֮ǰ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::preShow(){
	__super::preShow();

	LOG("CIssueAreaMapDlg::preShow--- m_bServiceNewStart = %d", pSvc->m_bServiceNewStart?1:0);
	m_pAllLineDlg->End(0);
	if (pSvc->m_bServiceNewStart){
		pSvc->m_bServiceNewStart = false;
		InitializeLineGroup();
		// �������Ҳ๦��������Ҫ�ж��Ƿ�֧�ַ��۵���Ʊ
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
		//// ��ʾ��������ť������ѡ��ť������ȡ����ť
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
		//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������ʾ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::showComplete(){
	__super::showComplete();
	this->RedrawWindow();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		����ѡ��ť�仯ʱ��Ӧ����  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueAreaMapDlg::OnClientLanguageModeChanged(LANGUAGETYPE_T& langue)
{	
	// ����Ϊ�ȴ���...
	theSERVICE_MGR.SetState(READY);
	// ��ʼ������ť
	InitializeLineGroup();

	// �������Ҳ๦��������Ҫ�ж��Ƿ�֧�ַ��۵���Ʊ
	long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
	m_Left_1_Group->ShowButtons(false);//�������Ҫ
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
@brief      �ȵ㳵վ��ť�����Ӧ�¼�

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)  ��ť֪ͨ��ϢΪ��վ����

@retval     

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnTopStationClick(WPARAM wParam, LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	// ��ǰվ����Ӧ
	if(_ttoi(btnInfo->btnMsg) == theMAINTENANCE_INFO.GetStationCodeWithLine()){
		theSERVICE_MGR.SetState(READY);
		return TRUE;
	}	

	m_wCurrClickStation = _ttoi(btnInfo->btnMsg);//ע�⣬�����վ��������ĵط���ͬ
	// �ж��Ƿ�֧�ַ��۵���Ʊ
	if(!IsSupportIssueCard())
	{
		return FALSE;
	}

	pSvc->GetDataModel().productInfo.dtValidateDateTime = ComGetCurTime();
	pSvc->SetProductType(SJT);					// ��Ʒ���ͣ�һ���ǵ���Ʊ
	pSvc->SetSubProductType(0);					// ��Ʒ�����ͣ�SJT
	pSvc->SetBeginStation();						// ��ʼ��վ��һ���ǵ�ǰ��վ
	pSvc->SetEndStation(_ttoi(btnInfo->btnMsg));	// Ŀ�ĳ�վ��

	if(pSvc->CalculatePrice()==RW_RSP_NOT_AVALIABLE_STATION){// ��Ʊ���ۣ�
		CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_NOT_SALE_TO_THIS_STATION),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		return FALSE;
	}
	pSvc->SetTicketPiece(1);						// Ĭ�����÷���һ��	
	pSvc->DoneToNextDialog();						// ת��֧������

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��·��尴ť�����Ӧ�¼�

@param      WPARAM wParam groupID

@retval     LPARAM lParam(BTN_INFO*)  ��ť֪ͨ��Ϣ����תҳID��վ����LocationCode

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::XLineGroupButtonClick(WPARAM wParam, LPARAM lParam)
{	// ���ó˿Ͳ�����
	theSERVICE_MGR.SetState(RECEPT);
	//pLocationDlg->ShowWindow(SW_HIDE);
	// ִ���������۲���
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD layoutID = _ttoi(btnInfo->btnMsg);
	// ��ǰվ����Ӧ
	if(btnInfo->btnID == theMAINTENANCE_INFO.GetStationCodeWithLine()){
		theSERVICE_MGR.SetState(READY);
		return TRUE;
	}	
	
	// ��վ��ť
	if (layoutID == 0)
	{
		m_wCurrClickStation = btnInfo->btnID;
		// �ж��Ƿ�֧����Ʊ
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
		pSvc->SetProductType(SJT);										// ��Ʒ���ͣ�һ���ǵ���Ʊ
		pSvc->SetSubProductType(0);										// ��Ʒ�����ͣ���ΪĬ��0xFF
		pSvc->SetBeginStation();											// ��ʼ��վ��һ���ǵ�ǰ��վ
		pSvc->SetEndStation(locationNum);									// Ŀ�ĳ�վ��
		if(pSvc->CalculatePrice()==RW_RSP_NOT_AVALIABLE_STATION){// ��Ʊ���ۣ�
			CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_NOT_SALE_TO_THIS_STATION),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
			return FALSE;
		}
		pSvc->SetTicketPiece(1);											// Ĭ�����÷���һ��	
		pSvc->DoneToNextDialog();											// ת��֧������
		
	}
	// ��ת��ť
	else
	{
		if (m_currentLayoutID == layoutID || !InitializeLineGroup(layoutID))
		{
			return TRUE;
		}

		//InitializeFunctionBtns(TOP_STATION_BUTTON);lichao:�����ȵ㳵վ

		// �����ɵĲ��� -> �����µĲ��� -> ��ֵ�µĲ��� -> ɾ���ɵĲ���
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
			// ʹ������Ч����Ҫ���»���
			m_Right_1_Group->Invalidate();
			m_Left_1_Group->Invalidate();
			//m_Right_2_Group->UpdateWindow();
			theSERVICE_MGR.SetState(READY);
			theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_NORMAL);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
			// ��ʾ��������ť������ѡ��ť������ȡ����ť

			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
			//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);//lichao: ��ʱ���������л���ť
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
@brief		�ж��Ƿ�֧����Token  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueAreaMapDlg::IsSupportIssueCard(){
	// 1.�жϵ�ǰģʽ�Ƿ�֧���ۿ���
	WORD device_service_mode = theTVM_STATUS_MGR.GetWorkMode();
	// 1.1�ж��Ƿ�֧���ۿ�
	if((device_service_mode & DEVICE_CONTRL_CODE_BUSINESS_CARD) != DEVICE_CONTRL_CODE_BUSINESS_CARD){		
		// 1.2��ʾû�п�����
		CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_SORRY_NO_CARD_AVALIABLE),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		// 1.2��յ�����ȵ㳵վѡ��״̬
		m_Right_1_Group->ClearSelection();
		m_Left_1_Group->ClearSelection();
		// 1.3��ʱ���治��ת�����Ǵ���Ĭ��ҵ������״̬Ϊ�ȴ�״̬
		theSERVICE_MGR.SetState(READY);
		return false;
	}
	// 2.��Ʊ��������
	if(theISSUE_PERMIT.IsPermitIssueStation(m_wCurrClickStation) && m_wCurrClickStation != 0){
		// 1.2��ʾû�п�����
		CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_GUIDE_NOT_SALE_TO_THIS_STATION),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		// 1.2��յ�����ȵ㳵վѡ��״̬
		m_Right_1_Group->ClearSelection();
		m_Left_1_Group->ClearSelection();
		// 1.3��ʱ���治��ת�����Ǵ���Ĭ��ҵ������״̬Ϊ�ȴ�״̬
		theSERVICE_MGR.SetState(READY);
		return false;
	}

	// 2.�ж��Ƿ�֧����Token
	/*if (!pSvc->SupportSJT()){
	return false;
	}*/
	// 3.�ж�Token�����Ƿ�֧��
	//CTTokenCountInfo::TOKEN_BOX_INFO Token_Box_A = theTOKEN_COUNT.GetHopperAInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO Token_Box_B = theTOKEN_COUNT.GetHopperBInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO Token_Box_Wast = theTOKEN_COUNT.GetTokenboxWastedInfo();

	//CAfcTVMOperationParam::MAGAZINE_PARAM magazine_Box;
	//theAFC_TVM_OPERATION.GetMagazineParam(magazine_Box);
	//if (Token_Box_A.ulCurCount < magazine_Box.TokenAEmptyValue && Token_Box_B.ulCurCount < magazine_Box.TokenBEmptyValue )
	//{
	//	if(!theAPP_SESSION.IsBlindManOperation()){
	//		// 3.1��ʾû��token����
	//		CMessagePictureBox::Show(_T("")/*_tl(TXT_ISSUE_INFO)*/,_tl(TXT_GUIDE_SORRY_NO_TOKEN_AVALIABLE),NULL/*TVM_IMAGE_BOX_PICTURE*/,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
	//		// 3.2��յ�����ȵ㳵վѡ��״̬
	//		m_Right_2_Group->ClearSelection();
	//		// 3.3��ʱ���治��ת�����Ǵ���Ĭ��ҵ������״̬Ϊ�ȴ�״̬
	//		theSERVICE_MGR.SetState(READY);
	//	}
	//	else{
	//		// ��ǰ��֧�ִ�Ʊ��
	//		pSvc->PlayAudioInDlg(AUDIO_COMMON_MSG_NOT_SUPPORT_TICKET_TYPE);
	//	}
	//	return false;
	//}
	// 4.��ʾ�Ƿ�֧�ִ�ӡ��
	if(false/*theTVM_STATUS_MGR.GetPrintMode() != 0*/){
		if(!theAPP_SESSION.IsBlindManOperation()){
			INT_PTR res = CMessagePictureBox::Show(_tl(TXT_ISSUE_INFO),_tl(TXT_NOPRINT_CONTINUE)/*_T("��ǰ��֧�ִ�ӡ���Ƿ������")*/,NULL,CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);

			if(IDCANCEL == res || IDABORT == res){
				m_Right_1_Group->ClearSelection();
				m_Left_1_Group->ClearSelection();
				theSERVICE_MGR.SetState(READY);
				return false;
			}
		}			
	}

	// 5.��Ʊ��������
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ʊ��ѡ��ť

@param      WPARAM wParam;
			LPARAM lParam;

@retval     LRESULT TRUE,FALSE

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnPriceTypeClick(WPARAM wParam,LPARAM lParam){

	// �жϵ�ǰ�Ƿ�֧�ֵ���Ʊ
	BTN_INFO* btnInfo = (BTN_INFO*)lParam; 
	// �ж��Ƿ�֧����Ʊ
	if(!IsSupportIssueCard())
	{
		return FALSE;
	}
	pSvc->GetDataModel().productInfo.dtValidateDateTime = ComGetCurTime();
	pSvc->SetProductType(SJT);						// ��Ʒ���ͣ�һ���ǵ���Ʊ
	pSvc->SetSubProductType(0);						// ��Ʒ�����ͣ�SJT
	pSvc->SetBeginStation();						// ��ʼ��վ��һ���ǵ�ǰ��վ
	//pSvc->SetEndStation(_ttoi(btnInfo->btnMsg));	// Ŀ�ĳ�վ
	pSvc->SetTicketAmount(_ttoi(btnInfo->btnMsg));	// ���õ���
	pSvc->SetTicketPiece(pSvc->GetDataModel().productInfo.nProductPiece >0?pSvc->GetDataModel().productInfo.nProductPiece:1);						// Ĭ�����÷���һ��
	pSvc->DoneToNextDialog();						// ת��֧������

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��·ѡ��ť�������

@param      WPARAM wParam;
			LPARAM lParam;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnLineGroupClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btn = (BTN_INFO*)lParam;
	// ��鰴ť����¼�
	DWORD dwLineId = 0;
	if(NULL != btn){
		dwLineId = _ttoi(btn->btnMsg);
		if (dwLineId==0){//��ͼ��ԭ
			theSERVICE_MGR.SetState(READY);
			long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
			InitializeFunctionBtns(component);
			//m_Left_1_Group->Invalidate(TRUE);
			//m_Left_1_Group->UpdateWindow();
		}
		else if(dwLineId==255){//ȫ����·
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
			// ѡ�ж�Ӧ��·��ť
			CString strLineId;
			strLineId.Format(_T("%d"), dwLineId);
			m_Left_1_Group->ClearSelection();
			m_Left_1_Group->SetSelection(strLineId);
		}

		// ���䵽��·
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
@brief      ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnFunctionClick(WPARAM wParam,LPARAM lParam){

	BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
	Menu_Item* pMenuItem=theMENU_MGR.GetMainMenuItemByID((MENU_ITEM_ID)pBtnInfo->btnID);
	if(pMenuItem!=NULL)
	{
		SERVICE_ID serviceID=pMenuItem->serviceID;	
		// �жϵ�ǰ����ǳ�ֵҵ���Ƿ�֧�ֳ�ֵҵ��
		if (CHARGE_SVC == serviceID){
			// ��ʾ��ǰ��֧�ֳ�ֵҵ��
			if (!theAPP_SESSION.IsRechargeServerEnabled()
				|| theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
				|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
				|| (theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) != DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_RECHARGE_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				// ���ð�ťΪδѡ��״̬
				return TRUE;
			}
			if (theTVM_STATUS_MGR.GetPrintMode() != 0){
				INT_PTR res = CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_NOPRINT_CONTINUE),NULL,CMessagePictureBox::BTN_OKCANCEL,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_CANCEL);
				if(IDCANCEL == res || IDABORT == res){
					theSERVICE_MGR.SetState(READY);
					// ���ð�ťΪδѡ��״̬
					return TRUE;
				}		
			}
		}
		else if (ANALYZE_SVC == serviceID){
			// ��ʾ��ǰ��֧��ҵ��
			if (theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
				|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
				|| (theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_QUERY) != DEVICE_CONTRL_CODE_BUSINESS_QUERY){
					CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_ANALYZE_SVC),NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
					theSERVICE_MGR.SetState(READY);
					// ���ð�ťΪδѡ��״̬
					return TRUE;
			}
		}
		else if ( serviceID == ISSUE_USE_SVT_SVC){			// ��ѯ��ֵ����Ʊҵ���Ƿ����
			// �жϵ�ǰ�Ƿ�֧�ֳ�ֵҵ��
			if (theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)
				|| theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)
				|| (theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_SVT) != DEVICE_CONTRL_CODE_PAYMENT_SVT){
				CMessagePictureBox::Show(_T("")/*_tl(TXT_CHARGE_INFO)*/,_tl(TXT_PSG_NO_SVT_ISSUE)/*_tl(TXT_PSG_NO_RECHARGE_SVC)*/,NULL,CMessagePictureBox::BTN_OK,DT_CENTER,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				theSERVICE_MGR.SetState(READY);
				// ���ð�ťΪδѡ��״̬
				//m_Right_4_Group->ClearSelection();
			}
			else{
				if(pSvc->m_ePayType != CIssueSvc::PAY_WITH_SVT){
					pSvc->m_ePayType = CIssueSvc::PAY_WITH_SVT;
					//m_Right_2_Group->ClickFirstAvailButton();
				}
				else{
					pSvc->m_ePayType = CIssueSvc::PAY_WITH_NORMAL;
					// ���ð�ťΪδѡ��״̬
					//m_Right_4_Group->ClearSelection();
				}
			}

			return TRUE;
		}
		else if ( serviceID == LANGUAGE_SWITH_SVC){//�����л�
			LANGUAGETYPE_T curLanguageId = theAPP_SESSION.GetCurrentClientLanguageType();
			if (curLanguageId==LANGUAGE_ID_CHINESE){
				// �������õ�session
				theAPP_SESSION.SetCurrentClientLanguageType(LANGUAGE_ID_ENGLISH);
			}
			else{
				// �������õ�session
				theAPP_SESSION.SetCurrentClientLanguageType(LANGUAGE_ID_CHINESE);
			}
			// ���ð�ťΪδѡ��״̬
			//m_Right_4_Group->ClearSelection();
			return TRUE;
		}

		// ����Service
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
@brief      ��Ʊ����ѡ��ť

@param      WPARAM wParam;
			LPARAM lParam;

@retval     LRESULT TRUE,FALSE

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueAreaMapDlg::OnTicketNumClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	pSvc->SetTicketPiece(_ttoi(btnInfo->btnMsg));
	pSvc->NotifyDataModelChanged();
	//// ���ö�ʱ��
	//pSvc->ResetMoneyNeedTimer();

	return TRUE;
}


//
////////////////////////////////////////////////////////////////////////////
///*
//@brief      �����л�
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//LRESULT CIssueAreaMapDlg::OnLanguageClick(WPARAM wParam,LPARAM lParam){
//
//	BTN_INFO* pBtnInfo = (BTN_INFO*)lParam;
//	LANGUAGETYPE_T selectLanguage = (LANGUAGETYPE_T)pBtnInfo->btnID;
//
//	// �������õ�session
//	theAPP_SESSION.SetCurrentClientLanguageType(selectLanguage);
//	return TRUE;
//}
