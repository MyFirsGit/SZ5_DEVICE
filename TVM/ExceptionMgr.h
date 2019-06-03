#pragma once
#include "Service.h"
#include "SysException.h"
#include "ExceptionLog.h"
#include <map>
#include "ServiceDefinition.h"
#include "ExceptionInfo.h"
#include "exceptionprocessor.h"

using namespace std;

#define theEXCEPTION_MGR CExceptionMgr::GetInstance()

/**
@brief      异常管理类定义
*/
class CExceptionMgr : public CExceptionProcessor
{
public:
	
	static CExceptionMgr& GetInstance();                           // 获取该类的对象
	
	bool OnlyHasHardwareException();                                // 是否只有硬件异常
	bool HasMaintainException();                                        // 是否有维护异常
	
	// 写异常日志
	void WriteExceptionLog(CSysException& e,CExceptionLog::EXCEPTION_LEVEL exLevel = CExceptionLog::LEVEL_WARNING);  
	virtual void ProcessException(CSysException& e);				   // 处理异常
	virtual bool RepareException(bool bMenualRepare = false);          // 异常解除
	unsigned long GetMTC(CSysException&) const;				    // 获取MTC码(不含服务状态码)
	unsigned short GetEC(CSysException&) const;						// 获取EC码
	unsigned long GetActualMTC(CSysException&) const;			// 获取MTC码(含服务状态码)
	SERVICE_STATE GetServiceState(CSysException&) const;       // 获取异常对象中服务状态码
	CString GetDescription(CSysException&) const;						// 获取异常描述
	CString GetPrompt(CSysException&) const;							// 获取异常处理建议
	
	BYTE GetDevicePartTypeOfException(CSysException& e);

private:
	CExceptionMgr();                                            // 构造函数
	virtual ~CExceptionMgr();                                   // 析构函数
	bool m_isBusy;												// 是否有异常正在处理	
	static CExceptionMgr theInstance;                           // 该类的唯一接口
	void ResetTimesOfRepareException(); // 重置自动异常修复限制次数
	int m_nTimesRepareRWRecharge;		// 充值读卡器解除异常次数						
	int m_nTimesRepareRWCard;			// 票卡读卡器异常解除次数
	int m_nTimesRepareRWToken;			// Token读卡器异常解除次数
	int m_nTimesReparePrint;			// 打印机异常解除次数
	int m_nTimesRepareSNTP;				// NTP对时服务异常解除次数
	int m_nTimesRepareFTP;				// FTP异常解除次数
	int m_nTimesRepareBankcard;			// 银行卡异常解除次数
	int m_nTimesRepareBanknote;			// 纸币模块异常解除次数
	int m_nTimesRepareBanknoteChange;	// 纸币找零模块异常解除次数
	int m_nTimesRepareCard;				// 票卡模块异常解除次数
	int m_nTimesRepareCoin;				// 硬币模块异常解除次数
	int m_nTimesRepareCoinShutter;		// 硬币投币口异常解除次数
	int m_nTimesRepareIO;				// IO异常解除次数
	int m_nTimesReparePinPad;			// 密码键盘异常解除次数
	int m_nTimesRepareDisplay;			// 显示屏异常解除次数
	int m_nTimesRepareToken;			// Token模块异常解除次数
	int m_nTimerRepareRFID;				// 纸币找零RFID
};
