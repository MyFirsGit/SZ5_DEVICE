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

// 初始化常量字符串
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
@brief      构造函数

@param      (i)CService*     pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchDlg::CDeviceModeSwitchDlg(CService* pService)
: COperationDlg(CDeviceModeSwitchDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = _tl(TITLE_MODE_SWITCH_INFO);              // 标题
	m_baseInfo->detailInfo.row = 2;                                        // 行数
	m_baseInfo->detailInfo.labelGroup = m_modeSwitchInfo;                  // 标签文字（2列）
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceModeSwitchDlg::~CDeviceModeSwitchDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CDeviceModeSwitchDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//try{
	//	// 创建基本信息区域、返回、确定按钮和功能按钮区域
	//	if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA | BTN_OK)){
	//		return -1;
	//	}

	//	m_btnOK->SetEnabled(false);

	//	// 初始化模式信息
	//	InitModeInfo();

	//	// 初始化普通模式按钮
	//	BTN_INFO normalMode;
	//	normalMode.btnMsg = WM_BTN_NOMAL_MODE;
	//	normalMode.text= STR_NORMAL_MODE;
	//	m_vec_Right_1_BTN_INFO.push_back(normalMode);
	//	// 初始化测试模式按钮
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
@brief      按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDeviceModeSwitchDlg::OnModeItemClick(WPARAM wParam,LPARAM lParam)
{
	//try{
	//	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	//	// 正常模式按钮按下
	//	if (btnInfo->btnMsg == WM_BTN_NOMAL_MODE){
	//		m_modeSwitchInfo[1].nameRight = STR_NORMAL_MODE;
	//		m_SelectedMode = 0;
	//		// 如果当前模式为正常模式
	//		if (0 == m_CurrentMode){
	//			m_btnOK->SetEnabled(false);
	//		}
	//		// 如果当前模式为测试模式
	//		else{				
	//			m_btnOK->SetEnabled(true);
	//		}
	//	}
	//	// 测试模式按钮按下
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
	//	//显示基本客户区域
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
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDeviceModeSwitchDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//try{
	//	theSERVICE_MGR.SetForeServiceBusy(true);
	//	// 设置当前模式
	//	theSETTING.SetDeviceTestMode(m_SelectedMode);
	//	// 重新获取当前模式
	//	m_CurrentMode = theSETTING.GetDeviceTestMode();
	//	// 显示当前模式
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

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CDeviceModeSwitchDlg::InitModeInfo()
{
	// 初始化模式切换信息
	m_modeSwitchInfo[0].nameLeft = add_hsp(TXT_CURRENT_MODE,MAX_HALF_CASE_LEFT_LEN);
	m_modeSwitchInfo[1].nameLeft = add_hsp(TXT_SWITCH_MODE,MAX_HALF_CASE_LEFT_LEN);

	// 根据当前的设备模式，设置基本信息
	m_CurrentMode = theMAINTENANCE_INFO.GetDeviceTestMode();
	// 如果deviceMode等于0，设置为普通模式；否则，设置为测试模式
	if (0 == m_CurrentMode){
		m_modeSwitchInfo[0].nameRight = STR_NORMAL_MODE;
	}
	else{
		m_modeSwitchInfo[0].nameRight = STR_TEST_MODE;
	}
}