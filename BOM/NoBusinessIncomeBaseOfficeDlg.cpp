#include "stdafx.h"
#include "nobusinessincomebaseofficedlg.h"
#include "nobusinessincomesvc.h"
#include "guideinfo.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR* const INCOME_NAME				= _T("����");
const TCHAR* const EXPEND_NAME				= _T("֧��");
static const CString STR_INCOME_REASON		= _T("INCOME_REASON");
static const CString STR_EXPENSE_REASON		= _T("EXPENSE_REASON");
static const CString STR_COUNT				= _T("COUNT");
static const int EXPEND_TYPE_ID[] = {50,51,52,53,99};
static const int INCOME_TYPE_ID[] = {99};

//�༭��λ��
static CRect editBusinessInfo[] = {BASE_INFO_RECT(4,7)};

IMPLEMENT_DYNAMIC(CNoBusinessIncomeBaseOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CNoBusinessIncomeBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnExpendBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnIncomeBtnClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CNoBusinessIncomeBaseOfficeDlg::CNoBusinessIncomeBaseOfficeDlg(CService* pService) :
    COperationDlg(CNoBusinessIncomeBaseOfficeDlg::IDD, pService)
{
	CNoBusinessIncomeSvc* pNoBusinessIncomeSvc = (CNoBusinessIncomeSvc*)GetService();
	// ������Ϣ����
	m_baseInfo->titleInfo.titleName = TITLE_ADMIN_PROCESS;								// ����
	m_baseInfo->detailInfo.row = 4;														// ����
	m_baseInfo->detailInfo.labelGroup = pNoBusinessIncomeSvc->GetBusinessInfo();		// ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 1;											// �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editBusinessInfo;						// �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CNoBusinessIncomeBaseOfficeDlg::~CNoBusinessIncomeBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
  @brief      ���洴������

  @param      (i)LPCREATESTRUCT lpCreateStruct

  @retval     int  0:�ɹ�  -1:ʧ��

  @exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CNoBusinessIncomeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ���������򣬻�����Ϣ����ȷ�ϼ�
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA | BTN_OK)){
		return -1;
	}
	GetEdit(0)->SetScope(CGEdit::CASH);
	GetEdit(0)->SetLimitText(7);
	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CNoBusinessIncomeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CNoBusinessIncomeSvc* pNoBusinessIncomeSvc = (CNoBusinessIncomeSvc*)GetService();
	try{
		// �������Ϸ���
		if(!CheckInputValid()){
			return FALSE;
		}

		pNoBusinessIncomeSvc->SetAmount(ComGetAmount(GetEditText(0)));

		// ��Ӫҵ����֧����
		pNoBusinessIncomeSvc->DoAdministrationProcess();

		// ��ʾBOM���ԣ���Ӫҵ����֧ҵ����������
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SUCCEED));

		// ���ر༭���ȷ�ϰ�ť
		HideCompoment(BTN_OK | EDIT_BOX | KEYBOARD_AREA);

		// ���ñ�ǩ����
		m_baseInfo->detailInfo.labelGroup[3].nameRight = ComFormatAmount(ComGetAmount(GetEditText(0))) + MONEY_UNIT;
		
		// ˢ�´���
		ShowCompoment(BASE_AREA);

		// �����Ҳఴť
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_vec_Right_2_BTN_INFO.clear();
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);

	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_FAIL));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_FAIL));
		theEXCEPTION_MGR.ProcessException(CInnerException(NOBUSINESS_INCOME_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���༭�������Ƿ���ȷ

@param      none

@retval     bool	true:������Ч  false:������Ч

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CNoBusinessIncomeBaseOfficeDlg::CheckInputValid()
{
	// �����֧�����Ƿ���ѡ�� 
	if(!m_BtnChecked){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SELECT));
		return false;
	}
	// ����Ƿ���������֧���
	if(GetEditText(0).IsEmpty()){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_INPUT_NULL));
		// ���ù��
		GetEdit(0)->SetFocus();
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ԭ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNoBusinessIncomeBaseOfficeDlg::OnIncomeBtnClick(WPARAM wParam,LPARAM lParam)
{
	m_Right_1_Group->ClearSelection();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	m_baseInfo->detailInfo.labelGroup[0].nameRight = add_hsp(btnInfo->text,MAX_SBC_CASE_LEN_RIGHT);
	m_baseInfo->detailInfo.labelGroup[1].nameRight = add_hsp(INCOME_NAME,MAX_SBC_CASE_LEN_RIGHT);

	CNoBusinessIncomeSvc* pNoBusinessIncomeSvc = (CNoBusinessIncomeSvc*)GetService();
	pNoBusinessIncomeSvc->SetBusinessType(BUSINESS_TYPE_INCOME,INCOME_NAME);
	pNoBusinessIncomeSvc->SetProcessType((AdministrationProcessType)btnInfo->btnID,btnInfo->text);
	
	// ��ȡ������
	DWORD serviceFee = pNoBusinessIncomeSvc->GetFee((AdministrationProcessType)btnInfo->btnID);
	m_baseInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(serviceFee) + MONEY_UNIT;

	m_BtnChecked = true;

	m_baseInfoArea->Invalidate();
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֧��ԭ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n   TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CNoBusinessIncomeBaseOfficeDlg::OnExpendBtnClick(WPARAM wParam,LPARAM lParam)
{
	m_Right_2_Group->ClearSelection();

	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	CString name = btnInfo->text;
	name.Replace(_T("\n"),_T(""));
	m_baseInfo->detailInfo.labelGroup[0].nameRight = add_hsp(name,MAX_SBC_CASE_LEN_RIGHT);
	m_baseInfo->detailInfo.labelGroup[1].nameRight = add_hsp(EXPEND_NAME,MAX_SBC_CASE_LEN_RIGHT);

	CNoBusinessIncomeSvc* pNoBusinessIncomeSvc = (CNoBusinessIncomeSvc*)GetService();
	pNoBusinessIncomeSvc->SetBusinessType(BUSINESS_TYPE_EXPENSE,EXPEND_NAME);
	pNoBusinessIncomeSvc->SetProcessType((AdministrationProcessType)btnInfo->btnID,name);

	// ��ȡ������
	DWORD serviceFee = pNoBusinessIncomeSvc->GetFee((AdministrationProcessType)btnInfo->btnID);
	m_baseInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(serviceFee) + MONEY_UNIT;

	m_BtnChecked = true;

	m_baseInfoArea->Invalidate();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ի����ʼ��

@param      ��

@retval     BOOL  \n   TRUE:��ʼ���ɹ�  FALSE:��ʼ��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CNoBusinessIncomeBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	m_BtnChecked = false;

	// ��ʾBOM���ԣ���ѡ����֧���ͺ���֧ԭ��
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SELECT));

	// ��ʼ����𰴼�
	InitButtons();

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		��ʼ����𰴼�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeBaseOfficeDlg::InitButtons()
{
	// �ȶ�������
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);

	int incomeCount = getini.GetDataInt(STR_INCOME_REASON,STR_COUNT);
	int expendCount = getini.GetDataInt(STR_EXPENSE_REASON,STR_COUNT);

	CString strExpendNo;
	for (int m = 0;m < expendCount;m++){
		BTN_INFO pExpendBtnInfo;
		strExpendNo.Format(_T("%d"),m+1);
		CString expendTypeText = getini.GetDataString(STR_EXPENSE_REASON,strExpendNo);
		expendTypeText.Replace(_T("\\n"),_T("\n"));
		pExpendBtnInfo.btnID = EXPEND_TYPE_ID[m];
		pExpendBtnInfo.btnMsg = _T("Expend") + strExpendNo;
		pExpendBtnInfo.text = expendTypeText;
		m_vec_Right_1_BTN_INFO.push_back(pExpendBtnInfo);
	}
	
	CString strIncomeNo;
	for (int n = 0;n < incomeCount;n++){
		BTN_INFO pIncomeBtnInfo;
		strIncomeNo.Format(_T("%d"),n+1);
		CString incomeTypeText = getini.GetDataString(STR_INCOME_REASON,strIncomeNo);
		pIncomeBtnInfo.btnID = INCOME_TYPE_ID[n];
		pIncomeBtnInfo.btnMsg = _T("Income") + strIncomeNo;
		pIncomeBtnInfo.text = incomeTypeText;
		m_vec_Right_2_BTN_INFO.push_back(pIncomeBtnInfo);
	}
	
	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
}