#include "stdafx.h"
#include "SignCardApplyOverOfficeDlg.h"
#include "SignCardApplySvc.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSignCardApplyOverOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CSignCardApplyOverOfficeDlg,COperationDlg)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyOverOfficeDlg::CSignCardApplyOverOfficeDlg(CService* pService) :
COperationDlg(CSignCardApplyOverOfficeDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TXT_SIGNCARD_APPLY_SVC_CN;								// 标题
	m_baseInfo->detailInfo.labelGroup = GetService<CSignCardApplySvc>()->GetuserInfo();
	m_baseInfo->detailInfo.row = GetService<CSignCardApplySvc>()->GetuserInfoCount();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardApplyOverOfficeDlg::~CSignCardApplyOverOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int \n
0:成功  -1:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CSignCardApplyOverOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA |BASE_AREA)){
		return -1;
	}
 
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
void CSignCardApplyOverOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	m_baseInfo->detailInfo.labelGroup = GetService<CSignCardApplySvc>()->GetuserInfo();
	m_baseInfo->detailInfo.row =  GetService<CSignCardApplySvc>()->GetuserInfoCount();
	HideCompoment(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
	m_baseInfoArea->Invalidate();
	CSignCardApplySvc* pCSignCardApplySvcc = (CSignCardApplySvc*)GetService();
	if(pCSignCardApplySvcc->GetCurDialogGroupID()==CSignCardApplySvc::DIALOG_GROUP_ERROR){
		HideCompoment(BASE_AREA);
	}		
	theSERVICE_MGR.SetState(FINISH);
}

