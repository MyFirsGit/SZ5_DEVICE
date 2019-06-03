#include "stdafx.h"
#include "StationSetMaintainDlg.h"
#include "StationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CStationSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

static CRect editStationinfo[] = 
{
	BASE_INFO_RECT(1,4),
	BASE_INFO_RECT(2,4),
	BASE_INFO_RECT(3,4),
	BASE_INFO_RECT(4,4),
	BASE_INFO_RECT(5,4),
};

static const TCHAR* const TXT_LINE_CODE			=	_T("线路代码：");
static const TCHAR* const TXT_STATION_CODE		= 	_T("车站代码：");
static const TCHAR* const TXT_STATION_NAME		= 	_T("车站名：");
static const TCHAR* const TXT_MACHINE_CODE		= 	_T("本机号码：");
static const TCHAR* const TXT_GROUP_NUMBER		=	_T("组号：");
static const TCHAR* const TXT_EQUNOINGROUP		=	_T("组内编号：");

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::CStationSetMaintainDlg(CService* pService) 
	: COperationDlg(CStationSetMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TITLE_STATION_INFO;                  // 标题
	m_baseInfo->detailInfo.row = 5;                                        // 行数
	m_baseInfo->detailInfo.labelGroup = m_labelStationinfo;                // 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 5;                              // 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editStationinfo;           // 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::~CStationSetMaintainDlg()
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
int CStationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		if (__super::OnCreate(lpCreateStruct,  BASE_AREA | BTN_RESET | BTN_OK | GUIDE_AREA) == -1){
			return -1;
		}
		// 设置对话框输入限制
		GetEdit(0)->SetLimitText(2);
		GetEdit(1)->SetLimitText(2);
		GetEdit(2)->SetLimitText(2);
		GetEdit(3)->SetLimitText(2);
		GetEdit(4)->SetLimitText(2);
		// 初始化基本区域的label信息
		InitlabelStationInfo();
		// 初始化Edit框里的数据
		InitEditText();
	}
	catch (CSysException& e)	{
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
void CStationSetMaintainDlg::InitlabelStationInfo()
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try {
		// 初始化车站信息区域文字
		m_labelStationinfo[0].nameLeft = add_wsp(TXT_LINE_CODE,MAX_SBC_CASE_LEN);
		m_labelStationinfo[1].nameLeft = add_wsp(TXT_STATION_CODE,MAX_SBC_CASE_LEN);
		m_labelStationinfo[2].nameLeft = add_wsp(TXT_MACHINE_CODE,MAX_SBC_CASE_LEN);
		m_labelStationinfo[3].nameLeft = add_wsp(TXT_GROUP_NUMBER,MAX_SBC_CASE_LEN);
		m_labelStationinfo[4].nameLeft = add_wsp(TXT_EQUNOINGROUP,MAX_SBC_CASE_LEN);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...)	{
		throw CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域EDIT里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::InitEditText()
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try{
		if(theMAINTENANCE_INFO.Initialize() == 0){
			// 线路代码，车站代码，本机代码，组号，组内编号
			CString sLineCode,sStationCode,sMachineCode,sGroupNumber,sEquNumberInGroup;
			// 从维护信息中取得线路代码，车站代码，本机代码，组号，组内编号
			sLineCode.Format(_T("%.2X"),theMAINTENANCE_INFO.GetLineCode());
			sStationCode.Format(_T("%.2X"),theMAINTENANCE_INFO.GetStationCode());
			sMachineCode.Format(_T("%.2X"),theMAINTENANCE_INFO.GetMachineCode());
			sGroupNumber.Format(_T("%.2X"),theMAINTENANCE_INFO.GetGroupNumber());
			sEquNumberInGroup.Format(_T("%.2X"),theMAINTENANCE_INFO.GetEquNumberInGroup());

			// 将车站代码，本机代码，组号，组内编号设置到界面
			SetEditText(STATIONINFO_EDIT_GROUP_LINECODE,sLineCode);
			SetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE,sStationCode);
			SetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE,sMachineCode);
			SetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER,sGroupNumber);
			SetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP,sEquNumberInGroup);

			// 设置编辑框焦点
			SetEditFocus(STATIONINFO_EDIT_GROUP_LINECODE);
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_T("（红）获取车站信息失败"));
			HideCompoment(BTN_OK | BTN_RESET | BASE_AREA);
		}
	}
	catch(CSysException&){
		throw;
	}
	catch (...) {
		throw CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置车站信息

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::SetStationInfo()
{
	// 从维护信息中取得线路代码，车站代码，本机代码，组号，组内编号
	int iLineCode  = _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_LINECODE).GetBuffer(),NULL,16);
	int iStationCode = _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE).GetBuffer(),NULL,16);
	int iMachineCode = _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE).GetBuffer(),NULL,16);
	int iGroupNumber =  _tcstol(GetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER).GetBuffer(),NULL,16);
	int iEquNumberInGroup =_tcstol(GetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP).GetBuffer(),NULL,16);
	// 设置车站信息
	theMAINTENANCE_INFO.SetLineCode(iLineCode);
	theMAINTENANCE_INFO.SetStationCode(iStationCode);
	theMAINTENANCE_INFO.SetMachineCode(iMachineCode);
	theMAINTENANCE_INFO.SetGroupNumber(iGroupNumber);
	theMAINTENANCE_INFO.SetEquNumberInGroup(iEquNumberInGroup);
	theMAINTENANCE_INFO.Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查EDIT框里的数据是否合法

@param      void

@retval     int  0  成功
-1 不成功
1  所设的车站不在票价表

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CStationSetMaintainDlg::CheckInputText()
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try {
		// 检查线路代码是否在(0 - 255之间)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_LINECODE),0,255)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_LINECODE);
			return -1;
		}
		// 检查车站代码是否在(0 - 255之间)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE),0,255)) {
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_STATIONCODE);
			return -1;
		}
		// 检查本机号码是否在(01 - 254之间)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE),1,254)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_MACHINECODE);
			return -1 ;
		}
		// 检查组号是否在(0 - 255之间)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER),0,255)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_GROUPNUMBER);
			return -1 ;
		}
		// 检查组内编号是否在(0 - 255之间)
		if (!CheckNumText(GetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP),0,255)){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INPUT_ERROR));
			SetEditFocus(STATIONINFO_EDIT_GROUP_EQUNOINGROUP);
			return -1 ;
		}
		// 通过车站代码获得车站名称，如果获得不到，返回false,如果获得到了，返回true
		BYTE theLineCode = (_tcstol(GetEditText(STATIONINFO_EDIT_GROUP_LINECODE).GetBuffer(),NULL,16) & 0xFF);
		BYTE theStationCode = (_tcstol(GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE).GetBuffer(),NULL,16) & 0xFF);
		// 由线路号，车站号获取车站ID
		WORD dStationId = MAKEWORD(theStationCode,theLineCode);
		// 由车站ID获取车站信息列表
		CAccLineInfoParam::_station_Info vStationInfoList;
		bool bSuccess = theACC_LINE.GetStationInfobyCode(dStationId,vStationInfoList);
		if (!bSuccess){
			return 1;
		}
		else{
			// 设置当前车站名称
			CString station_name_cn = vStationInfoList.station_Chinese_Name;
			CString station_name_en = vStationInfoList.station_Eng_Name;
			theAPP_SESSION.SetStationName(station_name_cn,station_name_en);
		}
		return 0 ;
	}
	catch(CSysException&){
		throw;
	}
	catch (...) {
		throw CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认、返回、重置 按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CStationSetMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	// 初始化编辑框
	InitEditText();
	// 设置导航栏
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_INFO_INPUT));
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CStationSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CStationSetSvc* pStationSetSvc = (CStationSetSvc*)GetService();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// 检查输入是否符合要求
		int errorCode = CheckInputText();
		// 如果输入合法
		if(errorCode == 0){
			// 设置车站信息
			SetStationInfo();
			HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_FINISH));
		}
		else if(errorCode == 1){
			SetStationInfo();
			HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_STATION_SET_FARE_ERROR));
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
		CInnerException e = CInnerException(pStationSetSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏界面组件

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// 设置标签文字
	m_labelStationinfo[0].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_LINECODE);
	m_labelStationinfo[1].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_STATIONCODE);
	m_labelStationinfo[2].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_MACHINECODE);
	m_labelStationinfo[3].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_GROUPNUMBER);
	m_labelStationinfo[4].nameRight = GetEditText(STATIONINFO_EDIT_GROUP_EQUNOINGROUP);
	// 隐藏组件
	HideCompoment(ulCompomentFlag);
}
