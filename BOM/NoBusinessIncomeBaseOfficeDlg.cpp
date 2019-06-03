#include "stdafx.h"
#include "nobusinessincomebaseofficedlg.h"
#include "nobusinessincomesvc.h"
#include "guideinfo.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const TCHAR* const INCOME_NAME				= _T("收入");
const TCHAR* const EXPEND_NAME				= _T("支出");
static const CString STR_INCOME_REASON		= _T("INCOME_REASON");
static const CString STR_EXPENSE_REASON		= _T("EXPENSE_REASON");
static const CString STR_COUNT				= _T("COUNT");
static const int EXPEND_TYPE_ID[] = {50,51,52,53,99};
static const int INCOME_TYPE_ID[] = {99};

//编辑框位置
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
@brief      构造函数

@param      (i)CService* pService

@retval     none 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CNoBusinessIncomeBaseOfficeDlg::CNoBusinessIncomeBaseOfficeDlg(CService* pService) :
    COperationDlg(CNoBusinessIncomeBaseOfficeDlg::IDD, pService)
{
	CNoBusinessIncomeSvc* pNoBusinessIncomeSvc = (CNoBusinessIncomeSvc*)GetService();
	// 基本信息区域
	m_baseInfo->titleInfo.titleName = TITLE_ADMIN_PROCESS;								// 标题
	m_baseInfo->detailInfo.row = 4;														// 行数
	m_baseInfo->detailInfo.labelGroup = pNoBusinessIncomeSvc->GetBusinessInfo();		// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 1;											// 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editBusinessInfo;						// 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

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
  @brief      画面创建函数

  @param      (i)LPCREATESTRUCT lpCreateStruct

  @retval     int  0:成功  -1:失败

  @exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CNoBusinessIncomeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建向导区域，基本信息区域，确认键
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
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CNoBusinessIncomeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CNoBusinessIncomeSvc* pNoBusinessIncomeSvc = (CNoBusinessIncomeSvc*)GetService();
	try{
		// 检查输入合法性
		if(!CheckInputValid()){
			return FALSE;
		}

		pNoBusinessIncomeSvc->SetAmount(ComGetAmount(GetEditText(0)));

		// 非营业性收支处理
		pNoBusinessIncomeSvc->DoAdministrationProcess();

		// 显示BOM文言：非营业性收支业务正常结束
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SUCCEED));

		// 隐藏编辑框和确认按钮
		HideCompoment(BTN_OK | EDIT_BOX | KEYBOARD_AREA);

		// 设置标签文字
		m_baseInfo->detailInfo.labelGroup[3].nameRight = ComFormatAmount(ComGetAmount(GetEditText(0))) + MONEY_UNIT;
		
		// 刷新窗体
		ShowCompoment(BASE_AREA);

		// 隐藏右侧按钮
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
@brief      检测编辑框输入是否正确

@param      none

@retval     bool	true:输入有效  false:输入无效

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CNoBusinessIncomeBaseOfficeDlg::CheckInputValid()
{
	// 检查收支类型是否已选择 
	if(!m_BtnChecked){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SELECT));
		return false;
	}
	// 检查是否已输入收支金额
	if(GetEditText(0).IsEmpty()){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_INPUT_NULL));
		// 设置光标
		GetEdit(0)->SetFocus();
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      收入原因按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n  TRUE:处理成功  FALSE:处理失败

@exception  无
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
	
	// 获取手续费
	DWORD serviceFee = pNoBusinessIncomeSvc->GetFee((AdministrationProcessType)btnInfo->btnID);
	m_baseInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(serviceFee) + MONEY_UNIT;

	m_BtnChecked = true;

	m_baseInfoArea->Invalidate();
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      支出原因按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n   TRUE:处理成功  FALSE:处理失败

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

	// 获取手续费
	DWORD serviceFee = pNoBusinessIncomeSvc->GetFee((AdministrationProcessType)btnInfo->btnID);
	m_baseInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(serviceFee) + MONEY_UNIT;

	m_BtnChecked = true;

	m_baseInfoArea->Invalidate();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		对话框初始化

@param      无

@retval     BOOL  \n   TRUE:初始化成功  FALSE:初始化失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CNoBusinessIncomeBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	m_BtnChecked = false;

	// 显示BOM文言：请选择收支类型和收支原因
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_NON_BUISNESS_SELECT));

	// 初始化类别按键
	InitButtons();

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		初始化类别按键

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNoBusinessIncomeBaseOfficeDlg::InitButtons()
{
	// 先读出个数
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