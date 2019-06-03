#include "stdafx.h"
#include "AdjustSvc.h"
#include "AdjustBaseOfficeDlg.h"
#include "OperationDlg.h"
#include "tickethelper.h"
#include "functioninfo.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString PAYMATHOD_TYPE	    	= _T("PAYMATHOD_TYPE");
static const CString STR_COUNT				= _T("COUNT");
static const int IDX_ACTUAL_ADJUST_AMOUNT   = 0;
static const int IDX_PAID					= 1;

//��ֵ��Ϣ��ʾ�༭���������
static CRect editAdjust[] = {
	BASE_INFO_RECT(8,7),
};

IMPLEMENT_DYNAMIC(CAdjustBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CAdjustBaseOfficeDlg, COperationDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
    ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnLineClick)
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnStationClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPayMathodClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnOverTimeReasonClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CAdjustBaseOfficeDlg::CAdjustBaseOfficeDlg(CService* pService)
	: COperationDlg(CAdjustBaseOfficeDlg::IDD,pService)
{
   InitBaseInfo();

    // ��Ʊ��Ϣ
    m_baseInfo->titleInfo.titleName = TITLE_TICKET_INFO;                    // ����
    m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_8;
    m_baseInfo->detailInfo.labelGroup =m_AdjustInfo;        // ��ǩ���֣�2�У�
    m_baseInfo->detailInfo.editGroup.cnt = 1;   
	m_baseInfo->detailInfo.editGroup.editRect = editAdjust;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CAdjustBaseOfficeDlg::~CAdjustBaseOfficeDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
int CAdjustBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try{
        if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA|/*BTN_RESET | */BTN_OK |  BASE_AREA | CASH_AREA )){
            return -1;
        }
    }
	
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(ADJUST_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

void CAdjustBaseOfficeDlg::InitBaseInfo()
{
	// Ʊ�濨��
	m_AdjustInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ��Ʊ����
	m_AdjustInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_KIND)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//�������
	m_AdjustInfo[2].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//��վ
	m_AdjustInfo[3].nameLeft = add_wsp(CString(TXT_ENTER_STATION)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//��վʱ��
	m_AdjustInfo[4].nameLeft = add_wsp(CString(TXT_ENTER_STATION_TIME)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//��Ʊ��ʽ
	m_AdjustInfo[5].nameLeft = add_wsp(CString(TXT_ADJUST_TYPE)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//��Ʊ���
	m_AdjustInfo[6].nameLeft = add_wsp(CString(TXT_ADJUST_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//ʵ�ʲ�Ʊ���
	m_AdjustInfo[7].nameLeft = add_wsp(CString(TXT_ACTUAL_ADJUST_AMOUNT)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAdjustBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{	
	__super::OnActivate(nState,pWndOther,bMinimized);
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	if(pSvc->GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_FINISH)
	{
		HideCompoment(BTN_OK |CASH_AREA| KEYBOARD_AREA);
		m_Left_1_Group->ShowButtons(false);
		m_Left_2_Group->ShowButtons(false);
		m_Right_1_Group->ShowButtons(false);
		m_Right_2_Group->ShowButtons(false);
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->EnableWindow(FALSE);
		GetEdit(IDX_PAID)->EnableWindow(FALSE);
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_ERROR)
	{
		HideCompoment(BTN_OK |CASH_AREA|BASE_AREA| EDIT_BOX | KEYBOARD_AREA);
		m_Left_1_Group->ShowButtons(false);
		m_Left_2_Group->ShowButtons(false);
		m_Right_1_Group->ShowButtons(false);
		m_Right_2_Group->ShowButtons(false);
	}
	else
	{
		CAdjustSvc::Model & dataModel = pSvc->GetDataModel();
		if(dataModel.canSelectStation)
		{
			BuildLineGroup();
		}
		BuildPayMathodGroup();
		m_Right_1_Group->EnableButton(0,dataModel.availablePayMethod&0x01);
		m_Right_1_Group->EnableButton(1,dataModel.availablePayMethod&0x02);
		m_Right_1_Group->ClickButton(dataModel.payMethod == BY_CASH?0:1);
		if(dataModel.ticketCategory==MULTIRIDE){

			CString strActualAdjustAmount = _T("");
			strActualAdjustAmount.Format(_T("%d"),dataModel.actualAdjustAmount);
			SetEditText(IDX_ACTUAL_ADJUST_AMOUNT,strActualAdjustAmount);
		}
		else{
			SetEditText(IDX_ACTUAL_ADJUST_AMOUNT,ComFormatAmount(dataModel.actualAdjustAmount));
		}
		if(dataModel.adjustCode == OUT_STATION_UPDATE_24)
		{
			BuildOverTimeGroup();
			m_Right_2_Group->ClickFirstAvailButton();
		}
	}
	m_baseInfoArea->Invalidate();
	theSERVICE_MGR.SetState(FINISH);	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������á����ذ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
            TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CAdjustBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	if (ValidateInput())
	{
		GetService<CAdjustSvc>()->OnOK();
		return TRUE;
	}
	return FALSE;	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAdjustBaseOfficeDlg::ValidateInput()
{
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model & dataModel = pSvc->GetDataModel();
	if(dataModel.cardPHType == ULTRALIGHT && dataModel.journeyStatus == CULCard::UNENTER_STATION && dataModel.enterStation<=0)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_SELECT_ENTER_STATION));
		return false;
	}
	if(dataModel.cardPHType == CPU_TICKET && dataModel.journeyStatus == CCPUCard::UNENTER_STATION && dataModel.enterStation<=0)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_SELECT_ENTER_STATION));
		return false;
	}
	if(dataModel.actualAdjustAmount>dataModel.adjustAmount)
	{
		theAPP_SESSION.ShowOfficeGuide(_T("ʵ�ʲ�Ʊ�����ڽ��鲹Ʊ���"));
		return false;
	}
	if(GetEditText(IDX_PAID)=="")
	{
		SetEditText(IDX_PAID,ComFormatAmount(dataModel.due));
		dataModel.paid = dataModel.due;
	}
	if(dataModel.due > dataModel.paid)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ISSUE_INPUT_CASH_ERROR));
		GetEdit(IDX_PAID)->SetFocus();
		return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CAdjustBaseOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	CGEdit* currentEdit = (CGEdit*)lParam;
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model & dataModel = pSvc->GetDataModel();
	if(currentEdit==GetEdit(IDX_PAID)){
		dataModel.paid = ComGetAmount(GetEditText(IDX_PAID));
	}
	if(currentEdit == GetEdit(IDX_ACTUAL_ADJUST_AMOUNT))
	{
		if(dataModel.ticketCategory == MULTIRIDE)
		{
			dataModel.actualAdjustAmount = _ttoi(GetEditText(IDX_ACTUAL_ADJUST_AMOUNT));
		}
		else
		{
			dataModel.actualAdjustAmount = ComGetAmount(GetEditText(IDX_ACTUAL_ADJUST_AMOUNT));
		}
	}
	pSvc->NotifyDataModelChanged();
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
void CAdjustBaseOfficeDlg::BuildPayMathodGroup()
{
	// �ȶ�������
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);

	int payMethodCount = getini.GetDataInt(PAYMATHOD_TYPE,STR_COUNT);
	// ��������
	BTN_INFO * pPayMethodBtnInfo = new BTN_INFO[payMethodCount];
	// ��ʼ�����ѷ�ʽѡ�ť
	for (int i = 0;i < payMethodCount;i++){
		CString strPayMethodNo;
		strPayMethodNo.Format(_T("%d"),i+1);
		CString sexTypeText = getini.GetDataString(PAYMATHOD_TYPE,strPayMethodNo);
		pPayMethodBtnInfo[i].btnID = i+1;
		pPayMethodBtnInfo[i].btnMsg = strPayMethodNo;
		pPayMethodBtnInfo[i].text = sexTypeText;
		m_vec_Right_1_BTN_INFO.push_back(pPayMethodBtnInfo[i]);
	}
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);

	delete [] pPayMethodBtnInfo;
}

void CAdjustBaseOfficeDlg::BuildOverTimeGroup()
{
	CString strOverTimeReason[]={_T("�˿�ԭ��\n��ʱ"),_T("����ԭ��\n��ʱ")};
	m_vec_Right_2_BTN_INFO.clear();
	for(int i=0;i<2;i++)
	{
		BTN_INFO btnInfo;
		btnInfo.btnID = i;
		btnInfo.btnMsg.Format(_T("%d"),i);
		btnInfo.text = strOverTimeReason[i];
		m_vec_Right_2_BTN_INFO.push_back(btnInfo);
	}
	m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
	m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CAdjustBaseOfficeDlg::OnPayMathodClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	WORD payMethodIndex = _ttoi(btnInfo->btnMsg);
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model & dataModel = pSvc->GetDataModel();
	dataModel.payMethod =  payMethodIndex == 1 ? BY_CASH :  BY_CARD;
	SetEditText(IDX_PAID,_T(""));
	dataModel.paid = 0;
	pSvc->NotifyDataModelChanged();
	return TRUE;
}

LRESULT  CAdjustBaseOfficeDlg::OnOverTimeReasonClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model & dataModel = pSvc->GetDataModel();
	dataModel.overTimeReason = btnInfo->btnID == 0 ? 1:2;	//1�˿�ԭ��2����ԭ��
	if(dataModel.overTimeReason == 1)
	{
		if(dataModel.ticketCategory==MULTIRIDE){
			CString strActualAdjustAmount = _T("");
			strActualAdjustAmount.Format(_T("%d"),dataModel.adjustAmount);
			SetEditText(IDX_ACTUAL_ADJUST_AMOUNT,strActualAdjustAmount);
		}
		else{
			SetEditText(IDX_ACTUAL_ADJUST_AMOUNT,ComFormatAmount(dataModel.adjustAmount));
		}
		dataModel.actualAdjustAmount = dataModel.adjustAmount;
		
	}
	pSvc->NotifyDataModelChanged();
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
LRESULT CAdjustBaseOfficeDlg::OnLineClick(WPARAM wParam, LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD lineCode = _ttoi(btnInfo->btnMsg);
	typedef CAccLineInfoParam::STATION_INFO STATION_t; 
	const vector<STATION_t>& vecStations = theACC_LINE.GetStationInfo(lineCode);
	m_vec_Left_1_BTN_INFO.clear();
	for(unsigned int i=0;i<vecStations.size();i++){
		const STATION_t& station = vecStations[i];
		if(station.Active_flag){
			BTN_INFO btnInfo;
			//2012/2/15�ӳ�վ�����Ϊʹ�üƷѱ���
			//btnInfo.btnMsg.Format(_T(_"%d"),station.stationNum);
			btnInfo.btnMsg.Format(_T("%d"),station.fareStationNum);
			CString stationName = station.station_Chinese_Name;
			if (stationName.GetLength() > 16){
				stationName.Insert(12,_T("\n"));
			}
			btnInfo.text = stationName;
			btnInfo.isEnable = theFunction_INFO.IsStationEnabled(station.stationNum);
			m_vec_Left_1_BTN_INFO.push_back(btnInfo);
		}
	}
	m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
	m_Left_1_Group->SetSelectType(CButtonGroup::SIGLE);
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
LRESULT CAdjustBaseOfficeDlg::OnStationClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	WORD selectedStationCode = _ttoi(btnInfo->btnMsg);
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model & dataModel = pSvc->GetDataModel();
	dataModel.enterStation = selectedStationCode;
	dataModel.enterDateTime = ComGetCurTime();
	pSvc->NotifyDataModelChanged();
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
void CAdjustBaseOfficeDlg::BuildLineGroup()
{
	typedef CAccLineInfoParam::LINE_INFO LINE_t;
	const vector<LINE_t>& vecLines =theACC_LINE.GetLinInfo();
	for(unsigned int i=0;i<vecLines.size();i++){
		const LINE_t& line = vecLines[i];
		if(line.Active_flag){
			BTN_INFO btnInfo;
			btnInfo.btnMsg.Format(_T("%d"),line.lineNum);
			btnInfo.text = line.line_Chinese_Name;
			btnInfo.isEnable = theFunction_INFO.IsLineEnabled(line.lineNum);
			m_vec_Left_2_BTN_INFO.push_back(btnInfo);
		}
	}
	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
	m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Left_2_Group->ClickFirstAvailButton();
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_ADJUST_PRINT));
}

void CAdjustBaseOfficeDlg::UpdateUI()
{
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model& dataModel = pSvc->GetDataModel();
	if(pSvc->GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_FINISH)
	{
		return;
	}
	else if(pSvc->GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_ERROR)
	{
		return;
	}
	// Ʊ�濨��
	m_AdjustInfo[0].nameRight = dataModel.strTicketNumber;
	// ��Ʊ����
	m_AdjustInfo[1].nameRight = dataModel.strTicketTypeName;

	// ���/��� ��ʾ�趨	
	if(dataModel.ticketCategory==MULTIRIDE)
	{
		m_AdjustInfo[2].nameLeft = add_wsp(CString(TXT_TICKET_RIDES)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_AdjustInfo[2].nameRight = ComFormatCount(dataModel.balance)+TIMES_UNIT;
	}
	else if(dataModel.ticketCategory ==PURSE)
	{
		m_AdjustInfo[2].nameRight =  ComFormatAmount(dataModel.balance)+MONEY_UNIT;
	}
	else
	{
		m_AdjustInfo[2].nameRight = TXT_NOTHING;
	}
	CAccLineInfoParam::STATION_INFO enterStationInfo;
	theACC_LINE.GetStationInfobyCode(dataModel.enterStation,enterStationInfo);
	// ��վ��վ
	m_AdjustInfo[3].nameRight = enterStationInfo.station_Chinese_Name==_T("")?TXT_NOTHING:enterStationInfo.station_Chinese_Name;
	// ��վʱ��
	m_AdjustInfo[4].nameRight = dataModel.enterDateTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
	// ��Ʊ��ʽ
	m_AdjustInfo[5].nameRight = dataModel.strAdjustGuide;
	// ��Ʊ���
	if(dataModel.ticketCategory==MULTIRIDE){
		m_AdjustInfo[6].nameLeft = add_wsp(CString(TXT_ADJUST_RIDES)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_AdjustInfo[6].nameRight = ComFormatCount(dataModel.adjustAmount)+TIMES_UNIT;
		m_AdjustInfo[7].nameLeft = add_wsp(CString(TXT_ACTUAL_ADJUST_RIDES)+CString(TXT_COLON),MAX_SBC_CASE_LEN);
		m_AdjustInfo[7].nameRight = add_hsp(TIMES_UNIT,MAX_CASH_RIGHT_LEN);
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->SetScope(CGEdit::DIGIT_CHAR);
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->SetLimitText(5);

	}
	else{
		m_AdjustInfo[6].nameRight = ComFormatAmount(dataModel.adjustAmount)+MONEY_UNIT;
		m_AdjustInfo[7].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->SetScope(CGEdit::CASH);
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->SetLimitText(7);
	}
	
	m_cashInfo->detailInfo.labelGroup[0].nameRight = ComFormatAmount(dataModel.due) + MONEY_UNIT;
	m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(dataModel.change) + MONEY_UNIT;
	if(dataModel.overTimeReason == 2)//����ԭ��ʱʱ���ɵ�����Ʊ���
	{
		m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_8;
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_7;
		GetEdit(IDX_ACTUAL_ADJUST_AMOUNT)->ShowWindow(SW_HIDE);
	}
	m_baseInfoArea->Invalidate();
	m_cashInfoArea->ShowWindow(dataModel.actualAdjustAmount>0 && dataModel.payMethod == BY_CASH ? SW_SHOW : SW_HIDE);
	m_cashInfoArea->Invalidate();
	this->Invalidate();
	
}
