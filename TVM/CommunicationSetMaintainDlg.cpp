#include "stdafx.h"
#include "CommunicationSetMaintainDlg.h"
#include "CommunicationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#pragma comment ( lib, "iphlpapi.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCommunicationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CCommunicationSetMaintainDlg, COperationDlg)
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
CCommunicationSetMaintainDlg::CCommunicationSetMaintainDlg(CService* pService) 
	: COperationDlg(CCommunicationSetMaintainDlg::IDD, pService)
{ 
	m_baseInfo->detailInfo.labelGroup = m_labelGroup;// ����		
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetMaintainDlg::~CCommunicationSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int   0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CCommunicationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct,  GUIDE_AREA | BASE_AREA) == -1)
	{
		return -1;
	}
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN | KEY_DOT);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);				// �����������λ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,200);		// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,400);		// ���õڶ��п��

	//InitlabelCommunicationInfo();						// ��ʼ��BASE����LABEL���������
	//SetFromReadIniText();								// ��ȡMaintenance.INI�����ļ�����ʾ�ڶ�Ӧ����

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
void CCommunicationSetMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelCommunicationInfo();						// ��ʼ��BASE����LABEL���������
	SetFromReadIniText();								// ��ȡMaintenance.INI�����ļ�����ʾ�ڶ�Ӧ����
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::InitlabelCommunicationInfo()
{
	// ��ʼ����վ��Ϣ��������
	m_labelGroup[0].nameLeft = _T("[1] ")+_opl(COMMUNICATION_TXT_MYIPADDRESS)+_opl(TXT_COLON);
	m_labelGroup[1].nameLeft = _T("[2] ")+_opl(COMMUNICATION_TXT_MYSUBNETMASK)+_opl(TXT_COLON);
	m_labelGroup[2].nameLeft = _T("[3] ")+_opl(COMMUNICATION_TXT_MYGATEWAY)+_opl(TXT_COLON);
	m_labelGroup[3].nameLeft = _T("[4] ")+_opl(COMMUNICATION_TXT_CPSHOSTADDRESS)+_opl(TXT_COLON);
	m_labelGroup[4].nameLeft = _T("[5] ")+_opl(COMMUNICATION_TXT_CPSPORT)+_opl(TXT_COLON);
	m_labelGroup[5].nameLeft = _T("[6] ")+_opl(COMMUNICATION_TXT_NTPHOSTADDRESS)+_opl(TXT_COLON);
	m_labelGroup[6].nameLeft = _T("[7] ")+_opl(COMMUNICATION_TXT_NTPPORT)+_opl(TXT_COLON);
	m_labelGroup[7].nameLeft = _T("[8] ")+_opl(COMMUNICATION_TXT_FTPHOSTADDRESS)+_opl(TXT_COLON);
	m_labelGroup[8].nameLeft = _T("[9] ")+_opl(COMMUNICATION_TXT_FTPPORT)+_opl(TXT_COLON);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡMaintenance.INI�����ļ�����ʾ�ڶ�Ӧ����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::SetFromReadIniText()
{
	CCommunicationSetSvc* pSvc = (CCommunicationSetSvc*)GetService();
	CCommunicationSetSvc::Model& model = pSvc->GetDataModel();
	m_labelGroup[0].nameRight = model.myIpAddress;
	m_labelGroup[1].nameRight = model.mySubnetAddress;
	m_labelGroup[2].nameRight = model.myGatewayAddress;
	m_labelGroup[3].nameRight = model.scHostAddress;
	m_labelGroup[4].nameRight = model.scHostPort;
	m_labelGroup[5].nameRight = model.scNtpAddress;
	m_labelGroup[6].nameRight = model.scNtpPort;
	m_labelGroup[7].nameRight = model.scFtpAddress;
	m_labelGroup[8].nameRight = model.scFtpPort;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::UpdateUI()
{
	CCommunicationSetSvc * pSvc = (CCommunicationSetSvc*)GetService();
	CCommunicationSetSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.IpChange)			//IP����
	{
		ShowIPContent();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����ʾ��Ӧ�����õ�����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::ShowIPContent()
{
	CCommunicationSetSvc* pSvc = (CCommunicationSetSvc*)GetService();
	CCommunicationSetSvc::Model& model = pSvc->GetDataModel();
	if(FALSE == model.IpLegal)		//IP���ݷǷ�
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	switch(model.MenuNumber)
	{
	case MENU_CODE_1:				//������ַ
		m_labelGroup[0].nameRight = model.myIpAddress;
		break;
	case MENU_CODE_2:				//��������
		m_labelGroup[1].nameRight = model.mySubnetAddress;
		break;
	case MENU_CODE_3:				//����
		m_labelGroup[2].nameRight = model.myGatewayAddress;
		break;
	case MENU_CODE_4:				//������ַ
		m_labelGroup[3].nameRight = model.scHostAddress;
		break;
	case MENU_CODE_5:				//�����˿�
		m_labelGroup[4].nameRight = model.scHostPort;
		break;
	case MENU_CODE_6:				//NTP��ַ
		m_labelGroup[5].nameRight = model.scNtpAddress;
		break;
	case MENU_CODE_7:				//NTP�˿�
		m_labelGroup[6].nameRight = model.scNtpPort;
		break;
	case MENU_CODE_8:				//FTP��ַ
		m_labelGroup[7].nameRight = model.scFtpAddress;
		break;
	case MENU_CODE_9:				//FTP�˿�
		return;
	default:
		break;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}
