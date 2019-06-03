#include "stdafx.h"
#include "CoinAddingBoxSetMaintainDlg.h"
#include "CoinAddingBoxSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinAddingBoxMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CCoinAddingBoxMaintainDlg,COperationDlg)
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
CCoinAddingBoxMaintainDlg :: CCoinAddingBoxMaintainDlg(CService* pService) 
	: COperationDlg(CCoinAddingBoxMaintainDlg::IDD, pService)
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
CCoinAddingBoxMaintainDlg::~CCoinAddingBoxMaintainDlg()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CCoinAddingBoxMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	// ����GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_labelGroup);

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 150);//���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 100);//���õڶ��п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_3, 350);//���õ����п��
	
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);

	//InitlabelAddingBoxInfo();//��ʼ�����������label��Ϣ
	//SetFromReadIniText();//��ȡTVM.INI�����ļ�����ʾ�ڶ�Ӧ����

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���漤��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelAddingBoxInfo();//��ʼ�����������label��Ϣ
	SetFromReadIniText();//��ȡTVM.INI�����ļ�����ʾ�ڶ�Ӧ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::InitlabelAddingBoxInfo()
{
	// ��ʼ����Ϣ��������
	m_labelGroup[0].nameLeft = _T("[1]")+_opl(GUIDE_ADDBOX_SET_A)+_opl(TXT_COLON) ;	//������A
	m_labelGroup[0].nameRight = _opl(GUIDE_ADDBOX_SET_MENU);
		
	m_labelGroup[1].nameLeft = _T("[2]")+_opl(GUIDE_ADDBOX_SET_B)+_opl(TXT_COLON);	//������B
	m_labelGroup[1].nameRight = _opl(GUIDE_ADDBOX_SET_MENU);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��TVM.INI��ȡ�������ļ��ڶ�Ӧ����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::SetFromReadIniText()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();
	m_labelGroup[0].nameCenter = model.hoponecurrency;
	m_labelGroup[1].nameCenter = model.hoptwocurrency;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::UpdateUI()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();

	if(TRUE == model.CodeChange)//�������
	{
		ShowCodeContent();
	}
	if(TRUE == model.MenuChange)//�˵�����
	{
		ShowGuideDisplay();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ʾ��ʾ����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::ShowGuideDisplay()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();
	switch(model.MenuNumber)
	{
	case MENU_CODE_1:		//������A
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ADDBOX_SET_A)+_opl(TXT_COLON) );
		break;
	case MENU_CODE_2:		//������B
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ADDBOX_SET_B)+_opl(TXT_COLON));
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����ʾ��Ӧ�����õ�����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CCoinAddingBoxMaintainDlg::ShowCodeContent()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();	
	if(FALSE == model.CodeContent )		//�������ݷǷ�
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	switch (model.MenuNumber)
	{
	case MENU_CODE_1:		//������A
		m_labelGroup[0].nameCenter = model.hoponecurrency;
		break;
	case MENU_CODE_2:		//������B
		m_labelGroup[1].nameCenter = model.hoptwocurrency;
		break;
	default:
		break;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}