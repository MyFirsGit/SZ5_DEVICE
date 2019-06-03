#pragma once
#include "CommonDefine.h"
#include "servicedefinition.h"
#include "service.h"
#include "bomforeservice.h"
#include "delegate.h"

#define theSERVICE_MGR CServiceMgr::GetInstance()

/**
@brief Service����
*/
class CServiceMgr
{
public:
	
	delegate<void (CService*)>  StartingService;//��Ҫ����Service�¼�
	delegate<void (CService*)>  StartedService;//����Service����¼�
	
	delegate<void (CService*)>  StopingService;//��ҪֹͣService�¼�
	delegate<void (CService*)>  StopedService;//Serviceֹͣ

	delegate<void (SERVICE_STATE,SERVICE_STATE)> StateChanged;//״̬�ı�

    static CServiceMgr& GetInstance();                          // ��ȡCServiceMgr���ʵ��
    void StartService(SERVICE_ID serviceID, bool post = true);  // ����Service
    void StopService(SERVICE_ID serviceID, bool post = false);  // ֹͣService
    CService* GetCurService();                                  // ��ȡ��ǰService(ǰ̨)
    void RestartCurService();                                   // ����������ǰService(ǰ̨)
    CService* GetService(SERVICE_ID serviceID);                 // ����Service ID��ȡService����
	template<class T>
	T* GetService(SERVICE_ID serviceID)
	{
		return (T*)GetService(serviceID);
	};
    void NotifyFinish();                                        // ��ֹ��Service, ����Ĭ��Service
	void SetForeServiceBusy(bool bBusy);                             // �ѵ�ǰ�����Ϊæµ
    SERVICE_STATE GetState();                                   // ��ȡҵ��״̬
    void SetState(SERVICE_STATE state);                         // ����ҵ��״̬
	bool IsRunning();
	void SetRunningStatus(bool isRunning);
	
	~CServiceMgr();                                             // ��������
private:
    CServiceMgr();                                              // ���캯��


    void StartService(CService* pService);                      // ����Service
    CService* CreateService(SERVICE_ID serviceID);              // ����Service
    CService* SearchService(SERVICE_ID serviceID);              // ��ServiceTree�и���ServiceID����Service
    void StopService(CService* pService);                       // ֹͣService
	
	void OnShowingMessageBox();

    static CServiceMgr theInstance;                             // �����Ψһʵ��
    CService* m_pRootService;                                   // Root of service tree
    CService* m_pCurService;                                    // ��ǰService(ǰ̨)
    SERVICE_STATE m_serviceState;				                // ҵ��״̬
	bool m_ServiceBusy;
	bool m_IsRunning;											// BOM����״̬��ֻ����ǿ�Ƶǳ�
};

// BOM����״̬���ã����OK��ťʱ����
typedef struct _running_status
{
	_running_status()
	{
		theSERVICE_MGR.SetRunningStatus(true);
	}
	~_running_status()
	{
		theSERVICE_MGR.SetRunningStatus(false);
	}
}RUNNING_STATUS, *LPRUNNING_STATUS;