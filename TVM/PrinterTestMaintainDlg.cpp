#include "stdafx.h"
#include "PrinterTestMaintainDlg.h"
#include "PrinterTestSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPrinterTestMaintainDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CPrinterTestMaintainDlg,COperationDlg)
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
CPrinterTestMaintainDlg::CPrinterTestMaintainDlg(CService* pService)
: COperationDlg(CPrinterTestMaintainDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestMaintainDlg::~CPrinterTestMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      窗体创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct 窗体创建的区域

@retval     int		\n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA)) {
		return -1;
	}

	// 设置GStatic
	m_baseInfo->detailInfo.row        = _MAX_ROW;
	m_baseInfo->detailInfo.labelGroup = m_LableGroup;
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
void CPrinterTestMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	Clear();
	//Update Menu status when Service input [Enter] 
	CPrinterTestSvc* pSvc = (CPrinterTestSvc*)GetService();
	if (pSvc->GetCurDialogGroupID() == CPrinterTestSvc::DIALOG_GROUP_PRINT)
	{
		theAPP_SESSION.SetKeyboard(KEY_ENTER|KEY_INPUT|KEY_RETURN);
		theAPP_SESSION.SetMaxInputLength(1);
		InitMenuUI();
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      刷新界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestMaintainDlg::UpdateUI()
{
	Clear();
	CPrinterTestSvc* pSvc = (CPrinterTestSvc*)GetService();
	if (pSvc->GetCurDialogGroupID() == CPrinterTestSvc::DIALOG_GROUP_PRINT)
	{
		UpdateMenuData();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置主菜单界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestMaintainDlg::InitMenuUI()
{
	//设置菜单选项宽度
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);

	//Set aliagn
	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置菜单数据

@param      none

@retval     none

@exception  none1
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestMaintainDlg::UpdateMenuData()
{
	CPrinterTestSvc* pSvc = (CPrinterTestSvc*)GetService();
	CPrinterTestSvc::Modol& modol = pSvc->GetDataModol();
	//菜单
	m_LableGroup[0].nameLeft  = _T("[1]") + _opl(PRINT_TEST_TEXT_RESET);
	m_LableGroup[1].nameLeft  = _T("[2]") + _opl(PRINT_TEST_TEXT_PRINT);


	//菜单数据
	for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu)
	{
		m_LableGroup[nMenu].nameRight = modol.menuInfo[nMenu].strMenuStatus;
		m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 1,modol.menuInfo[nMenu].strMenuStatus == FAILED ? RED : SOFTGREEN);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清除界面显示结果

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestMaintainDlg::Clear()
{
	for (int nRow = 0; nRow < _MAX_ROW; ++nRow)
	{
		m_LableGroup[nRow].nameLeft    = UNKNOW;
		m_LableGroup[nRow].nameRight   = UNKNOW;

		for(int nCol = 0; nCol < _MAX_COL; ++nCol)
		{
			m_baseInfoArea->SetLabelColor(nRow,nCol,BLACK);
		}		
	} 
}