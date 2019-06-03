#include "stdafx.h"
#include "TVMFailureRefundSvc.h"
#include "TVMFailureRefundReceptionDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTVMFailureRefundReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CTVMFailureRefundReceptionDlg,CReceptionDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     void	

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTVMFailureRefundReceptionDlg::CTVMFailureRefundReceptionDlg(CService* pService):
	CReceptionDlg(CTVMFailureRefundReceptionDlg::IDD, pService)
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
CTVMFailureRefundReceptionDlg::~CTVMFailureRefundReceptionDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
			0:创建成功    -1:创建失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTVMFailureRefundReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct/*, OPER_AREA*/)){
		return -1;
	}
	return 0;
}