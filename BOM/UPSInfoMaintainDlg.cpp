#include "stdafx.h"
#include "UPSInfoMaintainDlg.h"
#include "AppSession.h"
#include "BOM.h"
#include "XButton.h"
#include "GlobalParameter.h"
#include "SystemSetSvc.h"
#include "CAccLineInfoParam.h"
#include "UPSInfoSvc.h"
#include "guideinfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CUPSInfoMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CUPSInfoMaintainDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoMaintainDlg::CUPSInfoMaintainDlg(CService* pService)
: COperationDlg(CUPSInfoMaintainDlg::IDD, pService)
{
	m_baseInfo->titleInfo.titleName = TITLE_UPS_INFO;					   // 标题
	m_baseInfo->detailInfo.row = 2;                                        // 行数
	m_baseInfo->detailInfo.labelGroup = m_labelUPSInfo;                   // 标签文字
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CUPSInfoMaintainDlg::~CUPSInfoMaintainDlg()
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
int CUPSInfoMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try
	{
		if (__super::OnCreate(lpCreateStruct, BASE_AREA | GUIDE_AREA) == -1)
		{
			return -1;
		}

		// 初始化基本区域的label信息
		InitLabelUPSInfo();
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)
	{
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
void CUPSInfoMaintainDlg::InitLabelUPSInfo()
{
	CUPSInfoSvc* pUPSInfoSvc = (CUPSInfoSvc*)GetService();
	try
	{
		// 初始化USDN信息区域文字
		m_labelUPSInfo[0].nameLeft = add_wsp(_T("剩余电量百分比："), MAX_SBC_CASE_LEN);
		m_labelUPSInfo[1].nameLeft = add_wsp(_T("剩余时间："), MAX_SBC_CASE_LEN);

		CString strTxt;
		strTxt.Format(_T("%d%%"), pUPSInfoSvc->GetUPSHelper()->GetUPSBatteryCapacity());
		m_labelUPSInfo[0].nameRight = strTxt;
		strTxt.Format(_T("%d分钟"), pUPSInfoSvc->GetUPSHelper()->GetUPSLeftStandbyTime());
		m_labelUPSInfo[1].nameRight = strTxt;

		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UPS_INFO_READ_SUCCESS));
	}
	catch(CSysException&)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_UPS_INFO_READ_FAIL));
		//throw;
	}
	catch (...)
	{
		throw CInnerException(pUPSInfoSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}