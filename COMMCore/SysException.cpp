#include "StdAfx.h"
#include "SysException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]= __FILE__;
#endif

/**
@brief	�쳣������
*/

//IMPLEMENT_DYNAMIC(CSysException, CSysException)

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)long moduleID       ģ�����
@param      (i)long detailID,      ģ������ϸ����
@param      (i)CString fileName    �쳣����ʱ�ļ���
@param      (i)long lineNo         �쳣����ʱ�к�
@param      (i)CString message     ������Ϣ

@retval     ��

@exception  ��
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
@brief      Copy���캯��

@param      (i)const CSysException& that

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSysException::~CSysException()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡģ�����

@param      ��

@retval     long ģ�����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CSysException::GetModuleID()
{
    return m_moduleID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡģ������ϸ����

@param      ��

@retval     long    ģ������ϸ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CSysException::GetDetailID()
{
    return m_detailID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�쳣����ʱ�ļ���

@param      ��

@retval     CString �쳣����ʱ�ļ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSysException::GetFileName()
{
    return m_fileName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�쳣����ʱ�к�

@param      ��

@retval     long    �쳣����ʱ�к�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CSysException::GetLineNo()
{
    return m_lineNo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���󸽼���Ϣ

@param      ��

@retval     CString ���󸽼���Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSysException::GetMessage()
{
    return m_message;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�쳣����ʱ��

@param      ��

@retval     _DATE_TIME, ʱ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COleDateTime CSysException::GetOccurTime()
{
    return m_occurTime;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��������������ڶ������ɺ󸽼�����ֵ  

@param    ��

@retval     CBag& ������

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CBag& CSysException::GetBag()
{
	return m_Bag;
}
