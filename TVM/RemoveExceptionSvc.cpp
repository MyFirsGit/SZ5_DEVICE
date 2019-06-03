#include "stdafx.h"
#include "RemoveExceptionSvc.h"
#include "RemoveExceptionDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRemoveExceptionSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::CRemoveExceptionSvc():CTVMForeService(REMOVE_EXCEPTION_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20701_REMOVE_EXCEPTION_DLG, new CRemoveExceptionDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION,_opl(GUIDE_REMOVE_EXCEPTION_INIT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REMOVE_EXCEPTION, IDD_20701_REMOVE_EXCEPTION_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);

	//初始化
	//InitExceptionData();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::~CRemoveExceptionSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionSvc::OnStart()
{
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);
	//m_modol = Modol();
	//初始化
	InitExceptionData();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_REMOVE_EXCEPTION_INIT);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::Modol& CRemoveExceptionSvc::GetDataModol()
{
	return m_modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新窗体显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      确认按键（异常解除）

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRemoveExceptionSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	try {
		theSERVICE_MGR.SetForeServiceBusy(true);
		//1.异常解除
		theEXCEPTION_MGR.RepareException();
		// 更新异常数据
		InitExceptionData();
		//2.刷新异常列表
		NotifyDataModelChanged();
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_REMOVE_EXCEPTION_FINISHED));
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		//theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_OUT_DEVICE_SELECT));
	}
	catch (...) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__)); 
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRemoveExceptionSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化异常数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionSvc::InitExceptionData()
{
	m_modol.m_ListException.clear();
	//初始化数据
	m_modol.m_ListException = theEXCEPTION_MGR.GetAllException();          //获取所有的异常项
	m_modol.m_ExceptionNum  = m_modol.m_ListException.size();              //异常项的大小

	if (m_modol.m_ExceptionNum == 0)
	{
		SetPageInfo(0,1,_MAX_ROW-1);
	}
	else
	{
		int nTotalPage = m_modol.m_ExceptionNum / (_MAX_ROW - 1)  ? m_modol.m_ExceptionNum / (_MAX_ROW - 1) : 1;//异常项共有页数
		SetPageInfo(nTotalPage,1,_MAX_ROW-1);
	}
}