#include "stdafx.h"
#include "IssueWithCashDlg.h"
#include "ParseStationMap.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CIssueWithCashDlg, CReceptionDlg)

	BEGIN_MESSAGE_MAP(CIssueWithCashDlg, CReceptionDlg)
		ON_WM_CREATE()
		ON_WM_ACTIVATE()
		ON_WM_CTLCOLOR()
		ON_BTN_BACK_CLICK(OnCancel)
		ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPriceTypeClick)	// Ʊ�۰�ť
		ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnTicketNumClick)	// ��Ʊ������ť
		ON_BTN_OK_CLICK(OnOK)										// ���ȡ����ť
		ON_BTN_OkAndCancel1_CLICK(OnOkAndCancel1)					// ���ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��
		ON_BTN_OkAndCancel2_CLICK(OnOkAndCancel2)					// ���ȡ����ť(ȷ����ȡ����ť�е�ȡ����ť��

		ON_MESSAGE(WM_NOTIFY_START_RECEIVE, OnNotifyStartReceive)	
		ON_MESSAGE(WM_NOTIFY_ISSUING, OnNotifyIssuing)	
		ON_MESSAGE(WM_START_ISSUING, OnStartIssue)	
		ON_MESSAGE(WM_NOTIFY_ACCEPTED, OnNotifyAccepted)
		//ON_MESSAGE(WM_NOTIFY_ERWEIMA_ORDER_RES, OnNotifyErweimaResult)
		ON_MESSAGE(WM_UPDATE_RECEPITON_GUIDE, OnUpdateGuide)
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���캯��

	@param      (i)CWnd*     pParent

	@retval     None

	@exception  None
	*/
	//////////////////////////////////////////////////////////////////////////
	CIssueWithCashDlg::CIssueWithCashDlg(CService* pService)
		: CReceptionDlg(CIssueWithCashDlg::IDD, pService)
	{
		m_pTipsDlg = new CStatic;

		pSvc = (CIssueSvc*)pService;

		// �����·ѡ��ť
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


		m_baseInfo->stytle = ISSUE_INFO_STYLE;
		m_baseInfo->point = CPoint(line_map_posX, BASEINFO_AREA_POINT.y);
		m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_SELECT_COUNT);
		m_baseInfo->detailInfo.labelGroup = m_TipsInfoGroup;
		m_baseInfo->detailInfo.row = 4;

		m_cashInfo->stytle = ISSUE_INFO_STYLE;
		m_cashInfo->point = RECT_BASE.TopLeft();/*CPoint(line_map_posX, CASHINFO_AREA_POINT.y);*/
		m_cashInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);
		m_cashInfo->detailInfo.labelGroup = m_TicketInfoGroup;
		m_cashInfo->detailInfo.row = MAX_TICKET_INFO_ROWS;
		m_cashInfo->size = RECT_BASE.Size();
		m_dwMaxTicketAmount = theFunction_INFO.GetTicketMaxAmount();
		CString szPath = _T("");
		GetAppPath(szPath);
		szPath += _T(".\\res\\TVM_center_little.png");
		m_bkgImage.Load(szPath);
		m_pImageBack_center = &m_bkgImage;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��������

	@param      None

	@retval     None

	@exception  None
	*/
	//////////////////////////////////////////////////////////////////////////
	CIssueWithCashDlg::~CIssueWithCashDlg()
	{

	}

	////////////////////////////////////////////////////////////////////////////
	/**
	@brief      ���洴������

	@param      (i)LPCREATESTRUCT lpCreateStruct

	@retval     int     0:�����ɹ�    -1:����ʧ��

	@exception  None
	*/
	//////////////////////////////////////////////////////////////////////////
	int CIssueWithCashDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (__super::OnCreate(lpCreateStruct, LINE_AREA | PRICES_AREA | NUMBER_AREA | TOP_STATION_AREA | FUNC_AREA  | CASH_AREA | GUIDE_AREA | CASH_COUNT_AREA) == -1) {
			return -1;
		}
		m_cashInInfo->ShowWindow(true);
		m_pTipsDlg->Create(_T(""), WS_CHILD, CRect(1140+50,850+30,1350+50,890+30), this, ID_TIPS_DLG);
// 		SetShowAreaInfo(CASH_AREA,m_cashInfo);
		m_strNotifyInfo = _T("");
		m_strTipInfo = _T("");

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
	void CIssueWithCashDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{		
		__super::OnActivate(nState,pWndOther,bMinimized);
		CIssueSvc::Model& model = pSvc->GetDataModel();
		theSERVICE_MGR.SetState(RECEPT);
		if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH){
			// ��ʾ��Ϣ����
			m_strNotifyInfo = _tl(TXT_TICKET_SELECT_COUNT_B);
			// ��ά��֧����ʽ�Ľ�������WEB�����������û򱾵ش���δ��������
			if (!theWEBSERVER_HELPER.GetConnectFlag() || theWEBSERVER_HELPER.IsExistLocalOrder()){
				//m_btnOKAndCancel1->EnableWindow(FALSE);
				m_btnOKAndCancel2->EnableWindow(FALSE);
			}
			else{
				//m_btnOKAndCancel1->EnableWindow(TRUE);
				m_btnOKAndCancel2->EnableWindow(TRUE);
			}
			model.paymentInfo.paymentMethod =  PAYMENT_METHOD_UNSPECIFIED;
			//// ���Ϳ�ʼ��Ǯ��Ϣ
			//PostMessage(WM_NOTIFY_START_RECEIVE, 0, 0);
		}
		else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_FINISH){
			theSERVICE_MGR.SetState(FINISH);
			m_strNotifyInfo = _tl(GUIDE_ISSUE_FINISH_B);
		}
		else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_ERROR){
			LOG("in error dialog, %d", model.operationInfo.finishType);
			theSERVICE_MGR.SetState(FINISH);
			if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + _tl(TXT_CONTACT_OFFICER);
			}
			if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL){
				CString strTmp = _tl(TXT_GUIDE_GET_MONEY_AND_CONTECT);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
				CString strTmp;
				strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_CANCELLED) + strTmp;

			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
				CString strTmp;
				strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT){
				CString strTmp;
				strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
				CString strTmp;
				strTmp =_tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_CHANGE_EXCEEDS_THE_MAX_COUNT) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
				CString strTmp;
				strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_SORRY_NOT_ENOUGH_CHANGE) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
				CString strTmp;
				strTmp = _tl(TXT_GUIDE_REFOND_AMOUNT) + ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(TXT_GUIDE_REFOND_AMOUNT_BACK);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_CANCELLED_TIME_OUT) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING){
				CString strTmp = _tl(TXT_CONTACT_OFFICER);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
				CString strTmp = _tl(TXT_CONTACT_OFFICER);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
			}
			else if (model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
				CString strTmp = _tl(TXT_CONTACT_OFFICER);
				m_strNotifyInfo = TXT_COLOR_RED + _tl(TXT_TRANSACTION_FAILED) + strTmp;
			}
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
	void CIssueWithCashDlg::preShow(){
		__super::preShow();

		// ���⸳ֵ
		m_baseInfo->titleInfo.titleName = _tl(TXT_TICKET_SELECT_COUNT);
		m_cashInfo->titleInfo.titleName = _tl(TXT_TICKET_INFO);

		CIssueSvc::Model& model = pSvc->GetDataModel();
		if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH){
			m_strNotifyInfo = _tl(TXT_TICKET_SELECT_COUNT_B);
			// ���ð�ť��
			long component = LINE_SELECT_BUTTON | PRICE_BUTTON | NUMBER_BUTTON |TOP_STATION_BUTTON | FUNC_SELECT_BUTTON ;
			InitializeFunctionBtns(component);
			// ���ð�ť��ѡ��
			vector<int> vecFareList;
			vecFareList = pSvc->GetTicketAmountTable();
			for (int i=0;i<vecFareList.size();i++){
				if(vecFareList[i]==model.productInfo.lProductSaleAmount){
					m_Right_1_Group->ClickButton(i);
				}
			}
			vector<int> vecCountList;
			vecCountList = pSvc->GetTicketCountTable();
			for (int i=0;i<vecCountList.size();i++){
				if(vecCountList[i]==model.productInfo.nProductPiece){
					m_Left_1_Group->ClickButton(i);
				}
			}
			// �ûҰ�ť��
			//m_Left_1_Group->EnableButtonGroup(FALSE);
			//m_Right_1_Group->EnableButtonGroup(FALSE);
			//m_Left_1_Group->UpdateWindow();
			//m_Right_1_Group->UpdateWindow();

			m_btnOK->SetText(_tl(STR_CANCEL));
			m_btnOK->SetSelect(false);
			m_btnOKAndCancel1->SetText(_tl(STR_CANCEL));
			m_btnOKAndCancel1->SetSelect(false);
			m_btnOKAndCancel2->SetText(_tl(TXT_TOW_DIMENSIONAL_PAYMENT));
			m_btnOKAndCancel2->SetSelect(false);
			m_btnOK->EnableWindow(TRUE);
			m_btnOKAndCancel1->EnableWindow(TRUE);
			m_btnOKAndCancel2->EnableWindow(TRUE);
			HideCompoment(BTN_OK);
			HideCompoment(BTN_OK_AND_CANCEL1);
			HideCompoment(BTN_OK_AND_CANCEL2);
		}
		else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_FINISH){
			if(model.lDeviceStatus == CPassengerForeSvc::DEVICE_ALL_OK){
				theSERVICE_MGR.SetState(FINISH);
				theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
			}

			// �ûҰ�ť��
			m_Left_1_Group->EnableButtonGroup(FALSE);
			m_Right_1_Group->EnableButtonGroup(FALSE);
			m_Left_1_Group->UpdateWindow();
			m_Right_1_Group->UpdateWindow();
			// ���ð�ťΪĬ����ʽ
			//m_btnOK->SetText(_tl(STR_BACK));
			//m_btnOK->MoveWindow(&REC_BTN_OK_RECT, FALSE);
			// 20171208, ֧������΢�ź�һ
			//m_btnOKAndCancel1->SetText(_T("ȷ��"));
			//m_btnOKAndCancel1->MoveWindow(&REC_BTN_OkAndCancel1_RECT, FALSE);
			//m_btnOKAndCancel2->SetText(_T("ȡ��"));
			//m_btnOKAndCancel2->MoveWindow(&REC_BTN_OkAndCancel2_RECT, FALSE);
			//ShowCompoment(BTN_OK);
			//m_btnOK->SetEnabled(true);
			//HideCompoment(BTN_OK_AND_CANCEL1);
			//HideCompoment(BTN_OK_AND_CANCEL2);
		}
		else if(pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_ERROR){
			// �ûҰ�ť��
			m_Left_1_Group->EnableButtonGroup(FALSE);
			m_Right_1_Group->EnableButtonGroup(FALSE);
			m_Left_1_Group->UpdateWindow();
			m_Right_1_Group->UpdateWindow();
			// ���ð�ťΪĬ����ʽ
			m_btnOK->SetText(_tl(STR_BACK));
			m_btnOK->MoveWindow(&REC_BTN_OK_RECT, FALSE);
			// 20171208, ֧������΢�ź�һ
			//m_btnOKAndCancel1->SetText(_T("ȷ��"));
			//m_btnOKAndCancel1->MoveWindow(&REC_BTN_OkAndCancel1_RECT, FALSE);
			//m_btnOKAndCancel2->SetText(_T("ȡ��"));
			//m_btnOKAndCancel2->MoveWindow(&REC_BTN_OkAndCancel2_RECT, FALSE);
			ShowCompoment(BTN_OK);
			m_btnOK->SetEnabled(true);
			HideCompoment(BTN_OK_AND_CANCEL1);
			HideCompoment(BTN_OK_AND_CANCEL2);
		}	
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

		// ��ʾ��Ϣ����
		InitializeBaseInfo();
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ������ʾ��

	@param      ��

	@retval     ��

	@exception  ��
	*/
	//////////////////////////////////////////////////////////////////////////
	void CIssueWithCashDlg::showComplete(){
		__super::showComplete();
		this->RedrawWindow();

	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

	@param      None

	@retval     None

	@exception  None
	*/
	//////////////////////////////////////////////////////////////////////////
	void CIssueWithCashDlg::UpdateUI()
	{
		InitializeBaseInfo();
		showPaymethodGuide();
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
	LRESULT CIssueWithCashDlg::OnPriceTypeClick(WPARAM wParam,LPARAM lParam){

		// �жϵ�ǰ�Ƿ�֧�ֵ���Ʊ
		BTN_INFO* btnInfo = (BTN_INFO*)lParam; 
		pSvc->GetDataModel().productInfo.dtValidateDateTime = ComGetCurTime();
		pSvc->SetProductType(SJT);						// ��Ʒ���ͣ�һ���ǵ���Ʊ
		pSvc->SetSubProductType(0);						// ��Ʒ�����ͣ�SJT
		pSvc->SetBeginStation();						// ��ʼ��վ��һ���ǵ�ǰ��վ
		//pSvc->SetEndStation(_ttoi(btnInfo->btnMsg));	// Ŀ�ĳ�վ
		pSvc->SetTicketAmount(_ttoi(btnInfo->btnMsg));	// ���õ���
		pSvc->NotifyDataModelChanged();
		InitializeBaseInfo();
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
	LRESULT CIssueWithCashDlg::OnTicketNumClick(WPARAM wParam,LPARAM lParam)
	{
		theSERVICE_MGR.SetState(RECEPT);
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		pSvc->SetTicketPiece(_ttoi(btnInfo->btnMsg));
		pSvc->NotifyDataModelChanged();//22065835
		InitializeBaseInfo();
		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief		���Label����  

	@param      none

	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CIssueWithCashDlg::ClearLabelData()
	{
		// ��ʼ��������Ϣ
		for(int n = 0;n < MAX_TICKET_INFO_ROWS;n++){
			m_TipsInfoGroup[n].nameLeft  = _T("");
			m_TipsInfoGroup[n].nameRight = _T("");
		}
		// ��ʼ����Ʒ��Ϣ
		for(int i = 0; i<MAX_TICKET_INFO_ROWS;i++){
			m_TicketInfoGroup[i].nameLeft  = _T("");
			m_TicketInfoGroup[i].nameRight = _T("");
		}

		for(int n = 0;n < 1;n++){
			m_NotifyInfoGroup[n].nameLeft  = _T("");
			m_NotifyInfoGroup[n].nameRight = _T("");
		}
		m_cashInfoArea->Invalidate(TRUE);
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ��ʼ��������Ϣ

	@param      None

	@retval     None 

	@exception  None
	*/
	//////////////////////////////////////////////////////////////////////////
	void CIssueWithCashDlg::InitializeBaseInfo()
	{	

		CIssueSvc::Model& model = pSvc->GetDataModel();
		LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
		// ��ʼ��Labelֵ
		if (model.operationInfo.finishType != CPassengerForeSvc::PASSAGE_CANCEL)
		{
			ClearLabelData();

		}

		DWORD textSize = theGUI_INFO.GetRecTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
		CString textFontName = theGUI_INFO.GetRecTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());

		// �쳣����
		if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
			CRect m_error_dealInfo_rc = CRect(CPoint(300,400),CSize(600,100));
			CString szAmount = _T("�˿��") + ComFormatAmount(model.paymentInfo.lPaidAmount >= 0?model.paymentInfo.lPaidAmount:0) + _tl(MONEY_UNIT);
			DrawText(this->GetDC(), m_error_dealInfo_rc,szAmount, textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		}
		// ȡ������
		else if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
			CRect m_error_dealInfo_rc = CRect(CPoint(300,400),CSize(600,100));
			CString szAmount = _T("�˿��") + ComFormatAmount(model.paymentInfo.lPaidAmount >= 0?model.paymentInfo.lPaidAmount:0) + _tl(MONEY_UNIT);
			DrawText(this->GetDC(), m_error_dealInfo_rc,szAmount, textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("���ú��˸������ֽ�ллʹ�ã�"));

		}
		// ���㳬�޻���
		else if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
			CRect m_cancle_msg_rc = CRect(50,280,450,380);
			DrawText(this->GetDC(), m_cancle_msg_rc,_T("����"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
			CRect m_refund_msg_rc = CRect(50,400,250,500);
			DrawText(this->GetDC(), m_refund_msg_rc,_T("�˿��"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
			CRect m_refund_amount_rc = CRect(280,400,450,500);
			DrawText(this->GetDC(), m_refund_amount_rc,ComFormatAmount(0)+_tl(MONEY_UNIT), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
			CRect m_refund_dealInfo_rc = CRect(50,600,450,500);
			DrawText(this->GetDC(), m_refund_dealInfo_rc,_T("���պ������ֽ�"), textFontName,textSize,FW_BOLD, RED,DT_CENTER|DT_VCENTER);
		}
		else
		{
			int index = 0;
			// ************* ��ʾ��Ϣ ***************
			CString strBanknoteTips = pSvc->BanknoteShowword();
			CString strCoinTips = pSvc->CoinShowword();
			m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(GUIDE_ISSUE_ACCEPTABLE_COIN), MAX_HALF_CASE_LEFT_LEN);
			m_TipsInfoGroup[index++].nameRight = strCoinTips;
			index++;
			m_TipsInfoGroup[index].nameLeft = add_hsp(_tl(GUIDE_ISSUE_ACCEPTABLE_NOTES), MAX_HALF_CASE_LEFT_LEN);
			m_TipsInfoGroup[index++].nameRight = strBanknoteTips;
			index++;

			// ************* ��Ʊ��Ϣ ***************
			index = 0;
			// ��Ʒ����
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_PRODUCT_TYPE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			m_TicketInfoGroup[index++].nameRight = theACC_PRODUCT.GetProductName(ComMakeWORD(model.productInfo.productType,model.productInfo.productSubType), language);
			index++;
			//// ��Ʒ��Ч��
			//m_TicketInfoGroup[index].nameLeft    = add_hsp(_tl(TXT_TICKET_VALIDATE_DATE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			//m_TicketInfoGroup[index++].nameRight = model.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d"));
			//index++;
			// ��ʼĿ�ĳ�վ/����
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_START_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			m_TicketInfoGroup[index++].nameRight = model.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.beginLocation, language);
			index++;		
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_END_STATION)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			m_TicketInfoGroup[index++].nameRight = model.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED ? CString(_tl(TXT_UNDEFINE)) : theACC_LOCATION.GetLocationName(model.productInfo.endLocation, language);		
			index++;
			// ��Ʒ����
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_ISSUE_AMOUNT)+_tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN) ;
			m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.productInfo.lProductSaleAmount) + _tl(MONEY_UNIT);
			index++;
			// ��������
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TICKET_COUNT)+_tl(TXT_COLON),MAX_HALF_CASE_LEFT_LEN) ;
			m_TicketInfoGroup[index++].nameRight =  ComFormatCount(model.productInfo.nProductPiece) ;
			index++;
			// Ӧ�����
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_TOTAL) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lDueAmount) + _tl(MONEY_UNIT);
			index++;
			// ʵ�ʸ�����
			m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_INCOME) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
			m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lPaidAmount) + _tl(MONEY_UNIT);
			index++;
			if (model.paymentInfo.lChangeAmount > 0)
			{
				// ������
				m_TicketInfoGroup[index].nameLeft = add_hsp(_tl(TXT_CHANGE) + _tl(TXT_COLON), MAX_HALF_CASE_LEFT_LEN);
				m_TicketInfoGroup[index++].nameRight = ComFormatAmount(model.paymentInfo.lChangeAmount >=0 ? model.paymentInfo.lChangeAmount : 0) + _tl(MONEY_UNIT);
				index++;

			}

			//************* ��Ϣ ***************
			index = 0;
			m_NotifyInfoGroup[index].nameLeft = m_strNotifyInfo;
			index++;

			CString szPaidMount = _T("");
			szPaidMount.Format(_T("%dԪ"),model.paymentInfo.lPaidAmount >=0 ? model.paymentInfo.lPaidAmount/100:0);//ComFormatAmount(model.paymentInfo.lPaidAmount >=0 ? model.paymentInfo.lPaidAmount : 0) + _tl(MONEY_UNIT);
			m_cashInInfo->SetContent(szPaidMount);
			m_cashInfoArea->Invalidate(FALSE);

		}


	}


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ����ȷ�ϰ�ť��Ϣ

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

	@exception  CSysException
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnOK(WPARAM wParam, LPARAM lParam)
	{
		if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH){
			// �����ʱ��δͶ���ֽ��򷵻���ͼ���棻����Ѿ�Ͷ��ֽ�ң����˳��ֽ�
			LOG("��Ʊ�����н��棬�˿͵��ȡ����ť");
			pSvc->OnCancel();
			//pSvc->GetDataModel().operationInfo.finishType = CPassengerForeSvc::PASSAGE_CANCEL;
			//pSvc->FlowToDefaultDlg();

		}
		else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_FINISH){
			// ������ͼ����
			pSvc->FlowToDefaultDlg();
		}
		else if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_ERROR){
			// ������ͼ����
			pSvc->FlowToDefaultDlg();
		}

		return TRUE;
	}
	LRESULT CIssueWithCashDlg::OnCancel(WPARAM wParam,LPARAM lParam)
	{
		//CIssueSvc::Model& model = pSvc->GetDataModel();
		//model.operationInfo.finishType = CPassengerForeSvc::PASSAGE_CANCEL;
		//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetCancleButtonEnable(false);
		// ��ʼ��Labelֵ
		ClearLabelData();
		m_Right_1_Group->EnableButtonGroup(false);
		m_Left_1_Group->EnableButtonGroup(false);
		m_Top_1_Group->EnableButtonGroup(false);
		m_cashInfo->titleInfo.titleName = _T("������ȡ��");
		//m_TicketInfoGroup[5].nameLeft = _T("Ӧ�˽��");
		//m_TicketInfoGroup[5].nameRight = _T("0.0Ԫ");
		//m_cashInfoArea->SetLableAlign(1,DT_CENTER);
		//m_cashInfoArea->SetLableAlign(5,0,DT_CENTER);
		//m_cashInfoArea->SetLableAlign(5,1,DT_CENTER);

		//m_cashInfoArea->SetLabelColor(5,0,RED);
		//m_cashInfoArea->SetLabelColor(5,1,RED);

		//int index = 0;
		//m_guideInfo->detailList[index].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		//m_guideInfo->detailList[index++].strfileName = TVM_TIP_GET_RETURN_CARD;

		//m_guideInfo->detailList[index].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		//m_guideInfo->detailList[index++].strfileName = TVM_TIP_GET_RETURN_MONEY;
		//m_guideInfo->tipsCount = index;
		//InitializeGuideArea();
		//m_guideInfoArea->renderGuideInfo();
		this->RedrawWindow();
		pSvc->OnCancel();
		return NULL;
	}


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �ؼ���ɫ���������

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

	@exception  CSysException
	*/
	//////////////////////////////////////////////////////////////////////////
	HBRUSH	CIssueWithCashDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
		if( ID_TIPS_DLG ==  pWnd->GetDlgCtrlID()){//�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
			CFont font;
			font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
				CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
				DEFAULT_PITCH|FF_SWISS, _T("����"));

			pDC->SelectObject(&font);
			pDC->SetTextColor(RGB(255,0,0));//�����ı���ɫΪ��ɫ
			//pDC->SetBkMode(OPAQUE);//�����ı�����ģʽΪ͸��
			//pDC->SetBkColor(RGB(0,0,255));//�����ı�����Ϊ��ɫ
			//hbr=CreateSolidBrush(RGB(0,255,0));//�ؼ��ı���ɫΪ��ɫ
		}
		else if( ID_TITLE_DLG ==  pWnd->GetDlgCtrlID()){//�жϷ�����Ϣ�Ŀռ��Ƿ��Ǹþ�̬�ı���
			CFont font;
			font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE+16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
				CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
				DEFAULT_PITCH|FF_SWISS, _T("����"));

			pDC->SelectObject(&font);
			pDC->SetTextColor(RGB(0,0,0));//�����ı���ɫΪ��ɫ
		}

		return hbr;//���������ػ�ؼ������Ļ�ˢ
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �յ���ʼ�����ֽ�֪ͨ

	@param      (i)UINT nIDEvent  

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnNotifyStartReceive(WPARAM wParam,LPARAM lParam)
	{
		LOG("OnNotifyStartReceive::�յ���ʼ�����ֽ�֪ͨ");
		// ��ʼ�����ֽ𣬲������������е���ʱ��ʱ��
		pSvc->InsertRecvStartTimer();
		pSvc->StartReceiveMoney();// ��ʼ����Ǯ�����п�

		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �յ���̨���۳�Ʊ��֪ͨ

	@param      (i)UINT nIDEvent  

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnNotifyIssuing(WPARAM wParam,LPARAM lParam)
	{
		LOG("OnNotifyIssuing::�յ���Ʊ������֪ͨ");
		pSvc->EndRecvTimer();
		// ���۳�Ʊ������
		m_strNotifyInfo = _tl(GUIDE_ISSUING);
		//HideCompoment(BTN_OK);
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);
		//pSvc->NotifyDataModelChanged();

		PostMessage(WM_START_ISSUING,0,0);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ��ʼ���۳�Ʊ

	@param      (i)UINT nIDEvent  

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnStartIssue(WPARAM wParam,LPARAM lParam)
	{
		LOG("OnStartIssue::���۳�Ʊ������");
		// ���۳�Ʊ������
		pSvc->StartIssueThread();
		pSvc->NotifyDataModelChanged();
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �յ���̨���ܵ��ֽ��֪ͨ

	@param      (i)UINT nIDEvent  

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnUpdateGuide(WPARAM wParam,LPARAM lParam)
	{
		//CIssueSvc::Model& model = pSvc->GetDataModel();
		CString strTips = (TCHAR*)(wParam);
		m_strNotifyInfo = TXT_COLOR_RED + strTips;
		pSvc->NotifyDataModelChanged();

		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      �յ���̨���ܵ��ֽ��֪ͨ

	@param      (i)UINT nIDEvent  

	@retval     

	@exception  
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnNotifyAccepted(WPARAM wParam,LPARAM lParam)
	{
		LOG("OnNotifyAccepted::");
		CIssueSvc::Model& model = pSvc->GetDataModel();

		m_Right_1_Group->EnableButtonGroup(FALSE); // Ʊ�۰�ť�鲻����
		//m_btnOKAndCancel1->EnableWindow(FALSE);// ΢�Ű�ť������	
		m_btnOKAndCancel2->EnableWindow(FALSE);// ֧������ť������	

		LOG("�������ٸ��Ĺ�Ʊ��Ϣ������%dԪ����Ʊ%d�š�", model.productInfo.lProductSaleAmount, model.productInfo.nProductPiece);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ������ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

	@exception  CSysException
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnOkAndCancel1(WPARAM wParam, LPARAM lParam)
	{
		CIssueSvc::Model& model = pSvc->GetDataModel();

		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);

		//theSERVICE_MGR.SetForeServiceBusy(true);
		//LOG("��¼��ʼʱ��:%s",ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
		//RETURN_CODE result = theWEBSERVER_HELPER.Weixin_RequesetOrder(
		//	model.productInfo.beginLocation, model.productInfo.endLocation, model.productInfo.lProductSaleAmount, model.productInfo.nProductPiece, 0);
		//LOG("��¼����ʱ��:%s",ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

		//if(result!=RSP_CODE_OK){
		//	theSERVICE_MGR.SetForeServiceBusy(false);
		//	m_btnOKAndCancel1->SetSelect(false);
		//	CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_T("���ӷ�����ʧ�ܣ�"),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		//	return TRUE;
		//}
		//// ѡ��΢��֧��
		//pSvc->m_ePayType = CIssueSvc::PAY_WITH_WEIXIN;
		//pSvc->SetPayment(PAYMENT_METHOD_WITH_WEIXIN);
		//pSvc->StopReceiveMoney();
		//pSvc->DoneToNextDialog();
		//
		//theSERVICE_MGR.SetForeServiceBusy(false);

		// 20171208, ֧������΢�ź�һ
		if (pSvc->GetDialogFlow()->GetCurDialogGroupID()==CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH){
			// �����ʱ��δͶ���ֽ��򷵻���ͼ���棻����Ѿ�Ͷ��ֽ�ң����˳��ֽ�
			LOG("��Ʊ�����н��棬�˿͵��ȡ����ť");
			pSvc->OnCancel();
		}

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      ������ȡ����ť(ȷ����ȡ����ť�е�ȷ����ť��

	@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
	@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

	@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

	@exception  CSysException
	*/
	//////////////////////////////////////////////////////////////////////////
	LRESULT CIssueWithCashDlg::OnOkAndCancel2(WPARAM wParam, LPARAM lParam)
	{
		CIssueSvc::Model& model = pSvc->GetDataModel();
		HideCompoment(BTN_OK_AND_CANCEL1);
		HideCompoment(BTN_OK_AND_CANCEL2);

		// ѡ��֧����֧��
		pSvc->m_ePayType = CIssueSvc::PAY_WITH_ZHIFUBAO;
		pSvc->SetPayment(PAYMENT_METHOD_WITH_ZHIFUBAO);
		pSvc->StopReceiveMoney();
		pSvc->DoneToNextDialog();

		return TRUE;
	}
	void CIssueWithCashDlg::InitializeGuideArea()
	{
		if(NULL!=m_guideInfoArea)
		{
			m_guideInfoArea->setGuideInfo(m_guideInfo);
			//m_guideInfoArea->Invalidate(FALSE);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief		֧������ʾ

	@param      
	@retval     none

	@exception  none
	*/
	//////////////////////////////////////////////////////////////////////////
	void CIssueWithCashDlg::showPaymethodGuide()
	{
		CIssueSvc::Model &model = pSvc->GetDataModel();
		// 1.�ֽ�֧��
		if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH || model.paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_NET){
			// ����֧����ʾ��Ϣ
			int index = 0;
			if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR
				|| model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
					// �쳣��ʾ
					m_guideInfo->detailList[index].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
					m_guideInfo->detailList[index++].strfileName = TVM_TIP_GET_RETURN_MONEY;

					m_guideInfo->detailList[index].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
					m_guideInfo->detailList[index++].strfileName = TVM_TIP_RETURN_COIN;
					m_guideInfo->tipsCount = index;		

			}
			else
			{
				if (theSERVICE_MGR.GetState() == ISSUEING)
				{
					index = 0;
					m_guideInfo->detailList[index].bitInfo = _T("");
					m_guideInfo->detailList[index++].strfileName = TVM_TIP_ISSUING;
					m_guideInfo->tipsCount = index;
					theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("���ڳ�Ʊ�����Ժ󡭡�"));

				}
				else if (pSvc->GetDialogFlow()->GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_ISSUE_WITH_CASH_FINISH)
				{
					index = 0;
					m_guideInfo->detailList[index].bitInfo = _T("");
					m_guideInfo->detailList[index++].strfileName = TVM_TIP_RETURN_COIN;
					m_guideInfo->tipsCount = index;
					theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("��ȡ��Ʊ�����㣬ллʹ�ã�"));

				}
				else
				{
					// Ӳ��
					if (!pSvc->CoinShowword().IsEmpty())
					{
						// ��֧��Ӳ��Ͷ�룿
						if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
							//m_guideInfo->detailList[index].bitInfo =  _tl(TXT_SHOW_NO_ACCEPT_COIN);
							m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_COINS;
						}
						else{
							m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_COIN;
							//m_guideInfo->detailList[index++].bitInfo =  pSvc->CoinShowword();
						}
					}
					// ֽ��
					if (!pSvc->BanknoteShowword().IsEmpty())
					{
						// �жϵ�ǰģʽ�Ƿ�֧��ֽ��Ͷ�룿
						// ��ǰģʽ������ֽ��Ͷ��
						if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
							//m_guideInfo->detailList[index].bitInfo =  _tl(TXT_SHOW_NO_ACCEPT_BANKNOTE);
							m_guideInfo->detailList[index++].strfileName = TVM_TIP_FORBID_INSERT_BANKNOTES;
						}
						else{
							m_guideInfo->detailList[index++].strfileName = TVM_IMAGE_TIP_INSERT_NOTE;

						}
					}
					//m_guideInfo->detailList[index-1].bitInfo =  _T("��Ͷ��ֽ�һ�Ӳ��");//pSvc->BanknoteShowword();
					theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T("��Ͷ��ֽ�ҡ�Ӳ��"));

					m_guideInfo->tipsCount = index;
				}
				

			}	

		}
		// 2.���п�֧��
		else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD){
			// ������ʾ����ʾ��Ϣ
			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES);
			m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_BANKCARD;
		}
		else if (model.paymentInfo.paymentMethod == PAYMENT_METHOD_UNSPECIFIED)
		{
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->SetNotifyContent(_T(""));

			m_guideInfo->tipsCount = 1;
			m_guideInfo->detailList[0].bitInfo =  _T("��ѡ��֧����ʽ");
			m_guideInfo->detailList[0].strfileName = _T("");
			//m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_INSERT_BANKCARD;
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowPayMethodButton(true);

		}
		InitializeGuideArea();
		m_guideInfoArea->renderGuideInfo();
	}
