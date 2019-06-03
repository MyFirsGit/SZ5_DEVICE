#pragma once
#include "afx.h"
#include "CoreExp.h"
#include "Bag.h"

/**
@brief	异常祖先类
*/
class CORE_API CSysException
{

public:
    CSysException(long moduleID, long detailID, CString fileName, long lineNo, CString message =  CString(_T("")));
    virtual ~CSysException();
    CSysException(const CSysException& that);

    long GetModuleID();                         // 获取模块编码
    long GetDetailID();                         // 获取模块内详细编码
    CString GetFileName();                      // 获取异常发生时文件名
    long GetLineNo();                           // 获取异常发生时行号
    CString GetMessage();                       // 获取附加错误信息
    COleDateTime GetOccurTime();                // 获取异常发生时间
	CBag& GetBag(); 
private:
    long m_moduleID;                            // 模块编码
    long m_detailID;                            // 模块内详细编码
    CString m_fileName;                         // 异常发生时文件名
    long m_lineNo;                              // 异常发生时行号
    CString m_message;                          // 附加错误信息
    COleDateTime m_occurTime;                   // 发生异常时间
	CBag m_Bag;
};
