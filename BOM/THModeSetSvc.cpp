#include "stdafx.h"
#include "THModeSetSvc.h"
#include "THModeSetMaintainDlg.h"
#include "GuideInfo.h"
#include "ExceptionMgr.h"
#include "THMonitorSvc.h"

BEGIN_MESSAGE_MAP(CTHModeSetSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetSvc::CTHModeSetSvc() : CBOMForeService(TH_MODE_SET_SVC)
{
	//��ӶԻ������
	m_pDialogFlow->AddDialog(IDD_20120_TH_MODE_SET_DLG,new CTHModeSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TH_MODE_SET,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_MODE_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TH_MODE_SET,IDD_20120_TH_MODE_SET_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_MODE_SET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetSvc::~CTHModeSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰTHģʽ

@param      ��

@retval     int		0 - ����ģʽ��1 - ����ģʽ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTHModeSetSvc::GetCurrentTHMode()
{
	return theSETTING.GetTHMode();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����THģʽ

@param      (i)int    sThMode  THģʽ

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTHModeSetSvc::SetTHMode(int sThMode)
{
	try{
		if(sThMode == TH_MODE_NORMAL){
			theEXCEPTION_MGR.RemoveException(CTHHardwareException::MODULE_ID);
			//theEXCEPTION_MGR.RemoveExceptionExcept(CTHException::MODULE_ID,CTHException::INVALID_EXCHANGE);
			m_pTHHelper->Repare();
			//m_pTHHelper->CheckDeviceExchange();
			if(theBOM_INFO.IsTHMonitorOpened()){
				theSERVICE_MGR.GetService<CTHMonitorSvc>(MONITOR_TH_SVC)->Resume();
			}
		}
		theSETTING.SetTHMode(sThMode);
	}
	catch(CSysException& e)
	{
		theSETTING.SetTHMode(sThMode);
		throw e;
	}
	
}