#include "stdafx.h"
#include "DeviceModeSwitchDlg.h"
#include "DeviceModeSwitchSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ʼ�������ַ���
#define STR_NORMAL_MODE    _opl(LANGUAGE_NORMAL_MODE_BT)                       
#define STR_TEST_MODE      _opl(LANGUAGE_TEST_MODE_BT)                         
#define TXT_CURRENT_MODE   _opl(LANGUAGE_TXT_CURRENT_MODE)+_opl(TXT_COLON)                     
#define TXT_SWITCH_MODE    _opl(LANGUAGE_TXT_SWITCH_MODE)+_opl(TXT_COLON)                      

static const TCHAR* const WM_BTN_NOMAL_MODE = _T("normal");
static const TCHAR* const WM_BTN_TEST_MODE  = _T("test");

IMPLEMENT_DYNAMIC(CDeviceModeSwitchDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CDeviceModeSwitchDlg, COperationDlg)
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
CDeviceModeSwitchDlg::CDeviceModeSwitchDlg(CService* pService)
: COperationDlg(CDeviceModeSwitchDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = _tl(TITLE_MODE_SWITCH_INFO);              // ����
	m_baseInfo->detailInfo.row = 2;                                        // ����
	m_baseInfo->detailInfo.labelGroup = m_modeSwitchInfo;                  // ��ǩ���֣�2�У�
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchDlg::~CDeviceModeSwitchDlg()
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
int CDeviceModeSwitchDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//try{
	//	// ����������Ϣ���򡢷��ء�ȷ����ť�͹��ܰ�ť����
	//	if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA | BTN_OK)){
	//		return -1;
	//	}

	//	m_btnOK->SetEnabled(false);

	//	// ��ʼ��ģʽ��Ϣ
	//	InitModeInfo();

	//	// ��ʼ����ͨģʽ��ť
	//	BTN_INFO normalMode;
	//	normalMode.btnMsg = WM_BTN_NOMAL_MODE;
	//	normalMode.text= STR_NORMAL_MODE;
	//	m_vec_Right_1_BTN_INFO.push_back(normalMode);
	//	// ��ʼ������ģʽ��ť
	//	BTN_INFO testMode;
	//	testMode.btnMsg = WM_BTN_TEST_MODE;
	//	testMode.text = STR_TEST_MODE;
	//	m_vec_Right_1_BTN_INFO.push_back(testMode);
	//	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	//	m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
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
LRESULT CDeviceModeSwitchDlg::OnModeItemClick(WPARAM wParam,LPARAM lParam)
{
	//try{
	//	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	//	// ����ģʽ��ť����
	//	if (btnInfo->btnMsg == WM_BTN_NOMAL_MODE){
	//		m_modeSwitchInfo[1].nameRight = STR_NORMAL_MODE;
	//		m_SelectedMode = 0;
	//		// �����ǰģʽΪ����ģʽ
	//		if (0 == m_CurrentMode){
	//			m_btnOK->SetEnabled(false);
	//		}
	//		// �����ǰģʽΪ����ģʽ
	//		else{				
	//			m_btnOK->SetEnabled(true);
	//		}
	//	}
	//	// ����ģʽ��ť����
	//	else if (btnInfo->btnMsg == WM_BTN_TEST_MODE){
	//		m_modeSwitchInfo[1].nameRight = STR_TEST_MODE;
	//		m_SelectedMode = 1;
	//		if (0 == m_CurrentMode){          
	//			m_btnOK->SetEnabled(true);
	//		}
	//		else{
	//			m_btnOK->SetEnabled(false);
	//		}
	//	}
	//	//��ʾ�����ͻ�����
	//	ShowCompoment(BASE_AREA);
	//}
	//catch (CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...){
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
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
LRESULT CDeviceModeSwitchDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//try{
	//	theSERVICE_MGR.SetForeServiceBusy(true);
	//	// ���õ�ǰģʽ
	//	theSETTING.SetDeviceTestMode(m_SelectedMode);
	//	// ���»�ȡ��ǰģʽ
	//	m_CurrentMode = theSETTING.GetDeviceTestMode();
	//	// ��ʾ��ǰģʽ
	//	if (0 == m_CurrentMode){
	//		m_modeSwitchInfo[0].nameRight= STR_NORMAL_MODE;
	//	}
	//	else{
	//		m_modeSwitchInfo[0].nameRight= STR_TEST_MODE;
	//	}
	//	ShowCompoment(BASE_AREA);
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_MODE_SWITCH_FINISH));
	//	HideCompoment(BTN_OK);
	//	m_vec_Right_1_BTN_INFO.clear();
	//	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	//	theSERVICE_MGR.SetForeServiceBusy(false);
	//}
	//catch (CSysException& e){
	//	theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
	//	HideCompoment(BTN_OK);
	//	m_vec_Right_1_BTN_INFO.clear();
	//	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	//	theSERVICE_MGR.SetForeServiceBusy(false);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	CInnerException e = CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	//	theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
	//	HideCompoment(BTN_OK);
	//	m_vec_Right_1_BTN_INFO.clear();
	//	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
	//	theSERVICE_MGR.SetForeServiceBusy(false);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      InitModeInfo

@param      void

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

void CDeviceModeSwitchDlg::InitModeInfo()
{
	// ��ʼ��ģʽ�л���Ϣ
	m_modeSwitchInfo[0].nameLeft = add_hsp(TXT_CURRENT_MODE,MAX_HALF_CASE_LEFT_LEN);
	m_modeSwitchInfo[1].nameLeft = add_hsp(TXT_SWITCH_MODE,MAX_HALF_CASE_LEFT_LEN);

	// ���ݵ�ǰ���豸ģʽ�����û�����Ϣ
	m_CurrentMode = theMAINTENANCE_INFO.GetDeviceTestMode();
	// ���deviceMode����0������Ϊ��ͨģʽ����������Ϊ����ģʽ
	if (0 == m_CurrentMode){
		m_modeSwitchInfo[0].nameRight = STR_NORMAL_MODE;
	}
	else{
		m_modeSwitchInfo[0].nameRight = STR_TEST_MODE;
	}
}