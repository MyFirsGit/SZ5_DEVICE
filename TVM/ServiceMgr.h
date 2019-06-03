#pragma once
#include "CommonDefine.h"
#include "servicedefinition.h"
#include "service.h"
#include "tvmforeservice.h"
#include "delegate.h"

#define theSERVICE_MGR CServiceMgr::GetInstance()


/**
@brief Service管理
*/
class CServiceMgr
{
public:
	
	delegate<void (CService*)>  StartingService;//将要启动Service事件
	delegate<void (CService*)>  StartedService;//启动Service完成事件
	
	delegate<void (CService*)>  StopingService;//将要停止Service事件
	delegate<void (CService*)>  StopedService;//Service停止

	delegate<void (SERVICE_STATE,SERVICE_STATE)> StateChanged;//状态改变

    static CServiceMgr& GetInstance();                          // 获取CServiceMgr类的实例
    void StartService(SERVICE_ID serviceID, bool post = true);  // 启动Service
    void StopService(SERVICE_ID serviceID, bool post = false);  // 停止Service
    CService* GetCurService();                                  // 获取当前Service(前台)
    void RestartCurService();                                   // 重新启动当前Service(前台)
    CService* GetService(SERVICE_ID serviceID);                 // 根据Service ID获取Service对象
	template<class T>
	T* GetService(SERVICE_ID serviceID)
	{
		return (T*)GetService(serviceID);
	};
    void NotifyFinish();                                        // 终止本Service, 启动默认Service
	void SetForeServiceBusy(bool bBusy);                        // 把当前画面变为忙碌
	bool IsForeServiceBusy();									// 判断当前是否为忙碌状态 
    SERVICE_STATE GetState();                                   // 获取业务状态
    void SetState(SERVICE_STATE state);                         // 设置业务状态
	
	~CServiceMgr();                                             // 析构函数
private:
    CServiceMgr();                                              // 构造函数


    void StartService(CService* pService);                      // 启动Service
    CService* CreateService(SERVICE_ID serviceID);              // 创建Service
    CService* SearchService(SERVICE_ID serviceID);              // 在ServiceTree中根据ServiceID查找Service
    void StopService(CService* pService);                       // 停止Service
	
	void OnShowingMessageBox();

    static CServiceMgr theInstance;                             // 该类的唯一实例
    CService* m_pRootService;                                   // Root of service tree
    CService* m_pCurService;                                    // 当前Service(前台)
    SERVICE_STATE m_serviceState;				                // 业务状态
	bool m_ServiceBusy;
};
