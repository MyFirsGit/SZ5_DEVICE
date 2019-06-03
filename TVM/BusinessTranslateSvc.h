#pragma once
#include "BackService.h"
#include "ParamHelper.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include <list>
#include "SCMessageContentDefine.h"
using namespace std;

/**
@brief 定期集计Service
*/
class CBusinessTranslateSvc :
	public CBackService
{
	DECLARE_MESSAGE_MAP()

public:
	CBusinessTranslateSvc(void);
	~CBusinessTranslateSvc(void);

public:
	void StartBusinessIntervalTimer();
	void EndBusinessIntervalTimer();
	void InsertMsgUpdateParameterAndProgram(CParamHelper::VEC_VERSION_INFO& Vec_VersionInfo);		// 6.4.2  参数和程序更新
	void InsertMsgDownloadParameterAndProgram(vector<ParameterAndSoftwareUpdate>&);  				// 6.4.3  参数和程序下载业务数据
	void InsertMsgImportantStatusChange(SERVICE_STATUS status);  									// 6.4.4  设备重要状态改变	
	//void InsertMsgIsamCheckIn(ISAM_AUTH isam_auth, BYTE result);									// 6.4.5  ISAM签到/签退
	void InsertMsgOperationLog(OPERATOR_LOG operLog);												// 6.4.6  操作日志(不推荐使用)
	void InsertMsgOperationLog(OPERATION_CODE operCode,bool operResult = true,CString strOperComment = _T("")); // 6.4.6	操作日志
	void InsertMsgDeviceErrorData(DEVICE_ERRORDATA& ErrorData);										// 6.4.7  设备故障数据
	void InsertMsgDeviceRegisterData(AR_REASON arReson);											// 6.4.8  设备寄存器数据
	void InsertMsgMagazineExchange(MAGAZINE_CHANGE& magazine_change);								// 6.4.10 票箱更换
	void InsertMsgBanknoteBoxExchange(CASHBOX_CHANGE& cashbox_change);								// 4.2.16 钱箱更换
	//void InsertMsgDeviceComponentInfo();															// 6.4.11 设备部件构成
	void InsertMsgBussnessFailedData(BUSINESS_FAILED_DATA inBusiness_data);							// TVM故障单业务数据

	long SendBusinessData();
protected:
	void OnStart();
	bool OnStop();
private:
	enum ENUM_TIMER {
		TIMER_REGISTER = 1,         // 业务数据寄存器定时器
		TIMER_PACKAGE = 2			//打包时间定时器
	};

	void OnTimer(UINT nEventID);
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnEndTimer(WPARAM,LPARAM);
	
};
