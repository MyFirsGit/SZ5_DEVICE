#pragma once
#include "TVMForeService.h"
#include "TimeSetBaseOfficeDlg.h"
/**
@brief    ʱ���趨
*/
class CTimeSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTimeSetSvc();
	~CTimeSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_TIMESET = 1,                       
	} DIALOG_GROUP;

	typedef struct _tagModel
	{
		int MenuNumber;			// �˵���
		BOOL MenuChange;		// �˵��仯��־
		BOOL TimeChange;		// ����仯��־
		BOOL TimeLegal;			// ���������Ƿ�Ƿ���־
		int TimeNumber;			// �����
		CString setYear;
		CString setMonth;
		CString setDay;
		CString setHour;
		CString setMinute;
		CString setSecond;
		_tagModel()
		{
			MenuNumber = 0;
			MenuChange = FALSE;
			TimeChange = FALSE;
			TimeLegal = TRUE;
			setYear = _T("");
			setMonth = _T("");
			setDay = _T("");
			setHour = _T("");
			setMinute = _T("");
			setSecond = _T("");
		}

	}Model;

	Model& GetDataModel();
	void CheckMenuNumber();														// �˵����Ƿ�Ƿ��ж� 
	void DoCheckInput();														// �����������Ƿ����������Ӧ���ʵ���ж�
	void CheckInputSave();														// ʱ�䱣��
	virtual void NotifyDataModelChanged();	
protected:
	void OnStart();																// Service��������	
private:
	int EnterTime;																// keyboard/Enter�����´���
	SYSTEMTIME dSystemTime;
	CString TimeDate;															// ������ȡ�����ʱ��
	int ErrorFlag;																// ʱ���Ƿ�Ƿ��ı�־
	Model m_DataModel;
	
	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Clear] ��������Ϣ��ɾ����
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
};