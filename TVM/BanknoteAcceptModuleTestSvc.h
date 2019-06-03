#pragma once
#include "TVMForeService.h"
#pragma warning(disable : 4482)

/**
@brief ֽ�Ҳ���Service
*/
#define  _MAX_ROW 15                 //�������
#define  _MAX_COL  4                 //�������

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")

class CBanknoteAcceptModuleTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteAcceptModuleTestSvc();
	virtual ~CBanknoteAcceptModuleTestSvc();

	typedef enum _dialog_group 
	{
		DIALOG_GROUP_BANKNOTE   =  1,                                // ֽ�Ҳ��Ի�����
	} DIALOG_GROUP;

	//Operation type
	typedef enum _tag_operation_type
	{
		OPERATION_TYPE_RESET = 0,									// ����
		OPERATION_TYPE_START_IDENTIFY,								// ��ʼ����ֽ��
		OPERATION_TYPE_STOP_IDENTIFY,								// ֹͣ����ֽ��		
		OPERATION_TYPE_RETURN_CASH,									// �˳�										
		OPERATION_TYPE_ACCEPT,										// ѹ��
		OPERATION_TYPE_CHANGE,										// ����ֽ��
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
		MENU_INFO menuInfo[5];
		BankNoteAndCoinType_t bh_face;
		_tag_data_modol(){
			bh_face = VALUE_UNKNOWN;
			/*for(int i = 0; i<5; i++){
			menuInfo[i] = MENU_INFO();
			}*/
		}
	}Modol;

	Modol& GetDataModol();            //����service������ģ��
	void NotifyDataModelChanged();    //���ݸı��֪ͨ�����������
	void OnBanknoteInserted(UINT,CBRHOpenAndValidate*);			// ֽ��ʶ����
private:
	Modol m_Modol;
	OPERATION_TYPE  m_OperationType;                                   //��������
	void            DoOperation();                                     //���ݲ������ͽ��в���
	bool			m_bIsBanknotesInserted;							   //�ж�ֽ���Ƿ����
	BOOL			m_bIsReceiving;									   //�ж��Ƿ����ڽ���
	BRH_STATUS       bhRspType; //ֽ��ʶ��

	//��Ϣ����
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              // ���� [Enter] ��������Ϣ��ִ�У�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					// ���� [F3/Ctrl]    ��������Ϣ
	LRESULT OnKeyboardEsc(WPARAM , LPARAM );				// ���� [ESC]   ��������Ϣ
	void OnStart();
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
};
