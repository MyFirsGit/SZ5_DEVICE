#include "StdAfx.h"
#include "BR.h"
#include "InnerException.h"

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
CBR::CBR(void)
{
//    Initialize(UNKNOWN_SVC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBR::~CBR(void)
{
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      DateC20_t����תΪ_DATE

@param      none

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
DateC20_t CBR::DateStructToDateC20(_DATE date)
{
	DateC20_t dateC20 = 0;
	tm stm;
	stm.tm_sec = 0;                                                                                           /* seconds after the minute - [0,59] */
	stm.tm_min = 0;                                                                                          /* minutes after the hour - [0,59] */
	stm.tm_hour = 0;                                                                               /* hours since midnight - [0,23] */
	stm.tm_mday = date.biDay;                                                           /* day of the month - [1,31] */
	stm.tm_mon = date.biMonth - 1;                                                  /* months since January - [0,11] */
	stm.tm_year = date.wYear - 1900;                                      /* years since 1900 */
	stm.tm_wday = 0;                                                                              /* days since Sunday - [0,6] */
	stm.tm_yday = 0;                                                                               /* days since January 1 - [0,365] */
	stm.tm_isdst = -1;                                                                             /* daylight savings time flag */
	dateC20 = (DateC20_t)DT_DateFromStructTm(&stm, JULIEN_OFFSET_FOR_DATEC20);
	return dateC20;
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      DateC20_t����תΪ_DATE

@param      (i)DateC20_t dateC20_t

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
_DATE CBR::DateC20ToDateStruct(DateC20_t dateC20_t)
{
	_DATE date;
	tm stm;
	tm * pStm;
	pStm = DT_StructTmFromDate((long)dateC20_t,&stm,JULIEN_OFFSET_FOR_DATEC20);
	date.wYear = stm.tm_year + 1900;
	date.biMonth = stm.tm_mon + 1;
	date.biDay = stm.tm_mday;
	return date;
}

/*
@brief      ��tm�ṹ����ʽת������������

@param      (i)tm *ptm   C��ʽʱ��ṹ��ָ��
@param      (i)long julienOffset   ����ƫ����

@retval     long   ��������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CBR::DT_DateFromStructTm(tm *ptm, long julienOffset)
{
	long julianDays;
	// The algorithm only works with 32-bit maths so get some local 32-bit items ready
	// The algorithm expects Jan to be 1, not 0, and the year values to be absolute
	long mon = ptm->tm_mon + 1;
	long year = ptm->tm_year + TM_YEAR_BASE;
	long day = ptm->tm_mday;
	julianDays = ( 1461 * ( year + 4800 + ( mon - 14 ) / 12 ) ) / 4 + ( 367 * ( mon - 2 - 12 * ( ( mon - 14 ) / 12 ) ) ) / 12 - 
		( 3 * ( ( year + 4900 + ( mon - 14 ) / 12 ) / 100 ) ) / 4 + day - 32075;
	// Subtract the julien day offset in order to get the julien day value into our desired date sequence number range
	julianDays -= julienOffset;
	return julianDays;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ڿ�ʼ���ڣ�������Ч��������

@param      (i)DateC20_t start   ��ʼ����
@param      (i)DurationValue_t duration   ��Ч����ʱ��
@param      (i)DurationUnit_t type   ��Ч����ʱ������

@retval     DateC20_t   ����,��ʾ��1899��12��31������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
DateC20_t CBR::CalculateValidityEndDate(DateC20_t start,DurationValue_t duration,DurationUnit_t type)
{
	DateC20_t result;
	switch (type){
	case DURATION_UNIT_DAY:
		{
			// Duration decremented to ensure that date calculation is inclusive
			result = start + (duration-1);
		}
		break;
	case DURATION_UNIT_MINUTE:
	case DURATION_UNIT_HOUR:
	case DURATION_UNIT_CALENDAR_MONTH:
		{
			TimeT timeT = Map_DateC20_to_TimeT(start);
			timeT = CalculateValidityEndDateTime(timeT,duration,type);
			result = Map_TimeT_to_DateC20(timeT);
		}
		break;
	default:
		result = start;
		break;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��DateC20_t ֵתΪTimeT ��ֵ

@param      (i)DateC20_t dateC20   ����,��ʾ��1899��12��31������

@retval     TimeT   ����/ʱ��,��ʾ��1970 ��1 ��1 �� 00��00 �������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
TimeT CBR::Map_DateC20_to_TimeT(DateC20_t dateC20 )
{
	tm stm;
	tm * pStm;
	TimeT utc = 0;
	pStm = DT_StructTmFromDate((long) dateC20, &stm,JULIEN_OFFSET_FOR_DATEC20); 
	// time_t begins in 1970 so if the date precedes that, we cannot compute a UTC value
	if ( (stm.tm_year + TM_YEAR_BASE) >= 1970 ) {
		if ( MkAbsTime(stm) < 0){
			return utc;
		}  
		utc = (TimeT)MkAbsTime(stm);
	}
	return utc;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��TimeT ֵתΪDateC20_t ��ֵ

@param      (i)TimeT timeT   ����/ʱ��,��ʾ��1970 ��1 ��1 �� 00��00 �������

@retval     DateC20_t   ����,��ʾ��1899��12��31������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
DateC20_t CBR::Map_TimeT_to_DateC20(TimeT timeT)
{
	CTime time(timeT);
	tm temptm;
	tm *ptm = &temptm; 
	time.GetGmtTm(ptm);
	return (DateC20_t)DT_DateFromStructTm(ptm, JULIEN_OFFSET_FOR_DATEC20);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ת����tm�ṹ����ʽ

@param      (i)long date   ��׼����
@param      (o)tm * ptm   C��ʽʱ��ṹ��ָ��
@param      (i)long julienOffset   ����ƫ����

@retval     tm*   C��ʽʱ��ṹ��ָ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
tm* CBR::DT_StructTmFromDate( long date, tm * ptm, long julienOffset )
{
	long l, n, i, j;
	long julienDays;
	// Add the julien day offset applicable to our date value in order to get the true julien day value from our date sequence number.
	julienDays = date + julienOffset;
	l = julienDays + 68569;
	n = ( 4 * l ) / 146097L;
	l = l - ( 146097L * n + 3 ) / 4;
	i = ( 4000 * ( l + 1 ) ) / 1461001L;
	l = l - ( 1461 * i ) / 4 + 31;
	j = ( 80 * l ) / 2447;
	ptm->tm_mday = l - ( 2447 * j ) / 80;
	l = j / 11;
	ptm->tm_mon = j + 2 - ( 12 * l );
	ptm->tm_year = 100 * ( n - 49 ) + i + l;
	// At this point we have a year (0..9999), a month (1..12), and a day(1..31) obtain year/week day numbers
	ptm->tm_wday = DT_DayOfWeek( ptm->tm_year, ptm->tm_mon, ptm->tm_mday );
	ptm->tm_yday = DT_DayOfYear( ptm->tm_year, ptm->tm_mon, ptm->tm_mday );
	// Now adjust year and month for ANSI C standard ranges
	ptm->tm_year -= TM_YEAR_BASE;
	// by definition the tm struct's month starts at zero
	ptm->tm_mon--;
	// clear other fields 
	ptm->tm_hour = 0;
	ptm->tm_min = 0;
	ptm->tm_sec = 0;
	// flag that we do not know what the daylight saving state is
	ptm->tm_isdst= -1;
	return ptm;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ض����������ڼ�     

@param      (i)int   ��(0..9999)
@param      (i)int   ��(1..12)
@param      (i)int   ��(1..31)

@retval     int   ���ڼ�(0..6)  

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CBR::DT_DayOfWeek( int year, int month, int day )
{
	int dayOfWeek = 6;
	// Adjust to our day numbering scheme where day 1 = 1st March 
	if (month < 3 ) {
		month += 12;
		year--;
	}
	// special handling for the years before year 0 
	if( year < 0 ) {
		year++;
		dayOfWeek = 4;
	}
	dayOfWeek += day + (month+1)*13/5 + year % 7 + (year/4 - year/100 + year/400) % 7;
	dayOfWeek %= 7;
	return dayOfWeek;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ض������Ǵ�1��1����ĵڼ���     

@param      (i)int   ��(0..9999)
@param      (i)int   ��(1..12)
@param      (i)int   ��(1..31)

@retval     int   �ڼ���(0..364/365)  

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
int CBR::DT_DayOfYear( int year, int month, int day )
{
	// cummulative days in a non-leap year, adjusted by -1 because we want the 1st Jan to be represented by the value 0 
	static const short CumDaysInNonLeapMonth[12] = {
		-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333
	};
	// Get the day of year number for all whole months that have already elapsed 
	int dayOfYear = (int)CumDaysInNonLeapMonth[ month - 1 ];
	// If month is march or beyond, and this is a leap year, we must add one to the day of year, to account for the 29th Feb 
	if ( (month >= 3) && DT_IsLeapYear( year ) ) {
		dayOfYear++;
	}
	// Now add in the current month's day number 
	dayOfYear += day;
	return dayOfYear;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�������     

@param      (i)int   ���      

@retval     int   �Ƿ������꣬�ж��Ƿ�������
0��������, ���������� 
@exception  ��  
*/
//////////////////////////////////////////////////////////////////////////
int CBR::DT_IsLeapYear( int year )
{
	return (((year) % 4) == 0 && ((year) % 100) != 0 || ((year) % 400) == 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ч��������ʱ�䣬���ݿ�ʼ����/ʱ�䡢��Ч����ʱ�����Ч����ʱ������

@param      (i)TimeT start   ��ʼ����/ʱ��
@param      (i)DurationValue_t duration   ��Ч����ʱ��
@param      (i)DurationUnit_t type   ��Ч����ʱ������

@retval     TimeT   ����/ʱ��,��ʾ��1970 ��1 ��1 �� 00��00 �������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
TimeT CBR::CalculateValidityEndDateTime(TimeT start, DurationValue_t duration, DurationUnit_t type)
{
	TimeT result = start;
	switch (type) {
	case DURATION_UNIT_MINUTE:
		{
			result = start + (duration * SECONDS_IN_A_MINUTE);
		}
		break;
	case DURATION_UNIT_HOUR:
		{
			result = start + (duration * SECONDS_IN_A_HOUR);
		}
		break;
	case DURATION_UNIT_DAY:
		{
			if( duration > 0){
				DateC20_t dateC20 = Map_TimeT_to_DateC20(start);
				result = CalculateDateTimeAtEndOfCalendarDay(dateC20 + duration - 1);   
			}
		}
		break;
	case DURATION_UNIT_CALENDAR_MONTH:
		{
			DateC20_t dateC20;
			CTime time(start);
			tm temptm;
			tm *ptm = &temptm; 
			time.GetGmtTm(ptm);
			if (ptm != NULL) {
				// Go back to the start of the month & add duration 
				ptm->tm_sec = 0;
				ptm->tm_min = 0;
				ptm->tm_hour = 0;
				ptm->tm_mday = 1;
				ptm->tm_isdst= -1;
				ptm->tm_mon += duration;
				// �·ݽ�λ
				ptm->tm_year += ptm->tm_mon/12;
				ptm->tm_mon = ptm->tm_mon%12;
				if ( MkAbsTime(*ptm) < 0){
					return 0;
				} 
				result = (TimeT)MkAbsTime(*ptm);
				dateC20 = Map_TimeT_to_DateC20(result);
				// Now have a date for beginning of the new month but we want end of month so decrement date & set time to 23:59:59			  
				result = CalculateDateTimeAtEndOfCalendarDay (dateC20-1); 
			}
			break;
		}
	default:
		result = start;
		break;
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ���ض��������ڵ��ս���������/ʱ��

@param      (i)DateC20_t dateC20   ����,��ʾ��1899��12��31������

@retval     TimeT   ����/ʱ��,��ʾ��1970 ��1 ��1 �� 00��00 �������

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
TimeT CBR::CalculateDateTimeAtEndOfCalendarDay(DateC20_t dateC20)
{
	if (dateC20 == 0) {
		return 0;
	}
	if( dateC20 < Map_TimeT_to_DateC20(0) ){
		return 0;
	}
	TimeT timeT = Map_DateC20_to_TimeT(dateC20);
	return (timeT + (SECONDS_24_HOURS - 1));
}