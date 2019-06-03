#pragma once

#include "SysException.h"

/**
  @brief 解析好的卡内数据存储类                            
*/
class CBR
{
public:
    CBR(void);
    ~CBR(void);
  //  void Initialize(SERVICE_ID);

public:
	DateC20_t	CalculateValidityEndDate(DateC20_t start,DurationValue_t duration,DurationUnit_t type);
	_DATE		DateC20ToDateStruct(DateC20_t dateC20_t);
	DateC20_t	DateStructToDateC20(_DATE date);
	long		DT_DateFromStructTm(tm *ptm, long julienOffset);
	TimeT		Map_DateC20_to_TimeT(DateC20_t dateC20 );
	DateC20_t	Map_TimeT_to_DateC20(TimeT timeT);
	tm*			DT_StructTmFromDate( long date, tm * ptm, long julienOffset );
	int			DT_DayOfYear( int year, int month, int day );
	int			DT_IsLeapYear( int year );
	int			DT_DayOfWeek( int year, int month, int day );
	TimeT		CalculateValidityEndDateTime(TimeT start, DurationValue_t duration, DurationUnit_t type);
	TimeT		CalculateDateTimeAtEndOfCalendarDay(DateC20_t dateC20);
};