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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetSvc::CTHModeSetSvc() : CBOMForeService(TH_MODE_SET_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(IDD_20120_TH_MODE_SET_DLG,new CTHModeSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TH_MODE_SET,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_MODE_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TH_MODE_SET,IDD_20120_TH_MODE_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_MODE_SET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHModeSetSvc::~CTHModeSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前TH模式

@param      无

@retval     int		0 - 正常模式，1 - 缩退模式

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTHModeSetSvc::GetCurrentTHMode()
{
	return theSETTING.GetTHMode();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置TH模式

@param      (i)int    sThMode  TH模式

@retval     void

@exception  无
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