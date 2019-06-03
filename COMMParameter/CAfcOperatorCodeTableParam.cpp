#include "StdAfx.h"
#include "CAfcOperatorCodeTableParam.h"
//#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAfcOperatorCodeTableParam CAfcOperatorCodeTableParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAfcOperatorCodeTableParam::CAfcOperatorCodeTableParam(void):CAfcParamMaster()
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
CAfcOperatorCodeTableParam::~CAfcOperatorCodeTableParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAfcOperatorCodeTableParam::GetParamFileName()
{
	//GetParamFileNameSub(AFC_PARAM_ID_1097);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcOperatorCodeTableParam::ParseBody(LPBYTE lpData)
{
    try{

    }
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出分段信息

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAfcOperatorCodeTableParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		//if(m_packageHead.sectionCount != 0){			
		//	for(int i =0;i<m_packageHead.sectionCount;i++){
		//		m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//		lpData+=4;
		//		m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
		//		lpData+=4;
		//	}
		//}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查电文结构调整部分内容正确性

@param      (i)LPBYTE           电文信息

@retval     bool   true:正确  false：不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAfcOperatorCodeTableParam::CheckBodyInfo(LPBYTE lpData)
{   
    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得运营时间参数

@param      (o)OPRATION_TIME& operationTm   运营时间参数

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CAfcOperatorCodeTableParam::GetOperationTimeParam(OPRATION_TIME& operationTm)
//{
//	operationTm = m_operation_param;
//}