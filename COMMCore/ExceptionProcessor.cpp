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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionProcessor::CExceptionProcessor()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionProcessor::~CExceptionProcessor()
{
	m_exceptionArr.clear();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����쳣��¼

@param      (i)CSysException& e
@param      (i)bool needSendStatus   �Ƿ���Ҫ����״̬����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::AddException(CSysException& e)
{
	
	m_exceptionArr.push_back(e);	
	ExceptionItemChanged.Invoke(e,true);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ɾ���쳣��¼

@param      (i)long m_moduleID    ģ�����
@param      (i)long detailID      ģ������ϸ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::RemoveException(long moduleID, long detailID)
{
	vector<CSysException>::iterator iter = m_exceptionArr.begin();
	while (iter != m_exceptionArr.end()) {
		// �쳣��ϸ����ΪNO_DETAIL_IDʱ����ʹ����ϸ����
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
@brief      ɾ��ָ���쳣�����ģ���������쳣��¼

@param      (i)long m_moduleID    ģ�����
@param      (i)long detailID      ���Ƴ����쳣��¼

@retval     ��

@exception  ��
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
@brief      �Ƿ����쳣

@param      ��

@retval     bool    true: ���쳣, false:���쳣

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::HasException()
{
	return (! m_exceptionArr.empty());
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ���ָ�������쳣

@param      (i)long moduleID    ģ�����

@retval     bool    true: ���쳣, false:���쳣

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
@brief      �Ƿ���ָ�����쳣

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
@brief      �Ƿ�ֻ���ض����쳣

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
@brief      �Ƿ�ֻ���ض����쳣

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::OnlyHasException(vector<long>& moduleIDs)
{
	// �o����
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
@brief      �Ƿ���ָ�����ָ����ϸ������쳣

@param      (i)long moduleID    ģ�����

@retval     bool    true: ���쳣, false:���쳣

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
@brief      ��ȡ���һ���쳣����

@param      ��

@retval     CSysException&      ���һ���쳣���������

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
@brief      �����쳣

@param      (i)CSysException& e

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionProcessor::ProcessException(CSysException& e)
{
	AddException(e);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ͻ��(���ܽ���Ƿ�����Ʊ����쳣)

@param      ��

@retval     bool    true:�ɹ�;  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionProcessor::RepareException()
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ����е��쳣

@param      ��

@retval     const vector<CSysException>&

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CSysException>& CExceptionProcessor::GetAllException()
{
	return m_exceptionArr;
}


