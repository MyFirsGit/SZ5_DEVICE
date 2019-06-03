#include "stdafx.h"
#include "StationSetSvc.h"
#include "StationSetMaintainDlg.h"
#include "GuideInfo.h"

BEGIN_MESSAGE_MAP(CStationSetSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStationSetSvc::CStationSetSvc()
:CBOMForeService(STATION_SET_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CStationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_STATION_SET, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_STATION_SET, IDD_20112_STATION_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STATION_SET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStationSetSvc::~CStationSetSvc()
{
}