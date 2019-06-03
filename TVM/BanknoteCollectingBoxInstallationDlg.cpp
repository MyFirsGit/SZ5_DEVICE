#include "StdAfx.h"
#include "BanknoteCollectingBoxInstallationDlg.h"


static const int FULL_WITDH = 600;
static const int NAME_WITDH = 400;
static const int ID_WITDH = 150;
static const int INDENT_WIDTH = 50;
static const int GIRD_WITDH = 160;
static const int STATUE_WIDTH = 100;
static const int FACE_WIDTH = 80;
static const int FACE_WIDTH_1 = 120;
static const int COUNT_WIDTH = 50;
static const int MONEY_WIDTH = 130;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CBanknoteCollectingBoxInstallationDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CBanknoteCollectingBoxInstallationDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxInstallationDlg::CBanknoteCollectingBoxInstallationDlg(CService* pService)
	: COperationDlg(CBanknoteCollectingBoxInstallationDlg::IDD,pService)
{
	pSvc = (CBanknoteCollectingBoxInstallationSvc*)pService;
}

////////////////////////////// ////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteCollectingBoxInstallationDlg::~CBanknoteCollectingBoxInstallationDlg()
{
	if (m_baseInfoArea != NULL)	{
		delete m_baseInfoArea;
	}
	m_baseInfoArea = NULL;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:创建成功    -1:创建失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CBanknoteCollectingBoxInstallationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA)) {
		return -1;
	}
	// 设置GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	CBanknoteCollectingBoxInstallationSvc::Model& model = pSvc->GetDataModel();

	// 清除界面内容
	Clear();

	theAPP_SESSION.SetKeyboard(KEY_RETURN|KEY_ENTER|KEY_INPUT|KEY_F2);
	theAPP_SESSION.SetMaxInputLength(2);
	// 画面激活处理
	InitAttachBoxUI();
	if (!pSvc->GetShowErrorMsg().IsEmpty() && pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_FINISH){
		theAPP_SESSION.ShowOfficeGuide(pSvc->GetShowErrorMsg());
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::UpdateUI()
{
	// 清除界面
	Clear();
	// 安装画面 
	ShowAttachBoxData();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		安装纸币箱画面布局初始化   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::InitAttachBoxUI()
{
	// 基本信息格式
	m_baseInfoArea->SetLableAlign(LABEL_COL_1, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_2, DT_LEFT | DT_VCENTER);
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 400);					// 纸币箱 ID 
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 100);					// 状态

	// 菜单栏格式
	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_OPERATION)
	{
		m_baseInfoArea->SetLableWidth(LABEL_ROW_2, LABEL_COL_1, FULL_WITDH);	// 菜单1
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		清空数据   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::Clear()
{
	for (int i = 0; i < 15; ++i)
	{
		m_lableGroup[i].name1 = _T(""); 
		m_lableGroup[i].name2 = _T("");
		m_lableGroup[i].name3 = _T("");
		m_lableGroup[i].name4 = _T("");
		m_lableGroup[i].name5 = _T("");
		m_lableGroup[i].name6 = _T("");
		m_lableGroup[i].name7 = _T("");
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief		显示安装纸币箱数据   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteCollectingBoxInstallationDlg::ShowAttachBoxData()
{
	CBanknoteCollectingBoxInstallationSvc::Model& model = pSvc->GetDataModel();

	if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_OPERATION)
	{
		m_lableGroup[LABEL_ROW_1].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+ _T(" %s"), model.strBoxIDSt);
		m_lableGroup[LABEL_ROW_1].name2 = model.strState;

		m_baseInfoArea->SetLabelColor(LABEL_ROW_1, LABEL_COL_2, model.clrState);
		m_lableGroup[LABEL_ROW_8].name1.Format(_T("[%d] ")+_opl(TXT_BANKNOTEMGR_MENU_ATTACH), MENU_CODE_1);	
	}
	else if (pSvc->GetCurDialogGroupID() == CBanknoteCollectingBoxInstallationSvc::DIALOG_GROUP_FINISH)
	{
		m_lableGroup[LABEL_ROW_1].name1 = _opl(TXT_BANKNOTEMGR_BOX_BEFORE_ATTACH);

		m_lableGroup[LABEL_ROW_2].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+ _T(" %s"),model.strBoxIDEx);

		m_lableGroup[LABEL_ROW_6].name1 = _opl(TXT_BANKNOTEMGR_BOX_AFTER_ATTACH);

		m_lableGroup[LABEL_ROW_7].name1.Format(_opl(TXT_BANKNOTEMGR_BANKNOTE_BOX) + _opl(TXT_COLON)+_T(" %s"), model.strBoxIDSt);
	}
	else
	{
		Clear();
	}
}
