#include "stdafx.h"
#include "TimeSetSvc.h"
#include "TimeSetBaseOfficeDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTimeSetSvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::CTimeSetSvc()
	:CTVMForeService(SYSTEM_TIME_SET_SVC)
{
	// 初始化对话框
	m_pDialogFlow->AddDialog(IDD_01801_TIMESET_BASE_DLG,new CTimeSetBaseOfficeDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TIMESET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TIMESET, IDD_01801_TIMESET_BASE_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TIMESET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::~CTimeSetSvc()
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
void CTimeSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键

	EnterTime = 1;				// 数字键、Enter键按下次数
	TimeDate = _T("");			// 向导栏获取输入的时间
	ErrorFlag = -1;			    // 时间是否非法的标志

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TIMESET);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_TIMESET);
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
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(1 == EnterTime)			// 第一次按下Enter键确认菜单的选择
	{
		CheckMenuNumber();
	}
	else if(2 == EnterTime)		// 第二次按下Enter键确认代码设置的选择
	{
		DoCheckInput();			// 对输入数据是否合理做出相应项的实际判断
	}
	return TRUE;
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
void CTimeSetSvc::CheckMenuNumber()
{
	// 验证输入的菜单编号
	m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
	if (1 != m_DataModel.MenuNumber)	// 主菜单编号除1之外的为非法菜单号
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	else
	{
		// 检测是否脱机状态
		if (!theAPP_SESSION.IsSCConnected())
		{
			theAPP_SESSION.DeleteAllInput();
			theAPP_SESSION.SetPrefixToGuide(_opl(RWTEST_TEXT_DATE_TIME));
			theAPP_SESSION.SetMaxInputLength(14);
			EnterTime = 2;
		}
		// 连线状态不可以时间设置
		else
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TIMESET_TIP_MASSAGEBOX));
		}
		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      时间是否非法判断

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetSvc::DoCheckInput()
{
	TimeDate = theAPP_SESSION.GetInputToString();
	if(_T("") == TimeDate)
	{
		return;
	}
	else		// 向导栏输入不为空检测时间是否非法
	{
		CString YearNumber = TimeDate.Left(4);
		CString MonthNumber = TimeDate.Mid(4,2);
		CString DayNumber = TimeDate.Mid(6,2);
		CString HourNumber = TimeDate.Mid(8,2);
		CString MinuteNumber = TimeDate.Mid(10,2);
		CString SecondNumber = TimeDate.Right(2);

		ErrorFlag = ComCheckDate(YearNumber,MonthNumber,DayNumber,HourNumber,MinuteNumber,SecondNumber);
		if(0 != ErrorFlag)
		{
			m_DataModel.TimeLegal = FALSE;
		}
		else
		{
			m_DataModel.setYear = YearNumber;
			m_DataModel.setMonth = MonthNumber;
			m_DataModel.setDay = DayNumber;
			m_DataModel.setHour = HourNumber;
			m_DataModel.setMinute = MinuteNumber;
			m_DataModel.setSecond = SecondNumber;
			CString DataStr = _T("");
			DataStr.Format(_T("%s-%s-%s %s:%s:%s"),m_DataModel.setYear,m_DataModel.setMonth,m_DataModel.setDay,m_DataModel.setHour,m_DataModel.setMinute,m_DataModel.setSecond);
			COleDateTime TimNum;
			TimNum.ParseDateTime(DataStr);
			TimNum.GetAsSystemTime(dSystemTime);
			m_DataModel.TimeLegal = TRUE;
		}
	}

	m_DataModel.TimeChange = TRUE;
	NotifyDataModelChanged();
	EnterTime = 1;						// 置按键次数为1，重新从选择菜单编号开始
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
LRESULT CTimeSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 如果在线则提示不允许设置
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardF2(0,0);
		theTVM_SETTING.SetTimeChangeValue(1);
		CString DataStr = _T("");
		DataStr.Format(_T("%s%s%s%s%s%s"),m_DataModel.setYear,m_DataModel.setMonth,m_DataModel.setDay,m_DataModel.setHour,m_DataModel.setMinute,m_DataModel.setSecond);
		theTVM_SETTING.SetCurSystemTime(DataStr);
		theAPP_SESSION.ShowOfficeGuide(TIMESET_SET_SUCESSFULLY);

		//SetLocalTime(&dSystemTime);			// 设置本地时间
	
		// 时间设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DATETIME_SETTINGS,true);
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     NumberDate

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::Model& CTimeSetSvc::GetDataModel()
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
void CTimeSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
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
LRESULT CTimeSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;							// 置按键次数为1，重新从选择菜单编号开始
		theAPP_SESSION.SetMaxInputLength(1);	// 设置最大输入位数
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
LRESULT CTimeSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}