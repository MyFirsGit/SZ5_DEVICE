#pragma once
#include "SysException.h"
#include "svcdef.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include "scdatamsg.h"

#define theBOM_STATUS_MGR  CBomStatusMgr::GetInstance()
class CBomStatusMgr
{
public:
	CBomStatusMgr();
	~CBomStatusMgr();
    static CBomStatusMgr& GetInstance();            // ��ȡ����Ķ���
	
	void SetServiceStatus(SERVICE_STATUS );         // ���÷���״̬         
	SERVICE_STATUS GetServiceStatus();              // ��ȡ����״̬

	void SetRunStatus(RUN_STATUS);                  // ��������״̬
	RUN_STATUS GetRunStatus();                      // ��ȡ����״̬

	void SetWorkMode(WORK_MODE, bool = false);      // ���ù���ģʽ
	WORK_MODE GetWorkMode();                        // ��ȡ����ģʽ
	
	void SetRunMode(OperationalMode_t runMode);					// ������Ӫģʽ
	OperationalMode_t GetRunMode();								// ��ȡ��Ӫģʽ
	CString GetRunModeName();

	void Set24HourMode(BYTE b24HourMode);
	BYTE Get24HourMode();

	void SetDelayTime(WORD delayTime);
	WORD GetDelayTime();

	void SetCommunicate(COMMUNICATION);             // ����ͨ��״̬
	COMMUNICATION GetCommunicate();                 // ��ȡͨ��״̬
 
	void SetTimeSynStatus(TIME_STATUS);             // ����ʱ��ͬ��״̬
	TIME_STATUS GetTimeSynStatus();                 // ��ȡʱ��ͬ��״̬

	void SetLoginStatus(LOGIN_STATUS);              // ���õ�¼״̬
	LOGIN_STATUS GetLoginStatus();                  // ��ȡ��¼״̬

	DEVICE_AUTORUN GetAutoRunStatus();              // ��ȡ�Զ�����

	void SetMagAStatus(STATUS_MAGAZINE);            // ����A��Set״̬
	STATUS_MAGAZINE GetMagAStatus();                // ��ȡA��Set״̬
	void SetMagANumStatus(MAGAZINE_NUM_STATUS);     // ����A������״̬
	MAGAZINE_NUM_STATUS GetMagANumStatus();         // ��ȡA������״̬

	void SetMagBStatus(STATUS_MAGAZINE);            // ����B��Set״̬
	STATUS_MAGAZINE GetMagBStatus();                // ��ȡB��Set״̬
	void SetMagBNumStatus(MAGAZINE_NUM_STATUS);     // ����B������״̬
	MAGAZINE_NUM_STATUS GetMagBNumStatus();         // ��ȡB������״̬

	void SetCycleBoxStatus(STATUS_MAGAZINE);        // ���û�����Set״̬
	STATUS_MAGAZINE GetCycleBoxStatus();            // ��ȡ������Set״̬
	void SetCycleBoxNumStatus(MAGAZINE_NUM_STATUS);  // ���û�������״̬
	MAGAZINE_NUM_STATUS GetCycleBoxNumStatus();     // ��ȡ����������״̬

	void SetInnerRWStatus(STATUS_RW);               // �����ڲ�RW״̬
	STATUS_RW GetInnerRWStatus();                   // ��ȡ�ڲ�RW״̬

	void SetOuterRWStatus(STATUS_RW);               // �����ⲿRW״̬
	STATUS_RW GetOuterRWStatus();                   // ��ȡ�ڲ�RW״̬

	void SetSam1Type(SAM_TYPE_);                    // ����Sam1����
	SAM_TYPE_ GetSam1Type();                        // ��ȡSam1����
 
	void SetSam2Type(SAM_TYPE_);                    // ����Sam2����
	SAM_TYPE_ GetSam2Type();                        // ��ȡSam2����

	void SetSam3Type(SAM_TYPE_);                    // ����Sam3����
	SAM_TYPE_ GetSam3Type();                        // ��ȡSam3����

	void SetSam1Status(SAM_STATUS);                 // ����Sam1��֤״̬
	SAM_STATUS GetSam1Status();                     // ��ȡSam1��֤״̬

	void SetSam2Status(SAM_STATUS);                 // ����Sam2��֤״̬
	SAM_STATUS GetSam2Status();                     // ��ȡSam2��֤״̬

	void SetSam3Status(SAM_STATUS);                 // ����Sam3��֤״̬
	SAM_STATUS GetSam3Status();                     // ��ȡSam3��֤״̬

	void SetDoorStatus(DOOR_STATUS);                // ����ά����״̬
	DOOR_STATUS GetDoorStatus();                    // ��ȡά����״̬

	void SetPrintStatus(PRINTER_STATUS);
	PRINTER_STATUS GetPrintStatus();

	void SetTHModuleStatus(TH_MODULE_STATUS);
	TH_MODULE_STATUS GetTHModuleStatus();


private:
	CBomStatusMgr(const CBomStatusMgr&);
	CBomStatusMgr& operator=(const CBomStatusMgr&);
	void OnExceptionItemNotify(CSysException&,bool);
	CSCDataMsg* MakeStatusChangeCommand(BOM_STATUS_ID statusID,BYTE bValue);

	static CBomStatusMgr theInstance;
	SERVICE_STATUS       m_sServiceStatus;     // ����״̬
	RUN_STATUS				m_sRunStatus;         // ����״̬
	WORK_MODE             m_sWorkMode;          // ����ģʽ
	OperationalMode_t								m_sRunMode;				// ��Ӫģʽ���μ�7.9��Ӫģʽ���롣
	BYTE								m_s24HourMode;		// 24Сʱ��Ӫģʽ
	WORD								m_sDelayTime;			// �ӳ���Ӫʱ��									
	COMMUNICATION         m_sConnectStatus;     // ����״̬
	TIME_STATUS           m_sTimeStatus;        // ʱ�Ӳ���
	LOGIN_STATUS          m_sLoginStatus;       // ��¼״̬
	DEVICE_AUTORUN        m_sAutorunStatus;     // �豸�Զ�����״̬
	STATUS_MAGAZINE       m_sMagazineAStatus;   // Ʊ��1��װ״̬
	MAGAZINE_NUM_STATUS   m_sMagANumStatus;     // Ʊ��1����״̬
	STATUS_MAGAZINE       m_sMagazineBStatus;   // Ʊ��2��װ״̬
	MAGAZINE_NUM_STATUS   m_sMagBNumStatus;     // Ʊ��2����״̬
	STATUS_MAGAZINE       m_sCycleBoxStatus;    // ��Ʊ�䰲װ״̬
	MAGAZINE_NUM_STATUS   m_sCycleBoxNumStatus; // ��Ʊ������״̬
	STATUS_RW             m_sRW1Status;         // ��д��1״̬(�ⲿ��д��)
	STATUS_RW             m_sRW2Status;         // ��д��2״̬(�ڲ���д��)
	SAM_TYPE_             m_sSam1Type;          // SAM1����(����YPT)
	SAM_TYPE_             m_sSam2Type;          // SAM2����(����YKT)
	SAM_TYPE_             m_sSam3Type;          // SAM3����(�ڲ�YPT)
	SAM_STATUS            m_sSam1Status;        // ISAM1״̬
	SAM_STATUS            m_sSam2Status;        // ISAM2״̬
	SAM_STATUS            m_sSam3Status;        // ISAM3״̬
	DOOR_STATUS           m_sDoorStatus;        // ά����״̬
	PRINTER_STATUS        m_sPrintStatus;       // ��ӡ��״̬
	TH_MODULE_STATUS      m_sThModuleStatus;    // THģ��


};