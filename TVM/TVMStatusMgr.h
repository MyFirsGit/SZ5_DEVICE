#pragma once
#include "svcdef.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include "scdatamsg.h"
#include "delegate.h"

// TVM״̬����Ψһ����
#define theTVM_STATUS_MGR  CStatusHelper::GetInstance()

/**
@brief  TVM״̬������
*/
class CStatusHelper
{
public:
	CStatusHelper();
	~CStatusHelper();


	delegate<void()> TVMPaymentChanged;			//֧��ģʽ����仯�¼�
	delegate<void()> TVMChangeChanged;			//��ֵģʽ����仯�¼�
	delegate<void()> TVMStatusDispMessageChanged;// ״̬��ʾ��Ϣ�仯�¼�
	
    static CStatusHelper& GetInstance();          // ��ȡ����Ķ���
	
	// 1.����״̬
	void SetServiceStatus(SERVICE_STATUS );       // ���÷���״̬         
	SERVICE_STATUS GetServiceStatus();            // ��ȡ����״̬

	// 2.����״̬
	void SetRunStatus(RUN_STATUS);                // ��������״̬
	RUN_STATUS GetRunStatus();                    // ��ȡ����״̬

	// 3.����ģʽ��ҵ��ģʽ��
	void SetWorkMode(WORD, bool = false);		  // ����ҵ��ģʽ
	WORD GetWorkMode();							  // ��ȡҵ��ģʽ
	inline WORK_MODE GetServerWorkMode(){
		return GetDegreeWorkMode();}			  // ��ȡҵ��ģʽ

	// 4.��Ӫģʽ
	void SetOperationMode(OperationalMode_t);		// ������Ӫģʽ
	OperationalMode_t GetOperationMode();			// ��ȡ��Ӫģʽ

	// 5.24Сʱ��Ӫģʽ
	void Set24HourMode(OPERATION_24HOURS_MODE);		//  ����24Сʱ��Ӫģʽ
	OPERATION_24HOURS_MODE Get24HourMode();			//  ��ȡ24Сʱ��Ӫģʽ

	// 6.�ӳ���Ӫģʽ
	void SetDelayTime(WORD wDelayTime);				 //  �����ӳ���Ӫģʽ
	WORD GetDelayTime();						     //  ��ȡ�ӳ���Ӫģʽ

	// 7.����״̬
	void SetCommunicate(COMMUNICATION);             // ����ͨ��״̬
	COMMUNICATION GetCommunicate();                 // ��ȡͨ��״̬
 
	// 8.ʱ��״̬
	void SetTimeSynStatus(TIME_STATUS);             // ����ʱ��ͬ��״̬
	TIME_STATUS GetTimeSynStatus();                 // ��ȡʱ��ͬ��״̬

	// 9.��¼״̬
	void SetLoginStatus(LOGIN_STATUS);              // ���õ�¼״̬
	LOGIN_STATUS GetLoginStatus();					// ��ȡ��¼״̬

	// 10.�豸�Զ�����״̬
	void  SetAutoRunStatus(DEVICE_AUTORUN);			// �����Զ�����(Ԥ��)
	DEVICE_AUTORUN GetAutoRunStatus();              // ��ȡ�Զ�����

	// 11~16Ʊ��״̬
	void SetCardBox1Status(STATUS_MAGAZINE);			// ����Card��1��װ״̬
	STATUS_MAGAZINE GetCardBox1Status();				// ��ȡCard��1��װ״̬
	void SetCardBox1NumStatus(MAGAZINE_NUM_STATUS);		// ����Card��1����״̬
	MAGAZINE_NUM_STATUS GetCardBox1NumStatus();			// ��ȡCard��1����״̬

	void SetCardBox2Status(STATUS_MAGAZINE);			// ����Card��2��װ״̬
	STATUS_MAGAZINE GetCardBox2Status();				// ��ȡCard��2��װ״̬
	void SetCardBox2NumStatus(MAGAZINE_NUM_STATUS);		// ����Card��2����״̬
	MAGAZINE_NUM_STATUS GetCardBox2NumStatus();			// ��ȡCard��2����״̬

	void SetCardWasteBoxStatus(STATUS_MAGAZINE);		// ����Card�����䰲װ״̬
	STATUS_MAGAZINE GetCardWasteBoxStatus();			// ��ȡCard�����䰲װ״̬
	void SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS);	// ����Card����������״̬
	MAGAZINE_NUM_STATUS GetCardWasteBoxNumStatus();		// ��ȡCard����������״̬


	// Token��״̬	
	void SetTokenAddBox1Status(STATUS_MAGAZINE);				// ����Token������1�䰲װ״̬
	STATUS_MAGAZINE GetTokenAddBox1Status();					// ��ȡToken������1�䰲װ״̬
	void SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS);			// ����Token������1������״̬
	MAGAZINE_NUM_STATUS GetTokenAddBox1NumStatus();				// ��ȡToken������1������״̬

	void SetTokenAddBox2Status(STATUS_MAGAZINE);				// ����Token������2�䰲װ״̬
	STATUS_MAGAZINE GetTokenAddBox2Status();					// ��ȡToken������2�䰲װ״̬
	void SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS);			// ����Token������2������״̬
	MAGAZINE_NUM_STATUS GetTokenAddBox2NumStatus();				// ��ȡToken������2������״̬

	void SetTokenHopper1Status(STATUS_MAGAZINE);				// ����Token Hopper1�䰲װ״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE GetTokenHopper1Status();					// ��ȡToken Hopper1�䰲װ״̬���ݲ�ʹ�ã�
	void SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS);			// ����Token Hopper1������״̬
	MAGAZINE_NUM_STATUS GetTokenHopper1NumStatus();				// ��ȡToken Hopper1������״̬

	void SetTokenHopper2Status(STATUS_MAGAZINE);				// ����Token Hopper2�䰲װ״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE GetTokenHopper2Status();					// ��ȡToken Hopper2�䰲װ״̬���ݲ�ʹ�ã�
	void SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS);			// ����Token Hopper2������״̬
	MAGAZINE_NUM_STATUS GetTokenHopper2NumStatus();				// ��ȡToken Hopper2������״̬

	void SetTokenWasteBoxStatus(STATUS_MAGAZINE);				// ����Token��Ʊ�䰲װ״̬
	STATUS_MAGAZINE GetTokenWasteBoxStatus();					// ��ȡToken��Ʊ�䰲װ״̬
	void SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS);		// ����Token��Ʊ������״̬
	MAGAZINE_NUM_STATUS GetTokenWasteBoxNumStatus();			// ��ȡToken��Ʊ������״̬

	void SetTokenCollectedBoxStatus(STATUS_MAGAZINE);			// ����Token�����䰲װ״̬
	STATUS_MAGAZINE GetTokenCollectedBoxStatus();				// ��ȡToken�����䰲װ״̬
	void SetTokenCollectedBoxNumStatus(MAGAZINE_NUM_STATUS);	// ����Token����������״̬
	MAGAZINE_NUM_STATUS GetTokenCollectedBoxNumStatus();		// ��ȡToken����������״̬

	// Ӳ����״̬
	void SetCoinAddBox1Status(STATUS_MAGAZINE);				// ����Ӳ�Ҳ�����1��װ״̬
	STATUS_MAGAZINE GetCoinAddBox1Status();					// ��ȡӲ�Ҳ�����1��װ״̬
	void SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS);		// ����Ӳ�Ҳ�����1����״̬
	MAGAZINE_NUM_STATUS GetCoinAddBox1NumStatus();			// ��ȡӲ�Ҳ�����1����״̬

	void SetCoinAddBox2Status(STATUS_MAGAZINE);				// ����Ӳ�Ҳ�����2��װ״̬
	STATUS_MAGAZINE GetCoinAddBox2Status();					// ��ȡӲ�Ҳ�����2��װ״̬
	void SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS);		// ����Ӳ�Ҳ�����2����״̬
	MAGAZINE_NUM_STATUS GetCoinAddBox2NumStatus();			// ��ȡӲ�Ҳ�����2����״̬

	void SetCoinHopper1Status(STATUS_MAGAZINE);				// ����Ӳ��Hopper1�䰲װ״̬(��ʹ��)
	STATUS_MAGAZINE GetCoinHopper1Status();					// ��ȡӲ��Hopper1�䰲װ״̬(��ʹ��)
	void SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS);		// ����Ӳ��Hopper1������״̬
	MAGAZINE_NUM_STATUS GetCoinHopper1NumStatus();			// ��ȡӲ��Hopper1������״̬

	void SetCoinHopper2Status(STATUS_MAGAZINE);				// ����Ӳ��Hopper2�䰲װ״̬(��ʹ��)
	STATUS_MAGAZINE GetCoinHopper2Status();					// ��ȡӲ��Hopper2�䰲װ״̬(��ʹ��)
	void SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS);		// ����Ӳ��Hopper2������״̬
	MAGAZINE_NUM_STATUS GetCoinHopper2NumStatus();			// ��ȡӲ��Hopper2������״̬

	void SetCoinCircleHopperStatus(STATUS_MAGAZINE);		// ����Ӳ��ѭ������Hopper�䰲װ״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE GetCoinCircleHopperStatus();			// ��ȡӲ��ѭ������Hopper�䰲װ״̬���ݲ�ʹ�ã�
	void SetCoinCircleHopperNumStatus(MAGAZINE_NUM_STATUS);	// ����Ӳ��ѭ������Hopper������״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS GetCoinCircleHopperNumStatus();		// ��ȡӲ��ѭ������Hopper������״̬���ݲ�ʹ�ã�

	void SetCoinCollectionBox1Status(STATUS_MAGAZINE);		 // ����Ӳ�һ�����1��װ״̬
	STATUS_MAGAZINE GetCoinCollectionBox1Status();			 // ��ȡӲ�һ�����1��װ״̬
	void SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS);// ����Ӳ�һ�����1����״̬
	MAGAZINE_NUM_STATUS GetCoinCollectionBox1NumStatus();	 // ��ȡӲ�һ�����1����״̬

	void SetCoinCollectionBox2Status(STATUS_MAGAZINE);		 // ����Ӳ�һ�����2��װ״̬
	STATUS_MAGAZINE GetCoinCollectionBox2Status();			 // ��ȡӲ�һ�����2��װ״̬
	void SetCoinCollectionBox2NumStatus(MAGAZINE_NUM_STATUS);// ����Ӳ�һ�����2����״̬
	MAGAZINE_NUM_STATUS GetCoinCollectionBox2NumStatus();	 // ��ȡӲ�һ�����2����״̬

	// ֽ����״̬
	void SetBanknoteChangeBox1Status(STATUS_MAGAZINE);			// ����ֽ��������1��װ״̬
	STATUS_MAGAZINE GetBanknoteChangeBox1Status();				// ��ȡֽ��������1��װ״̬
	void SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS);	// ����ֽ��������1����״̬
	MAGAZINE_NUM_STATUS GetBanknoteChangeBox1NumStatus();		// ��ȡֽ��������1����״̬

	void SetBanknoteChangeBox2Status(STATUS_MAGAZINE);			// ����ֽ��������2��װ״̬
	STATUS_MAGAZINE GetBanknoteChangeBox2Status();				// ��ȡֽ��������2��װ״̬
	void SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS);	// ����ֽ��������2����״̬
	MAGAZINE_NUM_STATUS GetBanknoteChangeBox2NumStatus();		// ��ȡֽ��������2����״̬

	/*void SetBanknoteWasteBoxStatus(STATUS_MAGAZINE);			// ����ֽ�ҷϱ��䰲װ״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE GetBanknoteWasteBoxStatus();				// ��ȡֽ�ҷϱ��䰲װ״̬���ݲ�ʹ�ã�
	void SetBanknoteWasteBoxNumStatus(MAGAZINE_NUM_STATUS);		// ����ֽ�ҷϱ�������״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS GetBanknoteWasteBoxNumStatus();			// ��ȡֽ�ҷϱ�������״̬���ݲ�ʹ�ã�*/

	void SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS);			// ����ֽ��ѭ����1����״̬
	MAGAZINE_NUM_STATUS GetBanknoteRE1NumStatus();				// ��ȡֽ��ѭ����1����״̬
	void SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS);			// ����ֽ��ѭ����2����״̬
	MAGAZINE_NUM_STATUS GetBanknoteRE2NumStatus();				// ��ȡֽ��ѭ����2����״̬
	void SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS);			// ����ֽ��ѭ����3����״̬
	MAGAZINE_NUM_STATUS GetBanknoteRE3NumStatus();				// ��ȡֽ��ѭ����3����״̬
	void SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS);			// ����ֽ��ѭ����4����״̬
	MAGAZINE_NUM_STATUS GetBanknoteRE4NumStatus();				// ��ȡֽ��ѭ����4����״̬

	void SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE);		// ����ֽ�һ����䰲װ״̬
	STATUS_MAGAZINE GetBanknoteCollectionBoxStatus();			// ��ȡֽ�һ����䰲װ״̬
	void SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS);// ����ֽ�һ���������״̬
	MAGAZINE_NUM_STATUS GetBanknoteCollectionBoxNumStatus();	// ��ȡֽ�һ���������״̬

	// �ֽ�ȫ��״̬
	void SetSecurityDoorStatus(SECURITY_DOOR_STATUS);			// �����ֽ�ȫ��״̬��Ԥ����
	SECURITY_DOOR_STATUS GetSecurityDoorStatus();				// ��ȡ�ֽ�ȫ��״̬��Ԥ����

	// ��Ԯ��ť
	void SetAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS);	// ������Ԯ��ť״̬
	ASSISTANCE_BUTTON_STATUS GetAssistanceButtonStatus();		// ��ȡ��Ԯ��ť״̬

	// ��д��1״̬
	void SetRW1Status(STATUS_RW);								// ���ö�д��1״̬
	STATUS_RW GetRW1Status();									// ��ȡ��д��1״̬

	// ��д��2״̬
	void SetRW2Status(STATUS_RW);								// ���ö�д��2״̬
	STATUS_RW GetRW2Status();									// ��ȡ��д��2״̬

	// SAM1����
	void SetSam1Type(SAM_STATUS);								// ����Sam1����
	SAM_STATUS GetSam1Type();									// ��ȡSam1����
	
	// SAM2����
	void SetSam2Type(SAM_STATUS);								// ����Sam2����
	SAM_STATUS GetSam2Type();									// ��ȡSam2����

	// SAM1״̬
	void SetSam1Status(SAM_AUTH_STATUS);						// ����Sam1��֤״̬
	SAM_AUTH_STATUS GetSam1Status();							// ��ȡSam1��֤״̬
	
	// SAM2״̬
	void SetSam2Status(SAM_AUTH_STATUS);						// ����Sam2��֤״̬
	SAM_AUTH_STATUS GetSam2Status();							// ��ȡSam2��֤״̬

	// ��ӡ��״̬
	void SetPrintStatus(PRINTER_STATUS);						// ���ô�ӡ��״̬
	PRINTER_STATUS GetPrintStatus();							// ��ȡ��ӡ��״̬

	// ά����״̬
	void SetMaintenanceDoorStatus(DOOR_STATUS);                // ����ά����״̬
	DOOR_STATUS GetMaintenanceDoorStatus();					   // ��ȡά����״̬

	// ֽ�һ���ģ��״̬
	void SetBanknoteModuleStatus(HARDWARE_MODULE_STATUS);		// ����ֽ��ģ��״̬
	HARDWARE_MODULE_STATUS GetBanknoteModuleStatus();			// ��ȡֽ��ģ��״̬

	// ֽ������ģ��״̬
	void SetBanknoteChangeModuleStatus(HARDWARE_MODULE_STATUS);	// ����ֽ������ģ��״̬
	HARDWARE_MODULE_STATUS GetBanknoteChangeModuleStatus();		// ��ȡֽ������ģ��״̬

	// Ʊ������ģ��״̬
	void SetCardModuleStatus(HARDWARE_MODULE_STATUS);			// ����Cardģ��״̬
	HARDWARE_MODULE_STATUS GetCardModuleStatus();				// ��ȡCardģ��״̬

	// Ӳ��ģ��״̬
	void SetCoinModuleStatus(HARDWARE_MODULE_STATUS);			 // ����Ӳ��ģ��״̬
	HARDWARE_MODULE_STATUS GetCoinModuleStatus();				 // ��ȡӲ��ģ��״̬

	// LEDģ��״̬
	void SetStatusDisplayStatus(HARDWARE_MODULE_STATUS);		// ����״̬��ʾ��ģ��״̬
	HARDWARE_MODULE_STATUS GetStatusDisplayStatus();			// ��ȡ״̬��ʾ��ģ��״̬
	// ������״̬
	void SetTouchDisplayStatus(TOUCH_SCREEN_STATUS);			// ���ô�����״̬��Ԥ����
	TOUCH_SCREEN_STATUS GetTouchDisplayStatus();				// ��ȡ������״̬��Ԥ����

	// �豸����״̬
	void SetDeviceErrStatus(long lMtcCode);						// �����豸����״̬
	long GetDeviceErrStatus();									// ��ȡ�豸����״̬

	// �������״̬
	void SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS);			// ���ô������״̬
	TRANSFER_AUDIT_STATUS GetDeviceTransferAudit();				// ȡ�������״̬

	// ֧��ģʽ
	void SetPaymentMode(WORD,bool bIsDeviceControl = true);	// ����֧��ģʽ
	WORD GetPaymentMode();										// ��ȡ֧��ģʽ

	// ��ӡģʽ
	void SetPrintMode(WORD,bool bIsDeviceControl = true);		// ���ô�ӡģʽ
	bool IsNoPrintMode();										// �Ƿ����޴�ӡģʽ
	WORD GetPrintMode();

	// ����ģʽ
	void SetChangeMode(WORD,bool bIsDeviceControl = true);		// ��������ģʽ
	WORD GetChangeMode();										// ��ȡ����ģʽ

	// ���̿ռ�״̬
	void SetDiskSpaceStatus(DISK_SPACE_CODE);					// ���ô��̿ռ�״̬(��ʱ����)
	DISK_SPACE_CODE GetDiskSpaceStatus();						// ��ȡ���̿ռ�״̬(��ʱ����)

	// ��������
	void SetOffLineDays(BYTE);									// ������������(��ʱ����)
	BYTE GetOffLineDays();										// ��ȡ��������(��ʱ����)


	/*-----------------------------------�ָ��ߣ�����֣�ݲ�ʹ�ã�---------------------------------------------*/

	// ��������ģʽ
	void SetDegradedMode(DEVICE_CONTRL_CODE);						// ���ý�������ģʽ
	bool IsCanAutoDegraded();										// �Ƿ�����������

	

	// Token��״̬
	/*
	void SetTokenAddBox1Status(STATUS_MAGAZINE);							// ����Token������1�䰲װ״̬
	STATUS_MAGAZINE GetTokenAddBox1Status();								// ��ȡToken������1�䰲װ״̬
	void SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS);			// ����Token������1������״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS GetTokenAddBox1NumStatus();				// ��ȡToken������1������״̬���ݲ�ʹ�ã�

	void SetTokenAddBox2Status(STATUS_MAGAZINE);							// ����Token������2�䰲װ״̬
	STATUS_MAGAZINE GetTokenAddBox2Status();								// ��ȡToken������2�䰲װ״̬
	void SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS);			// ����Token������2������״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS GetTokenAddBox2NumStatus();				// ��ȡToken������2������״̬���ݲ�ʹ�ã�

	void SetTokenHopper1Status(STATUS_MAGAZINE);							// ����Token Hopper1�䰲װ״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE GetTokenHopper1Status();									// ��ȡToken Hopper1�䰲װ״̬���ݲ�ʹ�ã�
	void SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS);			// ����Token Hopper1������״̬
	MAGAZINE_NUM_STATUS GetTokenHopper1NumStatus();				// ��ȡToken Hopper1������״̬

	void SetTokenHopper2Status(STATUS_MAGAZINE);							// ����Token Hopper2�䰲װ״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE GetTokenHopper2Status();									// ��ȡToken Hopper2�䰲װ״̬���ݲ�ʹ�ã�
	void SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS);					// ����Token Hopper2������״̬
	MAGAZINE_NUM_STATUS GetTokenHopper2NumStatus();							// ��ȡToken Hopper2������״̬

	void SetTokenWasteBoxStatus(STATUS_MAGAZINE);							// ����Token�����䰲װ״̬
	STATUS_MAGAZINE GetTokenWasteBoxStatus();								// ��ȡToken�����䰲װ״̬
	void SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS);		// ����Token����������״̬
	MAGAZINE_NUM_STATUS GetTokenWasteBoxNumStatus();				// ��ȡToken����������״̬*/

	// Card��״̬
	//void SetCardBox1Status(STATUS_MAGAZINE);									// ����Card��1��װ״̬
	//STATUS_MAGAZINE GetCardBox1Status();											// ��ȡCard��1��װ״̬
	//void SetCardBox1NumStatus(MAGAZINE_NUM_STATUS);					// ����Card��1����״̬
	//MAGAZINE_NUM_STATUS GetCardBox1NumStatus();						// ��ȡCard��1����״̬

	//void SetCardBox2Status(STATUS_MAGAZINE);									// ����Card��2��װ״̬
	//STATUS_MAGAZINE GetCardBox2Status();											// ��ȡCard��2��װ״̬
	//void SetCardBox2NumStatus(MAGAZINE_NUM_STATUS);						// ����Card��2����״̬
	//MAGAZINE_NUM_STATUS GetCardBox2NumStatus();						// ��ȡCard��2����״̬

	//void SetCardWasteBoxStatus(STATUS_MAGAZINE);							// ����Card�����䰲װ״̬
	//STATUS_MAGAZINE GetCardWasteBoxStatus();									// ��ȡCard�����䰲װ״̬
	//void SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS);			// ����Card����������״̬
	//MAGAZINE_NUM_STATUS GetCardWasteBoxNumStatus();				// ��ȡCard����������״̬

	//void SetCardRWStatus(STATUS_RW);								// ����Card RW״̬
	//STATUS_RW GetCardRWStatus();									// ��ȡCard RW״̬

	//void SetTokenRWStatus(STATUS_RW);								// ����Token RW״̬
	//STATUS_RW GetTokenRWStatus();									// ��ȡToken RW״̬

	
	//void SetSam3Type(SAM_STATUS);                     // ����Sam3���ͣ��ݲ�ʹ�ã�
	//SAM_STATUS GetSam3Type();							// ��ȡSam3���ͣ��ݲ�ʹ�ã�
	//void SetSam4Type(SAM_STATUS);                     // ����Sam4���ͣ��ݲ�ʹ�ã�
	//SAM_STATUS GetSam4Type();							// ��ȡSam4���ͣ��ݲ�ʹ�ã�

	
	//void SetSam3Status(SAM_STATUS);                 // ����Sam3��֤״̬
	//SAM_STATUS GetSam3Status();                       // ��ȡSam3��֤״̬
	//void SetSam4Status(SAM_STATUS);                 // ����Sam4��֤״̬
	//SAM_STATUS GetSam4Status();                       // ��ȡSam4��֤״̬	

	//void SetUPSStatus(UPS_STATUS);						// ����UPS״̬���ݲ�ʹ�ã�
	//UPS_STATUS GetUPSStatus();								// ��ȡUPS״̬���ݲ�ʹ�ã�
	

	//void SetTokenModuleStatus(HARDWARE_MODULE_STATUS); // ����Tokenģ��״̬
	//HARDWARE_MODULE_STATUS GetTokenModuleStatus();		// ��ȡTokenģ��״̬	

	//void SetTiltStatus(TILT_STATUS);				// ������б������״̬
	//TILT_STATUS GetTiltStatus();					// ��ȡ��б������״̬

	void SetAuthorizedAccessStatus(AUTHORIZED_STATUS);		// ������Ȩ����״̬
	AUTHORIZED_STATUS GetAuthorizedAccessStatus();			// ��ȡ��Ȩ����״̬

private:
	CStatusHelper(const CStatusHelper&);
	CStatusHelper& operator=(const CStatusHelper&);
	void OnExceptionItemNotify(CSysException&,bool);
	CSCDataMsg* MakeStatusChangeCommand(TVM_STATUS_ID statusID,BYTE bValue);
	CSCDataMsg* MakeErrorStatusCommand(long lMtcCode);

	static CStatusHelper						theInstance;
	SERVICE_STATUS							m_sServiceStatus;			// ����״̬
	RUN_STATUS								m_sRunStatus;				// ����״̬
	WORD									m_sWorkMode;				// ����ģʽ
	OperationalMode_t						m_OperationMode;			// ��Ӫģʽ���μ�7.9��Ӫģʽ���롣
	OPERATION_24HOURS_MODE				m_s24HourMode;					// 24Сʱ��Ӫģʽ
	WORD							m_sDelayTime;						// �ӳ���Ӫʱ��									
	COMMUNICATION						m_sConnectStatus;				// ����״̬
	TIME_STATUS								m_sTimeStatus;				// ʱ�Ӳ���
	LOGIN_STATUS								m_sLoginStatus;			// ��¼״̬
	DEVICE_AUTORUN						m_sAutorunStatus;				// �豸�Զ�����״̬
	long								m_lDeviceErrStatus;				// �豸����״̬
	TRANSFER_AUDIT_STATUS            m_sTransferAudit;					// �������״̬
	WORD											m_sPaymentMode;		// ֧��ģʽ
	WORD											m_sPrintMode;		// ��ӡģʽ
	WORD											m_sChangeMode;		// ����ģʽ
	DISK_SPACE_CODE						m_sDiskSpaceStatus;				// ���̿ռ�״̬

	STATUS_MAGAZINE						m_sTokenBox1Status;					// Token������1��װ״̬
	MAGAZINE_NUM_STATUS			m_sTokenBox1NumStatus;			// Token������1����״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE						m_sTokenBox2Status;					// Token������2��װ״̬
	MAGAZINE_NUM_STATUS			m_sTokenBox2NumStatus;			// Token������2����״̬���ݲ�ʹ�ã�
	STATUS_MAGAZINE						m_sTokenHopper1Status;				// Token Hopper��1��װ״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS			m_sTokenHopper1NumStatus;		// Token Hopper��1����״̬
	STATUS_MAGAZINE						m_sTokenHopper2Status;				// Token Hopper��2��װ״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS			m_sTokenHopper2NumStatus;		// Token Hopper��2����״̬
	STATUS_MAGAZINE						m_sTokenWasteBoxStatus;				// Token��Ʊ�䰲װ״̬
	MAGAZINE_NUM_STATUS			m_sTokenWasteBoxNumStatus;		// Token��Ʊ������״̬
	STATUS_MAGAZINE						m_sTokenCollectedBoxStatus;			// Token��Ʊ�䰲װ״̬
	MAGAZINE_NUM_STATUS			m_sTokenCollectedBoxNumStatus;	// Token��Ʊ������״̬


	STATUS_MAGAZINE						m_sCardBox1Status;					// Card��1��װ״̬
	MAGAZINE_NUM_STATUS			m_sCardBox1NumStatus;			// Card��1����״̬
	STATUS_MAGAZINE						m_sCardBox2Status;					// Card��2��װ״̬
	MAGAZINE_NUM_STATUS			m_sCardBox2NumStatus;			// Card��2����״̬
	STATUS_MAGAZINE						m_sCardWasteBoxStatus;			// Card��Ʊ�䰲װ״̬
	MAGAZINE_NUM_STATUS			m_sCardWasteBoxNumStatus;	// Card��Ʊ������״̬

	STATUS_MAGAZINE						m_sCoinBox1Status;					// Coin������1��װ״̬
	MAGAZINE_NUM_STATUS			m_sCoinBox1NumStatus;						// Coin������1����״̬
	STATUS_MAGAZINE						m_sCoinBox2Status;					// Coin������2��װ״̬
	MAGAZINE_NUM_STATUS			m_sCoinBox2NumStatus;						// Coin������2����״̬
	STATUS_MAGAZINE						m_sCoinHopper1Status;				// Coin Hopper��1��װ״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS			m_sCoinHopper1NumStatus;					// Coin Hopper��1����״̬
	STATUS_MAGAZINE						m_sCoinHopper2Status;				// Coin Hopper��2��װ״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS			m_sCoinHopper2NumStatus;					// Coin Hopper��2����״̬
	STATUS_MAGAZINE						m_sCoinCircleHopperStatus;			// Coin ѭ������Hopper�䰲װ״̬���ݲ�ʹ�ã�
	MAGAZINE_NUM_STATUS			m_sCoinCircleHopperNumStatus;				// Coin ѭ������Hopper������״̬
	STATUS_MAGAZINE						m_sCoinCollectionBox1Status;		// Coin������1��װ״̬
	MAGAZINE_NUM_STATUS			m_sCoinCollectionBox1NumStatus;				// Coin������1����״̬
	STATUS_MAGAZINE						m_sCoinCollectionBox2Status;		// Coin������2��װ״̬
	MAGAZINE_NUM_STATUS			m_sCoinCollectionBox2NumStatus;				// Coin������2����״̬

	STATUS_MAGAZINE				m_sBanknoteChangeBox1Status;				// ֽ��������1��װ״̬
	MAGAZINE_NUM_STATUS			m_sBanknoteChangeBox1NumStatus;				// ֽ��������1����״̬
	STATUS_MAGAZINE				m_sBanknoteChangeBox2Status;				// ֽ��������2��װ״̬
	MAGAZINE_NUM_STATUS			m_sBanknoteChangeBox2NumStatus;				// ֽ��������2����״̬

	MAGAZINE_NUM_STATUS			m_sBanknoteRE1NumStatus;					// ֽ��ѭ����1����״̬
	MAGAZINE_NUM_STATUS			m_sBanknoteRE2NumStatus;					// ֽ��ѭ����2����״̬
	MAGAZINE_NUM_STATUS			m_sBanknoteRE3NumStatus;					// ֽ��ѭ����3����״̬
	MAGAZINE_NUM_STATUS			m_sBanknoteRE4NumStatus;					// ֽ��ѭ����4����״̬

	STATUS_MAGAZINE				m_sBanknoteCollectionBoxStatus;				// ֽ�һ����䰲װ״̬
	MAGAZINE_NUM_STATUS			m_sBanknoteCollectionBoxNumStatus;			// ֽ�һ���������״̬

	SECURITY_DOOR_STATUS				m_sSecurityDoorStatus;				// �ֽ�ȫ��״̬���ݲ�ʹ�ã�
	TOUCH_SCREEN_STATUS				m_sTouchScreenStatus;					// ������״̬���ݲ�ʹ�ã�
	ASSISTANCE_BUTTON_STATUS		m_sAssistanceButtonStatus;				// ��Ԯ��ť״̬
	
	STATUS_RW									m_sRechargeRWStatus;		// ��ֵ��д��״̬
	STATUS_RW									m_sCardRWStatus;			// Card��д��״̬
	STATUS_RW									m_sTokenRWStatus;			// Token��д��״̬

	SAM_STATUS								m_sSam1Type;				// SAM1����(��ֵ��д��һƱͨ ISAM)���ݲ�ʹ�ã�
	SAM_STATUS								m_sSam2Type;				// SAM2����(Card��д��һƱͨ ISAM)���ݲ�ʹ�ã�
	SAM_STATUS								m_sSam3Type;				// SAM3����(Token��д��һƱͨ ISAM)���ݲ�ʹ�ã�

	SAM_AUTH_STATUS							m_sSAM1Status;				// SAM1״̬(��ֵ��д��һƱͨ ISAM)
	SAM_AUTH_STATUS							m_sSAM2Status;					// SAM2״̬(Card��д��һƱͨ ISAM)
	SAM_STATUS								m_sTokenRWYPTISAMStatus;				// SAM3״̬(Token��д��һƱͨ ISAM)
	SAM_STATUS								m_sRechargeRWYKTISAMStatus;				// SAM4״̬(��ֵ��д��һ��ͨISAM)(�ݲ�ʹ��)

	PRINTER_STATUS							m_sPrintStatus;						// ��ӡ��״̬
	UPS_STATUS									m_sUPSStatus;						// UPS״̬
	DOOR_STATUS								m_sMaintenanceDoorStatus;	// ά����״̬

	HARDWARE_MODULE_STATUS     m_sBanknoteModuleStatus;		 // ֽ��ģ��״̬
	HARDWARE_MODULE_STATUS	   m_sBanknoteChangeModuleStatus;// ֽ������ģ��״̬
	HARDWARE_MODULE_STATUS     m_sTokenModuleStatus;		 // Tokenģ��״̬
	HARDWARE_MODULE_STATUS     m_sCardModuleStatus;			 // Cardģ��״̬
	HARDWARE_MODULE_STATUS	    m_sCoinModuleStatus;	 	 // Ӳ��ģ��״̬
	HARDWARE_MODULE_STATUS	    m_sStatusDisplayStatus;		 // ״̬��ʾ��״̬
	TILT_STATUS									m_sTiltStatus;					// ��б������״̬(�𶯴�����״̬)
	BYTE										m_sOffLineDays;					// ��������
	AUTHORIZED_STATUS					m_sAuthorizedAccessStatus; // ��Ȩ����״̬ 
	
	// ����ģʽ����
	DEVICE_CONTRL_CODE				m_sDegradedMode;				// ��������ģʽ
	//CGetIni  *pINI;
	WORD GetStatusFromINI(CString strSection);

	bool IsSCConnected();

public:
	// ����ģʽ
	WORK_MODE GetDegreeWorkMode();
};