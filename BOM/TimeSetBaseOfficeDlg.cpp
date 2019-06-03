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

static const TCHAR* const STR_TIMESET_NOTEMSG	= _T("ʱ���趨");
static const TCHAR* const TXT_TIME_YEAR			= _T("��ݣ�");
static const TCHAR* const TXT_TIME_MONTH		= _T("�·ݣ�");
static const TCHAR* const TXT_TIME_DATE		    = _T("���ڣ�");
static const TCHAR* const TXT_TIME_HOUR		    = _T("ʱ����");
static const TCHAR* const TXT_TIME_MINUTE		= _T("������");

// �������λ�ü���С
static CRect editTime[] = {
	BASE_INFO_RECT(1,4),  // ��
	BASE_INFO_RECT(2,4),  // ��
	BASE_INFO_RECT(3,4),  // ��
	BASE_INFO_RECT(4,4),  // ʱ
	BASE_INFO_RECT(5,4)   // ��
};

IMPLEMENT_DYNAMIC(CTimeSetBaseOfficeDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CTimeSetBaseOfficeDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::CTimeSetBaseOfficeDlg(CService* pService)
:COperationDlg(CTimeSetBaseOfficeDlg::IDD, pService)
{
	// ʱ���趨��Ϣ
	m_baseInfo->titleInfo.titleName = STR_TIMESET_NOTEMSG;      // ����
	m_baseInfo->detailInfo.row = 5;								// ����
	m_baseInfo->detailInfo.labelGroup = timeSetInfo;            // ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 5;          // �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editTime;       // �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetBaseOfficeDlg::~CTimeSetBaseOfficeDlg()
{
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
int CTimeSetBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		// ����������Ϣ���򡢷��ء�ȷ����ť�����ּ���
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA  | BTN_OK | BTN_RESET)){
			return -1;
		}
		// ���öԻ�����������
		GetEdit(0)->SetLimitText(4);
		GetEdit(1)->SetLimitText(2);
		GetEdit(2)->SetLimitText(2);
		GetEdit(3)->SetLimitText(2);
		GetEdit(4)->SetLimitText(2);
		// ��ʼ��BASE����LABEL���������
		InitlabelTimeInfo();
		// ȡ��ϵͳʱ��,�����ʼʱ��
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
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
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
@brief      �������ð�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CTimeSetSvc* pTimeSetSvc = (CTimeSetSvc*)GetService();
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// �ж��Ƿ��ѻ�״̬
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
			CMessageBox::Show(_T("��ʾ"),_T("����״̬����������ֶ�ʱ������"),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
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
@brief      ��������Ƿ���ȷ

@param      none

@retval     bool		\n
TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTimeSetBaseOfficeDlg::CheckInput()
{
	// ���������ʱ��
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
@brief      ���ؽ������

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetBaseOfficeDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// ���ñ�ǩ����
	timeSetInfo[0].nameRight = add_hsp(GetEditText(0) + _T("  ") + YEAR_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[1].nameRight = add_hsp(GetEditText(1) + _T("    ") + MONTH_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[2].nameRight = add_hsp(GetEditText(2) + _T("    ") + DAY_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[3].nameRight = add_hsp(GetEditText(3) + _T("    ") + HOUR_UNIT,MAX_SBC_CASE_LEN);
	timeSetInfo[4].nameRight = add_hsp(GetEditText(4) + _T("    ") + MINUTE_UNIT,MAX_SBC_CASE_LEN);
	// �������
	HideCompoment(ulCompomentFlag);
	// ˢ�»�����Ϣ��
	m_baseInfoArea->Invalidate();
}
