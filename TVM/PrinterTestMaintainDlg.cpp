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
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestMaintainDlg::CPrinterTestMaintainDlg(CService* pService)
: COperationDlg(CPrinterTestMaintainDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterTestMaintainDlg::~CPrinterTestMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���崴������

@param      (i)LPCREATESTRUCT lpCreateStruct ���崴��������

@retval     int		\n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA)) {
		return -1;
	}

	// ����GStatic
	m_baseInfo->detailInfo.row        = _MAX_ROW;
	m_baseInfo->detailInfo.labelGroup = m_LableGroup;
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

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
@brief      ˢ�½���

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
@brief      �������˵�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestMaintainDlg::InitMenuUI()
{
	//���ò˵�ѡ����
	m_baseInfoArea->SetLableWidth(0,300);
	m_baseInfoArea->SetLableWidth(1,300);

	//Set aliagn
	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ò˵�����

@param      none

@retval     none

@exception  none1
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterTestMaintainDlg::UpdateMenuData()
{
	CPrinterTestSvc* pSvc = (CPrinterTestSvc*)GetService();
	CPrinterTestSvc::Modol& modol = pSvc->GetDataModol();
	//�˵�
	m_LableGroup[0].nameLeft  = _T("[1]") + _opl(PRINT_TEST_TEXT_RESET);
	m_LableGroup[1].nameLeft  = _T("[2]") + _opl(PRINT_TEST_TEXT_PRINT);


	//�˵�����
	for (int nMenu = 0; nMenu < _MAX_ROW; ++nMenu)
	{
		m_LableGroup[nMenu].nameRight = modol.menuInfo[nMenu].strMenuStatus;
		m_baseInfoArea->SetLabelColor(nMenu,_MAX_COL - 1,modol.menuInfo[nMenu].strMenuStatus == FAILED ? RED : SOFTGREEN);
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������ʾ���

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