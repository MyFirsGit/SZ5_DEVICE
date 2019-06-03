#pragma once
#include "TVMForeService.h"

/**
@brief Ӳ���Լ�Service
*/

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")

class CTokenModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CTokenModuleTestSvc();
	virtual ~CTokenModuleTestSvc();

	typedef enum _dialog_group
	{
		DIALOG_GROUP_MENU = 1,    //�˵�����
		DIALOG_GROUP_OPERATION,  //��������
	}DIALOG_GROUP;

	enum _menu_num
	{
		Menu_Code_1  = 1,

		//Menu_Code_7  =7,
		//Menu_Code_8,
		//Menu_Code_9,

		Menu_Code_21 = 21,
		Menu_Code_22,

		Menu_Code_3 = 3,

		Menu_Code_4 = 4,
		//Menu_Code_41 = 41,
		//Menu_Code_42,

		Menu_Code_51 = 51,
		Menu_Code_52,

		//Menu_Code_61 = 61,
		//Menu_Code_62,

	};
	//��������ģ��
	enum OPERATION_TYPE
	{
		OPERATION_TYPE_READ_ID = 0,       //��RFID
		OPERATION_TYPE_WRITE_ID,          //дRFID
		OPERATION_TYPE_UNKNOW,            //δ֪����
	};
	
	//�����
	typedef struct _tag_menu_info
	{
		CString strMenuStatus;   //�ɹ���ʧ�ܡ���ɵ�
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	//RFID��Ϣ
	typedef struct _tag_box_id
	{
		CString strRFIDBox1;
		CString strRFIDBox2;
		_tag_box_id()
		{
			strRFIDBox1 = UNKNOW;
			strRFIDBox2 = UNKNOW;
		}

	}TOKEN_BOX_ID;

	//Token����ģ��
	typedef struct _tag_data_modol
	{
		MENU_INFO       MenuInfo[9];
		TOKEN_BOX_ID    box_id;
		OPERATION_TYPE  operationType;

		_tag_data_modol()
		{
			operationType = OPERATION_TYPE_UNKNOW; 
			box_id = TOKEN_BOX_ID();
			for(int i=0;i<9;i++){
				MenuInfo[i] = MENU_INFO();
			}
		}
	}Modol;

	Modol& GetDataModol();            //����service������ģ��
	void NotifyDataModelChanged();    //���ݸı��֪ͨ�����������

protected:
	
	//����ѡ��
	void DoMenuOperation();             //���˵�����ҵ��
	void DoOperation();                 //������ҵ��
	//����
	bool ResetTest();                        //���ò���	
	bool CatchTokenTest(bool bIsLeft);       //ץȡToken����
	bool PutoutTokenTest();					 //�ų�Token����
	bool RecycleTokenBoxTest(bool bIsLeft);  //����Token����
	bool ClearTokenBoxTest(bool bIsLeft);    //���Token����
	bool DetachTokenBoxTest(bool bIsLeft);   //ж��Token�����
	void ReadTokenBoxIdTest();               //��RFID����
	void WriteTokenBoxIdTest();              //дRFID����
	void ReadVersionTest();                  //���汾����
	
	void ClearModolData();                  //���ز˵�����ʱ���Modol����
private:	
	Modol m_Modol;
	bool m_bIsCatchLeftToken;          //�Ƿ�ץ����ߵ�Token
	bool m_bIsCatchRightToken;         //�Ƿ�ץ���ұߵ�Token
	//��Ϣ����
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //���� [Enter] ��������Ϣ��ִ�У�
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //���� [F3]    ��������Ϣ�����ˣ�
	LRESULT OnKeyboardF2(WPARAM , LPARAM );                 //���� [F2]    ��������Ϣ�����棩
	void OnStart();
};
