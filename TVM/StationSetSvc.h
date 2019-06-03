#pragma once
#include "TVMForeService.h"
#include "StationSetMaintainDlg.h"
#define MAX_SMMENU_NUM 5		// ��վ���ý���˵��������
#define MAX_LineCode_NUM 255	// ��·����������

/**
@brief    ����ҵ�� - ��վ����ҵ��
*/
class CStationSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CStationSetSvc();
	~CStationSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_STATION_SET	= 1,                       
	} DIALOG_GROUP;

	// SVC����ģ��
	typedef struct _StationModel
	{
		int MenuNumber;			// �˵���
		BOOL MenuChange;		// �˵��仯��־(�˵�����1�䵽2�����ֱ仯��MenuChangeΪTRUE)
		BOOL CodeChange;		// ����仯��־(��·����ķ����˸ı䣬��CodeChangeΪTRUE)
		BOOL CodeLegal;			// ���������Ƿ�Ƿ���־
		CString sLineCode;
		CString sStationCode;
		CString sMachineCode;
		CString sGroupNumber;
		CString sEquNumberInGroup;
		_StationModel(){
			MenuNumber = 0;
			MenuChange = FALSE;
			CodeChange = FALSE;
			CodeLegal = TRUE;
			sLineCode = _T("");
			sStationCode = _T("");
			sMachineCode = _T("");
			sGroupNumber = _T("");
			sEquNumberInGroup = _T("");
		}

	}Model;

	typedef struct _StationWrite
	{
		int LineIndex;
		int StationIndex;
		int MachineIndex;
		int GroupIndex;
		int EquNumberIndex;
		_StationWrite(){
			LineIndex = 0;
			StationIndex = 0;
			MachineIndex = 0;
			GroupIndex = 0;
			EquNumberIndex = 0;
		}
	}NumberIndex;

	Model& GetDataModel();
	void CheckMenuNumber();														// �˵����Ƿ�Ƿ��ж�
	void DoCheckInput();														// �����������Ƿ����������Ӧ���ʵ���ж�
	void ReadIniText();															// ��ȡMaintenance.INI�����ļ�
	void InputGuideMsg();														// ��GuideMsg�ֱ�ֵ

	virtual void NotifyDataModelChanged();	
protected:
	void OnStart();																// Service��������
private:
	int EnterTime;																// keyboard/Enter�����´���
	CString GuideMsg;															// ������ʾ
	Model m_DataModel;
	NumberIndex n_NubIndex;

	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Clear] ��������Ϣ��ɾ����
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
};



//====================================================================================================================
/* ��д˼·����ѡ��˵���������Ӧ���ã���ҪEnter�����������Σ���Enter��������Ϣ��Ӧ�������¼Enter�������´�������
   CStationSetSvc��Ա����int EnterTime��ֵ���ڵ�һ�ΰ���Enter�����ﴦ������˵�����Ƿ�����ڵڶ��ΰ���Enter����
   �ﴦ���������˵����������Ĳ˵�����趨ֵ�Ƿ����															*/
//====================================================================================================================