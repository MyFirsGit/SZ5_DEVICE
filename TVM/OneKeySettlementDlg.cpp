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
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COneKeySettlementDlg::COneKeySettlementDlg(CService* pService)
	: COperationDlg(COneKeySettlementDlg::IDD,pService)
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
COneKeySettlementDlg::~COneKeySettlementDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int COneKeySettlementDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA))
	{
		return -1;
	}

	//����Gstatic
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LableGroupFour);

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
void COneKeySettlementDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);

	COneKeySettlementSvc* pSvc = (COneKeySettlementSvc*)GetService();

	if (pSvc->GetCurDialogGroupID() == COneKeySettlementSvc::DIALOG_GROUP_REMOVE_EXCEPTION)
	{
		//���ü��̿���
		theAPP_SESSION.SetKeyboard(KEY_F2 | KEY_INPUT |KEY_ENTER|KEY_PAGE|KEY_RETURN);
		InitDialogUI();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ˢ�½�������

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
@brief      ��ʼ������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementDlg::InitDialogUI()
{
	m_baseInfoArea->SetLableWidth(0,280);	//�ջ���
	m_baseInfoArea->SetLableWidth(1,100);	//����
	m_baseInfoArea->SetLableWidth(2,100);	//�ر�
	m_baseInfoArea->SetLableWidth(3,100);	//������
	m_baseInfoArea->SetLableWidth(4,150);	// ״̬

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);
	m_baseInfoArea->SetLableAlign(3,DT_RIGHT);
	m_baseInfoArea->SetLableAlign(4,DT_CENTER);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ǿ�Ƹ��½���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementDlg::UpdateLabelData(){
	UpdateUI();
	if(NULL != m_baseInfoArea){
		m_baseInfoArea->UpdateWindow();
	}
}
