#pragma once
#include "TVMForeService.h"

/**
@brief Ӳ���Լ�Service
*/

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")

class CCoinModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCoinModuleTestSvc();
	virtual ~CCoinModuleTestSvc();

	typedef enum _dialog_group
	{
		DIALOG_GROUP_MENU = 1,    //�˵�����
		DIALOG_GROUP_OPERATION,  //��������
	}DIALOG_GROUP;

	//�˵����
	enum MenuNum
	{
		MenuNum1 = 1,
		MenuNum2,
		MenuNum3,
		MenuNum4,
		MenuNum5,
		MenuNum6,
		MenuNum7,
		MenuNum8,
		MenuNum9,
		MenuNum10,
		MenuNum11,
		MenuNum12,
		MenuNum13,
		MenuNum14,

		MenuNum21 = 21,
		MenuNum22,
		MenuNum31 = 31,
		MenuNum32,
		MenuNum41 = 41,
		MenuNum42,
	};
	//��������ģ��
	enum OPERATION_TYPE
	{
		OPERATION_TYPE_RETURN_COIN = 0 ,  //�˱Ҳ���
		OPERATION_TYPE_CLEAR_COIN,        //��Ҳ���
		//OPERATION_TYPE_SAFELOCK_CTRL,     //��ȫ��
		//OPERATION_TYPE_READ_ID,           //��RFID
		//OPERATION_TYPE_WRITE_ID,          //дRFID
		//OPERATION_TYPE_ADDING_COIN,       //���Ҳ���
		OPERATION_TYPE_UNKNOW,            //δ֪
	};

	//�����-�˵���Ϣ����
	typedef struct _tag_menu_info
	{
		CString strMenuStatus;   //�ɹ���ʧ�ܡ���ɵ�
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	//����Ӳ�Ҳ˵�ѡ�����
	typedef struct _tag_return_coin_menu_info
	{
		//bool bMenu_A1;   //Coin Cycle Change Box
		//bool bMenu_A2;   //Coin Collection Box1
		//bool bMenu_A3;   //Passenger
		//bool bMenu_A4;   //Coin Collection Box2

		//bool bMenu_B1;   //To Passenger
		//bool bMenu_B2;   //To Coin Collection Box2
		CString str_cancel;
		CString str_accept;
		_tag_return_coin_menu_info()
		{
			str_cancel = UNKNOW;
			str_accept = UNKNOW;
			/*bMenu_A1 = false;
			bMenu_A2 = false;
			bMenu_A3 = false;
			bMenu_A4 = false;

			bMenu_B1 = false;
			bMenu_B2 = false;*/
		}

	}RETURN_COIN_MENU_INFO;

	//���Ӳ�Ҳ���
	typedef struct _tag_clear_coin_menu_info
	{
		WORD iClsMenu_A1;
		WORD iClsMenu_A2;
		bool bClsMenu_A1;
		bool bClsMenu_A2;

		WORD iClsMenu_B1;
		WORD iClsMenu_B2;
		bool bClsMenu_B1;
		bool bClsMenu_B2;

		bool bClsMenu_A1_All ;
		bool bClsMenu_A2_All ;
		bool bClsMenu_B1_All ;
		bool bClsMenu_B2_All ;
		_tag_clear_coin_menu_info()
		{
			iClsMenu_A1 = 0;
			iClsMenu_A2 = 0;
			iClsMenu_B1 = 0;
			iClsMenu_B2 = 0;
			bClsMenu_A1 = false;
			bClsMenu_A2 = false;
			bClsMenu_B1 = false;
			bClsMenu_B2 = false;
			bClsMenu_A1_All = false;
			bClsMenu_A2_All = false;
			bClsMenu_B1_All = false;
			bClsMenu_B2_All = false;
		}
	}CLEAR_COIN_MENU_INFO;
	enum CurInputOP
	{
		NULL_OP = 0,
		MenuA1,
		MenuA2,
		MenuB1,
		MenuB2,
	};


	//�ӱҰ�ȫ�۲˵�ѡ��״̬
	typedef struct _tag_safelock_ctrl_menu_info
	{
		bool bSfMenu_A1;
		bool bSfMenu_A2;

		bool bSfMenu_B1;
		bool bSfMenu_B2;
		_tag_safelock_ctrl_menu_info()
		{
			bSfMenu_A1 = false;
			bSfMenu_A2 = false;

			bSfMenu_B1 = false;
			bSfMenu_B2 = false;
		}
	}SAFELOCK_CTRL_MENU_INFO;

	//��дǮ��RFID����
	typedef struct _tag_read_write_coin_box_id
	{
		CString strCAddingBoxA;     //�ӱ���1
		CString strCAddingBoxB;     //�ӱ���2
		CString strCCollectionBoxA; //������1
		CString strCCollectionBoxB; //������2

		_tag_read_write_coin_box_id()
		{
			strCAddingBoxA = UNKNOW;
			strCAddingBoxB = UNKNOW;
			strCCollectionBoxA = UNKNOW;
			strCCollectionBoxB = UNKNOW;
		}
	}READ_WRITE_COIN_BOX_ID;

	typedef struct _tag_data_modol
	{
		OPERATION_TYPE operationType;
		MENU_INFO menuInfo[14];
		RETURN_COIN_MENU_INFO returnCoinMenuInfo;
		CLEAR_COIN_MENU_INFO clearCoinMenuInfo;
		SAFELOCK_CTRL_MENU_INFO safeLockCtrlMenuInfo;
		READ_WRITE_COIN_BOX_ID rwCoinBoxId;

		_tag_data_modol()
		{
			// ��ʼ���������
			operationType = OPERATION_TYPE_UNKNOW;
			/*operationType = OPERATION_TYPE();
			returnCoinMenuInfo = RETURN_COIN_MENU_INFO();
			clearCoinMenuInfo = CLEAR_COIN_MENU_INFO();
			rwCoinBoxId = READ_WRITE_COIN_BOX_ID();
			safeLockCtrlMenuInfo = SAFELOCK_CTRL_MENU_INFO();
			for(int i = 0; i<14; i++){
			menuInfo[i] = MENU_INFO();
			}*/
		}

	}Modol;

	Modol& GetDataModol();            //����service������ģ��
	BYTE GetHopperType(int hopetype); // ��ȡӲ����Ӳ������ 
protected:
	//void ReflashUI();                 //ˢ�½���
	void NotifyDataModelChanged();    //���ݸı��֪ͨ�����������
	void DoMenuOperation();           //���˵�����ҵ��
	void DoOperation();               //������ҵ��

	bool ResetTest();                 //���ò���
	bool OpenCoinShutterTest();       //��Ӳ�ҿڲ���
	bool CloseCoinShutterTest();      //�ر�Ӳ�ҿڲ���
	bool StartCoinRecvTest();         //��ʼ����Ӳ�Ҳ���
	bool StopCoinRecvTest();          //ֹͣ����Ӳ�Ҳ���
	bool AcceptCoinTest();            //CH����Ӳ��
	bool ReleaseCoinTest();           //CHӲ������
	void ReadAmountTest();            //�����
	void ReturnCoinTest();            //�˱Ҳ���
	void ClearCoinTest();             //��Ҳ���
	void CoinAddingTest();            //���Ҳ���
	
	//֣����Ŀ��Щ�޵ײ�ӿ�
	//bool StopCoinAddingTest();      //�����ӱҲ���
	//void ContorlSafeLockTest();       //��ȫ�ۿ��Ʋ���
	//void ReadCoinBoxIdTest();         //��RFID����
	//void WriteCoinBoxIdTest();        //дRFID����
	//void ReadVersionTest();           //���汾����

	void ClearModolData();              //���ز˵�����ʱ���Modol����
private:
	bool bIsCoinShutterOpen;            //Ӳ�ҿ��Ƿ��
	bool bIsCoinRecvStart;              //�Ƿ�ʼ����Ӳ��
	//bool bIsCoinAddingStart;           
private:	
	Modol m_Modol;
	CurInputOP type_op;
	//��Ϣ����
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //���� [Enter] ��������Ϣ��ִ�У�
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //���� [F3/Ctrl]    ��������Ϣ�����ˣ�
	LRESULT OnKeyboardF2(WPARAM , LPARAM );                 //���� [F2/Alt]    ��������Ϣ�����棩
	void OnStart();											//ҵ��ʼʱ��ʼ������
};
