#include "stdafx.h"
#include "FunctionDlg.h"
#include "ButtonGroup.h"
#include "Panel.h"
#include "BitmapManager.h"

IMPLEMENT_DYNAMIC(CFunctionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CFunctionDlg,CBaseDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief   构造函数   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CFunctionDlg::CFunctionDlg(UINT ID,CRect rect,UINT rows,UINT cols,UINT groupID,CWnd* pParent)
:CBaseDlg(ID,pParent,rect,OPERATOR_FUNC_BK_COLOR)
{
	UIINFO groupUI;
	groupUI.m_pParentWnd = this;
	groupUI.m_DockStyle = DOCKSTYLE::NONE;
	groupUI.m_Size = CPoint(rect.Width(),rect.Height());
	groupUI.m_BKMode = BKMODE_LAST;
	groupUI.m_BorderWidth=0;
	groupUI.m_pBackgroundImage = &theBitmap_MGR.m_pGroup;
	pButtonGroup = new CButtonGroup(groupUI,rows,cols,groupID);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CFunctionDlg::~CFunctionDlg()
{
	delete pButtonGroup;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      重载OnCreate

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CFunctionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);
	return pButtonGroup->Create();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     显示或隐藏按钮 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CFunctionDlg::ShowButtons(bool showButtons)
{
	pButtonGroup->ShowButtons(showButtons);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转发消息到OperationDialog

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CFunctionDlg::ForwardGroupMessage(WPARAM wParam,LPARAM lParam)
{
	CTVMForeService* currentForeService = (CTVMForeService*)theSERVICE_MGR.GetCurService();
	if(currentForeService!=NULL)
	{
		COperationDlg* currentOperationDlg = currentForeService->GetCurOperationDlg();
		if(currentOperationDlg!=NULL)
		{
			return currentOperationDlg->CallWndProc(WM_XGROUP,wParam,lParam);
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief    窗口函数过程  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CFunctionDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_XGROUP)
	{
		ForwardGroupMessage(wParam,lParam);
	}
	return __super::WindowProc(message,wParam,lParam);
}