#include "stdafx.h"
#include "RemoveExceptionDlg.h"
#include "RemoveExceptionSvc.h"
#include "ServiceMgr.h"
#include "ExceptionMgr.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const char* const TXT_BTN_PAGE_UP		= "��һҳ";
static const char* const TXT_BTN_PAGE_DOWN		= "��һҳ";
static const char* const WM_BTN_PAGE_UP			= "PAGEUP";
static const char* const WM_BTN_PAGE_DOWN		= "PAGEDOWN";

static const char* const TITLE_EXCEPTION_INFO   = "�쳣��Ϣ";

IMPLEMENT_DYNAMIC(CRemoveExceptionDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRemoveExceptionDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_MESSAGE(WM_XBUTTON,XButtonClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionDlg::CRemoveExceptionDlg(CService* pService)
: COperationDlg(CRemoveExceptionDlg::IDD,pService)
{
	m_ExceptionNum = 0;
	m_TotalPageNum = 0;
	m_CurrentPageNum = 0;

	m_baseInfo->titleInfo.titleName = TITLE_EXCEPTION_INFO;				// ����
	m_baseInfo->detailInfo.row = 11;                                    // ����

	UIINFO pageUpButtonUI;
	pageUpButtonUI.m_pParentWnd = this;
	pageUpButtonUI.m_Location.y = BTN_OK_RECT.top;
	pageUpButtonUI.m_Location.x = 10;
	pageUpButtonUI.m_Size = CSize(80,50);
	BTN_INFO pageUpButtonInfo;
	pageUpButtonInfo.btnMsg = WM_BTN_PAGE_UP;
	pageUpButtonInfo.isEnable = true;
	pageUpButtonInfo.btnID = 0;
	pageUpButtonInfo.text = TXT_BTN_PAGE_UP;
	m_btnPageUp = new CXButton(pageUpButtonUI,pageUpButtonInfo);

	UIINFO pageDownButtonUI;
	pageDownButtonUI.m_pParentWnd = this;
	pageDownButtonUI.m_Location.y = BTN_OK_RECT.top;
	pageDownButtonUI.m_Location.x = 110;
	pageDownButtonUI.m_Size = CSize(80,50);
	BTN_INFO pageDownButtonInfo;
	pageDownButtonInfo.btnMsg = WM_BTN_PAGE_DOWN;
	pageDownButtonInfo.isEnable = true;
	pageDownButtonInfo.btnID = 0;
	pageDownButtonInfo.text = TXT_BTN_PAGE_DOWN;
	m_btnPageDown = new CXButton(pageDownButtonUI,pageDownButtonInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionDlg::~CRemoveExceptionDlg()
{
	delete m_btnPageUp;
	m_btnPageUp = NULL;

	delete m_btnPageDown;
	m_btnPageDown = NULL;
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
int CRemoveExceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK) == -1){
			return -1;
		}
		m_btnPageUp->Create();
		m_btnPageDown->Create();

		delete m_baseInfoArea;
		m_baseInfoArea = NULL;
		m_baseInfoArea = new CGStatic(this,m_baseInfo,m_LabelExceptionInfo);

		InitExceptionInfo();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ���쳣��Ϣ�б�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionDlg::InitExceptionInfo()
{
	m_LabelExceptionInfo[0].name1 = "����ʱ��";
	m_LabelExceptionInfo[0].name2 = "����";
	m_LabelExceptionInfo[0].name3 = "            MTC";
	m_LabelExceptionInfo[0].name4 = "      EC";

	m_ExceptionList = theEXCEPTION_MGR.GetAllException();
	m_ExceptionNum = m_ExceptionList.size();
	if(m_ExceptionNum == 0){
		for(int i = 1;i < m_baseInfo->detailInfo.row;i++)
		{
			m_LabelExceptionInfo[i].name1 = "";
			m_LabelExceptionInfo[i].name2 = "";
			m_LabelExceptionInfo[i].name3 = "";
			m_LabelExceptionInfo[i].name4 = "";
		}
		m_btnPageUp->SetVisible(false);
		m_btnPageDown->SetVisible(false);
		m_btnOK->SetVisible(false);
	}
	else{
		m_TotalPageNum = (m_ExceptionNum%(m_baseInfo->detailInfo.row - 1) == 0) ? m_ExceptionNum/(m_baseInfo->detailInfo.row - 1) : m_ExceptionNum/(m_baseInfo->detailInfo.row - 1) + 1;
		m_CurrentPageNum = 1;
		RefreshExceptionInfo();
		if(m_TotalPageNum == 1){
			m_btnPageUp->SetVisible(false);
			m_btnPageDown->SetVisible(false);
		}
		else{
			m_btnPageUp->SetVisible(true);
			m_btnPageDown->SetVisible(true);
			m_btnPageUp->SetEnabled(false);
			m_btnPageDown->SetEnabled(true);
		}
		m_btnOK->SetVisible(true);
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ˢ���쳣��Ϣ�б�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveExceptionDlg::RefreshExceptionInfo()
{
	int indexStar = (m_CurrentPageNum - 1)*(m_baseInfo->detailInfo.row - 1);
	for(int i = 1;i < m_baseInfo->detailInfo.row;i++)
	{
		if(indexStar < m_ExceptionNum){
			CSysException &e = m_ExceptionList[indexStar];
			COleDateTime expTime = e.GetOccurTime();
			CString strExpTime;
			strExpTime.Format(_T("%.4d/%.2d/%.2d %.2d:%.2d"),expTime.GetYear(),expTime.GetMonth(),expTime.GetDay(),expTime.GetHour(),expTime.GetMinute(),expTime.GetSecond());
			m_LabelExceptionInfo[i].name1 = strExpTime;
			m_LabelExceptionInfo[i].name2 = theEXCEPTION_MGR.GetDescription(e);
			CString strMTC;
			strMTC.Format(_T("            %.8X"),theEXCEPTION_MGR.GetActualMTC(e));
			m_LabelExceptionInfo[i].name3 = strMTC;
			CString strEc;
			strEc.Format(_T("      %.4X"),theEXCEPTION_MGR.GetEC(e));
			m_LabelExceptionInfo[i].name4 = strEc;
		}
		else{
			m_LabelExceptionInfo[i].name1 = _T("");
			m_LabelExceptionInfo[i].name2 = _T("");
			m_LabelExceptionInfo[i].name3 = _T("");
			m_LabelExceptionInfo[i].name4 = _T("");
		}
		indexStar++;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRemoveExceptionDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try {
		theSERVICE_MGR.SetForeServiceBusy(true);
		// �쳣���
		theEXCEPTION_MGR.RepareException();
		// ˢ���쳣�б�
		InitExceptionInfo();
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_OUT_DEVICE_SELECT));
	}
	catch (...) {
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_OUT_DEVICE_SELECT)); 
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**8
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRemoveExceptionDlg::XButtonClick(WPARAM wParam, LPARAM lParam)
{
	try {
		CString sMsg = (LPCTSTR)lParam;
		if(sMsg == WM_BTN_PAGE_UP){
			m_CurrentPageNum--;
			RefreshExceptionInfo();
			if(m_CurrentPageNum == 1){
				m_btnPageUp->SetEnabled(false);
				m_btnPageDown->SetEnabled(true);
			}
			else{
				m_btnPageUp->SetEnabled(true);
				m_btnPageDown->SetEnabled(true);
			}
		}
		else if(sMsg == WM_BTN_PAGE_DOWN){
			m_CurrentPageNum++;
			RefreshExceptionInfo();
			if(m_CurrentPageNum == m_TotalPageNum){
				m_btnPageUp->SetEnabled(true);
				m_btnPageDown->SetEnabled(false);
			}
			else{
				m_btnPageUp->SetEnabled(true);
				m_btnPageDown->SetEnabled(true);
			}
		}
		m_btnPageUp->SetSelect(false);
		m_btnPageDown->SetSelect(false);
		m_baseInfoArea->Invalidate();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}