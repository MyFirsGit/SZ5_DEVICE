#include "stdafx.h"
#include "SignCardReportLossOverOfficeDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSignCardReportLossOverOfficeDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CSignCardReportLossOverOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     void 

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardReportLossOverOfficeDlg::CSignCardReportLossOverOfficeDlg(CService* pService) :
COperationDlg(CSignCardReportLossOverOfficeDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSignCardReportLossOverOfficeDlg::~CSignCardReportLossOverOfficeDlg()
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
int CSignCardReportLossOverOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 创建基本信息区域，返回键，确认键
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA |  BTN_RESET | BTN_OK)){
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardReportLossOverOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CSignCardReportLossOverOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}


