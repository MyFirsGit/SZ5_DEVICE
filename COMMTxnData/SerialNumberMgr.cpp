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
@brief      构造函数

@param      无

@retval     无

@exception  无
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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSerialNumberMgr::~CSerialNumberMgr(void)
{
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      获得打包序列号

@param      (i)_DATE_TIME dateTime时间戳

@retval     WORD 打包序列号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CSerialNumberMgr::GetSelNo(_DATE_TIME dateTime)
{
	WORD ret = 1;
	// 进入临界区
	m_criticalSection.Lock();
	//if(0 != ComCheckDateTimeValid(dateTime)){
	//	// 退出临界区
	//	m_criticalSection.Unlock();
	//	throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__,"时间戳非法");
	//}
	
	//时间戳相等
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
	// 退出临界区
	m_criticalSection.Unlock();

	return mstos(ret);
}

