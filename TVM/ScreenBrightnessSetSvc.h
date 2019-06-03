#pragma once
#include "TVMForeService.h"
#include "ScreenBrightnessSetMaintainDlg.h"
/**
@brief    ʱ���趨
*/
class CScreenBrightnessSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CScreenBrightnessSetSvc();
	~CScreenBrightnessSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_SCREENBRIGHTNESS_SET = 1,                       
	} DIALOG_GROUP;

	typedef struct _tagModel
	{
		int BrightnessNumber;		// ����
		_tagModel()
		{
			BrightnessNumber = 5;
		}

	}Model;

	Model& GetDataModel();
	void CheckMenuNumber();														// �˵����Ƿ�Ƿ��ж� 
	void DoCheckInput();														// �����������Ƿ����������Ӧ���ʵ���ж�
	void CheckInputSave();														// ʱ�䱣��
	virtual void NotifyDataModelChanged();	
private:
	Model m_DataModel;

	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
	LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
};