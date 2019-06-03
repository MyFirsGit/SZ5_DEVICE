#pragma once
#include "tvmforeservice.h"

#define  _MAX_ROW  15     //�������
#define  _MAX_COL   2     //�������

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")
/**
@brief      IO����service
*/

class CIOModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()
public:
	CIOModuleTestSvc();
	~CIOModuleTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_IO_TEST = 1,  
		DIALOG_GROUP_IO_STATUS,
	} DIALOG_GROUP;

	//��������
	typedef enum _tag_operation_type
	{
		//OPERATION_TYPE_STATUS = 0,          //IO״̬
		//OPERATION_TYPE_OPEN_ALARM,			//�򿪷�������
		//OPERATION_TYPE_BACKLIGHT,			//��ά����屳��
		//OPERATION_TYPE_CARD_LOCKER,			//��ֵ������
		//OPERATION_TYPE_CARD_SENDER,			//��ֵ������
		//OPERATION_TYPE_COIN_SHUTTER,		//Ͷ�ҿڵ����
		//OPERATION_TYPE_PAS_SENSER,			//�˿ʹ�������Դ
		//OPERATION_TYPE_CHANGE_INDICATOR,    //����ָʾ��
		//OPERATION_TYPE_CHANGE_LIGHT,		//����������
		//OPERATION_TYPE_OPEN_ALL,			//������IO����
		//OPERATION_TYPE_CLOSE_ALL,			//�ر�����IO����
		//OPERATION_TYPE_UNKNOW,


		OPERATION_TYPE_STATUS = 0,					// IO״̬
		//OPERATION_TYPE_CHANGE_INDICATOR,			// ȡƱ����ָʾ��
		//OPERATION_TYPE_CHANGE_LIGHT,				// ȡƱ����������
		//OPERATION_TYPE_CHARGE_INDICATOR,			// ��ֵ��ָʾ��
		//OPERATION_TYPE_ALARM,						// ������
		//OPERATION_TYPE_CH_INDICATOR,				// Ӳ��Ͷ�ҿ�ָʾ��
		//OPERATION_TYPE_RECEIPT_INDICATOR,			// ƾ����ָʾ��
		//OPERATION_TYPE_BAFFLE,						// Ӳ�ҵ������������Ͷ�ң�
		//OPERATION_TYPE_CARDFFLE,					// ��ֵ���������������忨��

		IO_OUTPUT_PORT_NO_CH_INDICATOR										= 0x31,	  // Ͷ�ҿ�ָʾ��
		IO_OUTPUT_PORT_NO_RETURN_CUP_INDICATOR								= 0x32,   // ��Ʊ��ָʾ��
		IO_OUTPUT_PORT_NO_RETURN_CUP_LIGHT									= 0x33,   // ��Ʊ��������
		IO_OUTPUT_PORT_NO_CARDFFLE_INDICATOR								= 0x35,	  // ��ֵ��ָʾ��
		IO_OUTPUT_PORT_NO_BAFFLE_B											= 0x39,	  // B�����
		IO_OUTPUT_PORT_NO_OPERATION_LIGHT									= 0x41,	  // ����������
		IO_OUTPUT_PORT_NO_BAFFLE_T											= 0x42,	  // T�����
		IO_OUTPUT_PORT_NO_ALARM												= 0x43,   // ������
		IO_OUTPUT_PORT_NO_HELP_BTN_LIGHT									= 0x00,	  // ��Ԯ��ť��(ͣ��)


		OPERATION_TYPE_OPEN_ALL,			//������IO����
		OPERATION_TYPE_CLOSE_ALL,			//�ر�����IO����
		OPERATION_TYPE_UNKNOW,
	}OPERATION_TYPE;

	//�����
	typedef struct _tag_status_data
	{
		bool bIsThisActive;
		_tag_status_data()
		{
			bIsThisActive = false;
		}
	}STATUS_DATA_INFO;
	//�˵�����
	typedef struct _tag_menu_info 
	{
		CString strMenuStatus;
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	typedef struct _tag_data_modol
	{
		MENU_INFO menuInfo[_MAX_ROW];
		STATUS_DATA_INFO IO_Status[13];
		/*_tag_data_modol(){
		for(int i = 0;i<_MAX_ROW;i++){
		menuInfo[i] = MENU_INFO();
		IO_Status[i<8 ? i : 0] = STATUS_DATA_INFO();
		}
		}	*/

	}Modol;

	Modol& GetDataModol();            //����service������ģ��	
	void NotifyDataModelChanged();    //���ݸı��֪ͨ�����������
	volatile bool   m_bIsStopTesting; //ֹͣ���
	
private:
	int DeviceType;													   //�豸����
	Modol           m_Modol;                                           //����ģ��
	OPERATION_TYPE  m_OperationType;                                   //��������
	void            DoOperation();                                     //���ݲ������ͽ��в���
	static DWORD WINAPI    BeginTestingIOStatus(LPVOID lpParameter);   //����IO״̬����߳�  
	HANDLE          m_hHandle;
	DWORD           dwThreadID;
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //���� [Enter] ��������Ϣ��ִ�У�
	LRESULT OnKeyboardEsc(WPARAM /*wParam*/, LPARAM /*lParam*/);		// ���� [Esc] ��������Ϣ�����˵���
	//LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );     //���� [0~9]   ��������Ϣ�����֣�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //���� [F3/Ctrl]    ��������Ϣ�����ˣ�
	void OnStart();
};
