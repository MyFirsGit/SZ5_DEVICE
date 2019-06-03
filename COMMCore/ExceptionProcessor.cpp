#include "StdAfx.h"
#include "exceptionprocessor.h"
#include "SysException.h"
#include "InnerException.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionProcessor::CExceptionProcessor()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionProcessor::~CExceptionProcessor()
{
	m_exceptionArr.clear();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      添加异常记录

@param      (i)CSysException& e
@param      (i)bool needSendStatus   是否需要发送状态电文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::AddException(CSysException& e)
{
	
	m_exceptionArr.push_back(e);	
	ExceptionItemChanged.Invoke(e,true);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除异常记录

@param      (i)long m_moduleID    模块编码
@param      (i)long detailID      模块内详细编码

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::RemoveException(long moduleID, long detailID)
{
	vector<CSysException>::iterator iter = m_exceptionArr.begin();
	while (iter != m_exceptionArr.end()) {
		// 异常详细编码为NO_DETAIL_ID时，不使用详细编码
		if ((*iter).GetModuleID() == moduleID && (detailID == NO_DETAIL_ID
			|| detailID != NO_DETAIL_ID && (*iter).GetDetailID() == detailID)) {
				CSysException newException = *iter;
				iter = m_exceptionArr.erase(iter);
				ExceptionItemChanged.Invoke(newException,false);
		}
		else {
			iter++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定异常以外的模块内所有异常记录

@param      (i)long m_moduleID    模块编码
@param      (i)long detailID      不移除的异常记录

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::RemoveExceptionExcept(long moduleID, long detailID)
{
	vector<CSysException>::iterator iter = m_exceptionArr.begin();
	while (iter != m_exceptionArr.end()) {
		if ((*iter).GetModuleID() == moduleID && (*iter).GetDetailID() != detailID){
			CSysException newException = *iter;
			iter = m_exceptionArr.erase(iter);
			ExceptionItemChanged.Invoke(newException,false);
		}
		else {
			iter++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有异常

@param      无

@retval     bool    true: 有异常, false:无异常

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::HasException()
{
	return (! m_exceptionArr.empty());
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有指定类别的异常

@param      (i)long moduleID    模块编码

@retval     bool    true: 有异常, false:无异常

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::HasException(long moduleID)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		if(iter->GetModuleID() == moduleID)
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否有指定的异常

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::HasException(vector<long>& moduleIDs)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		vector<long>::iterator position = find(moduleIDs.begin(),moduleIDs.end(),iter->GetModuleID());
		if(position != moduleIDs.end())
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否只有特定的异常

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::OnlyHasException(long moduleID)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		if(iter->GetModuleID() != moduleID)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否只有特定的异常

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::OnlyHasException(vector<long>& moduleIDs)
{
	// 無異常
	if(m_exceptionArr.size() == 0){
		return false;
	}
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		vector<long>::iterator position = find(moduleIDs.begin(),moduleIDs.end(),iter->GetModuleID());
		if(position != moduleIDs.end())
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有指定类别指定详细错误的异常

@param      (i)long moduleID    模块编码

@retval     bool    true: 有异常, false:无异常

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::HasException(long moduleID, long detailID)
{
	bool found = false;
	vector<CSysException>::iterator iter = m_exceptionArr.begin();
	while (iter != m_exceptionArr.end()) {
		if ((*iter).GetModuleID() == moduleID && (*iter).GetDetailID() == detailID) {
			found = true;
			break;
		}
		iter++;
	}
	return found;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取最后一个异常对象

@param      无

@retval     CSysException&      最后一个异常对象的引用

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CSysException& CExceptionProcessor::GetLastException()
{
	if (HasException()) {
		CSysException& lastException = m_exceptionArr.back();
		return lastException;
	}
	else {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::FUNC_ERR_RETURN, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理异常

@param      (i)CSysException& e

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::ProcessException(CSysException& e)
{
	AddException(e);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      故障解除(不能解除非法更换票箱的异常)

@param      无

@retval     bool    true:成功;  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::RepareException()
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      得到所有的异常

@param      无

@retval     const vector<CSysException>&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CSysException>& CExceptionProcessor::GetAllException()
{
	return m_exceptionArr;
}


