#pragma once
#include "tvmforeservice.h"
#define  _MAX_ROW  15     //�������
#define  _MAX_COL   2     //�������

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")
/**
@brief    ƾ֤��ӡ��-Ӳ���Լ�ҵ��
*/
class CPrinterTestSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestSvc();
	~CPrinterTestSvc();

	//����
	typedef enum _dialog_group 
	{
		DIALOG_GROUP_PRINT   =  1,                
	} DIALOG_GROUP;

	//��������
	typedef enum _tag_operation_type
	{
		OPERATION_TYPE_RESET = 0,
		OPERATION_TYPE_TEST_PRINT,
		OPERATION_TYPE_UNKNOW,
	}OPERATION_TYPE;

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
		/*_tag_data_modol(){
		for(int i=0;i<_MAX_ROW;i++){
		menuInfo[i] = MENU_INFO();
		}
		}*/
	}Modol;

	Modol& GetDataModol();             //����service������ģ��	
	void NotifyDataModelChanged();     //���ݸı��֪ͨ�����������

private:
	Modol m_Modol;                                //����ģ��
	OPERATION_TYPE  m_OperationType;              //��������
	void            DoOperation();               //���ݲ������ͽ��в���
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //���� [Enter] ��������Ϣ��ִ�У�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );					//���� [F3/Ctrl]	   ��������Ϣ�����أ�
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
	void OnStart();
};