#include "stdafx.h"
#include "StationSetSvc.h"
#include "StationSetMaintainDlg.h"
#include "GuideInfo.h"
#include "AudioHelper.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CStationSetSvc, CTVMForeService)

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
	:CTVMForeService(STATION_SET_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CStationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_STATION_SET, _opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_STATION_SET, IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STATION_SET);// 设置初始画面组
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键

	EnterTime = 1;
	GuideMsg = _T("");
	
	ReadIniText();

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STATION_SET);	// 设置初始画面组
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_STATION_SET);	
	// 如果在线则提示不允许设置
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CStationSetSvc::Model& CStationSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStationSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardEnter(0,0);
		if(1 == EnterTime)			// 第一次按下Enter键确认菜单的选择
		{
			CheckMenuNumber();		// 菜单号是否非法判断
		}
		else if(2 == EnterTime )	// 第二次按下Enter键确认代码设置的选择
		{
			DoCheckInput();			// 对输入数据是否合理做出相应项的实际判断
			theAPP_SESSION.SetMaxInputLength(1);
		}
		return TRUE;
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      菜单号是否非法判断

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::CheckMenuNumber()
{
	// 验证输入的菜单编号
	m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
	if (m_DataModel.MenuNumber < 1 || m_DataModel.MenuNumber > MAX_SMMENU_NUM)		// 主菜单编号除1-5之外的为非法菜单号
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	else
	{
		InputGuideMsg();
		theAPP_SESSION.DeleteAllInput();
		theAPP_SESSION.SetPrefixToGuide(GuideMsg);
		EnterTime = 2;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     读取Maintenance.INI配置文件

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::ReadIniText()
{
	// 从维护信息中取得车站代码，本机代码，组号，组内编号
	m_DataModel.sLineCode.Format(_T("%d"),(theMAINTENANCE_INFO.GetStationCodeWithLine()/100));
	m_DataModel.sStationCode.Format(_T("%d"),(theMAINTENANCE_INFO.GetStationCodeWithLine()%100));
	m_DataModel.sMachineCode.Format(_T("%d"),theMAINTENANCE_INFO.GetMachineCode());
	m_DataModel.sGroupNumber.Format(_T("%d"),theMAINTENANCE_INFO.GetGroupNumber());
	m_DataModel.sEquNumberInGroup.Format(_T("%d"),theMAINTENANCE_INFO.GetEquNumberInGroup());

	n_NubIndex.LineIndex = theMAINTENANCE_INFO.GetStationCodeWithLine()/100;
	n_NubIndex.StationIndex = theMAINTENANCE_INFO.GetStationCodeWithLine()%100;
	n_NubIndex.MachineIndex = theMAINTENANCE_INFO.GetMachineCode();
	n_NubIndex.GroupIndex = theMAINTENANCE_INFO.GetGroupNumber();
	n_NubIndex.EquNumberIndex = theMAINTENANCE_INFO.GetEquNumberInGroup();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     对输入数据是否合理做出相应项的实际判断

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::DoCheckInput()
{
	CString GuideContent = theAPP_SESSION.GetInputToString();	// 向导栏输入内容为空不作处理
	if(_T("") == GuideContent)
	{
		return;
	}
	int index = theAPP_SESSION.GetInputToInt();					// 获取向导栏输入内容
	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:			// 线路代码
		if(index<0 || index>99) // 0-255之外非法
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.LineIndex = index;
			m_DataModel.sLineCode.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_2:		// 车站代码		
		if(index<0 || index>99)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.StationIndex = index;
			m_DataModel.sStationCode.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_3:		// 本机代码
		if(index<1 || index>254)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.MachineIndex = index;
			m_DataModel.sMachineCode.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_4:		// 组编号
		if(index<0 || index>255)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.GroupIndex = index;
			m_DataModel.sGroupNumber.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	case MENU_CODE_5:		// 组内编号
		if(index<0 || index>255)
		{
			m_DataModel.CodeLegal = FALSE;
		}
		else
		{
			n_NubIndex.EquNumberIndex = index;
			m_DataModel.sEquNumberInGroup.Format(_T("%d"),index);
			m_DataModel.CodeLegal = TRUE;
		}
		break;
	default:
		break;

	}
	m_DataModel.CodeChange = TRUE;	// 代码发生变化标志
	NotifyDataModelChanged();

	EnterTime = 1;					// 置按键次数为1，重新从选择菜单编号开始

}

//////////////////////////////////////////////////////////////////////////
/**
@brief     通知数据模型更新

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief     对GuideMsg分别赋值

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetSvc::InputGuideMsg()
{
	switch(m_DataModel.MenuNumber)	// 依据菜单号选择相应的提示
	{
	case MENU_CODE_1:				// 线路代码
		GuideMsg = _opl(STATIONSET_LINE_CODE)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(2);
		break;
	case MENU_CODE_2:				// 车站代码
		GuideMsg =  _opl(STATIONSET_STATION_CODE)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(2);
		break;
	case MENU_CODE_3:				// 本机代码
		GuideMsg =  _opl(STATIONSET_MACHINE_CODE)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(3);
		break;
	case MENU_CODE_4:				// 组编号
		GuideMsg = _opl(STATIONSET_GROUP_NUMBER)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(3);
		break;
	case MENU_CODE_5:				// 组内编号
		GuideMsg = _opl(STATIONSET_EQUNOINGROUP)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(3);
		break;
	default:
		break;
	}
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
LRESULT CStationSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 如果在线则提示不允许设置
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardF2(0,0);
		theMAINTENANCE_INFO.SetStationCode(n_NubIndex.LineIndex*100+n_NubIndex.StationIndex);
		theMAINTENANCE_INFO.SetMachineCode(n_NubIndex.MachineIndex);
		theMAINTENANCE_INFO.SetGroupNumber(n_NubIndex.GroupIndex);
		theMAINTENANCE_INFO.SetEquNumberInGroup(n_NubIndex.EquNumberIndex);
		theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));

		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEVICE_SETTINGS,true);
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Clear] 键按下消息（删除）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStationSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;		// 置按键次数为1，重新从选择菜单编号开始
		theAPP_SESSION.SetMaxInputLength(1);
	}
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
LRESULT CStationSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}