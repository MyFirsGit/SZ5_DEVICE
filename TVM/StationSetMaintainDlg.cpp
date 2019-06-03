#include "stdafx.h"
#include "StationSetMaintainDlg.h"
#include "StationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CStationSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::CStationSetMaintainDlg(CService* pService) 
	: COperationDlg(CStationSetMaintainDlg::IDD, pService)
{
	m_baseInfo->detailInfo.labelGroup = m_lableGroup;               // ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::~CStationSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval      int 0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CStationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct,  BASE_AREA | GUIDE_AREA) == -1){
		return -1;
	}

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������
	theAPP_SESSION.SetMaxInputLength(1);							// �����������λ��

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 200);				// ���õ�һ�п��
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 400);				// ���õڶ��п��

	//InitlabelStationInfo();											// ��ʼ�����������label��Ϣ
	//SetFromReadIniText();											// ��ȡMaintenance.INI�����ļ�����ʾ�ڶ�Ӧ����

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
void CStationSetMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelStationInfo();											// ��ʼ�����������label��Ϣ
	SetFromReadIniText();											// ��ȡMaintenance.INI�����ļ�����ʾ�ڶ�Ӧ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::InitlabelStationInfo()
{
	// ��ʼ����վ��Ϣ��������
	m_lableGroup[0].nameLeft = _T("[1] ")+_opl(STATIONSET_LINE_CODE)+_opl(TXT_COLON);
	m_lableGroup[1].nameLeft = _T("[2] ")+ _opl(STATIONSET_STATION_CODE)+_opl(TXT_COLON);
	m_lableGroup[2].nameLeft = _T("[3] ")+ _opl(STATIONSET_MACHINE_CODE)+_opl(TXT_COLON);
	m_lableGroup[3].nameLeft = _T("[4] ")+_opl(STATIONSET_GROUP_NUMBER)+_opl(TXT_COLON);
	m_lableGroup[4].nameLeft = _T("[5] ")+_opl(STATIONSET_EQUNOINGROUP)+_opl(TXT_COLON);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::UpdateUI()
{
	CStationSetSvc* pSvc = (CStationSetSvc*)GetService();
	CStationSetSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.CodeChange)										// �������
	{
		ShowCodeContent();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��Maintenance.INI��ȡ�������ļ��ڶ�Ӧ����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::SetFromReadIniText()
{
	CStationSetSvc* pSvc = (CStationSetSvc*)GetService();
	CStationSetSvc::Model& model = pSvc->GetDataModel();

	// ����վ���룬�������룬��ţ����ڱ�����õ�����
	m_lableGroup[0].nameRight = model.sLineCode;
	m_lableGroup[1].nameRight = model.sStationCode;
	m_lableGroup[2].nameRight = model.sMachineCode;
	m_lableGroup[3].nameRight = model.sGroupNumber;
	m_lableGroup[4].nameRight = model.sEquNumberInGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ����ʾ��Ӧ�����õ�����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::ShowCodeContent()
{
	CStationSetSvc* pSvc = (CStationSetSvc*)GetService();
	CStationSetSvc::Model& model = pSvc->GetDataModel();	
	if(FALSE == model.CodeLegal )									// �������ݷǷ�
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	switch (model.MenuNumber)
	{
	case MENU_CODE_1:		// ��·����
		m_lableGroup[0].nameRight = model.sLineCode;
		break;
	case MENU_CODE_2:		// ��վ����
		m_lableGroup[1].nameRight = model.sStationCode;
		break;
	case MENU_CODE_3:		// ��������
		m_lableGroup[2].nameRight = model.sMachineCode;
		break;
	case MENU_CODE_4:		// ����
		m_lableGroup[3].nameRight = model.sGroupNumber;
		break;
	case MENU_CODE_5:		// ���ڱ��
		m_lableGroup[4].nameRight = model.sEquNumberInGroup;
		break;
	default:
		break;
	}

	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}