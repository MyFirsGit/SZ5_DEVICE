#include "stdafx.h"
#include "CommonFunc.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief   　 取营运日期

@param      (i) unsigned short nBeforeDayNum       当前日期之前的天数

@retval     _DATE  \n
营运日期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE ComGetBusiDate(unsigned short nBeforeDayNum )
{
	__int64 ltime,lBusiStartTime,lBusiDate;
	struct tm tmTime;
	_DATE dtRetDate ;
	BYTE wHour = 0,wMinute = 0,wSecond = 0;

	//当前的日期
	_time64( &ltime );
	GetBusiDateSwitchTime(wHour,wMinute,wSecond);
	//wHour = g_wHour;
	//wMinute = g_wMinute;

	if ( wHour > 23 || wHour < 0) {
		wHour = 0;
	}
	if ( wMinute > 59 || wMinute < 0) {
		wMinute = 0;
	}

	//业务开始时间
	_localtime64_s(&tmTime,&ltime);
	tmTime.tm_sec = (int)wSecond;
	tmTime.tm_min = (int)wMinute;
	tmTime.tm_hour = (int)wHour;
	lBusiStartTime = _mktime64(&tmTime);

	if ( ltime>= lBusiStartTime ) {
		lBusiDate = lBusiStartTime;
	}
	else
	{
		lBusiDate = lBusiStartTime - 24*60*60;
	}
	lBusiDate -= 24*60*60*nBeforeDayNum;
	_localtime64_s(&tmTime,&lBusiDate);

	dtRetDate.wYear = (WORD)(tmTime.tm_year + 1900);
	dtRetDate.biMonth = (BYTE)tmTime.tm_mon + 1;
	dtRetDate.biDay = (BYTE)tmTime.tm_mday;
	return dtRetDate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     设置营运日全局变量的值

@param     BYTE wHour      　　　　　营运日开始的时间（小时）
@param     BYTE wMinute 　　　　　　 营运日开始的时间（分钟）

@retval   _DATE  营运日

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//营运日付变更时刻
//BYTE g_wHour=0 ;   //小时
//BYTE g_wMinute=0 ; //分钟

void ComSetBusiDateSwitchTime(BYTE Hour ,BYTE Minute,BYTE Second)
{
	if (Hour > 23 || Minute > 59 || Second > 59) {
		return ;
	}
	else{
		m_uHour = Hour;
		m_uMinute = Minute;
		m_uSecond = Second;
	}	
	return ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到营运开始时刻

@param      (i)unsigned short& Hour　　小时
@param      (i)unsigned short& Minute　分种
@param      (i)unsigned short& Second　秒

@retval      bool 
true:成功　 false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool GetBusiDateSwitchTime(unsigned char& Hour, unsigned char& Minute, unsigned char& Second)
{
	if (m_uHour > 23 || m_uMinute > 59 || m_uSecond > 59) {
		return false;
	}

	Hour = m_uHour;
	Minute = m_uMinute;
	Second = m_uSecond;
	return true;
}
