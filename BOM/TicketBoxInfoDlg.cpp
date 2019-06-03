#include "stdafx.h"
#include "TicketBoxInfoDlg.h"
#include "ticketboxexchangesvc.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTicketBoxInfoDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CTicketBoxInfoDlg, COperationDlg)
	ON_WM_ACTIVATE()   
	ON_WM_CREATE()
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnMoveTypeBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnBoxTypeBtnClick)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()
// ��������29��EDIT���λ��
static CRect editComInfo[] = {	
	BASE_INFO_RECT_SP(2,3,26),
	BASE_INFO_RECT_SP(3,3,26),
	/*BASE_INFO_RECT_SP(4,3,17),*/
};
static const CString STR_MOVE_TYPE_MSG[] = {_T("��װ"),_T("ж��")};
static const CString STR_BOX_TYPE_MSG[] = {_T("Ʊ��A"),_T("Ʊ��B"),_T("��Ʊ��")};

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTicketBoxInfoDlg::CTicketBoxInfoDlg(CService* pService)
: COperationDlg(CTicketBoxInfoDlg::IDD,pService)
{
	m_baseInfo->titleInfo.titleName = "Ʊ����Ϣ";
	m_baseInfo->detailInfo.row = 4;           // ����
	m_baseInfo->detailInfo.labelGroup = 0;     // ��ǩ���֣�3�У�
	m_baseInfo->detailInfo.editGroup.cnt = 2;                               // �༭����Ŀ
	m_baseInfo->detailInfo.editGroup.editRect = editComInfo;            // �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTicketBoxInfoDlg::~CTicketBoxInfoDlg()
{
}
////////////////////////////////////////////////////////////////////////////
///*
//@brief      ȡ��Ʊ��������ֱ�ǩ
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::GetTicketInfoBeforeLabel()
{	
	CTicketBoxExchangeSvc* pExchangeBoxSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DataModel &model = pExchangeBoxSvc->GetDataModel();

	m_TicketInfoLabel[0].nameLeft = _T("λ ��");
	m_TicketInfoLabel[0].nameCenter = _T("�� ��");
	m_TicketInfoLabel[0].nameRight = _T("�� ��");

	m_TicketInfoLabel[1].nameLeft = _T("Ʊ��1");
	CCTLOGCountInfo::TICKETBOX_ID emptyTicketBoxID;
	if(model.ticketBoxAInfoBefore.ticketBoxID == emptyTicketBoxID){
		m_TicketInfoLabel[1].nameCenter =_T("δ��װ");
		m_TicketInfoLabel[1].nameRight = _T("");
	}
	else{
		m_TicketInfoLabel[1].nameCenter = model.ticketBoxAInfoBefore.ticketBoxID.ToString();
		m_TicketInfoLabel[1].nameRight.Format(_T("%d"),model.ticketBoxAInfoBefore.ulCurCount);
	}
	m_TicketInfoLabel[2].nameLeft = _T("Ʊ��2");
	if(model.ticketBoxBInfoBefore.ticketBoxID == emptyTicketBoxID){
		m_TicketInfoLabel[2].nameCenter =_T("δ��װ");
		m_TicketInfoLabel[2].nameRight = _T("");
	}
	else{
		m_TicketInfoLabel[2].nameCenter=model.ticketBoxBInfoBefore.ticketBoxID.ToString();
		m_TicketInfoLabel[2].nameRight.Format(_T("%d"),model.ticketBoxBInfoBefore.ulCurCount);
	}
	m_TicketInfoLabel[3].nameLeft = _T("��Ʊ��");
	if(model.ticketBoxCInfoBefore.ticketBoxID == emptyTicketBoxID){
		m_TicketInfoLabel[3].nameCenter =_T("δ��װ");
		m_TicketInfoLabel[3].nameRight = _T("");
	}
	else{
		m_TicketInfoLabel[3].nameCenter=model.ticketBoxCInfoBefore.ticketBoxID.ToString();
		m_TicketInfoLabel[3].nameRight.Format(_T("%d"),model.ticketBoxCInfoBefore.ulCurCount);
	}
	m_baseInfoArea->Invalidate();
}


////////////////////////////////////////////////////////////////////////////
///*
//@brief      ȡ��Ʊ��������ֱ�ǩ
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::GetTicketInfoAfterLabel()
{
	CTicketBoxExchangeSvc* pExchangeBoxSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DataModel &model = pExchangeBoxSvc->GetDataModel();

	m_TicketInfoLabel[0].nameLeft = _T("λ ��");
	m_TicketInfoLabel[0].nameCenter = _T("�� ��");
	m_TicketInfoLabel[0].nameRight = _T("�� ��");

	CCTLOGCountInfo::TICKETBOX_ID emptyTicketBoxID;
	m_TicketInfoLabel[3].nameLeft = _T("��Ʊ��");
	if (!model.moveBoxInfo[3]){
		if(model.ticketBoxCInfoBefore.ticketBoxID == emptyTicketBoxID){
			m_TicketInfoLabel[3].nameCenter =_T("δ��װ");
			m_TicketInfoLabel[3].nameRight = _T("");
		}
		else{
			m_TicketInfoLabel[3].nameCenter = model.ticketBoxCInfoBefore.ticketBoxID.ToString();
			m_TicketInfoLabel[3].nameRight.Format(_T("%d"),model.ticketBoxCInfoBefore.ulCurCount);
		}
	}
	else{
		if(!model.statusAfterExchange.bBoxCArrive){
			m_TicketInfoLabel[3].nameCenter =_T("δ��װ");
			m_TicketInfoLabel[3].nameRight = _T("");
		}
		else{
			m_TicketInfoLabel[3].nameCenter = model.ticketBoxCInfoAfter.ticketBoxID.ToString();
			m_TicketInfoLabel[3].nameRight.Format(_T("%d"), model.ticketBoxCInfoAfter.ulCurCount);
		}
	}

 	m_TicketInfoLabel[2].nameLeft = _T("Ʊ��2");
	if (!model.moveBoxInfo[2]){
		if(model.ticketBoxBInfoBefore.ticketBoxID == emptyTicketBoxID){
			m_TicketInfoLabel[2].nameCenter =_T("δ��װ");
			m_TicketInfoLabel[2].nameRight = _T("");
		}
		else{
			m_TicketInfoLabel[2].nameCenter = model.ticketBoxBInfoBefore.ticketBoxID.ToString();
			m_TicketInfoLabel[2].nameRight.Format(_T("%d"),model.ticketBoxBInfoBefore.ulCurCount);
		}
	}
	else{
 		if(!model.statusAfterExchange.bBoxBArrive){
 			m_TicketInfoLabel[2].nameCenter =_T("δ��װ");
 			m_TicketInfoLabel[2].nameRight = _T("");
 		}
 		else{
			m_TicketInfoLabel[2].nameCenter = model.ticketBoxBInfoAfter.ticketBoxID.ToString();
			m_TicketInfoLabel[2].nameRight.Format(_T("%d"), model.ticketBoxBInfoAfter.ulCurCount);
 		}
	}
	
	m_TicketInfoLabel[1].nameLeft = _T("Ʊ��1");
	if (!model.moveBoxInfo[1]){
		if(model.ticketBoxAInfoBefore.ticketBoxID == emptyTicketBoxID){
			m_TicketInfoLabel[1].nameCenter =_T("δ��װ");
			m_TicketInfoLabel[1].nameRight = _T("");
		}
		else{
			m_TicketInfoLabel[1].nameCenter = model.ticketBoxAInfoBefore.ticketBoxID.ToString();
			m_TicketInfoLabel[1].nameRight.Format(_T("%d"),model.ticketBoxAInfoBefore.ulCurCount);
		}
	}
	else{
		if(!model.statusAfterExchange.bBoxAArrive){
			m_TicketInfoLabel[1].nameCenter =_T("δ��װ");
			m_TicketInfoLabel[1].nameRight = _T("");
		}
		else{
			m_TicketInfoLabel[1].nameCenter = model.ticketBoxAInfoAfter.ticketBoxID.ToString();
			m_TicketInfoLabel[1].nameRight.Format(_T("%d"), model.ticketBoxAInfoAfter.ulCurCount);
		}
	}

	m_baseInfoArea->Invalidate();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  none
*/
////////////////////////////////////////////////
//////////////////////////
int CTicketBoxInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		// ����������Ϣ���򡢷��ء�ȷ����ť
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA|BTN_OK )){
			return -1;
		}
		delete m_baseInfoArea;
		m_baseInfoArea = NULL;
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_TicketInfoLabel);

		// ����ҵ�񲿷ֺ�����������ʾ��Ϣ����
		// ���öԻ�����������
		m_baseInfoArea->GetEdit(0)->SetScope(CGEdit::DIGIT_CHAR);
		m_baseInfoArea->GetEdit(1)->SetScope(CGEdit::DIGIT_CHAR);
		m_baseInfoArea->GetEdit(0)->SetLimitText(3);
		m_baseInfoArea->GetEdit(1)->SetLimitText(3);
		//m_baseInfoArea->GetEdit(2)->SetScope(CGEdit::DIGIT_CHAR);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::InitialButtons()
{
	m_vec_Right_1_BTN_INFO.clear();

	// ���õ�ѡ
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);

	for (int i = 2; i >0; i--){
		BTN_INFO pMoveTypeInfo;
		pMoveTypeInfo.btnID = i;
		pMoveTypeInfo.btnMsg = STR_MOVE_TYPE_MSG[i-1];
		pMoveTypeInfo.text = STR_MOVE_TYPE_MSG[i-1];
		m_vec_Right_1_BTN_INFO.push_back(pMoveTypeInfo);
	}

	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->ClickFirstAvailButton();
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ƶ����Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxInfoDlg::OnMoveTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	try{
		HideCompoment(BTN_OK);
		bool showOk = false;
		CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
		CTicketBoxExchangeSvc::DataModel &model = pSvc->GetDataModel();
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
        pSvc->SetMoveType((TICKETBOX_OPERATION_TYPE)btnInfo->btnID);

		InitialBoxButtons();

		UpdateUI();
		return TRUE;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Ʊ�����Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::InitialBoxButtons()
{
	theSERVICE_MGR.SetState(RECEPT);
	HideCompoment(BTN_OK);
	bool showOk = false;
	CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DataModel &model = pSvc->GetDataModel();

	m_vec_Right_2_BTN_INFO.clear();
	m_Right_2_Group->SetSelectType(CButtonGroup::MULTIPLE);
	for (int i = 0; i < 3; i++){
		BTN_INFO pBoxTypeInfo;
		pBoxTypeInfo.btnID = i + 1;
		pBoxTypeInfo.btnMsg = STR_BOX_TYPE_MSG[i];
		pBoxTypeInfo.text = STR_BOX_TYPE_MSG[i];
		pBoxTypeInfo.isEnable = model.moveBoxBtnInfo[i+1];
		showOk = showOk||pBoxTypeInfo.isEnable;
		m_vec_Right_2_BTN_INFO.push_back(pBoxTypeInfo);
	}
	if(showOk){
		ShowCompoment(BTN_OK);
	}
	m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
	m_Right_2_Group->ClickFirstAvailButton();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ƶ����Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxInfoDlg::OnBoxTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	try{
		theSERVICE_MGR.SetState(RECEPT);
		CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
		CTicketBoxExchangeSvc::DataModel &model = pSvc->GetDataModel();
		model.moveBoxInfo.clear();
		vector<BTN_INFO> btnInfo = m_Right_2_Group->GetBtnInfo();
		for (int i = 0; i < btnInfo.size(); i++){
			if (btnInfo[i].isSelected){
				model.moveBoxInfo[i+1] =  true;
			}
		}
		m_baseInfoArea->Invalidate();
		UpdateUI();
		return TRUE;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::InitialEdit()
{
	CTicketBoxExchangeSvc* pExchangeBoxSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DataModel &model = pExchangeBoxSvc->GetDataModel();

	for (int i=2;i>0;i--){
		if(!model.moveBoxInfo[i]){
			m_baseInfoArea->GetEdit(i-1)->ShowWindow(false);
		}
		else{
			m_baseInfoArea->GetEdit(i-1)->ShowWindow(true);
			m_baseInfoArea->GetEdit(i-1)->SetFocus();
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      �༭����Ϣ��Ӧ  

@param      WPARAM wParam groupID
@param      LPARAM lParam(BTN_INFO*)

@retval     

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxInfoDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	theAPP_SESSION.ShowOfficeGuide(_T("������Ʊ����Ϣ��ȷ�ϼ���"));
	theSERVICE_MGR.SetState(RECEPT);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �������á����ذ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTicketBoxInfoDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DataModel &model = pSvc->GetDataModel();
	theSERVICE_MGR.SetState(RECEPT);
	
	//if (!pSvc->IsPrinterErrorContinue()){
	//	return FALSE;
	//}
	if(pSvc->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_BASE)
	{	
		if (model.moveType==ATTACH)	{
			pSvc->OnGetBoxInfo();
			return TRUE;
		}
		else{
			if(!(model.moveBoxInfo[1]||model.moveBoxInfo[2]||model.moveBoxInfo[3])){
				theAPP_SESSION.ShowOfficeGuide(_T("��ǰ�޿ɽ��еĲ����"));
				return false;
			}
			pSvc->OnDetachBox();
			
			InitialButtons();
			return TRUE;
		}
	}
	else if(pSvc->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_NEW){	
		if(!(model.moveBoxInfo[1]||model.moveBoxInfo[2]||model.moveBoxInfo[3])){
			theAPP_SESSION.ShowOfficeGuide(_T("��ǰ�޿ɽ��еĲ����"));
			return false;
		}
		if (ValidateCheck()){
			CString strPiece = _T("");
			int piece = 0;
			if (model.moveBoxInfo[1]){
				m_baseInfoArea->GetEdit(0)->GetWindowText(strPiece);
				piece = _ttoi(strPiece);
				model.ticketBoxAInfoAfter.ulInitCount = piece;
				model.ticketBoxAInfoAfter.ulCurCount = piece;
			}
		    if (model.moveBoxInfo[2]){
				m_baseInfoArea->GetEdit(1)->GetWindowText(strPiece);
				piece = _ttoi(strPiece);
				model.ticketBoxBInfoAfter.ulInitCount = piece;
				model.ticketBoxBInfoAfter.ulCurCount = piece;
			}
			if (model.moveBoxInfo[3]){
				//m_baseInfoArea->GetEdit(2)->GetWindowText(strPiece);
				piece = 0/*_ttoi(strPiece)*/;
				model.ticketBoxCInfoAfter.ulInitCount = piece;
				model.ticketBoxCInfoAfter.ulCurCount = piece;
			}
			pSvc->OnAttachBox();
			return TRUE;
		}		
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ч�Լ��

@param      none

@retval     BOOL   TRUE:����ɹ�  FALSE:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTicketBoxInfoDlg::ValidateCheck()
{
	if((m_baseInfoArea->GetEdit(0)->GetWindowTextLength()== 0 &&m_baseInfoArea->GetEdit(0)->IsWindowVisible())
		||(m_baseInfoArea->GetEdit(1)->GetWindowTextLength()== 0&&m_baseInfoArea->GetEdit(1)->IsWindowVisible())
		/*||(m_baseInfoArea->GetEdit(2)->GetWindowTextLength()== 0&&m_baseInfoArea->GetEdit(2)->IsWindowVisible())*/){
		theAPP_SESSION.ShowOfficeGuide(_T("Ʊ����Ϣ���벻������"));
		return FALSE;
	}
	return TRUE;
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
void CTicketBoxInfoDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	if(GetService<CTicketBoxExchangeSvc>()->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_BASE)
	{	
		InitialButtons();
	}
	if(GetService<CTicketBoxExchangeSvc>()->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_NEW)
	{	
		InitialBoxButtons();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTicketBoxInfoDlg::UpdateUI()
{
	CTicketBoxExchangeSvc* pSvc = (CTicketBoxExchangeSvc*)GetService();
	CTicketBoxExchangeSvc::DataModel &model = pSvc->GetDataModel();
	if(pSvc->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_BASE)
	{	
		if (model.moveType == ATTACH){
			m_vec_Right_2_BTN_INFO.clear();
			m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TICKETBOX_CHANGE_FINISH));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TICKETBOX_CHANGE_START));
		}
		if(!m_btnOK->IsWindowVisible()){
			theAPP_SESSION.ShowOfficeGuide(_T("��ǰ�޿ɽ��еĲ����"));
		}
		HideCompoment(EDIT_BOX);
		GetTicketInfoBeforeLabel();
	}
	if(pSvc->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_NEW)
	{	
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		InitialEdit();
		GetTicketInfoAfterLabel();
	}
	if(pSvc->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_FINISH)
	{
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_vec_Right_2_BTN_INFO.clear();
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		if (model.moveType == DETACH){
			GetTicketInfoBeforeLabel();
			HideCompoment(BTN_OK|EDIT_BOX);
		}
		else{
			GetTicketInfoAfterLabel();
			HideCompoment(BTN_OK|EDIT_BOX);
		}
	}
	if(pSvc->GetCurDialogGroupID() == CTicketBoxExchangeSvc::DIALOG_GROUP_ERROR)
	{
		m_vec_Right_2_BTN_INFO.clear();
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
		HideCompoment(BASE_AREA|BTN_OK|EDIT_BOX);
	}
	m_baseInfoArea->Invalidate();
}