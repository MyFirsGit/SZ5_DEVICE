
// CStatusDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "BOM.h"
#include "StatusDLG.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_STS 1000
//#define STATUS_FONT       "宋体"

IMPLEMENT_DYNAMIC(CStatusDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CStatusDlg, CBaseDlg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusDlg::CStatusDlg(CWnd* pParent)
:CBaseDlg(CStatusDlg::IDD, pParent, STS_FRAME_RECT, RECEPTION_HEAD_BK_COLOR)
,m_userID("———")
,m_networkStatus("脱机")
,m_printerStatus("正常")
,m_outRWStatus("正常")
,m_TWStatus("正常")
,m_THStatus("正常")
{
	m_rectUserID = CRect(0,0,270,55);
	m_rectNetworkStatus = CRect(270,0,490,55);
	m_rectPrinterStatus = CRect(490,0,735,55);
	m_rectOutRWStatus = CRect(735,0,1020,55);
	m_rectTWStatus=CRect(1020,0,1297,55);
	m_rectTHStatus = CRect(1297,0,1540,55);
	m_rectTime = CRect(1540,0,1920,55);
	if(!theBOM_INFO.IsRWUsed())
	{
		m_outRWStatus = "未用";
	}
	if(!theBOM_INFO.IsRWUsed(false))
	{
		m_TWStatus = "未用";
	}
	if(theSETTING.GetTHMode() == TH_MODE_ABNORMAL)
	{
		m_THStatus ="未用";
	}
	if(!theBOM_INFO.IsPrinterUsed())
	{
		m_printerStatus = "未用";
	}
	theSETTING.THModeChanged.AddHandler(this,	&CStatusDlg::OnTHModeChanged);
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CStatusDlg::OnExceptionNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusDlg::~CStatusDlg()
{
	theSETTING.THModeChanged.RemoveHandler(this,	&CStatusDlg::OnTHModeChanged);
	theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CStatusDlg::OnExceptionNotify);
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
int CStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	// 启动时钟显示计时器
	SetTimer(TIMER_STS, TIMER_INTERVAL, NULL);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面销毁函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnDestroy()
{
	// 关闭时钟显示计时器
	this->KillTimer(TIMER_STS);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	DrawRect(pDC,m_rectUserID,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectNetworkStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectPrinterStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectOutRWStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectTWStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectTHStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectTime,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	
	DrawText(pDC,m_rectUserID,_T(" 操作员: ") + (m_userID.IsEmpty() ? CString(_T("———")) : m_userID),DEFAULT_FONT,WHITE);
	DrawText(pDC,m_rectNetworkStatus,_T("        网络:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectPrinterStatus,_T("        打印机:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectOutRWStatus,_T("        外部读写器:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTWStatus,_T("       内部读写器:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTHStatus,_T("       出票机:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);


	DrawText(pDC,m_rectNetworkStatus,m_networkStatus+_T("       "),DEFAULT_FONT,m_networkStatus == _T("连接") ? SOFTGREEN:RED ,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	DrawText(pDC,m_rectPrinterStatus,m_printerStatus+_T("        "),DEFAULT_FONT,m_printerStatus == _T("正常") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectOutRWStatus,m_outRWStatus+_T("        "),DEFAULT_FONT,m_outRWStatus == _T("正常") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTWStatus,m_TWStatus+_T("        "),DEFAULT_FONT,m_TWStatus == _T("正常") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTHStatus,m_THStatus+_T("        "),DEFAULT_FONT,m_THStatus == _T("正常") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);


	DrawText(pDC, m_rectTime, ComGetSysTimeText(YYYYMMDDHHMMSS_TYPE5) + _T(" ") + ComGetSysTimeText(WEEKDAY_CN),DEFAULT_FONT,WHITE);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnExceptionNotify(CSysException&,bool)
{
	RefreshHardwareStatus();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnTHModeChanged(int&)
{
	RefreshHardwareStatus();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     设置硬件的启用状态 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::RefreshHardwareStatus()
{
	if(!theBOM_INFO.IsRWUsed())
	{
		SetOutRWStatus( _T("未用"));
	}
	else
	{
		bool hasRWException = theEXCEPTION_MGR.HasException(CRWException::MODULE_ID) || theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID);
		SetOutRWStatus(hasRWException?_T("异常"):_T("正常"));
	}
	if(!theBOM_INFO.IsRWUsed(false))
	{
		SetTWStatus(_T("未用"));
	}
	else
	{
		bool hasTWException = theEXCEPTION_MGR.HasException(CTWException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID);
		SetTWStatus(hasTWException?_T("异常"):_T("正常"));
	}
	if(theSETTING.GetTHMode() == TH_MODE_ABNORMAL)
	{
		SetTHStatus(_T("未用"));
	}
	else
	{
		bool hasTHException = theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID);
		SetTHStatus(hasTHException?_T("异常"):_T("正常"));
	}
	if(!theBOM_INFO.IsPrinterUsed())
	{
		SetPrinterStatus(_T("未用"));
	}
	else
	{
		bool hasPrinterException = theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID);
		SetPrinterStatus(hasPrinterException?_T("异常"):_T("正常"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置管理员ID

@param      (i)CString userID   管理员ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetUserID(CString userID)
{
	m_userID = userID;
	InvalidateRect(m_rectUserID, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置网络连接状态

@param      (i)CString networkStatus   网络状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetNetWorkStatus(CString networkStatus)
{
	m_networkStatus = networkStatus;
	InvalidateRect(m_rectNetworkStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置打印机状态

@param      (i)CString printerStatus   打印机状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetPrinterStatus(CString printerStatus)
{
	m_printerStatus = printerStatus;
	InvalidateRect(m_rectPrinterStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置外部读写器状态

@param      (i)CString outRWStatus   外部读写器状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetOutRWStatus(CString outRWStatus)
{
	m_outRWStatus = outRWStatus;
	InvalidateRect(m_rectOutRWStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置内部读写器状态

@param      (i)CString twStatus   内部读写器状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetTWStatus(CString twStatus)
{
	m_TWStatus = twStatus;
	InvalidateRect(m_rectTWStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置内部读写器状态

@param      (i)CString twStatus   内部读写器状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetTHStatus(CString thStatus)
{
	m_THStatus = thStatus;
	InvalidateRect(m_rectTHStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      时间控制

@param      UINT nIDEvent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnTimer(UINT nIDEvent) 
{
	_DATE_TIME curTime = ComGetCurTime();

	// 初始或分钟改变时，刷新时间显示
	if (m_preTime!=curTime) {
		InvalidateRect(m_rectTime, TRUE);
		m_preTime = curTime;
	}

}

