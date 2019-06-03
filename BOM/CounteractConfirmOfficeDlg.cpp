#include "stdafx.h"
#include "CounteractConfirmOfficeDlg.h"
#include "CounteractSvc.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCounteractConfirmOfficeDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CCounteractConfirmOfficeDlg, COperationDlg)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCounteractConfirmOfficeDlg::CCounteractConfirmOfficeDlg(CService* pService) :
	COperationDlg(CCounteractConfirmOfficeDlg::IDD, pService)
{
	CCounteractSvc* pCounteractSvc = (CCounteractSvc*)GetService();	
	m_baseInfo->titleInfo.titleName = TITLE_TICKET_INFO;								// 标题
	m_baseInfo->detailInfo.row = pCounteractSvc->GetCounteractConfirmInfoSize();							// 行数
	m_baseInfo->detailInfo.labelGroup = pCounteractSvc->GetCounteractConfirmInfo();		// 标签文字（2列）
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCounteractConfirmOfficeDlg::~CCounteractConfirmOfficeDlg()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CCounteractConfirmOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域，返回键，确认键
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA|BASE_AREA  | BTN_OK)){
		return -1;
	}
    return 0;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认、返回,按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
			TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCounteractConfirmOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CCounteractSvc* pCounteractSvc = (CCounteractSvc*)GetService();
	pCounteractSvc->OnOK();
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractConfirmOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CCounteractSvc* pCounteractSvc = (CCounteractSvc*)GetService();
	if (pCounteractSvc->GetCurDialogGroupID()==CCounteractSvc::DIALOG_GROUP_ERROR) {
		HideCompoment(BASE_AREA);
		HideCompoment(BTN_OK);
	}
	else if (pCounteractSvc->GetCurDialogGroupID()==CCounteractSvc::DIALOG_GROUP_CONFIRM) {
		ShowCompoment(BASE_AREA);
		ShowCompoment(BTN_OK);
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏画面上所有组件

@param      无

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CCounteractConfirmOfficeDlg::HideAllComponent()
//{
//    HideCompoment(BTN_BACK | BTN_OK | BTN_PRINT | BASE_AREA | CASH_AREA | EDIT_BOX);
//}
