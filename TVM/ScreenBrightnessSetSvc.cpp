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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetSvc::CScreenBrightnessSetSvc()
	:CTVMForeService(SCREEN_BRIGHTNESS_SET)
{
	// ��ʼ���Ի���
	m_pDialogFlow->AddDialog(IDD_01801_TIMESET_BASE_DLG,new CScreenBrightnessSetDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SCREENBRIGHTNESS_SET,_opl(GUIDE_ENTER_F5_F6_ESC_BRIGHTNESS));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SCREENBRIGHTNESS_SET, IDD_01801_TIMESET_BASE_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SCREENBRIGHTNESS_SET);

	m_DataModel.BrightnessNumber = theTVM_SETTING.GetScreenBrightnessValue();
	NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetSvc::~CScreenBrightnessSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

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
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CScreenBrightnessSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CScreenBrightnessSetSvc::OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.BrightnessNumber--,m_DataModel.BrightnessNumber <= 1)
	{
		m_DataModel.BrightnessNumber = 1;
	}
	// 5�ָ�����������
	if(5 == m_DataModel.BrightnessNumber)
	{
		theScreen_HELPER.recoverBrightness();
	}
	// ��Ϊ5
	else
	{
		theScreen_HELPER.setBrightness(m_DataModel.BrightnessNumber);
	}
	
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CScreenBrightnessSetSvc::OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.BrightnessNumber++,m_DataModel.BrightnessNumber >= 10)
	{
		m_DataModel.BrightnessNumber = 10;
	}
	
	// 5�ָ�����������
	if(5 == m_DataModel.BrightnessNumber)
	{
		theScreen_HELPER.recoverBrightness();
	}
	// ��Ϊ5
	else
	{
		theScreen_HELPER.setBrightness(m_DataModel.BrightnessNumber);
	}
	
	NotifyDataModelChanged();
	return TRUE;
}