#include "stdafx.h"
#include "StatusDlg.h"
#include "MainFrm.h"
#include "TVM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SYSTEM_CLOCK_TIMER		1000		// ϵͳʱ��ˢ�¶�ʱ��

IMPLEMENT_DYNAMIC(CStatusDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(CStatusDlg, CBaseDlg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd* pParent

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStatusDlg::~CStatusDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:�����ɹ�  -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(SYSTEM_CLOCK_TIMER, TIMER_INTERVAL, NULL);	// ����ϵͳʱ��ˢ�¶�ʱ��

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ٺ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnDestroy()
{
	this->KillTimer(SYSTEM_CLOCK_TIMER);	// �ر�ϵͳʱ����ʾ��ʱ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	CRect roundRect;
	GetClientRect(roundRect);
	// �����ڴ�DC
	CDC MemDc;
	MemDc.CreateCompatibleDC(NULL);// �ڴ���׼��ͼ��
	// �����ڴ�λͼ
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC,roundRect.Width(),roundRect.Height());
	// �ڴ�λͼ���ͼDC����
	MemDc.SelectObject(MemBitmap);
	// ���ñ���ɫ
	MemDc.FillSolidRect(0,0,roundRect.Width(),roundRect.Height(),BLUE/*RGB(255,255,255)*/);
	// ʹ���ڴ�DC��ͼ
	CDC* pTmpDc = &MemDc;

	// ϵͳʱ��
	m_system_clock_time = CTime::GetCurrentTime();
	DrawText(pTmpDc, m_system_clock_rc, ComGetSysTimeText(YYYYMMDDHHMM_TYPE3), DEFAULT_FONT, WHITE, DT_SINGLELINE|DT_VCENTER);
	// ����ԱID
	DrawText(pTmpDc, m_operator_id_rc, m_operator_id, DEFAULT_FONT, WHITE, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	// ����ģʽ
	if(!m_operating_mode.IsEmpty()){
		DrawText(pTmpDc, m_operating_mode_rc, m_operating_mode, DEFAULT_FONT, WHITE, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}
	// �豸״̬��Ϣ
	else{
		DrawText(pTmpDc, m_operating_mode_rc, m_bIsShowMsg?_opl(m_status_msg):_T(""), DEFAULT_FONT, RED, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	}	
	// ����״̬
	m_network_status_color = (m_network_status == TXT_ID_STATUES_DISCONNECTED) ? RED : SOFTGREEN;
	DrawText(pTmpDc, m_network_status_rc, _opl(m_network_status), DEFAULT_FONT, m_network_status_color, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	// ����������ͼ�ο�������ͼDC
	pDC->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// ��ͼ����
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// ��ͼ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ò���ԱID

@param      (i)CString id ����ԱID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetOperatorID(CString id)
{
	m_operator_id = id.IsEmpty() ? _T(""): _T("ID:") + id;
	InvalidateRect(m_operator_id_rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������״̬

@param      (i)CString status ����״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetNetWorkStatus(LANGUAGE_MSG status)
{
	m_network_status = status;
	InvalidateRect(m_network_status_rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ģʽ

@param      (i)CString mode ����ģʽ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetOperatingMode(CString mode)
{
	m_operating_mode = mode;
	InvalidateRect(m_operating_mode_rc, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ״̬��Ϣ��վԱ

@param      (in)CString strMsg ��ʾ��վԱ����Ϣ

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
@brief      ϵͳʱ����ʾ����

@param      UINT nIDEvent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnTimer(UINT nIDEvent) 
{
	// ʱ��仯ʱ��ˢ��ϵͳʱ����ʾ����
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
@brief      ˢ�½�����ʾ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::ReFlashStatusDlg(){
	InvalidateRect(m_network_status_rc, TRUE);
	InvalidateRect(m_operating_mode_rc,TRUE);
	UpdateWindow();
}