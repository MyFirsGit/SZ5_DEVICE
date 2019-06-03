#pragma once
#include "TVMForeService.h"
#pragma warning(disable : 4482)

/**
@brief ֽ���������Service
*/
#define  _MAX_ROW 15                 //�������
#define  _MAX_COL  4                 //�������

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������
#define  ZHENGZHOU_CHENGJIAO 60

#define  UNKNOW      _T("")

class CBanknoteChangeModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteChangeModuleTestSvc();
	virtual ~CBanknoteChangeModuleTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_BANKNOTE_CHANGE   =  1,                                // ֽ�Ҳ��Ի�����
	} DIALOG_GROUP;

	//Operation type
	typedef enum _tag_operation_type
	{
		OPERATION_TYPE_RESET = 0,
		OPERATION_TYPE_COUNT_BILL, //�������
		OPERATION_TYPE_READ_BOX_A,
		OPERATION_TYPE_READ_BOX_B,
		OPERATION_TYPE_READ_BOX_INVALID,
		OPERATION_TYPE_WRITE_BOX_A,
		OPERATION_TYPE_WRITE_BOX_B,
		OPERATION_TYPE_WRITE_BOX_INVALID,
		OPERATION_TYPE_UNKNOW,
		OPERATION_TYPE_F3,
		OPERATION_TYPE_ESC
	}OPERATION_TYPE;

	//�����-�˵���Ϣ����
	typedef struct _tag_menu_info
	{
		CString strMenuStatus;     //�ɹ���ʧ�ܡ���ɵ�
		_tag_menu_info()
		{
			strMenuStatus = UNKNOW;
		}
	}MENU_INFO;

	typedef struct _tag_data_modol
	{
		MENU_INFO menuInfo[_MAX_ROW];

		CTBanknoteCountInfo::BANKNOTE_BOX_ID readRfidBoxA;
		CTBanknoteCountInfo::BANKNOTE_BOX_ID readRfidBoxB;
		CTBanknoteCountInfo::BANKNOTE_BOX_ID readRfidInvalid;


		CTBanknoteCountInfo::BANKNOTE_BOX_ID WriteRfidBoxA;
		CTBanknoteCountInfo::BANKNOTE_BOX_ID WriteRfidBoxB;
		CTBanknoteCountInfo::BANKNOTE_BOX_ID WriteRfidInvalid;
		/*_tag_data_modol(){
		for(int i = 0; i<5; i++){
		menuInfo[i] = MENU_INFO();
		}
		}*/

		_tag_data_modol(){
			readRfidBoxA = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
			readRfidBoxB = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
			readRfidInvalid = CTBanknoteCountInfo::BANKNOTE_BOX_ID();

			WriteRfidBoxA = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
			WriteRfidBoxB = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
			WriteRfidInvalid = CTBanknoteCountInfo::BANKNOTE_BOX_ID();
		}
	}Modol;

	Modol& GetDataModol();            //����service������ģ��
	void NotifyDataModelChanged();    //���ݸı��֪ͨ�����������
private:	
	Modol m_Modol;
	OPERATION_TYPE  m_OperationType;									//��������
	void            DoOperation();										//���ݲ������ͽ��в���
	void			DoWriteRfid(int);									// дrfid
	BOOL IsInit;                                            //�Ƿ��ʼ��
	//BOOL Is;
	//��Ϣ����
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              // ���� [Enter] ��������Ϣ��ִ�У�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					// ���� [F3/Ctrl]    ��������Ϣ
	LRESULT OnKeyboardEsc(WPARAM , LPARAM );				// ���� [ESC]   ��������Ϣ
	void OnStart();
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
};
