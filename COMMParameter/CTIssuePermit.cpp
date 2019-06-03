#include "StdAfx.h"
#include "CTIssuePermit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCTIssuePermit::CCTIssuePermit(void)
{
	m_IssueStationList.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTIssuePermit::~CCTIssuePermit(void)
{
	try{

	}

	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}        
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加发售限制车站

@param      unsigned short

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTIssuePermit::AddIssueStationPermit(unsigned short usStation)
{
	try{
		CString sStation(_T(""));
		sStation.Format(_T("%d"),usStation);
		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,sStation + _T("<<"));
		bool bExist = false;
		vector<unsigned short>::iterator vIter;
		for(vIter = m_IssueStationList.begin(); vIter!=m_IssueStationList.end();vIter++){
			// 比较卡号是否相等
			if(usStation == *vIter){
				bExist = true;
				break;
			}
		}
		if (!bExist)
		{
			m_IssueStationList.push_back(usStation);
			theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,sStation + _T(">>"));
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除发售限制车站

@param      unsigned short

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTIssuePermit::DeleteIssueStation(unsigned short usStation)
{
	try{
		vector<unsigned short>::iterator vIter;
		for(vIter = m_IssueStationList.begin(); vIter!=m_IssueStationList.end();vIter++){
			// 比较卡号是否相等
			if(usStation == *vIter){
				m_IssueStationList.erase(vIter);
				break;
			}
		}
		CString sStation("");
		sStation.Format(_T("%d"),usStation);
		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,sStation);
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否是发售限制车站

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTIssuePermit::IsPermitIssueStation(unsigned short usStation)
{
	try{
		bool bExist = false;
		vector<unsigned short>::iterator vIter;
		for(vIter = m_IssueStationList.begin(); vIter!=m_IssueStationList.end();vIter++){
			// 比较卡号是否相等
			if(usStation == *vIter){
				bExist = true;
				break;
			}
		}
		//CString sStation("");
		//sStation.Format("%d-%d",usStation,bExist);
		//theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,sStation + "<<");
		return bExist;
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      解除所有禁止发售车站

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTIssuePermit::DeleteAllIssueStation()
{
	try{
		m_IssueStationList.clear();
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}
CCTIssuePermit CCTIssuePermit::theInstance;