#include "stdafx.h"
#include "ScreenBrightnessSetSvc.h"
#include "GuideInfo.h"
#include "ScreenHelper.h"
#include "tvmsetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CScreenBrightnessSetSvc, CTVMForeService)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F5,OnKeyboardF5)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F6,OnKeyboardF6)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetSvc::CScreenBrightnessSetSvc()
	:CTVMForeService(SCREEN_BRIGHTNESS_SET)
{
	// 初始化对话框
	m_pDialogFlow->AddDialog(IDD_01801_TIMESET_BASE_DLG,new CScreenBrightnessSetDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SCREENBRIGHTNESS_SET,_opl(GUIDE_ENTER_F5_F6_ESC_BRIGHTNESS));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SCREENBRIGHTNESS_SET, IDD_01801_TIMESET_BASE_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SCREENBRIGHTNESS_SET);

	m_DataModel.BrightnessNumber = theTVM_SETTING.GetScreenBrightnessValue();
	NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetSvc::~CScreenBrightnessSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     NumberDate

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetSvc::Model& CScreenBrightnessSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     通知数据模型更新

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CScreenBrightnessSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CScreenBrightnessSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF2(0,0);
	theTVM_SETTING.SetScreenBrightnessValue(m_DataModel.BrightnessNumber);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CScreenBrightnessSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CScreenBrightnessSetSvc::OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.BrightnessNumber--,m_DataModel.BrightnessNumber <= 1)
	{
		m_DataModel.BrightnessNumber = 1;
	}
	// 5恢复到正常亮度
	if(5 == m_DataModel.BrightnessNumber)
	{
		theScreen_HELPER.recoverBrightness();
	}
	// 不为5
	else
	{
		theScreen_HELPER.setBrightness(m_DataModel.BrightnessNumber);
	}
	
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CScreenBrightnessSetSvc::OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.BrightnessNumber++,m_DataModel.BrightnessNumber >= 10)
	{
		m_DataModel.BrightnessNumber = 10;
	}
	
	// 5恢复到正常亮度
	if(5 == m_DataModel.BrightnessNumber)
	{
		theScreen_HELPER.recoverBrightness();
	}
	// 不为5
	else
	{
		theScreen_HELPER.setBrightness(m_DataModel.BrightnessNumber);
	}
	
	NotifyDataModelChanged();
	return TRUE;
}