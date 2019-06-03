#include "stdafx.h"
#include "DataOutputSelectMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "DataOutputSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDataOutputSelectMaintainDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CDataOutputSelectMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSelectMaintainDlg::CDataOutputSelectMaintainDlg(CService* pService) :
COperationDlg(CDataOutputSelectMaintainDlg::IDD, pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSelectMaintainDlg::~CDataOutputSelectMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int  CDataOutputSelectMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA))
	{
		return -1;
	}

	m_baseInfo->detailInfo.row = _MAX_ROW;
	m_baseInfo->detailInfo.labelGroup = m_LabelGroup;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void  CDataOutputSelectMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	//设置按键允许类型，及最大输入长度
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	theAPP_SESSION.SetMaxInputLength(1);
	//初始化界面
	InitOutputUI();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新界面显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSelectMaintainDlg::UpdateUI()
{
	UpdateOutputData();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSelectMaintainDlg::InitOutputUI()
{
	m_baseInfoArea->SetLableWidth(0,RESOLUTION * 600);
	m_baseInfoArea->SetLableWidth(1,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSelectMaintainDlg::UpdateOutputData()
{
	m_LabelGroup[0].nameLeft  = _T("[1]");
	m_LabelGroup[0].nameLeft +=	_opl(DATAOUTPUT_DATATYPE_BUSINESS_BT);
	m_LabelGroup[0].nameRight = _T("");

	m_LabelGroup[1].nameLeft  = _T("[2]") ;
	m_LabelGroup[1].nameLeft += _opl(DATAOUTPUT_DATATYPE_TRANSACTION_BT);
	m_LabelGroup[1].nameRight = _T("");

	m_LabelGroup[2].nameLeft  = _T("[3]") ;
	m_LabelGroup[2].nameLeft += _opl(DATAOUTPUT_DATATYPE_LOG_BT);
	m_LabelGroup[2].nameRight = _T("");

}