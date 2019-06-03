#include "stdafx.h"
#include "THModeSetMaintainDlg.h"
#include "THModeSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ʼ�������ַ���
static const TCHAR* const STR_NORMAL_MODE		= _T("�Զ�ģʽ");
static const TCHAR* const STR_BACK_MODE			= _T("�ֶ�ģʽ");
static const TCHAR* const TITLE_SELECT_TH_MODE	= _T("��ѡ���Ʊģʽ");
static const TCHAR* const TXT_CURRENT_MODE		= _T("��ǰģʽ��");
static const TCHAR* const TXT_SWITCH_MODE		= _T("ѡ��ģʽ��");
static const TCHAR* const WM_BTN_NOMAL_MODE		= _T("normal");
static const TCHAR* const WM_BTN_BACK_MODE		= _T("back");

IMPLEMENT_DYNAMIC(CTHModeSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CTHModeSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnModeItemClick)
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*     pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetMaintainDlg::CTHModeSetMaintainDlg(CService* pService)
	: COperationDlg(CTHModeSetMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TITLE_TH_MODE_INFO;			// ����
	m_baseInfo->detailInfo.row = 2;									// ����
	m_baseInfo->detailInfo.labelGroup = m_lableTHModeInfo;          // ��ǩ���֣�2�У�
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetMaintainDlg::~CTHModeSetMaintainDlg()
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
int CTHModeSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		// ����������Ϣ���򡢷��ء�ȷ����ť�͹��ܰ�ť����
		if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA | BTN_OK)){
			return -1;
		}

		m_btnOK->SetEnabled(false);

		// ��ʼ��ģʽ��Ϣ
		InitTHModeInfo();

		// ��ʼ����ͨģʽ�������
		BTN_INFO normalMode;
		normalMode.btnMsg = WM_BTN_NOMAL_MODE;
		normalMode.isVisible = true;
		normalMode.text= STR_NORMAL_MODE;
		normalMode.type = BUTTON_TYPE_NORMAL;
		m_vec_Right_1_BTN_INFO.push_back(normalMode);

		// ��ʼ������ģʽ�������
		BTN_INFO backMode;
		backMode.btnMsg = WM_BTN_BACK_MODE;
		backMode.isVisible = true;
		backMode.text = STR_BACK_MODE;
		normalMode.type = BUTTON_TYPE_NORMAL;
		m_vec_Right_1_BTN_INFO.push_back(backMode);

		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ť���º�Ĵ���

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTHModeSetMaintainDlg::OnModeItemClick(WPARAM wParam,LPARAM lParam)
{
	CTHModeSetSvc* pTHModeSetSvc = (CTHModeSetSvc*)GetService();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		// ����ģʽ��ť����
		if (btnInfo->btnMsg == WM_BTN_NOMAL_MODE){
			m_lableTHModeInfo[1].nameRight = STR_NORMAL_MODE;
			m_SelectedMode = 0;
			// �����ǰģʽΪ����ģʽ
			if (0 == m_CurrentMode){
				m_btnOK->SetEnabled(false);
			}
			// �����ǰģʽΪ����ģʽ
			else{
				m_btnOK->SetEnabled(true);
			}
		}
		// ����ģʽ��ť����
		else if (btnInfo->btnMsg == WM_BTN_BACK_MODE){
			m_lableTHModeInfo[1].nameRight = STR_BACK_MODE;
			m_SelectedMode = 1;
			// �����ǰģʽΪ����ģʽ
			if (1 == m_CurrentMode){
				m_btnOK->SetEnabled(false);
			}
			// �����ǰģʽΪ����ģʽ
			else{
				m_btnOK->SetEnabled(true);
			}
		}
		//��ʾ�����ͻ�����
		ShowCompoment(BASE_AREA);
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		theEXCEPTION_MGR.ProcessException(CInnerException(pTHModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTHModeSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CTHModeSetSvc* pTHModeSetSvc = (CTHModeSetSvc*)GetService();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// ���õ�ǰģʽ
		pTHModeSetSvc->SetTHMode(m_SelectedMode);

		// ���»�ȡ��ǰģʽ
		m_CurrentMode = pTHModeSetSvc->GetCurrentTHMode();
		// ��ʾ��ǰģʽ
		if (0 == m_CurrentMode){
			m_lableTHModeInfo[0].nameRight= STR_NORMAL_MODE;
		}
		else{
			m_lableTHModeInfo[0].nameRight= STR_BACK_MODE;
		}
		ShowCompoment(BASE_AREA);
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_MODE_SET_FINISH));
		HideCompoment(BTN_OK);
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK);
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		CInnerException e = CInnerException(pTHModeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideCompoment(BTN_OK);
		m_vec_Right_1_BTN_INFO.clear();
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      InitTHModeInfo

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CTHModeSetMaintainDlg::InitTHModeInfo()
{
	// ��ʼ��ģʽ�л���Ϣ
	m_lableTHModeInfo[0].nameLeft = add_wsp(TXT_CURRENT_MODE,MAX_SBC_CASE_LEN);
	m_lableTHModeInfo[1].nameLeft = add_wsp(TXT_SWITCH_MODE,MAX_SBC_CASE_LEN);
	// ���ݵ�ǰ��THģʽ�����û�����Ϣ
	CTHModeSetSvc* pTHModeSetSvc = (CTHModeSetSvc*)GetService();
	m_CurrentMode = pTHModeSetSvc->GetCurrentTHMode();
	// ���sTHMode����0������Ϊ����ģʽ������1������Ϊ����ģʽ
	if (0 == m_CurrentMode){
		m_lableTHModeInfo[0].nameRight = STR_NORMAL_MODE;
	}
	else{
		m_lableTHModeInfo[0].nameRight = STR_BACK_MODE;
	}
}
