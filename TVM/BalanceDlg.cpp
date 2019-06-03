#include "StdAfx.h"
#include "BalanceDlg.h"


IMPLEMENT_DYNAMIC(CBalanceDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CBalanceDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CBalanceDlg::CBalanceDlg(CService* pService)
	:COperationDlg(CBalanceDlg::IDD, pService)
{
	m_pSvc = (CBalanceSvc *)pService;
	m_baseInfo->detailInfo.row = MAX_ROW;
	m_baseInfo->detailInfo.labelGroup = m_lableGroup;
}


CBalanceDlg::~CBalanceDlg(void)
{
	m_pSvc = NULL;
}


int CBalanceDlg::OnCreate(LPCREATESTRUCT lpCreateStruct )
{
	if (__super::OnCreate(lpCreateStruct,BASE_AREA | GUIDE_AREA) != 0) {
		return -1;
	}
	//m_baseInfoArea = new CGStatic(this, m_baseInfo, m_lableGroup);
	return 0;

}

void CBalanceDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	Initialize();
	theAPP_SESSION.SetKeyboard(KEY_RETURN | KEY_F2);
	__super::OnActivate(nState, pWndOther, bMinimized);
	
}

void CBalanceDlg::UpdateUI()
{	
	Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      界面初始化

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBalanceDlg::Initialize()
{
	for ( int i(0); i < MAX_ROW; i++)
	{
		m_lableGroup[i].nameLeft = _T("");
		m_lableGroup[i].nameRight = _T("");
	}
	//theAPP_SESSION.ShowOfficeGuide(TXT_BALANCE_IN_PROCESS);
	m_baseInfoArea->Invalidate();
}