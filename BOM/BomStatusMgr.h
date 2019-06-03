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
    static CBomStatusMgr& GetInstance();            // 获取该类的对象
	
	void SetServiceStatus(SERVICE_STATUS );         // 设置服务状态         
	SERVICE_STATUS GetServiceStatus();              // 获取服务状态

	void SetRunStatus(RUN_STATUS);                  // 设置运行状态
	RUN_STATUS GetRunStatus();                      // 获取运行状态

	void SetWorkMode(WORK_MODE, bool = false);      // 设置工作模式
	WORK_MODE GetWorkMode();                        // 获取工作模式
	
	void SetRunMode(OperationalMode_t runMode);					// 设置运营模式
	OperationalMode_t GetRunMode();								// 获取运营模式
	CString GetRunModeName();

	void Set24HourMode(BYTE b24HourMode);
	BYTE Get24HourMode();

	void SetDelayTime(WORD delayTime);
	WORD GetDelayTime();

	void SetCommunicate(COMMUNICATION);             // 设置通信状态
	COMMUNICATION GetCommunicate();                 // 获取通信状态
 
	void SetTimeSynStatus(TIME_STATUS);             // 设置时钟同步状态
	TIME_STATUS GetTimeSynStatus();                 // 获取时钟同步状态

	void SetLoginStatus(LOGIN_STATUS);              // 设置登录状态
	LOGIN_STATUS GetLoginStatus();                  // 获取登录状态

	DEVICE_AUTORUN GetAutoRunStatus();              // 获取自动运行

	void SetMagAStatus(STATUS_MAGAZINE);            // 设置A箱Set状态
	STATUS_MAGAZINE GetMagAStatus();                // 获取A箱Set状态
	void SetMagANumStatus(MAGAZINE_NUM_STATUS);     // 设置A箱数量状态
	MAGAZINE_NUM_STATUS GetMagANumStatus();         // 获取A箱数量状态

	void SetMagBStatus(STATUS_MAGAZINE);            // 设置B箱Set状态
	STATUS_MAGAZINE GetMagBStatus();                // 获取B箱Set状态
	void SetMagBNumStatus(MAGAZINE_NUM_STATUS);     // 设置B箱数量状态
	MAGAZINE_NUM_STATUS GetMagBNumStatus();         // 获取B箱数量状态

	void SetCycleBoxStatus(STATUS_MAGAZINE);        // 设置回收箱Set状态
	STATUS_MAGAZINE GetCycleBoxStatus();            // 获取回收箱Set状态
	void SetCycleBoxNumStatus(MAGAZINE_NUM_STATUS);  // 设置回收数量状态
	MAGAZINE_NUM_STATUS GetCycleBoxNumStatus();     // 获取回收箱数量状态

	void SetInnerRWStatus(STATUS_RW);               // 设置内部RW状态
	STATUS_RW GetInnerRWStatus();                   // 获取内部RW状态

	void SetOuterRWStatus(STATUS_RW);               // 设置外部RW状态
	STATUS_RW GetOuterRWStatus();                   // 获取内部RW状态

	void SetSam1Type(SAM_TYPE_);                    // 设置Sam1类型
	SAM_TYPE_ GetSam1Type();                        // 获取Sam1类型
 
	void SetSam2Type(SAM_TYPE_);                    // 设置Sam2类型
	SAM_TYPE_ GetSam2Type();                        // 获取Sam2类型

	void SetSam3Type(SAM_TYPE_);                    // 设置Sam3类型
	SAM_TYPE_ GetSam3Type();                        // 获取Sam3类型

	void SetSam1Status(SAM_STATUS);                 // 设置Sam1认证状态
	SAM_STATUS GetSam1Status();                     // 获取Sam1认证状态

	void SetSam2Status(SAM_STATUS);                 // 设置Sam2认证状态
	SAM_STATUS GetSam2Status();                     // 获取Sam2认证状态

	void SetSam3Status(SAM_STATUS);                 // 设置Sam3认证状态
	SAM_STATUS GetSam3Status();                     // 获取Sam3认证状态

	void SetDoorStatus(DOOR_STATUS);                // 设置维修门状态
	DOOR_STATUS GetDoorStatus();                    // 获取维修门状态

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
	SERVICE_STATUS       m_sServiceStatus;     // 服务状态
	RUN_STATUS				m_sRunStatus;         // 运行状态
	WORK_MODE             m_sWorkMode;          // 工作模式
	OperationalMode_t								m_sRunMode;				// 运营模式，参见7.9运营模式代码。
	BYTE								m_s24HourMode;		// 24小时运营模式
	WORD								m_sDelayTime;			// 延长运营时间									
	COMMUNICATION         m_sConnectStatus;     // 连接状态
	TIME_STATUS           m_sTimeStatus;        // 时钟差异
	LOGIN_STATUS          m_sLoginStatus;       // 登录状态
	DEVICE_AUTORUN        m_sAutorunStatus;     // 设备自动运行状态
	STATUS_MAGAZINE       m_sMagazineAStatus;   // 票箱1安装状态
	MAGAZINE_NUM_STATUS   m_sMagANumStatus;     // 票箱1数量状态
	STATUS_MAGAZINE       m_sMagazineBStatus;   // 票箱2安装状态
	MAGAZINE_NUM_STATUS   m_sMagBNumStatus;     // 票箱2数量状态
	STATUS_MAGAZINE       m_sCycleBoxStatus;    // 废票箱安装状态
	MAGAZINE_NUM_STATUS   m_sCycleBoxNumStatus; // 废票箱数量状态
	STATUS_RW             m_sRW1Status;         // 读写器1状态(外部读写器)
	STATUS_RW             m_sRW2Status;         // 读写器2状态(内部读写器)
	SAM_TYPE_             m_sSam1Type;          // SAM1类型(桌面YPT)
	SAM_TYPE_             m_sSam2Type;          // SAM2类型(桌面YKT)
	SAM_TYPE_             m_sSam3Type;          // SAM3类型(内部YPT)
	SAM_STATUS            m_sSam1Status;        // ISAM1状态
	SAM_STATUS            m_sSam2Status;        // ISAM2状态
	SAM_STATUS            m_sSam3Status;        // ISAM3状态
	DOOR_STATUS           m_sDoorStatus;        // 维修门状态
	PRINTER_STATUS        m_sPrintStatus;       // 打印机状态
	TH_MODULE_STATUS      m_sThModuleStatus;    // TH模块


};