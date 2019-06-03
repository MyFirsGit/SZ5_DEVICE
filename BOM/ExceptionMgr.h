#pragma once
#include "Service.h"
#include "SysException.h"
#include "ExceptionLog.h"
#include <map>
#include "ServiceDefinition.h"
#include "GlobalParameter.h"
using namespace std;

#define theEXCEPTION_MGR CExceptionMgr::GetInstance()

/**
@brief      异常管理
*/
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
class CExceptionMgr
{
public:
	delegate<void(CSysException &,bool)> ExceptionItemChanged;	//异常队列发生变化事件
	static CExceptionMgr& GetInstance();                                // 获取该类的对象
	void AddException(CSysException& e,bool needSendStatus = true);     // 记录异常数据
	void RemoveException(long moduleID,long detailID = NO_DETAIL_ID);  // 根据模块编码与模块内详细编码删除异常对象
	void RemoveExceptionExcept(long moduleID,long detailID);			// 删除指定异常以外的模块内所有异常记录
	bool HasException();                                                // 是否有异常
	bool HasException(long moduleID);                                   // 是否有指定类别的异常
	bool HasException(vector<long>& moduleIDs);			// 是否有特定类型的异常
	bool OnlyHasHardwareException();                                    // 是否只有硬件异常
	bool OnlyHasException(long moduleID);							//是否只有特定的异常
	bool OnlyHasException(vector<long>& moduleIDs);		//是否只有特定的异常
	bool HasException(long moduleID, long detailID);                    // 是否有指定类别指定详细错误的异常
	bool HasMaintainException();                                        // 是否有维护异常
	CSysException& GetLastException();                                  // 获取最后一个异常对象
	void WriteExceptionLog(CSysException& e,CExceptionLog::EXCEPTION_LEVEL exLevel = CExceptionLog::LEVEL_WARNING);  // 写入异常Log
	void ProcessException(CSysException& e);							// 处理异常
	bool RepareException();                                             // 故障解除
	unsigned long GetMTC(CSysException&) const;
	unsigned short GetEC(CSysException&) const;
	unsigned long GetActualMTC(CSysException&) const;
	SERVICE_STATE GetServiceState(CSysException&) const;
	CString GetDescription(CSysException&) const;
	CString GetPrompt(CSysException&) const;
	const vector<CSysException>& GetAllException();                     // 得到所有的异常
	DEVICE_COMPONENT GetDevicePartTypeOfException(CSysException& e);
private:
	CExceptionMgr();                                                    // 构造函数
	~CExceptionMgr();                                                   // 析构函数
	
	CSysException RemoveLastException();                                // 删除并返回最后一个异常对象
	const static long NO_DETAIL_ID = 0;                                 // 无详细编码
	vector<CSysException> m_exceptionArr;	                            // 记录发生的异常对象
	bool m_isBusy;                                                      // 是否有异常正在处理	
	static CExceptionMgr theInstance;                                   // 该类的唯一接口

};
