#include "StdAfx.h"
#include "CTIssuePermit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCTIssuePermit::CCTIssuePermit(void)
{
	m_IssueStationList.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CCTIssuePermit::~CCTIssuePermit(void)
{
	try{

	}

	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}        
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӷ������Ƴ�վ

@param      unsigned short

@retval     ��

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
			// �ȽϿ����Ƿ����
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
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ���������Ƴ�վ

@param      unsigned short

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCTIssuePermit::DeleteIssueStation(unsigned short usStation)
{
	try{
		vector<unsigned short>::iterator vIter;
		for(vIter = m_IssueStationList.begin(); vIter!=m_IssueStationList.end();vIter++){
			// �ȽϿ����Ƿ����
			if(usStation == *vIter){
				m_IssueStationList.erase(vIter);
				break;
			}
		}
		CString sStation("");
		sStation.Format(_T("%d"),usStation);
		theDEBUG_LOG->WriteData(_T(__FILE__),_T(__FUNCTION__),__LINE__,sStation);
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��Ƿ������Ƴ�վ

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CCTIssuePermit::IsPermitIssueStation(unsigned short usStation)
{
	try{
		bool bExist = false;
		vector<unsigned short>::iterator vIter;
		for(vIter = m_IssueStationList.begin(); vIter!=m_IssueStationList.end();vIter++){
			// �ȽϿ����Ƿ����
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
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CParamException(CParamException::OTHER_ERR, _T(__FILE__), __LINE__);
	}   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������н�ֹ���۳�վ

@param      ��

@retval     ��

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