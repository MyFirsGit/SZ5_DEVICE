#include "stdafx.h"
#include "TVMFailureRefundSvc.h"
#include "TVMFailureRefundReceptionDlg.h"

// ����ڴ�й©
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
@brief      ���캯��

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
@brief      ��������

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
			0:�����ɹ�    -1:����ʧ��

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