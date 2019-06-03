#include "stdafx.h"
#include "StatusDlg.h"
#include "MainFrm.h"
#include "TVM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SYSTEM_CLOCK_TIMER		1000		// 系统时钟刷新定时器

IMPLEMENT_DYNAMIC(CStatusDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(CStatusDlg, CBaseDlg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd* pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusDlg::CStatusDlg(CWnd* pParent)
	:CBaseDlg(CStatusDlg::IDD, pParent, OPERATOR_STATUS_RECT, OPERATOR_STATE_BK_COLOR /*OPERATOR_STATUS_BK_COLOR*/)
	,m_operator_id(_T(""))
	,m_network_status(TXT_ID_STATUES_DISCONNECTED)
	,m_operating_mode(/*theTVM_STATUS_MGR.GetRunModeName()*/)
{
	//m_system_clock_rc = CRect(0,0,RESOLUTION * 150,HRESOLUTION * 25);
	//m_operator_id_rc = CRect(RESOLUTION * 150,0,RESOLUTION * 230,HRESOLUTION * 25);
	//m_operating_mode_rc = CRect(RESOLUTION * 230,0,RESOLUTION * 560,HRESOLUTION*25);
	//m_network_status_rc = CRect(560*RESOLUTION,0,OPERATOR_STATUS_RECT.right,OPERATOR_STATUS_RECT.bottom);
	int tmp = 40;
	//m_system_clock_rc = CRect(2,0,200,30);
	//m_operator_id_rc = CRect(200+tmp,0,300+tmp,30);
	//m_operating_mode_rc = CRect(300+2*tmp,0,560+2*tmp,30);
	//m_network_status_rc = CRect(560+3*tmp,0,638+3*tmp,30);
	m_system_clock_rc = CRect(5,0,185,30);
	m_operator_id_rc = CRect(185+tmp,0,285+tmp,30);
	m_operating_mode_rc = CRect(285+2*tmp,0,600+2*tmp,30);
	m_network_status_rc = CRect(600+3*tmp,0,678+3*tmp,30); 
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
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:创建成功  -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(SYSTEM_CLOCK_TIMER, TIMER_INTERVAL, NULL);	// 启动系统时钟刷新定时器

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
	this->KillTimer(SYSTEM_CLOCK_TIMER);	// 关闭系统时钟显示定时器
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

	CRect roundRect;
	GetClientRect(roundRect);
	// 创建内存DC
	CDC MemDc;
	MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
	// 创建内存位图
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC,roundRect.Width(),roundRect.Height());
	// 内存位图与绘图DC关联
	MemDc.SelectObject(MemBitmap);
	// 设置背景色
	MemDc.FillSolidRect(0,0,roundRect.Width(),roundRect.Height(),BLUE/*RGB(255,255,255)*/);
	// 使用内存DC绘图
	CDC* pTmpDc = &MemDc;

	// 系统时间
	m_system_clock_time = CTime::GetCurrentTime();
	DrawText(pTmpDc, m_system_clock_rc, ComGetSysTimeText(YYYYMMDDHHMM_TYPE3), DEFAULT_FONT, WHITE, DT_SINGLELINE|DT_VCENTER);
	// 操作员ID
	DrawText(pTmpDc, m_operator_id_rc, m_operator_id, DEFAULT_FONT, WHITE, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	// 运行模式
	if(!m_operating_mode.IsEmpty()){
		DrawText(pTmpDc, m_operating_mode_rc, m_operating_mode, DEFAULT_FONT, WHITE, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
	// 设备状态消息
	else{
		DrawText(pTmpDc, m_operating_mode_rc, m_bIsShowMsg?_opl(m_status_msg):_T(""), DEFAULT_FONT, RED, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}	
	// 网络状态
	m_network_status_color = (m_network_status == TXT_ID_STATUES_DISCONNECTED) ? RED : SOFTGREEN;
	DrawText(pTmpDc, m_network_status_rc, _opl(m_network_status), DEFAULT_FONT, m_network_status_color, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	// 将缓冲区的图形拷贝到绘图DC
	pDC->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// 绘图清理
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// 绘图结束
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置操作员ID

@param      (i)CString id 操作员ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetOperatorID(CString id)
{
	m_operator_id = id.IsEmpty() ? _T(""): _T("ID:") + id;
	InvalidateRect(m_operator_id_rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置网络状态

@param      (i)CString status 网络状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetNetWorkStatus(LANGUAGE_MSG status)
{
	m_network_status = status;
	InvalidateRect(m_network_status_rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置运行模式

@param      (i)CString mode 运行模式

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetOperatingMode(CString mode)
{
	m_operating_mode = mode;
	InvalidateRect(m_operating_mode_rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示状态信息给站员

@param      (in)CString strMsg 显示给站员的信息

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::ShowStatusMsg(LANGUAGE_MSG strMsg,bool bIsShowup){
	m_bIsShowMsg = bIsShowup;
	m_status_msg = strMsg;
	m_operating_mode = _T("");
	InvalidateRect(m_operating_mode_rc,TRUE);
	UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      系统时钟显示控制

@param      UINT nIDEvent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnTimer(UINT nIDEvent) 
{
	// 时间变化时，刷新系统时钟显示区域
	_DATE_TIME new_time = ComGetCurTime();
	if (m_old_time != new_time)
	{
		InvalidateRect(m_system_clock_rc, FALSE);
		m_old_time = new_time;
	}
}

void CStatusDlg::SetUserID(CString id)
{
	SetOperatorID(id);
}

void CStatusDlg::SetVMIssueAmount(DWORD money)
{
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      刷新界面显示

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::ReFlashStatusDlg(){
	InvalidateRect(m_network_status_rc, TRUE);
	InvalidateRect(m_operating_mode_rc,TRUE);
	UpdateWindow();
}