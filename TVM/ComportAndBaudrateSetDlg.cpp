#include "stdafx.h"
#include "ComportAndBaudrateSetDlg.h"
#include "ComportAndBaudrateSetSvc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CComportAndBaudrateSetDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CComportAndBaudrateSetDlg, COperationDlg)
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
CComportAndBaudrateSetDlg::CComportAndBaudrateSetDlg(CService* pService) 
	: COperationDlg(CComportAndBaudrateSetDlg::IDD, pService){
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetDlg::~CComportAndBaudrateSetDlg(){
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int   0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CComportAndBaudrateSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct){

	if (__super::OnCreate(lpCreateStruct,  GUIDE_AREA) == -1){
		return -1;
	}
	
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_labelGroupThree);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      画面激活函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(2);
	ClearLableData();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetDlg::UpdateUI(){
	ShowComportAndBaudRate();
	m_baseInfoArea->Invalidate(FALSE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      显示详细信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetDlg::ShowComportAndBaudRate(){
	// 设置行列宽度
	m_baseInfoArea->SetLableWidth(0,200);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,400);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);

	CComportAndBaudrateSetSvc* pSvc = (CComportAndBaudrateSetSvc*)GetService();
	CComportAndBaudrateSetSvc::Model model = pSvc->GetDataModel();

	// 显示数据
	for(int i = 0; i< 5; i++){
		m_labelGroupThree[i].nameLeft.Format(_T("[%d] %s"),i + 1, model.stMenuDetail[i].strMenuName);
		m_labelGroupThree[i].nameCenter.Format(_T("[1]%s%s"),_opl(TXT_DEVICE_COMPORT_NAME),model.stMenuDetail[i].strComport);
		m_labelGroupThree[i].nameRight.Format(_T("[2]%s%s"),_opl(TXT_DEVICE_BAUDRATE_NAME),model.stMenuDetail[i].strBaudRate);

		// 已选择：红色
		if(model.stMenuDetail[i].emComportStatus == CComportAndBaudrateSetSvc::MENU_STATUS_SELECT){
			m_baseInfoArea->SetLabelColor(i,1,RED);
		}
		// 已更新：绿色
		else if(model.stMenuDetail[i].emComportStatus == CComportAndBaudrateSetSvc::MENU_STATUS_UPDATED){
			m_baseInfoArea->SetLabelColor(i,1,SOFTGREEN);
		}

		// 已选择：红色
		if(model.stMenuDetail[i].emBaudRateStatus == CComportAndBaudrateSetSvc::MENU_STATUS_SELECT){
			m_baseInfoArea->SetLabelColor(i,2,RED);
		}
		// 已更新：绿色
		else if(model.stMenuDetail[i].emBaudRateStatus == CComportAndBaudrateSetSvc::MENU_STATUS_UPDATED){
			m_baseInfoArea->SetLabelColor(i,2,SOFTGREEN);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      清理基本信息区域数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetDlg::ClearLableData(){
	for(int i = 0; i < MAX_LEN ; i++){
		m_labelGroupThree[i].nameLeft = _T("");
		m_labelGroupThree[i].nameCenter = _T("");
		m_labelGroupThree[i].nameRight  = _T("");

		for(int j = 0;j < 3; j++){
			m_baseInfoArea->SetLabelColor(i,j,BLACK);
		}
	}
}