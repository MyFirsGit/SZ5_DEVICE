#include "stdafx.h"
#include "SignCardApplyCheckOfficeDlg.h"
#include "SignCardApplySvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include <stdlib.h>

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSignCardApplyCheckOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CSignCardApplyCheckOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSevBtnClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

// �༭��λ��
static CRect editCheck[] = 
{
	BASE_INFO_RECT(2,20),
};
static const char CHECK_EDIT_COUNT			= 1;			 // �༭��ĸ���

static const CString STR_CHECK_REASON		= _T("CHECK_REASON");
static const CString CERTIFI_TYPE		    = _T("CERTIFICATE_TYPE");
static const CString STR_COUNT				= _T("COUNT");

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyCheckOfficeDlg::CSignCardApplyCheckOfficeDlg(CService* pService) :
COperationDlg(CSignCardApplyCheckOfficeDlg::IDD, pService)
{
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->titleInfo.titleName = TXT_SIGNCARD_APPLY_SVC_CN;								// ����
	m_baseInfo->detailInfo.row = USER_INFO_CHECK_SIZE;											// ����
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();			         	// ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = CHECK_EDIT_COUNT;									// �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editCheck;								    	// �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyCheckOfficeDlg::~CSignCardApplyCheckOfficeDlg()
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
int CSignCardApplyCheckOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ����������Ϣ���򣬷��ؼ���ȷ�ϼ�
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA |  BASE_AREA | BTN_OK | BTN_RESET)){
		return -1;
	}
	GetEdit(0)->SetScope(CGEdit::NUMANDCHAR);
    SetEditFocus(0);
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
BOOL CSignCardApplyCheckOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// ��ʾBOM����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));

	// ��ʼ������
	InitialButtons();

	//// �����Ҳఴť
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
void CSignCardApplyCheckOfficeDlg::InitialButtons()
{
	// �ȶ�������
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);
	int checkReasonCount = getini.GetDataInt(STR_CHECK_REASON,STR_COUNT);
	int certificateCount = getini.GetDataInt(CERTIFI_TYPE,STR_COUNT);

	// ��������
	pCheckReasonBtnInfo = new BTN_INFO[checkReasonCount];
	pCertificatetypeBtnInfo = new BTN_INFO[certificateCount];
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
		m_vec_Right_1_BTN_INFO.push_back(pCheckReasonBtnInfo[j]);
	}
	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
	m_Left_2_Group->ClickFirstAvailButton();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->ClickButton(3);

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
LRESULT  CSignCardApplyCheckOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	theSERVICE_MGR.SetState(RECEPT);
	if (!ValidateInput()){
		return FALSE;
	}

	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_CHECKING));

	if (GetService<CSignCardApplySvc>()->OK() == 0)	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_CHECK_SUCCEED));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_FAIL));
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
LRESULT  CSignCardApplyCheckOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	// ��ձ༭�����Ϣ
	SetEditText(0,_T(""));
	SetEditFocus(0);
	// ��ʼ����Ա����
	m_Left_2_Group->ClickFirstAvailButton();
	return __super::XButtonClick(wParam,lParam);
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �༭����Ϣ�ı�ʱ�Ĵ���

@param      ��

@retval     bool true:��ȷ; false:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardApplyCheckOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
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
bool CSignCardApplyCheckOfficeDlg::ValidateInput()
{
	// ����Ϸ��Լ��
	if(GetEditText(0).GetLength() == 0) {
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_INPUT_ERROR));
		SetEditFocus(0);
		return FALSE;
	}
	// ��������֤��ID�Ƿ���Ч
	const vector<BTN_INFO>& selectedBtnInfo=m_Left_2_Group->GetSelectedBtnInfo();
	if(_ttoi(selectedBtnInfo[0].btnMsg) == CERTIFICATE_ID_CARD_MAINLAND){
		if (!CheckIDCard(GetEditText(0)))
		{
			SetEditFocus(0);
			return false;
		}
	}
	return true;
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
LRESULT CSignCardApplyCheckOfficeDlg::OnCertificateTypeClick(WPARAM wParam,LPARAM lParam)
{
	// ��ȡ��ť��Ϣ
	theSERVICE_MGR.SetState(RECEPT);
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE certifiType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	// ������Ϣ
	GetService<CSignCardApplySvc>()->SetCertifiType(certifiType,btnInfo->text);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
	m_baseInfoArea->Invalidate();
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
LRESULT CSignCardApplyCheckOfficeDlg::OnSevBtnClick(WPARAM wParam,LPARAM lParam)
{
	theSERVICE_MGR.SetState(RECEPT);
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	// ��ȡ��ť��Ϣ
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	BYTE sevType = _ttoi(btnInfo->btnMsg);
	btnInfo->text.Replace(_T("\n"),_T(""));
	// ������Ϣ
	pCSignCardApplySvcc->SetSevType(sevType,btnInfo->btnID);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_SELECT));
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
void CSignCardApplyCheckOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	m_baseInfo->detailInfo.labelGroup = pCSignCardApplySvcc->GetuserInfo();
	try{	
		// ��ѯʧ��
		if (pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_ERROR) {
			//�����������	
			HideCompoment( BASE_AREA | BTN_OK | BTN_RESET|KEYBOARD_AREA  );		
		}
		// ��ѯ����
		else if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_CHECK_OVER)
		{
			HideCompoment(BTN_RESET | BTN_OK | EDIT_BOX |KEYBOARD_AREA  );
			m_baseInfoArea->Invalidate();
		}	
		// ��ѯ
		else if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_CHECK)
		{	
			m_vec_Right_1_BTN_INFO[2].isSelected = true;
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO)	;	
			m_Left_2_Group->ClickFirstAvailButton();
			m_baseInfoArea->Invalidate();	
		}	
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pCSignCardApplySvcc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
