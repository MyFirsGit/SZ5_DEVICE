#include "stdafx.h"
#include "DataOutputSelectMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "DataOutputSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDataOutputSelectMaintainDlg,COperationDlg)
BEGIN_MESSAGE_MAP(CDataOutputSelectMaintainDlg,COperationDlg)
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
CDataOutputSelectMaintainDlg::CDataOutputSelectMaintainDlg(CService* pService) :
COperationDlg(CDataOutputSelectMaintainDlg::IDD, pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDataOutputSelectMaintainDlg::~CDataOutputSelectMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int  CDataOutputSelectMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA))
	{
		return -1;
	}

	m_baseInfo->detailInfo.row = _MAX_ROW;
	m_baseInfo->detailInfo.labelGroup = m_LabelGroup;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void  CDataOutputSelectMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	//���ð����������ͣ���������볤��
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);
	theAPP_SESSION.SetMaxInputLength(1);
	//��ʼ������
	InitOutputUI();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���½�����ʾ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSelectMaintainDlg::UpdateUI()
{
	UpdateOutputData();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSelectMaintainDlg::InitOutputUI()
{
	m_baseInfoArea->SetLableWidth(0,RESOLUTION * 600);
	m_baseInfoArea->SetLableWidth(1,0);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���½�������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDataOutputSelectMaintainDlg::UpdateOutputData()
{
	m_LabelGroup[0].nameLeft  = _T("[1]");
	m_LabelGroup[0].nameLeft +=	_opl(DATAOUTPUT_DATATYPE_BUSINESS_BT);
	m_LabelGroup[0].nameRight = _T("");

	m_LabelGroup[1].nameLeft  = _T("[2]") ;
	m_LabelGroup[1].nameLeft += _opl(DATAOUTPUT_DATATYPE_TRANSACTION_BT);
	m_LabelGroup[1].nameRight = _T("");

	m_LabelGroup[2].nameLeft  = _T("[3]") ;
	m_LabelGroup[2].nameLeft += _opl(DATAOUTPUT_DATATYPE_LOG_BT);
	m_LabelGroup[2].nameRight = _T("");

}