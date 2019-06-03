#include "StdAfx.h"
#include "SerialNumberMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSerialNumberMgr CSerialNumberMgr::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSerialNumberMgr::CSerialNumberMgr(void)
{
	//m_pekgDateTime.wYear = 2010;
	//m_pekgDateTime.biMonth = 1;
	//m_pekgDateTime.biDay = 4;
	//m_pekgDateTime.biHour = 10;
	//m_pekgDateTime.biMinute = 0;
	//m_pekgDateTime.biSecond = 0;
	m_pekgSelNo = 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSerialNumberMgr::~CSerialNumberMgr(void)
{
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ô�����к�

@param      (i)_DATE_TIME dateTimeʱ���

@retval     WORD ������к�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CSerialNumberMgr::GetSelNo(_DATE_TIME dateTime)
{
	WORD ret = 1;
	// �����ٽ���
	m_criticalSection.Lock();
	//if(0 != ComCheckDateTimeValid(dateTime)){
	//	// �˳��ٽ���
	//	m_criticalSection.Unlock();
	//	throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__,"ʱ����Ƿ�");
	//}
	
	//ʱ������
	//if(CmpTwoDateTime(dateTime,m_pekgDateTime) == 0){
	//	m_pekgSelNo ++;				
	//}
	//else{
	//	m_pekgDateTime = dateTime;
	//	m_pekgSelNo = 1;
	//}	

	m_pekgSelNo ++;	
	ret = m_pekgSelNo;
	if(m_pekgSelNo >= 0xFFFF){
		m_pekgSelNo = 0;
	}
	// �˳��ٽ���
	m_criticalSection.Unlock();

	return mstos(ret);
}

