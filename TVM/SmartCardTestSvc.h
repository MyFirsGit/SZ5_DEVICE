#pragma once
#include "tvmforeservice.h"

#define  _MAX_ROW  15     //�������
#define  _MAX_COL   5     //�������

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")
/**
@brief      Smart-Card����service
*/

class CSmardCardTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CSmardCardTestSvc();
	~CSmardCardTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_TH_TEST = 1,
		DIALOG_GROUP_MOTOR_TEST,
		DIALOG_GROUP_READ_RFID_TEST,
		//DIALOG_GROUP_WRITE_RFID_TEST,
		DIALOG_GROUP_SENSOR_TEST,
	} DIALOG_GROUP;


	//�˵�ѡ��
	enum MENU_CODE
	{
		MENU_CODE_1 = 1,
		MENU_CODE_21 = 21,
		MENU_CODE_22,
		MENU_CODE_3 = 3,
		MENU_CODE_41 = 41,
		MENU_CODE_42,
		MENU_CODE_43,
		MENU_CODE_44,
		MENU_CODE_5=5,
		//MENU_CODE_6,
		MENU_CODE_61=61,
		MENU_CODE_62,
		MENU_CODE_63,
		MENU_CODE_64,
		MENU_CODE_7=7,
		MENU_CODE_8,
		MENU_CODE_9,
		MENU_CODE_10,
		MENU_CODE_11,
	};
	//��������
	enum OPERATION_TYPE
	{
		OPERATION_TYPE_RETSET = 0 ,			//����
		OPERATION_TYPE_CATCH_CARD,			//ץȡƱ��  [1]A   [2]B
		OPERATION_TYPE_ISSUE_CARD,			//�ų�Ʊ��
		OPERATION_TYPE_MOVE_BOX,			//����Ʊ�����[1]A   [2]B
		OPERATION_TYPE_READ_RFID,			//��Ʊ��RFID
		//OPERATION_TYPE_WRITE_RFID,			//дƱ��RFID
		OPERATION_TYPE_ISSUE_TEST,			//��Ʊ��������[1]A   [2]B
		OPERATION_TYPE_MOVE_MOTOR_TEST,		//Ʊ��������[1]A   [2]B
		OPERATION_TYPE_MOTOR_TEST,			//���ֵ������    ����תҳ�桿
		OPERATION_TYPE_SENSOR_TEST,			//����������
		OPERATION_TYPE_ELECTROMAGNET_TEST,	//���������
		OPERATION_TYPE_CYCLE_TEMP_TICKS,	//������Ʊ������
		OPERATION_TYPE_CYCLE_TEST,			//Ʊ�����ղ���
		OPERATION_TYPE_UNKNOW,				//δ֪
	};

	//�˵�����
	typedef struct _tag_menu_info 
	{
		CString strMenuStatus;
		CString motorStatus;
		CString read_rfidStatus;
		CString write_rfidStatus;
		CString writeID;
		CString writeCount;
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
			motorStatus = UNKNOW;
			read_rfidStatus = UNKNOW;
			write_rfidStatus = UNKNOW;
			writeID = UNKNOW;
			writeCount = UNKNOW;
		}
	}MENU_INFO;

	enum CurInputOP
	{
		NULL_OP = 0,
		MenuA,
		MenuB,
		MenuC,
		MenuAB,
		NumA,
		NumB,
		NumC,
	};

	typedef struct _tag_data_modol
	{
		//BYTE INFS6Covered;          // 1:6 ��INFS �ڵ�
		//BYTE INFS5Covered;          // 1:5 ��INFS �ڵ�
		//BYTE INFS4Covered;          // 1:4 ��INFS �ڵ�
		//BYTE INFS3Covered;          // 1:3 ��INFS �ڵ�
		//BYTE INFS2Covered;          // 1:2 ��INFS �ڵ�
		//BYTE INFS1Covered;          // 1:1 ��INFS �ڵ�
		TH_RSP_GET_DEV_STATUS cardSenserStatus;
		bool bCardSenserTestSuccess;
		MENU_INFO menuInfo[_MAX_ROW];

		CString strPreBoxID;
		CString strPreInvalidID;

		_tag_data_modol(){
			strPreBoxID.Format(_T("%02d01"),theMAINTENANCE_INFO.GetLineCode());
			strPreInvalidID.Format(_T("%02d02"),theMAINTENANCE_INFO.GetLineCode());
			cardSenserStatus = _th_rsp_get_dev_status();
			bCardSenserTestSuccess = false;
		}
	}Modol;

	Modol& GetDataModol();            //����service������ģ��	
	void NotifyDataModelChanged();    //���ݸı��֪ͨ�����������

private:
	Modol m_Modol;                          //����ģ��
	OPERATION_TYPE  m_OperationType;        //��������
	CurInputOP type_op;
	//WORD       m_wSmartCardFirstCommond;  //�������ͱ��
	BYTE       m_wSmartCardSecondCommand;   //��ϸ��������
	BYTE		m_bActionType;				// Ʊ�������½���������
	bool		m_bIsCardSendOut;			// Ʊ�������Ƿ��ͳ���
	bool		m_bIsCardCatch;				// �Ƿ��Ѿ�ץ��Ʊ����
	//TEST_SPEED m_eTransferSpeedSet;       //���Ͳ����ٶ�
	//BYTE       m_bIssueData[16];          //��Ʊʱ������TH��16λBYTEBYTE
	BOOL       isMotorStart;
	BYTE       testType;             //����Ĳ�������
	void       DoOperation();        //���ݲ������ͽ��в���
	void       DoMotorTestOperation();  //���ݲ������ͽ��в���
	void       DoReadRFIDTestOperation();  //���ݲ������ͽ��в���
	void       DoWriteRFIDTestOperation();  //���ݲ������ͽ��в���
	void       DoSensorTestOperation();  //���ݲ������ͽ��в���
	void       ReadRFID();      //��Ʊ��RFID
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Esc] ��������Ϣ�����˵���
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //���� [Enter] ��������Ϣ��ִ�У�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					//���� [F3/Ctrl]    ��������Ϣ�����أ�
	void OnStart();
	tTIMRFIDInfo m_rfidInfo;
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
};
