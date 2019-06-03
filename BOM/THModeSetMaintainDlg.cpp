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

// 初始化常量字符串
static const TCHAR* const STR_NORMAL_MODE		= _T("自动模式");
static const TCHAR* const STR_BACK_MODE			= _T("手动模式");
static const TCHAR* const TITLE_SELECT_TH_MODE	= _T("请选择出票模式");
static const TCHAR* const TXT_CURRENT_MODE		= _T("当前模式：");
static const TCHAR* const TXT_SWITCH_MODE		= _T("选择模式：");
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
@brief      构造函数

@param      (i)CService*     pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetMaintainDlg::CTHModeSetMaintainDlg(CService* pService)
	: COperationDlg(CTHModeSetMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TITLE_TH_MODE_INFO;			// 标题
	m_baseInfo->detailInfo.row = 2;									// 行数
	m_baseInfo->detailInfo.labelGroup = m_lableTHModeInfo;          // 标签文字（2列）
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetMaintainDlg::~CTHModeSetMaintainDlg()
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
int CTHModeSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		// 创建基本信息区域、返回、确定按钮和功能按钮区域
		if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA | BASE_AREA | BTN_OK)){
			return -1;
		}

		m_btnOK->SetEnabled(false);

		// 初始化模式信息
		InitTHModeInfo();

		// 初始化普通模式相关数据
		BTN_INFO normalMode;
		normalMode.btnMsg = WM_BTN_NOMAL_MODE;
		normalMode.isVisible = true;
		normalMode.text= STR_NORMAL_MODE;
		normalMode.type = BUTTON_TYPE_NORMAL;
		m_vec_Right_1_BTN_INFO.push_back(normalMode);

		// 初始化缩退模式相关数据
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
@brief      按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTHModeSetMaintainDlg::OnModeItemClick(WPARAM wParam,LPARAM lParam)
{
	CTHModeSetSvc* pTHModeSetSvc = (CTHModeSetSvc*)GetService();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		// 正常模式按钮按下
		if (btnInfo->btnMsg == WM_BTN_NOMAL_MODE){
			m_lableTHModeInfo[1].nameRight = STR_NORMAL_MODE;
			m_SelectedMode = 0;
			// 如果当前模式为正常模式
			if (0 == m_CurrentMode){
				m_btnOK->SetEnabled(false);
			}
			// 如果当前模式为缩退模式
			else{
				m_btnOK->SetEnabled(true);
			}
		}
		// 缩退模式按钮按下
		else if (btnInfo->btnMsg == WM_BTN_BACK_MODE){
			m_lableTHModeInfo[1].nameRight = STR_BACK_MODE;
			m_SelectedMode = 1;
			// 如果当前模式为缩退模式
			if (1 == m_CurrentMode){
				m_btnOK->SetEnabled(false);
			}
			// 如果当前模式为正常模式
			else{
				m_btnOK->SetEnabled(true);
			}
		}
		//显示基本客户区域
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
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTHModeSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CTHModeSetSvc* pTHModeSetSvc = (CTHModeSetSvc*)GetService();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// 设置当前模式
		pTHModeSetSvc->SetTHMode(m_SelectedMode);

		// 重新获取当前模式
		m_CurrentMode = pTHModeSetSvc->GetCurrentTHMode();
		// 显示当前模式
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

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CTHModeSetMaintainDlg::InitTHModeInfo()
{
	// 初始化模式切换信息
	m_lableTHModeInfo[0].nameLeft = add_wsp(TXT_CURRENT_MODE,MAX_SBC_CASE_LEN);
	m_lableTHModeInfo[1].nameLeft = add_wsp(TXT_SWITCH_MODE,MAX_SBC_CASE_LEN);
	// 根据当前的TH模式，设置基本信息
	CTHModeSetSvc* pTHModeSetSvc = (CTHModeSetSvc*)GetService();
	m_CurrentMode = pTHModeSetSvc->GetCurrentTHMode();
	// 如果sTHMode等于0，设置为正常模式；等于1，设置为缩退模式
	if (0 == m_CurrentMode){
		m_lableTHModeInfo[0].nameRight = STR_NORMAL_MODE;
	}
	else{
		m_lableTHModeInfo[0].nameRight = STR_BACK_MODE;
	}
}
