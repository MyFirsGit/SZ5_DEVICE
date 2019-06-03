
// CStatusDlg.cpp : ʵ���ļ�
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
//#define STATUS_FONT       "����"

IMPLEMENT_DYNAMIC(CStatusDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CStatusDlg, CBaseDlg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStatusDlg::CStatusDlg(CWnd* pParent)
:CBaseDlg(CStatusDlg::IDD, pParent, STS_FRAME_RECT, RECEPTION_HEAD_BK_COLOR)
,m_userID("������")
,m_networkStatus("�ѻ�")
,m_printerStatus("����")
,m_outRWStatus("����")
,m_TWStatus("����")
,m_THStatus("����")
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
		m_outRWStatus = "δ��";
	}
	if(!theBOM_INFO.IsRWUsed(false))
	{
		m_TWStatus = "δ��";
	}
	if(theSETTING.GetTHMode() == TH_MODE_ABNORMAL)
	{
		m_THStatus ="δ��";
	}
	if(!theBOM_INFO.IsPrinterUsed())
	{
		m_printerStatus = "δ��";
	}
	theSETTING.THModeChanged.AddHandler(this,	&CStatusDlg::OnTHModeChanged);
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CStatusDlg::OnExceptionNotify);
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
	theSETTING.THModeChanged.RemoveHandler(this,	&CStatusDlg::OnTHModeChanged);
	theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CStatusDlg::OnExceptionNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CStatusDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	// ����ʱ����ʾ��ʱ��
	SetTimer(TIMER_STS, TIMER_INTERVAL, NULL);

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
	// �ر�ʱ����ʾ��ʱ��
	this->KillTimer(TIMER_STS);
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

	DrawRect(pDC,m_rectUserID,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectNetworkStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectPrinterStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectOutRWStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectTWStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectTHStatus,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	DrawRect(pDC,m_rectTime,1,BLACK,RECEPTION_HEAD_BK_COLOR);
	
	DrawText(pDC,m_rectUserID,_T(" ����Ա: ") + (m_userID.IsEmpty() ? CString(_T("������")) : m_userID),DEFAULT_FONT,WHITE);
	DrawText(pDC,m_rectNetworkStatus,_T("        ����:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectPrinterStatus,_T("        ��ӡ��:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectOutRWStatus,_T("        �ⲿ��д��:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTWStatus,_T("       �ڲ���д��:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTHStatus,_T("       ��Ʊ��:"),DEFAULT_FONT,WHITE,DT_LEFT|DT_SINGLELINE|DT_VCENTER);


	DrawText(pDC,m_rectNetworkStatus,m_networkStatus+_T("       "),DEFAULT_FONT,m_networkStatus == _T("����") ? SOFTGREEN:RED ,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	DrawText(pDC,m_rectPrinterStatus,m_printerStatus+_T("        "),DEFAULT_FONT,m_printerStatus == _T("����") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectOutRWStatus,m_outRWStatus+_T("        "),DEFAULT_FONT,m_outRWStatus == _T("����") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTWStatus,m_TWStatus+_T("        "),DEFAULT_FONT,m_TWStatus == _T("����") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
	DrawText(pDC,m_rectTHStatus,m_THStatus+_T("        "),DEFAULT_FONT,m_THStatus == _T("����") ? SOFTGREEN:RED,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);


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
@brief     ����Ӳ��������״̬ 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::RefreshHardwareStatus()
{
	if(!theBOM_INFO.IsRWUsed())
	{
		SetOutRWStatus( _T("δ��"));
	}
	else
	{
		bool hasRWException = theEXCEPTION_MGR.HasException(CRWException::MODULE_ID) || theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID);
		SetOutRWStatus(hasRWException?_T("�쳣"):_T("����"));
	}
	if(!theBOM_INFO.IsRWUsed(false))
	{
		SetTWStatus(_T("δ��"));
	}
	else
	{
		bool hasTWException = theEXCEPTION_MGR.HasException(CTWException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID);
		SetTWStatus(hasTWException?_T("�쳣"):_T("����"));
	}
	if(theSETTING.GetTHMode() == TH_MODE_ABNORMAL)
	{
		SetTHStatus(_T("δ��"));
	}
	else
	{
		bool hasTHException = theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID);
		SetTHStatus(hasTHException?_T("�쳣"):_T("����"));
	}
	if(!theBOM_INFO.IsPrinterUsed())
	{
		SetPrinterStatus(_T("δ��"));
	}
	else
	{
		bool hasPrinterException = theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID);
		SetPrinterStatus(hasPrinterException?_T("�쳣"):_T("����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ù���ԱID

@param      (i)CString userID   ����ԱID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetUserID(CString userID)
{
	m_userID = userID;
	InvalidateRect(m_rectUserID, TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������״̬

@param      (i)CString networkStatus   ����״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetNetWorkStatus(CString networkStatus)
{
	m_networkStatus = networkStatus;
	InvalidateRect(m_rectNetworkStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô�ӡ��״̬

@param      (i)CString printerStatus   ��ӡ��״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetPrinterStatus(CString printerStatus)
{
	m_printerStatus = printerStatus;
	InvalidateRect(m_rectPrinterStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ⲿ��д��״̬

@param      (i)CString outRWStatus   �ⲿ��д��״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetOutRWStatus(CString outRWStatus)
{
	m_outRWStatus = outRWStatus;
	InvalidateRect(m_rectOutRWStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ڲ���д��״̬

@param      (i)CString twStatus   �ڲ���д��״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetTWStatus(CString twStatus)
{
	m_TWStatus = twStatus;
	InvalidateRect(m_rectTWStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ڲ���д��״̬

@param      (i)CString twStatus   �ڲ���д��״̬

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::SetTHStatus(CString thStatus)
{
	m_THStatus = thStatus;
	InvalidateRect(m_rectTHStatus,TRUE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʱ�����

@param      UINT nIDEvent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusDlg::OnTimer(UINT nIDEvent) 
{
	_DATE_TIME curTime = ComGetCurTime();

	// ��ʼ����Ӹı�ʱ��ˢ��ʱ����ʾ
	if (m_preTime!=curTime) {
		InvalidateRect(m_rectTime, TRUE);
		m_preTime = curTime;
	}

}

