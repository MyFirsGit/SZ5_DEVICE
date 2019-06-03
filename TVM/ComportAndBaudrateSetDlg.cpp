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
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetDlg::CComportAndBaudrateSetDlg(CService* pService) 
	: COperationDlg(CComportAndBaudrateSetDlg::IDD, pService){
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CComportAndBaudrateSetDlg::~CComportAndBaudrateSetDlg(){
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int   0:�����ɹ�    -1:����ʧ��

@exception  ��
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
@brief      ���漤���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(2);
	ClearLableData();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
@brief      ��ʾ��ϸ��Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CComportAndBaudrateSetDlg::ShowComportAndBaudRate(){
	// �������п��
	m_baseInfoArea->SetLableWidth(0,200);
	m_baseInfoArea->SetLableWidth(1,200);
	m_baseInfoArea->SetLableWidth(2,400);

	m_baseInfoArea->SetLableAlign(0,DT_LEFT);
	m_baseInfoArea->SetLableAlign(1,DT_LEFT);
	m_baseInfoArea->SetLableAlign(2,DT_LEFT);

	CComportAndBaudrateSetSvc* pSvc = (CComportAndBaudrateSetSvc*)GetService();
	CComportAndBaudrateSetSvc::Model model = pSvc->GetDataModel();

	// ��ʾ����
	for(int i = 0; i< 5; i++){
		m_labelGroupThree[i].nameLeft.Format(_T("[%d] %s"),i + 1, model.stMenuDetail[i].strMenuName);
		m_labelGroupThree[i].nameCenter.Format(_T("[1]%s%s"),_opl(TXT_DEVICE_COMPORT_NAME),model.stMenuDetail[i].strComport);
		m_labelGroupThree[i].nameRight.Format(_T("[2]%s%s"),_opl(TXT_DEVICE_BAUDRATE_NAME),model.stMenuDetail[i].strBaudRate);

		// ��ѡ�񣺺�ɫ
		if(model.stMenuDetail[i].emComportStatus == CComportAndBaudrateSetSvc::MENU_STATUS_SELECT){
			m_baseInfoArea->SetLabelColor(i,1,RED);
		}
		// �Ѹ��£���ɫ
		else if(model.stMenuDetail[i].emComportStatus == CComportAndBaudrateSetSvc::MENU_STATUS_UPDATED){
			m_baseInfoArea->SetLabelColor(i,1,SOFTGREEN);
		}

		// ��ѡ�񣺺�ɫ
		if(model.stMenuDetail[i].emBaudRateStatus == CComportAndBaudrateSetSvc::MENU_STATUS_SELECT){
			m_baseInfoArea->SetLabelColor(i,2,RED);
		}
		// �Ѹ��£���ɫ
		else if(model.stMenuDetail[i].emBaudRateStatus == CComportAndBaudrateSetSvc::MENU_STATUS_UPDATED){
			m_baseInfoArea->SetLabelColor(i,2,SOFTGREEN);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���������Ϣ��������

@param      ��

@retval     ��

@exception  ��
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