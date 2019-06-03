#include "StdAfx.h"
#include "SysException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

/**
@brief	异常祖先类
*/

//IMPLEMENT_DYNAMIC(CSysException, CSysException)

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)long moduleID       模块编码
@param      (i)long detailID,      模块内详细编码
@param      (i)CString fileName    异常发生时文件名
@param      (i)long lineNo         异常发生时行号
@param      (i)CString message     错误信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSysException::CSysException(long moduleID, long detailID, CString fileName, 
                             long lineNo, CString message)
{
    m_moduleID = moduleID;
    m_detailID = detailID;
    m_fileName = fileName;
    m_lineNo = lineNo;
    m_message = message;
	m_occurTime = COleDateTime::GetTickCount();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Copy构造函数

@param      (i)const CSysException& that

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSysException::CSysException(const CSysException& that)
{
	if(&that != this)
	{
			m_moduleID= that.m_moduleID;
			m_detailID=that.m_detailID;
			m_fileName=that.m_fileName;
			m_lineNo=that.m_lineNo;
			m_message=that.m_message;
			m_occurTime=that.m_occurTime;
			m_Bag=that.m_Bag;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSysException::~CSysException()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取模块编码

@param      无

@retval     long 模块编码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CSysException::GetModuleID()
{
    return m_moduleID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取模块内详细编码

@param      无

@retval     long    模块内详细编码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CSysException::GetDetailID()
{
    return m_detailID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取异常发生时文件名

@param      无

@retval     CString 异常发生时文件名

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CSysException::GetFileName()
{
    return m_fileName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取异常发生时行号

@param      无

@retval     long    异常发生时行号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CSysException::GetLineNo()
{
    return m_lineNo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取错误附加信息

@param      无

@retval     CString 错误附加信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CSysException::GetMessage()
{
    return m_message;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取异常发生时间

@param      无

@retval     _DATE_TIME, 时间

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COleDateTime CSysException::GetOccurTime()
{
    return m_occurTime;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    获得容器袋，用于对象生成后附加属性值  

@param    无

@retval     CBag& 容器袋

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CBag& CSysException::GetBag()
{
	return m_Bag;
}
