#include "stdafx.h"
#include "PassengerResultDlg.h"
#include "ChargeSvc.h"
#include "tickethelper.h"
#include "GuideInfo.h"


IMPLEMENT_DYNAMIC(CPassengerResultDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CPassengerResultDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_BACK_CLICK(OnCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPassengerResultDlg::CPassengerResultDlg(CService* pService) : CReceptionDlg(CPassengerResultDlg::IDD, pService)
{
	pSvc = dynamic_cast<CPassengerForeSvc*>(pService);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPassengerResultDlg::~CPassengerResultDlg()
{
	if (m_guideInfoArea!=NULL){
		delete m_guideInfoArea;
		m_guideInfoArea = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����ʾ������Ϣ

@param      (i)long lComponents  �Զ��������Ϣ

@retval     None 

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerResultDlg::ShowGuideArea()
{
	if(NULL!=m_guideInfoArea)
	{
		m_guideInfoArea->setGuideInfo(m_guideInfo);
		//ע�������򣬷���ֻ�в��ֽ���ˢ�¡�
		//m_guideInfoArea->Invalidate(FALSE);	
		m_guideInfoArea->renderGuideInfo();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerResultDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
	CPassengerForeSvc::CommonModel &model = pSvc->GetCommonModel();

	DWORD textSize = theGUI_INFO.GetRecTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	CString textFontName = theGUI_INFO.GetRecTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());
	// ��ϸ��Ϣ
	CString strText;
	CString strMoneyReturn = _tl(TXT_ALL_THE_INSERTED_CASH);
	strMoneyReturn.Replace(_T("999"),ComFormatAmount(model.paymentInfo.lPaidAmount)); // ����滻
	strMoneyReturn.Replace(_T("MOP"),_tl(MONEY_UNIT));
	strText.Format(_T("%s %s %s %s"),_tl(TXT_ALL_THE_INSERTED_CASH),ComFormatAmount(model.paymentInfo.lPaidAmount),_tl(MONEY_UNIT),_tl(TXT_WILL_BE_RETURNED));

	bool bPrintFalied = model.operationInfo.failurePrintState == CPassengerForeSvc::FAILURE_PRINT_STATE_FAIL;
	// 1.�쳣����
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		CRect m_error_msg_rc = CRect(50,280,650,380);
		DrawText(pDC, m_error_msg_rc,_tl(TXT_TRANSACTION_FAILED), textFontName,textSize + 30,FW_BOLD, BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		CRect m_error_dealInfo_rc = CRect(50,400,650,500);
		DrawText(pDC, m_error_dealInfo_rc,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER), textFontName,textSize,FW_BOLD, BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 2.ȡ������
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		// 2.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_CANCELLED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER);
		// 2.2��ϸ��Ϣ
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 2.3��ʾ��Ϣ
		CString strCol = _tl(TXT_COLLECT_YOUR_CASH);
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,strCol/*_tl(TXT_COLLECT_YOUR_CASH)*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 3.Ͷ�ֽ���޻���
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
		// 3.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 3.2��ϸ��Ϣ
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 3.3��ʾ��Ϣ
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 4.Ͷ�ֽ��������޻���(���Ų��������ͽ��ޣ���ʾ������һ��)
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT){
		// 4.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_INSERTED_EXCEEDS_THE_MAX_AMOUNT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 4.2��ϸ��Ϣ
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 4.3��ʾ��Ϣ
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 5.�����������޻���
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT){
		// 5.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_CHANGE_EXCEEDS_THE_MAX_COUNT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 5.2��ϸ��Ϣ
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 5.3��ʾ��Ϣ
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 6.���㲻��
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
		// 6.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_SORRY_NOT_ENOUGH_CHANGE),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.2��ϸ��Ϣ
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.3��ʾ��Ϣ
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 7.��ֵʧ����Ǯ����
	if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL){
		//����
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 9.2ƾ��
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(TXT_GUIDE_GET_MONEY_AND_CONTECT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 8.ͶǮ��ʱ��Ǯ����
	if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
		// 6.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_CANCELLED_TIME_OUT),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.2��ϸ��Ϣ
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,strMoneyReturn/*strText*/,textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 6.3��ʾ��Ϣ
		CRect Guid_Msg_Rect = CRect(50,520,650,620);
		DrawText(pDC,Guid_Msg_Rect,_tl(TXT_COLLECT_YOUR_CASH),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 9.Ʊ�����㣬�޷���Ʊ
	if (model.operationInfo.finishType ==  CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING){
		// 9.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER);
		// 9.2ƾ��
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER/*TXT_GUIDE_GET_MONEY_AND_CONTECT*/),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
	}
	// 10.����ʧ�ܡ�
	if (model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
		// 10.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 10.2ƾ��
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER | DT_WORDBREAK);
	}
	// 11.����ʧ��
	if(model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
		// 11.1ԭ��
		CRect Cancel_Msg_Rect = CRect(50,280,650,380);
		DrawText(pDC,Cancel_Msg_Rect,_tl(TXT_TRANSACTION_FAILED),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER|DT_WORDBREAK);
		// 11.2ƾ��
		CRect Detail_Msg_Rect = CRect(50,400,650,500);
		DrawText(pDC,Detail_Msg_Rect,_tl(bPrintFalied ? TXT_GUIDE_PRINT_FAILED_CONTACT_OFFICER : TXT_CONTACT_OFFICER),textFontName,textSize,FW_BOLD,BLACK,DT_CENTER|DT_VCENTER | DT_WORDBREAK);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
			0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
int CPassengerResultDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_guideInfoArea = new CReceptionGuideInfo();
	m_guideInfoArea->createWnd(this);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerResultDlg::UpdateUI()
{
	CPassengerForeSvc::CommonModel &model = pSvc->GetCommonModel();

	// �쳣����
	if (model.operationInfo.finishType == CPassengerForeSvc::EXCEPTION_ERROR){
		m_guideInfo->tipsCount = 2;
		// �쳣��ʾ
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_WARINGS;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}
	// ȡ������
	if (model.operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// Ͷ�ֽ���޻���
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// Ͷ�ֽ��������޻���
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// �����������޻���������㲻�����
	if (model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT ||
		model.operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// ��ֵʧ����Ǯ����
	if (model.operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL||
		model.operationInfo.finishType == CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// ͶǮ��ʱ��Ǯ����
	if (model.operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_IMAGE_TIP_RETURN_NOTE;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// ����ʧ��
	if(model.operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_WARINGS;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}

	// ����ʧ��
	if(model.operationInfo.finishType == CPassengerForeSvc::CHANGE_FALIED_RETURN){
		m_guideInfo->tipsCount = 2;
		m_guideInfo->detailList[0].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[0].strfileName = TVM_TIP_WARINGS;

		m_guideInfo->detailList[1].bitInfo =  _T("")/*_tl(TXT_TICKET_INFO)*/;
		m_guideInfo->detailList[1].strfileName = TVM_TIP_GET_RETURN_MONEY;
		ShowGuideArea();
	}
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
void CPassengerResultDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	m_btnOK->SetSelect(false);
	CPassengerForeSvc::CommonModel &model = pSvc->GetCommonModel();
	if(model.operationInfo.banknoteAcceptState != CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_INSERTED){
		// �˿ͷ�����ҵ��������棬Ӧ����״̬Ϊ���״̬
		theSERVICE_MGR.SetState(FINISH);
		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȡ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPassengerResultDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	// ���ذ�ť������Ĭ��Service
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	pMainSvc->StartDefaultService();
	return TRUE;
}
