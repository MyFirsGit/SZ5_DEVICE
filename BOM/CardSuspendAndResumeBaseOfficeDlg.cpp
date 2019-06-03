#include "stdafx.h"
#include "CardSuspendAndResumeBaseOfficeDlg.h"
#include "AppSession.h"
#include "CardSuspendAndResumeSvc.h"
#include "guideinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CCardSuspendAndResumeBaseOfficeDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CCardSuspendAndResumeBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
	ON_GROUP_BUTTON_CLICK(LEFT_1_GROUP_ID,OnSexTypeBtnClick)
	ON_GROUP_BUTTON_CLICK(LEFT_2_GROUP_ID,OnCertificateTypeBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnSevBtnClick)
END_MESSAGE_MAP()

static CRect editUserInfo[] = 
{
	BASE_INFO_RECT(1,20),
	BASE_INFO_RECT(4,20),
};

static const CString STR_SUSPEND_RESUME		= _T("SUSPEND_RESUME");
static const CString STR_CERTIFICATE_TYPE	= _T("CERTIFICATE_TYPE");
static const CString STR_SEX_TYPE		    = _T("SEX_TYPE");
static const CString STR_COUNT				= _T("COUNT");

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCardSuspendAndResumeBaseOfficeDlg::CCardSuspendAndResumeBaseOfficeDlg(CService* pService)
: COperationDlg(CCardSuspendAndResumeBaseOfficeDlg::IDD, pService)
{
	CCardSuspendAndResumeSvc* pCardSuspendAndResumeSvc = (CCardSuspendAndResumeSvc*)GetService();
	m_baseInfo->titleInfo.titleName = TXT_SUSPEND_AND_RESUME_SVC_CN;					// ����
	m_baseInfo->detailInfo.row = 4;														// ����
	m_baseInfo->detailInfo.labelGroup = pCardSuspendAndResumeSvc->GetLableUserInfo();	// ��ǩ����
	m_baseInfo->detailInfo.editGroup.cnt = 2;											// �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editUserInfo;							// �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCardSuspendAndResumeBaseOfficeDlg::~CCardSuspendAndResumeBaseOfficeDlg()
{
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
int CCardSuspendAndResumeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA | BTN_OK | BTN_RESET) == -1)
	{
		return -1;
	}
	SetEditFocus(USERINFO_EDIT_GROUP_USER_NAME);
	GetEdit(USERINFO_EDIT_GROUP_USER_NAME)->SetScope(CGEdit::NAME);
	GetEdit(USERINFO_EDIT_GROUP_USER_NAME)->SetLimitText(20);
	GetEdit(USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER)->SetScope(CGEdit::NUMANDCHAR);
	GetEdit(USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER)->SetLimitText(20);
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
BOOL CCardSuspendAndResumeBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	// ��ʾBOM���ԣ���ѡ���ʧ����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_SUSPEND_AND_RESUME_SELECT));

	// ��ʼ����ť
	InitialButtons();

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
void CCardSuspendAndResumeBaseOfficeDlg::InitialButtons()
{
	m_vec_Left_1_BTN_INFO.clear();
	m_vec_Left_2_BTN_INFO.clear();
	m_vec_Right_1_BTN_INFO.clear();

	// �ȶ�������
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);

	int svcCount = getini.GetDataInt(STR_SUSPEND_RESUME,STR_COUNT);
	int certificateCount = getini.GetDataInt(STR_CERTIFICATE_TYPE,STR_COUNT);
	int sexCount = getini.GetDataInt(STR_SEX_TYPE,STR_COUNT);

	CString strSexNo[] = {_T("1"),_T("2")};
	for (int i = 0;i < sexCount;i++){
		BTN_INFO pSexBtnInfo;
		CString sexTypeText = getini.GetDataString(STR_SEX_TYPE,strSexNo[i]);
		pSexBtnInfo.btnMsg = strSexNo[i];
		pSexBtnInfo.text = sexTypeText;
		m_vec_Left_1_BTN_INFO.push_back(pSexBtnInfo);
	}

	CString strCertificateNo[] = {_T("1"),_T("2"),_T("3"),_T("10")};
	for (int j = 0;j < certificateCount;j++){
		BTN_INFO pCertificateBtnInfo;
		CString certificateTypeText = getini.GetDataString(STR_CERTIFICATE_TYPE,strCertificateNo[j]);
		certificateTypeText.Replace(_T("\\n"),_T("\n"));
		pCertificateBtnInfo.btnMsg = strCertificateNo[j];
		pCertificateBtnInfo.text = certificateTypeText;
		m_vec_Left_2_BTN_INFO.push_back(pCertificateBtnInfo);
	}

	CString strSvcNo[] = {_T("1"),_T("2")};
	for (int k = 0;k < svcCount;k++){
		BTN_INFO pSvcBtnInfo;
		CString svcText = getini.GetDataString(STR_SUSPEND_RESUME,strSvcNo[k]);
		pSvcBtnInfo.btnMsg = strSvcNo[k];
		pSvcBtnInfo.text = svcText;
		m_vec_Right_1_BTN_INFO.push_back(pSvcBtnInfo);
	}

	m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
	m_Left_1_Group->ClickFirstAvailButton();
	m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
	m_Left_2_Group->ClickFirstAvailButton();
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_1_Group->ClickFirstAvailButton();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCardSuspendAndResumeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CCardSuspendAndResumeSvc* pCardSuspendAndResumeSvc = (CCardSuspendAndResumeSvc*)GetService();
	try	{
		if (!ValidateInput())
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_APPLY_INPUT_ERROR));
			return FALSE;
		}

		pCardSuspendAndResumeSvc->SetUserName(GetEditText(USERINFO_EDIT_GROUP_USER_NAME));
		pCardSuspendAndResumeSvc->SetCertificateNumber(GetEditText(USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER));
		
		if(pCardSuspendAndResumeSvc->GetSvcType() == SVC_TYPE_SUSPEND){
			pCardSuspendAndResumeSvc->SuspendCard();
			// ��ʾBOM���ԣ���ʧ��������
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_SUSPEND_SUCCESS));
		}
		else if(pCardSuspendAndResumeSvc->GetSvcType() == SVC_TYPE_RESUME){
			pCardSuspendAndResumeSvc->ResumeCard();
			// ��ʾBOM���ԣ������������
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_RESUME_SUCCESS));
		}

		// ���ñ�ǩ����
		m_baseInfo->detailInfo.labelGroup[0].nameRight = GetEditText(USERINFO_EDIT_GROUP_USER_NAME);
		m_baseInfo->detailInfo.labelGroup[3].nameRight = GetEditText(USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER);
		// ���ر༭��ȷ�ϰ�ť�ͼ���
		HideCompoment(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		// ˢ�´���
		ShowCompoment(BASE_AREA);
		// �������ఴť
		m_vec_Left_1_BTN_INFO.clear();
		m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
		m_vec_Left_2_BTN_INFO.clear();
		m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	}
	catch (CSysException& e) {
		if(pCardSuspendAndResumeSvc->GetSvcType() == SVC_TYPE_SUSPEND){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_SUSPEND_FAIL));
		}
		else if(pCardSuspendAndResumeSvc->GetSvcType() == SVC_TYPE_RESUME){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_RESUME_FAIL));
		}
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		if(pCardSuspendAndResumeSvc->GetSvcType() == SVC_TYPE_SUSPEND){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_SUSPEND_FAIL));
		}
		else if(pCardSuspendAndResumeSvc->GetSvcType() == SVC_TYPE_RESUME){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_RESUME_FAIL));
		}
		theEXCEPTION_MGR.ProcessException(CInnerException(pCardSuspendAndResumeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϡ����ء����� ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCardSuspendAndResumeBaseOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	//����ı���
	SetEditText(USERINFO_EDIT_GROUP_USER_NAME,_T(""));
	SetEditText(USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER,_T(""));

	//��ʼ����ť
	m_Left_1_Group->ClickFirstAvailButton();
	m_Left_2_Group->ClickFirstAvailButton();

	// ���õ�����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_SUSPEND_AND_RESUME_SELECT));
	return __super::XButtonClick(wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʧ��Ұ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardSuspendAndResumeBaseOfficeDlg::OnSevBtnClick(WPARAM wParam,LPARAM lParam)
{
	CCardSuspendAndResumeSvc* pCardSuspendAndResumeSvc = (CCardSuspendAndResumeSvc*)GetService();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	SR_SVC_TYPE svc = (SR_SVC_TYPE)_ttoi(btnInfo->btnMsg);
	pCardSuspendAndResumeSvc->SetSvcType(svc);
	
	m_baseInfoArea->Invalidate();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ա�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardSuspendAndResumeBaseOfficeDlg::OnSexTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	CCardSuspendAndResumeSvc* pCardSuspendAndResumeSvc = (CCardSuspendAndResumeSvc*)GetService();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	m_baseInfo->detailInfo.labelGroup[1].nameRight = btnInfo->text;

	BYTE sex = _ttoi(btnInfo->btnMsg);
	pCardSuspendAndResumeSvc->SetSexType(sex,btnInfo->text);

	m_baseInfoArea->Invalidate();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֤�����Ͱ�ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCardSuspendAndResumeBaseOfficeDlg::OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam)
{
	CCardSuspendAndResumeSvc* pCardSuspendAndResumeSvc = (CCardSuspendAndResumeSvc*)GetService();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;

	CString certificateName = btnInfo->text;
	certificateName.Replace(_T("\n"),_T(""));
	m_baseInfo->detailInfo.labelGroup[2].nameRight = certificateName;

	CERTIFICATE_TYPE certificate = (CERTIFICATE_TYPE)_ttoi(btnInfo->btnMsg);
	pCardSuspendAndResumeSvc->SetCertificateType(certificate,certificateName);

	m_baseInfoArea->Invalidate();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     �������Ϸ��Լ�� 

@param      ��

@retval     bool true:��ȷ false:����

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CCardSuspendAndResumeBaseOfficeDlg::ValidateInput()
{
	CCardSuspendAndResumeSvc* pCardSuspendAndResumeSvc = (CCardSuspendAndResumeSvc*)GetService();

	for(int i  =0;i < 2;i++){
		if(GetEditText(i) == ""){
			SetEditFocus(i);
			return false;
		}
	}
	
	if(pCardSuspendAndResumeSvc->GetCertificateType() == CERTIFICATE_ID_CARD_MAINLAND){
		if (!CheckIDCard(GetEditText(USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER))){
			SetEditFocus(1);
			return false;
		}
	}

	return true;
}