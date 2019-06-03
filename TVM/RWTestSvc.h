#pragma once
#include "TVMForeService.h"
#include "TVMRWHelper.h"
/**
@brief    ��д������
1.��ֵ        
2.smart-card  
3.Token       
*/

#define  SUCCESS     _opl(TEST_TEXT_SUCCESS)   //�ɹ���ʾ
#define  FAILED      _opl(TEST_TEXT_FAILURE)   //ʧ����ʾ
#define  COMPLETED   _opl(TEST_TEXT_COMPLETED) //�������

#define  UNKNOW      _T("")

class CRWTestSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CRWTestSvc();
	~CRWTestSvc();

	typedef enum _dialog_group
	{
		DIALOG_GROUP_MAIN = 1,
		DIALOG_GROUP_BASE,
		DIALOG_GROUP_OPERATION,
	} DIALOG_GROUP;

	//Main menu
	typedef enum _tag_Main_Menu
	{
		RECHARGE_RW = 0,
		CARD_RW=1,
		//TOKEN_RW,
		UNKNOW_RW,
	}MAIN_MENU;
	//��������
	enum OperationType
	{
		Operation_Reset,
		Operation_ReadCard,
		Operetion_Parameter_Ver,
		Operation_RW_Info,
		Operetion_Unknow,
	};
	//�����汾������
	typedef struct _tag_paramter_info
	{
		CString strID;
		CString strName;
		CString strVersion;
		_tag_paramter_info()
		{
			strID      = UNKNOW;
			strName    = UNKNOW;
			strVersion = UNKNOW;
		}
	}Paramter_Info;

	//RW��Ϣ����
	typedef struct _tag_RW_Info
	{
		CString strRWName;
		CString strRWValue; //��Ϊ��ʾ����Ҫת��ΪCString����������ͳһ����ΪCString
		_tag_RW_Info()
		{
			strRWName  = _T("R/W Device ID:");
			strRWValue = UNKNOW;
		}

	}RW_Info;

	//��������
	typedef struct _tag_BaseMenu_Info
	{
		int nMenuNum;
		CString strMenuStatus;
		_tag_BaseMenu_Info()
		{
			nMenuNum      = 1;
			strMenuStatus = UNKNOW;
		}

	}BaseMenu_Info;

	typedef struct _tag_Menu_Info
	{
		OperationType operation_type;          //��ǰ��������
		MAIN_MENU     rwType;                  //��������˵����ͣ���ֵ��Ʊ����
		BaseMenu_Info baseMenuInfo[4];
		Paramter_Info paramterInfo[22];
		RW_Info       rwInfo[6];
		int timesstatus;							// ��ҳ�еĵ�ǰҳ��

		_tag_Menu_Info()
		{
			timesstatus = 1;
			operation_type = Operetion_Unknow;
			rwType         = CARD_RW;
			/*for(int i=0;i<10;i++){
			baseMenuInfo[i<4 ? i:0] = BaseMenu_Info();
			paramterInfo[i<10?i:0] = Paramter_Info();
			rwInfo[i<4 ? i:0]		= RW_Info();
			}*/
		}
	}Modol;

	Modol& GetDataModol();//��������ģ��

	bool RWResetTest();                         //RW��λ����
	bool RWReadCardTest();                      //RW��������
	void RWParameterTest();                     //RW��Parameter����
	void RWInfoTest();                          //RWInfo����

	void ReFlashDlg();
	void ResetModolData();                      //����modol������

private:
	Modol m_Modol;
	int DeviceType;
	void DoMainMenu();    //���˵�����
	void DoBaseMenu();    //�Ӳ˵�����
	//��Ϣ����
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );						  //���� [Enter] ��������Ϣ��ִ�У�
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //���� [0~9]   ��������Ϣ�����֣�
	LRESULT OnKeyboardF3(WPARAM , LPARAM );							  //���� [F3/Ctrl]    ��������Ϣ�����ˣ�
	LRESULT OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F5] ��������Ϣ����һҳ��
	LRESULT OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/);			// ���� [F6] ��������Ϣ����һҳ��

	//void OnReadCard(UINT,CRWReadCard*);                             //��������ص�����
	void OnStart();
	bool m_bIsSupperUser;
};
