#include "stdafx.h"
#include "TimeSetSvc.h"
#include "TimeSetBaseOfficeDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTimeSetSvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::CTimeSetSvc()
	:CTVMForeService(SYSTEM_TIME_SET_SVC)
{
	// ��ʼ���Ի���
	m_pDialogFlow->AddDialog(IDD_01801_TIMESET_BASE_DLG,new CTimeSetBaseOfficeDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TIMESET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TIMESET, IDD_01801_TIMESET_BASE_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TIMESET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::~CTimeSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// ����ָ������

	EnterTime = 1;				// ���ּ���Enter�����´���
	TimeDate = _T("");			// ������ȡ�����ʱ��
	ErrorFlag = -1;			    // ʱ���Ƿ�Ƿ��ı�־

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TIMESET);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_TIMESET);
	// �����������ʾ����������
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(1 == EnterTime)			// ��һ�ΰ���Enter��ȷ�ϲ˵���ѡ��
	{
		CheckMenuNumber();
	}
	else if(2 == EnterTime)		// �ڶ��ΰ���Enter��ȷ�ϴ������õ�ѡ��
	{
		DoCheckInput();			// �����������Ƿ����������Ӧ���ʵ���ж�
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˵����Ƿ�Ƿ��ж�

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetSvc::CheckMenuNumber()
{
	// ��֤����Ĳ˵����
	m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
	if (1 != m_DataModel.MenuNumber)	// ���˵���ų�1֮���Ϊ�Ƿ��˵���
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return;
	}
	else
	{
		// ����Ƿ��ѻ�״̬
		if (!theAPP_SESSION.IsSCConnected())
		{
			theAPP_SESSION.DeleteAllInput();
			theAPP_SESSION.SetPrefixToGuide(_opl(RWTEST_TEXT_DATE_TIME));
			theAPP_SESSION.SetMaxInputLength(14);
			EnterTime = 2;
		}
		// ����״̬������ʱ������
		else
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TIMESET_TIP_MASSAGEBOX));
		}
		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʱ���Ƿ�Ƿ��ж�

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetSvc::DoCheckInput()
{
	TimeDate = theAPP_SESSION.GetInputToString();
	if(_T("") == TimeDate)
	{
		return;
	}
	else		// �������벻Ϊ�ռ��ʱ���Ƿ�Ƿ�
	{
		CString YearNumber = TimeDate.Left(4);
		CString MonthNumber = TimeDate.Mid(4,2);
		CString DayNumber = TimeDate.Mid(6,2);
		CString HourNumber = TimeDate.Mid(8,2);
		CString MinuteNumber = TimeDate.Mid(10,2);
		CString SecondNumber = TimeDate.Right(2);

		ErrorFlag = ComCheckDate(YearNumber,MonthNumber,DayNumber,HourNumber,MinuteNumber,SecondNumber);
		if(0 != ErrorFlag)
		{
			m_DataModel.TimeLegal = FALSE;
		}
		else
		{
			m_DataModel.setYear = YearNumber;
			m_DataModel.setMonth = MonthNumber;
			m_DataModel.setDay = DayNumber;
			m_DataModel.setHour = HourNumber;
			m_DataModel.setMinute = MinuteNumber;
			m_DataModel.setSecond = SecondNumber;
			CString DataStr = _T("");
			DataStr.Format(_T("%s-%s-%s %s:%s:%s"),m_DataModel.setYear,m_DataModel.setMonth,m_DataModel.setDay,m_DataModel.setHour,m_DataModel.setMinute,m_DataModel.setSecond);
			COleDateTime TimNum;
			TimNum.ParseDateTime(DataStr);
			TimNum.GetAsSystemTime(dSystemTime);
			m_DataModel.TimeLegal = TRUE;
		}
	}

	m_DataModel.TimeChange = TRUE;
	NotifyDataModelChanged();
	EnterTime = 1;						// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// �����������ʾ����������
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardF2(0,0);
		theTVM_SETTING.SetTimeChangeValue(1);
		CString DataStr = _T("");
		DataStr.Format(_T("%s%s%s%s%s%s"),m_DataModel.setYear,m_DataModel.setMonth,m_DataModel.setDay,m_DataModel.setHour,m_DataModel.setMinute,m_DataModel.setSecond);
		theTVM_SETTING.SetCurSystemTime(DataStr);
		theAPP_SESSION.ShowOfficeGuide(TIMESET_SET_SUCESSFULLY);

		//SetLocalTime(&dSystemTime);			// ���ñ���ʱ��
	
		// ʱ��������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DATETIME_SETTINGS,true);
		return TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     NumberDate

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTimeSetSvc::Model& CTimeSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTimeSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Clear] ��������Ϣ��ɾ����

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;							// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
		theAPP_SESSION.SetMaxInputLength(1);	// �����������λ��
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CTimeSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}