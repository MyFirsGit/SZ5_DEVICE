#include "stdafx.h"
#include "TimeSetBaseOfficeDlg.h"
#include "TimeSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const STR_TIMESET_NOTEMSG	= _T("时间设定");
static const TCHAR* const TXT_TIME_YEAR			= _T("年份：");
static const TCHAR* const TXT_TIME_MONTH		= _T("月份：");
static const TCHAR* const TXT_TIME_DATE		    = _T("日期：");
static const TCHAR* const TXT_TIME_HOUR		    = _T("时数：");
static const TCHAR* const TXT_TIME_MINUTE		= _T("分数：");

// 各输入框位置及大小
static CRect editTime[] = {
	BASE_INFO_RECT(1,4),  // 年
	BASE_INFO_RECT(2,4),  // 月
	BASE_INFO_RECT(3,4),  // 日
	BASE_INFO_RECT(4,4),  // 时
	BASE_INFO_RECT(5,4)   // 分
};

IMPLEMENT_DYNAMIC(CTimeSetBaseOfficeDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CTimeSetBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::CTimeSetBaseOfficeDlg(CService* pService)
:COperationDlg(CTimeSetBaseOfficeDlg::IDD, pService)
{
	// 时间设定信息
	m_baseInfo->titleInfo.titleName = STR_TIMESET_NOTEMSG;      // 标题
	m_baseInfo->detailInfo.row = 5;								// 行数
	m_baseInfo->detailInfo.labelGroup = timeSetInfo;            // 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 5;          // 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editTime;       // 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::~CTimeSetBaseOfficeDlg()
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
int CTimeSetBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		// 创建基本信息区域、返回、确定按钮和数字键盘
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA  | BTN_OK | BTN_RESET)){
			return -1;
		}
		// 设置对话框输入限制
		GetEdit(0)->SetLimitText(4);
		GetEdit(1)->SetLimitText(2);
		GetEdit(2)->SetLimitText(2);
		GetEdit(3)->SetLimitText(2);
		GetEdit(4)->SetLimitText(2);
		// 初始化BASE区域LABEL里面的数据
		InitlabelTimeInfo();
		// 取得系统时间,赋予初始时间
		_DATE_TIME currentTime = ComGetCurTime();
		CString strCurYear,strCurMonth,strCurDay,strCurHour,strCurMinute;
		strCurYear.Format(_T("%.4d"),currentTime.wYear);
		strCurMonth.Format(_T("%.2d"),currentTime.biMonth);
		strCurDay.Format(_T("%.2d"),currentTime.biDay);
		strCurHour.Format(_T("%.2d"),currentTime.biHour);
		strCurMinute.Format(_T("%.2d"),currentTime.biMinute);
		SetEditText(0,strCurYear);
		SetEditText(1,strCurMonth);
		SetEditText(2,strCurDay);
		SetEditText(3,strCurHour);
		SetEditText(4,strCurMinute);
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::InitlabelTimeInfo()
{
	timeSetInfo[0].nameLeft = add_wsp(TXT_TIME_YEAR,MAX_SBC_CASE_LEN);
	timeSetInfo[0].nameRight = add_hsp(YEAR_UNIT,8);
	timeSetInfo[1].nameLeft = add_wsp(TXT_TIME_MONTH,MAX_SBC_CASE_LEN);
	timeSetInfo[1].nameRight =add_hsp(MONTH_UNIT,8);
	timeSetInfo[2].nameLeft = add_wsp(TXT_TIME_DATE,MAX_SBC_CASE_LEN);
	timeSetInfo[2].nameRight= add_hsp(DAY_UNIT,8);
	timeSetInfo[3].nameLeft = add_wsp(TXT_TIME_HOUR,MAX_SBC_CASE_LEN);
	timeSetInfo[3].nameRight= add_hsp(HOUR_UNIT,8);
	timeSetInfo[4].nameLeft = add_wsp(TXT_TIME_MINUTE,MAX_SBC_CASE_LEN);
	timeSetInfo[4].nameRight= add_hsp(MINUTE_UNIT,8);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetBaseOfficeDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	try{
		_DATE_TIME currentTime = ComGetCurTime();
		CString strCurYear,strCurMonth,strCurDay,strCurHour,strCurMinute;
		strCurYear.Format(_T("%.4d"),currentTime.wYear);
		strCurMonth.Format(_T("%.2d"),currentTime.biMonth);
		strCurDay.Format(_T("%.2d"),currentTime.biDay);
		strCurHour.Format(_T("%.2d"),currentTime.biHour);
		strCurMinute.Format(_T("%.2d"),currentTime.biMinute);
		SetEditText(0,strCurYear);
		SetEditText(1,strCurMonth);
		SetEditText(2,strCurDay);
		SetEditText(3,strCurHour);
		SetEditText(4,strCurMinute);
		SetEditFocus(0);
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TIMESET_INPUT_AND_CONFIRM));
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CTimeSetSvc* pTimeSetSvc = (CTimeSetSvc*)GetService();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// 判断是否脱机状态
		if (!theAPP_SESSION.IsSCConnected()){
			if (CheckInput()){
				SYSTEMTIME dSystemTime;
				GetSystemTime(&dSystemTime);
				dSystemTime.wYear	= _ttoi(GetEditText(0));
				dSystemTime.wMonth	= _ttoi(GetEditText(1));
				dSystemTime.wDay	= _ttoi(GetEditText(2));
				dSystemTime.wHour	= _ttoi(GetEditText(3));
				dSystemTime.wMinute = _ttoi(GetEditText(4));
				SetLocalTime(&dSystemTime);

				theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TIMESET_FINISH));
				HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
			}
		}
		else{
			CMessageBox::Show(_T("提示"),_T("联机状态不允许进行手动时钟设置"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(pTimeSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测输入是否正确

@param      none

@retval     bool		\n
TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTimeSetBaseOfficeDlg::CheckInput()
{
	// 检查年月日时分
	BYTE errCode = ComCheckDate(GetEditText(0),GetEditText(1),GetEditText(2),GetEditText(3),GetEditText(4));
	if (0 != errCode){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_TIMESET_INPUT_ERROR));
		SetEditFocus(errCode - 1);
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏界面组件

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// 设置标签文字
	timeSetInfo[0].nameRight = add_hsp(GetEditText(0) + _T("  ") + YEAR_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[1].nameRight = add_hsp(GetEditText(1) + _T("    ") + MONTH_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[2].nameRight = add_hsp(GetEditText(2) + _T("    ") + DAY_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[3].nameRight = add_hsp(GetEditText(3) + _T("    ") + HOUR_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[4].nameRight = add_hsp(GetEditText(4) + _T("    ") + MINUTE_UNIT,MAX_SBC_CASE_LEN);
	// 隐藏组件
	HideCompoment(ulCompomentFlag);
	// 刷新基本信息区
	m_baseInfoArea->Invalidate();
}
