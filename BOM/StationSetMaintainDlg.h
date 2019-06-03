#pragma once
#include "OperationDlg.h"

/**
@brief      ��վ��Ϣ���뻭��
*/
class CStationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CStationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStationSetMaintainDlg(CService* pService);                             // ��׼���캯��
	~CStationSetMaintainDlg();
	enum { IDD = IDD_20112_STATION_SET_DLG };                               // �Ի�������

private:
	typedef enum _stationinfo_edit_group {                                  // ��վ����EDIT��Ӧ��EDITGROUP
		STATIONINFO_EDIT_GROUP_LINECODE = 0,
		STATIONINFO_EDIT_GROUP_STATIONCODE  =  1,
		STATIONINFO_EDIT_GROUP_MACHINECODE  =  2,
		STATIONINFO_EDIT_GROUP_GROUPNUMBER  =  3,
		STATIONINFO_EDIT_GROUP_EQUNOINGROUP =  4
	} STATIONINFO_EDIT_GROUP;

	LABEL_GROUP m_labelStationinfo[LABEL_GROUP_EDIT_ROWS_5];                // ������ʾ�ĳ�վ��Ϣ

	void InitlabelStationInfo();											// ��ʼ��BASE����LABEL���������
	void InitEditText();													// ��ʼ��BASE����EDIT�����������
	void SetStationInfo();													// ���ó�վ��Ϣ
	int CheckInputText();													// ���EDIT����������Ƿ�Ϸ�
	void HideGUIModule(ULONG ulCompomentFlag);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnReset(WPARAM,LPARAM);									// �������ð�ť��Ϣ
};
