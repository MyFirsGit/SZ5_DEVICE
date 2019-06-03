#include "stdafx.h"
#include "SignCardApplyBaseOfficeDlg.h"
#include "SignCardApplySvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include <stdlib.h>
#include "Util.h"
#include <afxwin.h>
#include "GEdit.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSignCardApplyBaseOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CSignCardApplyBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeClick)
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnSexClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSevBtnClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// �༭��λ��
static CRect editApply[] = 
{
	BASE_INFO_RECT(1,20),
	BASE_INFO_RECT(4,20),
	//BASE_INFO_RECT(5,3),
	BASE_INFO_RECT_SP(5,4,0),//��
	BASE_INFO_RECT_SP(5,2,8),//��
	BASE_INFO_RECT_SP(5,2,14),//��
	BASE_INFO_RECT(6,20),
	BASE_INFO_RECT(7,30),
	BASE_INFO_RECT(8,20),
	BASE_INFO_RECT(9,20),

};

static const char EDIT_COUNT				= 9;			 // �༭��ĸ���
static const CString STR_CHECK_REASON		= _T("CHECK_REASON");
static const CString CERTIFI_TYPE		    = _T("CERTIFICATE_TYPE");
static const CString Sex_type		        = _T("SEX_TYPE");
static const CString STR_COUNT				= _T("COUNT");

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyBaseOfficeDlg::CSignCardApplyBaseOfficeDlg(CService* pService) :
COperationDlg(CSignCardApplyBaseOfficeDlg::IDD, pService)
{
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->titleInfo.titleName = TXT_SIGNCARD_APPLY_SVC_CN;								// ����
	m_baseInfo->detailInfo.row = USER_INFO_SIZE;												// ����
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();			         	// ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = EDIT_COUNT;											// �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editApply;								    	// �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyBaseOfficeDlg::~CSignCardApplyBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int \n
0:�ɹ�  -1:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CSignCardApplyBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ����������Ϣ���򣬷��ؼ���ȷ�ϼ�
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA |  BASE_AREA | BTN_OK | BTN_RESET)){
		return -1;
	}
    SetEditFocus(0);
	GetEdit(2)->SetLimitText(4);
	GetEdit(0)->SetLimitText(20);
	GetEdit(1)->SetScope(CGEdit::NUMANDCHAR);
	GetEdit(0)->SetScope(CGEdit::NAME);
	GetEdit(6)->SetScope(CGEdit::ALL_CHAR);
	GetEdit(7)->SetScope(CGEdit::ALL_CHAR);
	GetEdit(8)->SetScope(CGEdit::ALL_CHAR);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ի����ʼ��

@param      ��

@retval     BOOL  \n   TRUE:��ʼ���ɹ�  FALSE:��ʼ��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CSignCardApplyBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// ��ʾBOM����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));

	// ��ʼ��ԭ�򰴼�
	InitialButtons();

	// �����Ҳఴť
	m_Left_1_Group->EnableWindow(TRUE);
	m_Left_1_Group->Invalidate();
	m_Left_2_Group->EnableWindow(TRUE);
	m_Left_2_Group->Invalidate();

	return TRUE;  
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʼ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSignCardApplyBaseOfficeDlg::InitialButtons()
{
	// �ȶ�������
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);

	int checkReasonCount = getini.GetDataInt(STR_CHECK_REASON,STR_COUNT);
	int certificateCount = getini.GetDataInt(CERTIFI_TYPE,STR_COUNT);
	int sexCount = getini.GetDataInt(Sex_type,STR_COUNT);

	// ��������
	pCheckReasonBtnInfo = new BTN_INFO[checkReasonCount];
	pCertificatetypeBtnInfo = new BTN_INFO[certificateCount];
	pSexBtnInfo = new BTN_INFO[sexCount];

	// ��ʼ���Ա�ѡ�ť
	CString strSexNo;
	for (int i = 0;i < sexCount;i++){
		strSexNo.Format(_T("%d"),i+1);
		CString sexTypeText = getini.GetDataString(Sex_type,strSexNo);
		pSexBtnInfo[i].btnID = i+1;
		pSexBtnInfo[i].btnMsg = strSexNo;
		pSexBtnInfo[i].text = sexTypeText;
		m_vec_Left_1_BTN_INFO.push_back(pSexBtnInfo[i]);
	}
	// ��ʼ��֤�����Ͱ�ť
	CString strcertifiNo[] = {_T("1"),_T("2"),_T("3"),_T("10")};
	for (int j = 0;j < certificateCount;j++){
		CString certificateType = getini.GetDataString(CERTIFI_TYPE,strcertifiNo[j]);
		certificateType.Replace(_T("\\n"),_T("\n"));
		pCertificatetypeBtnInfo[j].btnID = j+1;
		pCertificatetypeBtnInfo[j].btnMsg = strcertifiNo[j];
		pCertificatetypeBtnInfo[j].text = certificateType;
		m_vec_Left_2_BTN_INFO.push_back(pCertificatetypeBtnInfo[j]);
	}

	// ��ʼ��ҵ�����Ͱ�ť
	CString checkNo;
	for (int j = 0;j < checkReasonCount;j++){
		checkNo.Format(_T("%d"),j+1);
		CString checkReasonText = getini.GetDataString(STR_CHECK_REASON,checkNo);
		checkReasonText.Replace(_T("\\n"),_T("\n"));
		pCheckReasonBtnInfo[j].btnID = j+1;
		pCheckReasonBtnInfo[j].btnMsg = checkNo;
		pCheckReasonBtnInfo[j].text = checkReasonText;
		if(checkNo == "2") //�ֿ���\n��Ϣ�޸�
		{
			pCheckReasonBtnInfo[j].isEnable = (!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID));
		}
		m_vec_Right_1_BTN_INFO.push_back(pCheckReasonBtnInfo[j]);
	}
	m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
	m_Left_1_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Left_1_Group->ClickFirstAvailButton();
	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
	m_Left_2_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Left_2_Group->ClickFirstAvailButton();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Right_1_Group->ClickFirstAvailButton();

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ա�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ

*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::ClickSexButton(int index)
{
	theSERVICE_MGR.SetState(RECEPT);
	m_Left_1_Group->ClickButton(index-1);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ����֤�����Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ

*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::ClickCredentialsTypeButton(int index)
{
	theSERVICE_MGR.SetState(RECEPT);
	m_Left_2_Group->ClickButton(index-1);
	if (index==10){
		m_Left_2_Group->ClickButton(4);
	}
	else{
		m_Left_2_Group->ClickButton(index-1);
	}
	SetEditFocus(2);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ���÷������Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ

*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::ClickSevTypeButton(int index)
{
	theSERVICE_MGR.SetState(RECEPT);
	m_Right_1_Group->ClickButton(index);	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	theSERVICE_MGR.SetState(RECEPT);
	if(!ValidateInput()){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_INPUT_ERROR));
		return FALSE;
	}
	GetService<CSignCardApplySvc>()->OK();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �༭����Ϣ�ı�ʱ�Ĵ���

@param      ��

@retval     bool true:��ȷ; false:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyBaseOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	theSERVICE_MGR.SetState(RECEPT);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     �������Ϸ��Լ�� 

@param      ��

@retval     bool TRUE:��ȷ FALSE:����

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CSignCardApplyBaseOfficeDlg::ValidateInput()
{
	// ���������Ϣ�Ƿ�����
	for(int i=0;i<2;i++){
		if(GetEditText(i) == "") {
			SetEditFocus(i);
			return FALSE;
		}
	}
	// ��������֤��ID�Ƿ���Ч
	const vector<BTN_INFO>& selectedBtnInfo=m_Left_2_Group->GetSelectedBtnInfo();
	if(_ttoi(selectedBtnInfo[0].btnMsg) == CERTIFICATE_ID_CARD_MAINLAND){
		if (!CheckIDCard(GetEditText(1)))
		{
			SetEditFocus(1);
			return false;
		}
	}
	// ��������֤����Ч���Ƿ���Ч
	if (GetEditText(3) != ""&&GetEditText(4) != ""&&GetEditText(2) != ""){
		_DATE data;
		data.wYear = _ttoi(GetEditText(2));
		data.biMonth = _ttoi(GetEditText(3));
		data.biDay = _ttoi(GetEditText(4));
		if(ComCheckDateValid(data)!=0){
			SetEditFocus(2);
			return FALSE;
		}	
		if (ComGetCurDate()>data)
		{
			SetEditFocus(2);
			return FALSE;
		}
	}
	else if(!(GetEditText(3) == ""&&GetEditText(4) == ""&&GetEditText(2) == "")){
		return false;
	}
	// EMAIL��Ч�Լ��	
	if (GetEditText(6) != ""&&!checkEmailValidate(GetEditText(6)))
	{
		SetEditFocus(6);
		return FALSE;
	}	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ð�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyBaseOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	Reset();
	return __super::XButtonClick(wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CSignCardApplyBaseOfficeDlg::Reset()
{
	// ��ձ༭�����Ϣ
	SetEditText(0,_T(""));
	SetEditText(1,_T(""));
	SetEditText(2,_T(""));
	SetEditText(3,_T(""));
	SetEditText(4,_T(""));
	SetEditText(5,_T(""));
	SetEditText(6,_T(""));
	SetEditText(7,_T(""));
	SetEditText(8,_T(""));
	SetEditFocus(0);
	// ��ʼ����Ա����
	m_Left_1_Group->ClickFirstAvailButton();
	m_Left_2_Group->ClickFirstAvailButton();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����֤�����Ͱ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSignCardApplyBaseOfficeDlg::OnCertificateTypeClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE certifiType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	GetService<CSignCardApplySvc>()->SetCertifiType(certifiType,btnInfo->text);
	m_baseInfoArea->Invalidate();
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ա�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ 

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSignCardApplyBaseOfficeDlg::OnSexClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE sex =  _ttoi(btnInfo->btnMsg);
	GetService<CSignCardApplySvc>()->SetSex(sex);
	m_baseInfoArea->Invalidate();
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ����������Ͱ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSignCardApplyBaseOfficeDlg::OnSevBtnClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE sevType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	pCSignCardApplySvcc->SetSevType(sevType,btnInfo->btnID);
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
void CSignCardApplyBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	
	__super::OnActivate(nState,pWndOther,bMinimized);
	Reset();
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();
	try{	
		// ���롢�����������
		if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_BASE&&pCSignCardApplySvcc->GetSevType()==1){
			m_Left_1_Group->ClickFirstAvailButton();
			m_Left_2_Group->ClickFirstAvailButton();	
			m_vec_Right_1_BTN_INFO[0].isSelected = true;
			m_vec_Right_1_BTN_INFO[1].isSelected = false;
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO)	;			
			m_baseInfoArea->Invalidate();
		}
		else if (pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_BASE&&pCSignCardApplySvcc->GetSevType()==2)
		{
			CCPUCard cpuCard = pCSignCardApplySvcc->GetCardInfo();
			m_vec_Right_1_BTN_INFO[1].isSelected = true;
			m_vec_Right_1_BTN_INFO[0].isSelected = false;
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO)	;	
			SetEditText(0,cpuCard.personalData.Name);
			SetEditText(1,cpuCard.personalData.CredentialsNumber);
			SetEditFocus(2);
			m_baseInfoArea->Invalidate();
		}					
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pCSignCardApplySvcc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}	
}
