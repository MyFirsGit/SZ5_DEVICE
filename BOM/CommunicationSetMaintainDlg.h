#pragma once
#include "OperationDlg.h"

/**
@brief      ͨ�Ų��������û���
*/
class CCommunicationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCommunicationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationSetMaintainDlg(CService* pService);
	~CCommunicationSetMaintainDlg();

	enum { IDD = IDD_20114_COMMUNICATION_SET_DLG };

private:
	typedef enum _communicationinfo_edit_group 
	{
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1  =  0,
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2 = 1,
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3 = 2,
		COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4 = 3,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1 = 4,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2 = 5,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3 = 6,
		COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4 = 7,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART1 = 8,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART2 = 9,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART3 = 10,
		COMMUNICATION_EDIT_GROUP_MyGateWay_PART4 = 11,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1 = 12,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2 = 13,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3 = 14,
		COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4 = 15,
		COMMUNICATION_EDIT_GROUP_CPSPort = 16,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1 = 17,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2 = 18,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3 = 19,
		COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4 = 20,
		COMMUNICATION_EDIT_GROUP_NTPPort = 21,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1 = 22,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2 = 23,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3 = 24,
		COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4 = 25,
		COMMUNICATION_EDIT_GROUP_FTPPort = 26,
	} COMMUNICATION_EDIT_GROUP;

	LABEL_GROUP m_labelCommunicationinfo[LABEL_GROUP_EDIT_ROWS_9];					// ������ʾ��ͨ�Ų�����Ϣ

	void InitlabelCommunicationInfo();												// ��ʼ��BASE����LABEL���������
	void InitEditText();															// ��ʼ��BASE����EDIT�����������
	void SetCommunicateParameters();												// ��EDIT���������ݵı��浽SVC����
	bool CheckEditText();															// ���EDIT����Ķ˿������Ƿ�Ϸ�
	void HideGUIModule(ULONG ulCompomentFlag);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);							// ���洴������							
	afx_msg LRESULT OnOK(WPARAM,LPARAM);											// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnReset(WPARAM,LPARAM);											// �������ð�ť��Ϣ
};
