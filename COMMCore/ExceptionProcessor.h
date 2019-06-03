#include "stdafx.h"
#include "SysException.h"
#include <map>
#include "delegate.h"
#include "CoreExp.h"



//////////////////////////////////////////////////////////////////////////
/*
@brief 异常处理基类      
*/
//////////////////////////////////////////////////////////////////////////
class CORE_API CExceptionProcessor
{
public:
	CExceptionProcessor();                                                    // 构造函数
	virtual ~CExceptionProcessor(); 
	delegate<void(CSysException &,bool)> ExceptionItemChanged;			//异常队列发生变化事件
	
	void RemoveException(long moduleID,long detailID = NO_DETAIL_ID);  // 根据模块编码与模块内详细编码删除异常对象
	void RemoveExceptionExcept(long moduleID,long detailID);			// 删除指定异常以外的模块内所有异常记录
	bool HasException();                                                // 是否有异常
	bool HasException(long moduleID);                                   // 是否有指定类别的异常
	bool HasException(vector<long>& moduleIDs);			// 是否有特定类型的异常
	bool OnlyHasException(long moduleID);							//是否只有特定的异常
	bool OnlyHasException(vector<long>& moduleIDs);		//是否只有特定的异常
	bool HasException(long moduleID, long detailID);                    // 是否有指定类别指定详细错误的异常
	CSysException& GetLastException();                                  // 获取最后一个异常对象
	virtual void ProcessException(CSysException& e);							// 处理异常
	virtual bool RepareException();                                             // 故障解除
	const vector<CSysException>& GetAllException();                     // 得到所有的异常
protected:
	
	void AddException(CSysException& e);     // 记录异常数据
	vector<CSysException> m_exceptionArr;	                            // 记录发生的异常对象
private:	
	const static long NO_DETAIL_ID = 0;                                 // 无详细编码
	
};