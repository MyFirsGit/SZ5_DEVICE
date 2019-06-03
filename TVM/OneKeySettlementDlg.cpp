#include "StdAfx.h"
#include "OneKeySettlementDlg.h"
#include "OneKeySettlementSvc.h"

IMPLEMENT_DYNAMIC(COneKeySettlementDlg, COperationDlg)
BEGIN_MESSAGE_MAP(COneKeySettlementDlg, COperationDlg)
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
COneKeySettlementDlg::COneKeySettlementDlg(CService* pService)
	: COperationDlg(COneKeySettlementDlg::IDD,pService)
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
COneKeySettlementDlg::~COneKeySettlementDlg()
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
int COneKeySettlementDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA))
	{
		return -1;
	}

	//设置Gstatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LableGroupFour);

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
void COneKeySettlementDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	COneKeySettlementSvc* pSvc = (COneKeySettlementSvc*)GetService();

	if (pSvc->GetCurDialogGroupID() == COneKeySettlementSvc::DIALOG_GROUP_REMOVE_EXCEPTION)
	{
		//设置键盘可用
		theAPP_SESSION.SetKeyboard(KEY_F2 | KEY_INPUT |KEY_ENTER|KEY_PAGE|KEY_RETURN);
		InitDialogUI();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      刷新界面数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementDlg::UpdateUI()
{
	 COneKeySettlementSvc* pSvc = (COneKeySettlementSvc*)GetService();
	 map<COneKeySettlementSvc::PROCESS,COneKeySettlementSvc::PROCESS_INFO> mapProc = pSvc->GetDataModol().mapSettlement;

	 map<COneKeySettlementSvc::PROCESS,COneKeySettlementSvc::PROCESS_INFO>::iterator iter = mapProc.begin();
	 int i=0;
	 for (;iter!= mapProc.end() && i <mapProc.size();i ++,iter++)
	 {
		 m_LableGroupFour[i].name1.Format(_T("[%d] %s"),i+1, iter->second.procName);
		 m_LableGroupFour[i].name2 = _T("[1]") + _opl(DEVICESTATUS_YES);
		 m_LableGroupFour[i].name3 = _T("[2]") + _opl(DEVICESTATUS_NO);
		 m_LableGroupFour[i].name4 = iter->second.GetSwitchString();
		 m_baseInfoArea->SetLabelColor(i,3,iter->second.GetSwitchColor());
		 m_LableGroupFour[i].name5 = iter->second.GetStateString();
		 m_baseInfoArea->SetLabelColor(i,4,iter->second.GetStateColor());
	 }
	 //m_LableGroupFour[i].name1.Format(_T("[%d]") + _opl(DEVICESTATUS_RESET),i+1);
	 m_baseInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化界面

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementDlg::InitDialogUI()
{
	m_baseInfoArea->SetLableWidth(0,280);	//收机项
	m_baseInfoArea->SetLableWidth(1,100);	//开启
	m_baseInfoArea->SetLableWidth(2,100);	//关闭
	m_baseInfoArea->SetLableWidth(3,100);	//开、关
	m_baseInfoArea->SetLableWidth(4,150);	// 状态

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
	m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(4,DT_CENTER);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      强制更新界面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementDlg::UpdateLabelData(){
	UpdateUI();
	if(NULL != m_baseInfoArea){
		m_baseInfoArea->UpdateWindow();
	}
}
