#ifndef __MIMUTILS_H_INCLUDED_
#define __MIMUTILS_H_INCLUDED_

#include "Winspool.h"
#include "Windows.h"
#include <vector>
#include <atlcomtime.h>

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#define E_INVALID_PARAMS		-1			/* parameter INVALID	*/
#define E_WIN32_ERROR			-2			/* win32 error check	*/
#define	E_FILE_NOT_FOUND		-3			/* file not found	*/
#define E_FILE_IS_EMPTY			-4
#define E_INVALID_STAFF_FILE	-5	/* staff file INVALID	*/

typedef long                S32_t;
typedef S32_t       	    Time_t;   
typedef Time_t              TimeT; 
typedef unsigned long       DateC19_t;                  // 天数从1799年12月31日算起。0=“未设定”。1=1800年1月1日
typedef unsigned short      DateC20_t;                  // 天数从1899年12月31日算起。0=“未设定”。1=1900年1月1日
typedef unsigned short      Duration_t;   	            // 通过单位或值来表示持续时间。DurationUnit_t 占用最有效的4 位。DurationValue_t 占用最低有效的12 位。
typedef unsigned long       DurationValue_t;            // 指出某事物的持续期间（例如，产品的持续期）。


static const short          SECONDS_IN_A_MINUTE                                 = 60;
static const short          SECONDS_IN_A_HOUR                                   = 3600;
static const short          MINUTES_IN_A_DAY                                    = 1440;
static const long           SECONDS_24_HOURS                                    = MINUTES_IN_A_DAY * 60;
static const long           JULIEN_OFFSET_FOR_DATEC26                           = 2453737;
static const long           JULIEN_OFFSET_FOR_DATEC20                           = 2415020;
static const short          TM_YEAR_BASE                                        = 1900;
static const short          TM_YEAR_2006                                        = 2006;

static const CString  W_SPACE = _T("　");
static const CString  H_SPACE = _T(" ");


/* define for check bit value	*/
typedef enum {
	CHK_BIT0 = 0x01,
	CHK_BIT1 = 0x02,
	CHK_BIT2 = 0x04,
	CHK_BIT3 = 0x08,
	CHK_BIT4 = 0x10,
	CHK_BIT5 = 0x20,
	CHK_BIT6 = 0x40,
	CHK_BIT7 = 0x80
};
//定义产品单位的持续期间
typedef enum _durationUnit_t{  
	DURATION_UNIT_SECOND                = 1,        // 以秒为单位定义。
	DURATION_UNIT_MINUTE                = 2,        // 以分钟为单位定义
	DURATION_UNIT_HOUR                  = 3,        // 以小时为单位定义。
	DURATION_UNIT_DAY                   = 4,        // 以日为单位定义。
	DURATION_UNIT_WEEK                  = 5,        // 以周为单位定义。
	DURATION_UNIT_CALENDAR_MONTH        = 6,        // 以月为单位定义。
	DURATION_UNIT_YEAR                  = 7,        // 以年为单位定义。
	DURATION_UNIT_UNSPECIFIED           = 0xF       // 持续时间的单位未定义
}DurationUnit_t;
//////////////////////////////////////////////////////////////////////////
/**
@brief      检查字符的有效性，包括26个大小写字母、0~9及_-.@符号
@param      (i)char c 需要 检查的char编码

@retval     bool     true;正确 false:错误

@exception  无

*/
//////////////////////////////////////////////////////////////////////////
inline bool IsValidChar(TCHAR ch)
{
	if( (ch>=97) && (ch<=122) ) //26个小写字母
		return TRUE;
	if( (ch>=65) && (ch<=90) ) //26个大写字母
		return TRUE;
	if((ch>=48) && (ch<=57)) //0~9
		return TRUE;
	if( ch==95 || ch==45 || ch==46 || ch==64 ) //_-.@
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查邮箱有效性

@param      (i)CString email 邮箱

@retval     bool     true;正确 false:错误

@exception  无

sample      

*/
//////////////////////////////////////////////////////////////////////////
inline bool checkEmailValidate(CString& strEmail)
{ 
	if(strEmail.GetLength()<12){ //a@b.cn
		 return FALSE;
	 }
	TCHAR ch = strEmail[0];
	//1. 首字符必须用字母或数字
	if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) ||(ch>=48) && (ch<=57)){
		int atCount =0;
		int atPos = 0;
		int dotCount = 0;
		int subPos=0;
		int underlinePos=0;
		for(int i=1;i<strEmail.GetLength();i++){ //0已经判断过了，从1开始
			ch = strEmail[i];
			if(IsValidChar(ch)){
				if( ch==64 ){ //"@"
					atCount ++;
					atPos = i;
				}
				else if( ch==46 ){//@符号后的"."号
					if (atCount>0){
						dotCount ++;
					}else{
						return FALSE;
					}
				}
				else if( ch==95 ){//_符号后的"_"号
					if (underlinePos>0&&underlinePos+1!=i){
						return FALSE;
					}
					underlinePos=i;
				}
				else if( ch==45 ){
					if (subPos>0&&subPos+1!=i){//-符号后的"-"号
						return FALSE;
					}
					subPos =i;
				}
			}
		}
		//5. 结尾不得是字符“@”或者“.”,"-","_"
		if( ch==95 || ch==45 || ch==46 || ch==64  )
			return FALSE;
		//2. 必须包含一个并且只有一个符号“@”
		//3. @后必须包含至少一个至多三个符号“.”
		if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
		return FALSE;
		// @后不包含"_","-"
		if(subPos>atPos||underlinePos>atPos){
			return FALSE;
		}
		//4. 不允许出现“@.”或者.@或者“..”,"..."，"-"
		if( strEmail.Find(_T("@."))>0 || strEmail.Find(_T(".@"))>0 || strEmail.Find(_T(".."))>0|| strEmail.Find(_T("..."))>0){
			return FALSE;
		}
		else{
			return TRUE;	
		}		
	}
	 return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      char转换为BCD

@param      (i)char c 需要转换的char编码

@retval     char      转换后的BCD编码

@exception  无

sample      c=35 -> ret=0x35

*/
//////////////////////////////////////////////////////////////////////////
inline char char2BCD(char c)
{ 
	char ret(0);
	ret  = c%10;
	c  = c/10;
	ret |= (c%10)<<4;
	return ret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      BYTE(unsigned char)转换为BCD

@param      (i)BYTE c 需要转换的BYTE编码

@retval     BYTE      转换后的BCD编码

@exception  无

sample      c=35 -> ret=0x35

*/
//////////////////////////////////////////////////////////////////////////
inline BYTE BYTE2BCD(BYTE c)
{ 
	char ret(0);
	ret  = c%10;
	c  = c/10;
	ret |= (c%10)<<4;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      int转换为BCD

@param      (i)char  s   需要转换的数
@param      (o)char* p   转换后的BCD
@param      (i)int   len 需要转换的长度

@retval     char*        转换后的BCD

@exception  无

sample      s =1235 , len=2 -> ret=0x1235

*/
//////////////////////////////////////////////////////////////////////////
inline char* int2BCD(int s, char* p, int len)
{ 
	for (int i=len-1; i>=0; i--){
		p[i]=char2BCD(s%100);
		s=s/100;
	}; 
	return p;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCD转换为int

@param      (i)char* s   需要砖换的BCD
@param      (i)int   len 需要转换的长度

@retval     int          转换后的int数

@exception  无

sample      S=0x1235 , len=2 -> ret=1235

*/
//////////////////////////////////////////////////////////////////////////
inline int BCD2int(char* s, int len)
{ 
	int ret(0);
	for (int i=0; i<len; i++){
		ret *= 100;
		ret += ((s[i]&0xf0)>>4)*10+(s[i]&0x0f);
	}; 
	return ret;
}


// 时间结构（时分秒）
typedef struct _time
{
	BYTE bHour;
	BYTE bMin;
	BYTE bSec;
	_time()
	{
		bHour = 0;
		bMin = 0;
		bSec = 0;
	}
	_time(BYTE hour,BYTE minute,BYTE second)
	{
		bHour = hour;
		bMin = minute;
		bSec = second;
	}
	bool operator == (const _time& another) const
	{
		return bHour == another.bHour && bMin == another.bMin && bSec == another.bSec;
	}
	bool operator != (const _time& another) const
	{
		return bHour != another.bHour || bMin != another.bMin ||bSec != another.bSec;
	}
	bool operator <(const _time& another) const{
		if(bHour < another.bHour) return true;
		if(bHour>another.bHour) return false;
		if(bMin < another.bMin) return true;
		if(bMin>another.bMin) return false;
		if(bSec < another.bSec) return true;
		if(bSec>another.bSec) return false;
		return false;
	}

	bool operator >(const _time& another) const{
		if(bHour > another.bHour) return true;
		if(bHour<another.bHour) return false;
		if(bMin > another.bMin) return true;
		if(bMin<another.bMin) return false;
		if(bSec > another.bSec) return true;
		if(bSec<another.bSec) return false;

		return false;
	}
} _TIME;

// 日期结构（年月日时分秒）
typedef struct _date_time
{
	WORD wYear;
	BYTE biMonth;
	BYTE biDay;
	BYTE biHour;
	BYTE biMinute;
	BYTE biSecond;
	_date_time()
	{
		wYear = 0;
		biMonth = 0;
		biDay = 0 ;
		biHour = 0;
		biMinute = 0;
		biSecond = 0;
	}
	_date_time(WORD year,BYTE month,BYTE day,BYTE hour,BYTE minute,BYTE second)
	{
		wYear = year;
		biMonth = month;
		biDay = day;
		biHour = hour;
		biMinute = minute;
		biSecond = second;
	}
	bool operator == (const _date_time& another) const
	{
		return wYear == another.wYear && biMonth == another.biMonth && biDay == another.biDay
			&& biHour == another.biHour && biMinute == another.biMinute && biSecond == another.biSecond;
	}
	bool operator != (const _date_time& another) const
	{
		return wYear != another.wYear || biMonth != another.biMonth || biDay != another.biDay
			|| biHour != another.biHour || biMinute != another.biMinute || biSecond != another.biSecond;
	}
	bool operator < (const _date_time& another) const
	{
		if(wYear < another.wYear) return true;
		if(wYear>another.wYear) return false;
		if(biMonth < another.biMonth) return true;
		if(biMonth>another.biMonth) return false;
		if(biDay < another.biDay) return true;
		if(biDay>another.biDay) return false;
		if(biHour < another.biHour) return true;
		if(biHour>another.biHour) return false;
		if(biMinute < another.biMinute) return true;
		if(biMinute>another.biMinute) return false;
		if(biSecond < another.biSecond) return true;
		if(biSecond>another.biSecond) return false;


		return false;
	}
	bool operator > (const _date_time & another) const
	{
		if(wYear > another.wYear) return true;
		if(wYear<another.wYear) return false;
		if(biMonth > another.biMonth) return true;
		if(biMonth<another.biMonth) return false;
		if(biDay > another.biDay) return true;
		if(biDay<another.biDay) return false;
		if(biHour > another.biHour) return true;
		if(biHour<another.biHour) return false;
		if(biMinute > another.biMinute) return true;
		if(biMinute<another.biMinute) return false;
		if(biSecond > another.biSecond) return true;
		if(biSecond<another.biSecond) return false;
		return false;

	}
	bool TryParse(CString& strDateTime)
	{
		if(strDateTime.GetLength()!=14)
		{
			return false;
		}
		wYear = _ttoi(strDateTime.Left(4));
		biMonth = _ttoi(strDateTime.Mid(4,2));
		biDay = _ttoi(strDateTime.Mid(6,2));
		biHour = _ttoi(strDateTime.Mid(8,2));
		biMinute = _ttoi(strDateTime.Mid(10,2));
		biSecond = _ttoi(strDateTime.Mid(12,2));
		return true;
	}
	CString ToString(CString format)
	{
		CString str;
		str.Format(format,wYear,biMonth,biDay,biHour,biMinute,biSecond);
		return str;
	}

	void Serialize(LPBYTE lpByte)
	{
		int nDate = wYear * 10000 + biMonth * 100	+ biDay;
		int2BCD(nDate,(char*)lpByte,4);					// 年月日
		nDate = biHour * 10000 + biMinute * 100 + biSecond;
		int2BCD(nDate,(char*)(lpByte + 4),3);           // 时分秒
	}
	void Deserialize(LPBYTE lpByte)
	{
		wYear = BCD2int((char*)(lpByte + 0), 2);
		biMonth = BCD2int((char*)(lpByte + 2), 1);
		biDay =BCD2int((char*)(lpByte + 3),1);
		biHour = BCD2int((char*)(lpByte + 4),1);
		biMinute = BCD2int((char*)(lpByte + 5),1);
		biSecond = BCD2int((char*)(lpByte + 6),1);
	}
} _DATE_TIME;

// 日期结构（年月日）
typedef struct _date
{
	WORD wYear;
	BYTE biMonth;
	BYTE biDay;

	_date(COleDateTime date)
	{
		wYear = date.GetYear();
		biMonth = date.GetMonth();
		biDay = date.GetDay();
	}

	_date()
	{
		wYear = 0;
		biMonth = 0;
		biDay = 0;
	}
	bool operator == (const _date& another) const
	{
		return wYear == another.wYear && biMonth == another.biMonth && biDay == another.biDay;
	}
	bool operator != (const _date& another) const
	{
		return wYear != another.wYear || biMonth != another.biMonth ||biDay != another.biDay;
	}
	bool operator <(const _date& another) const{
		if(wYear < another.wYear) return true;
		if(wYear>another.wYear) return false;
		if(biMonth < another.biMonth) return true;
		if(biMonth>another.biMonth) return false;
		if(biDay < another.biDay) return true;
		if(biDay>another.biDay) return false;
		return false;
	}

	bool operator >(const _date& another) const{
		if(wYear > another.wYear) return true;
		if(wYear<another.wYear) return false;
		if(biMonth > another.biMonth) return true;
		if(biMonth<another.biMonth) return false;
		if(biDay > another.biDay) return true;
		if(biDay<another.biDay) return false;

		return false;
	}
	bool TryParse(CString& strDate)
	{
		if(strDate.GetLength()!=8)
		{
			return false;
		}
		wYear = _ttoi(strDate.Left(4));
		biMonth = _ttoi(strDate.Mid(4,2));
		biDay = _ttoi(strDate.Right(2));
		return true;
	}
	CString ToString(CString format)
	{
		CString str;
		str.Format(format,wYear,biMonth,biDay);
		return str;
	}
	void Serialize(LPBYTE lpByte)
	{
		int nDate = wYear * 10000 + biMonth * 100	+ biDay;
		int2BCD(nDate,(char*)lpByte,4);					// 年月日
	}
	void Deserialize(LPBYTE lpByte)
	{
		wYear = BCD2int((char*)(lpByte + 0), 2);
		biMonth = BCD2int((char*)(lpByte + 2), 1);
		biDay =BCD2int((char*)(lpByte + 3),1);
	}
} _DATE;


typedef union _sc {
	_sc() : val(0) {}
	short val;
	char  cs[2];
} schar;

//////////////////////////////////////////////////////////////////////////
/**
@brief      转换2字节类型数据的存放顺序(请勿使用)

@param      (i)short v 需要转换的数据

@retval     short      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline short srev(short v)
{
	schar s; s.val=v;
	schar d;
	char* sp=s.cs;
	char* dp=&d.cs[1];
	*dp-- = *sp++;
	*dp = *sp;
	return d.val;
}

typedef union _lc {
	_lc() : val(0) {}
	long val;
	char cs[4];
} lchar;

//////////////////////////////////////////////////////////////////////////
/**
@brief      转换4字节类型数据的存放顺序(请勿使用)

@param      (i)long l 需要转换的数据

@retval     long      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline long lrev(long l)
{
	lchar s; s.val = l;
	lchar d;
	char* sp=s.cs;
	char* dp=&d.cs[3];
	*dp-- = *sp++;
	*dp-- = *sp++;
	*dp-- = *sp++;
	*dp = *sp;
	return d.val;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      intel short -> motorola short

@param      (i)short s 需要转换的数据

@retval     short      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline short stoms(short s)
{ 
	return srev(s); 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      motorola short -> intel short

@param      (i)short s 需要转换的数据

@retval     short      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline short mstos(short s)
{ 
	return srev(s); 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      intel long -> motorola long

@param      (i)short s 需要转换的数据

@retval     short      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline long ltoml(long s)
{ 
	return lrev(s); 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      motorola long -> intel long

@param      (i)short s 需要转换的数据

@retval     short      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline long mltol(long s)
{ return lrev(s); }

// 时间格式
typedef enum _time_format								///< 定义时间格式
{
	YYYYMMDD,											///< 9999年99月99日
	YYYYMMDDHHMM_TYPE1,                                 ///< 9999年99月99日  99时99分
	YYYYMMDDHHMMSS_TYPE1,                                 ///< 9999年99月99日  99时99分99秒
	YYYYMMDDHHMM_TYPE2,                                 ///< 9999/99/99 99:99
	YYYYMMDDHHMMSS_TYPE2,                                 ///< 9999/99/99 99:99:99
	YYYYMMDDHHMM_TYPE3,                                 ///< 9999年99月99日 99:99
	YYYYMMDDHHMMSS_TYPE3,                                 ///< 9999年99月99日 99:99:99
	YYYYMMDDHHMM_TYPE4,                                 ///< 99:99 9999-99-99
	YYYYMMDDHHMMSS_TYPE4,                                 ///< 99:99:99 9999-99-99
	YYYYMMDDHHMMSS_TYPE5,								///< 9999-99-99 99:99:99
	DDMMYYYYHHMM_TYPE6,									///< 99-99-9999 99:99
	WEEKDAY_CN,                                         ///< 星期一、星期二、
	WEEKDAY_EN,                                         ///< Monday, Tuesday,,
} TIME_FORMAT;


//////////////////////////////////////////////////////////////////////////
/**
@brief      _DATE_TIME结构的时间转换为BCD

@param      (i)_DATE_TIME scDate 需要转换的时间

@retval     转换后的BCD编码

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE Time2BCD(_DATE_TIME scDate, LPBYTE lpBuf)
{
	int nDate = scDate.wYear * 10000 + scDate.biMonth * 100	+ scDate.biDay;
	int2BCD(nDate,(char*)lpBuf,4);					// 年月日
	nDate = scDate.biHour * 10000 + scDate.biMinute * 100 + scDate.biSecond;
	int2BCD(nDate,(char*)(lpBuf + 4),3);           // 时分秒
	return lpBuf;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     long转换为_TIME

@param      (i)long lTime 需要转换的数字

@retval     转换后的时间

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline _TIME lToTime(long lTime)
{
	_TIME timer;
	timer.bHour = (BYTE)(lTime / 3600);
	lTime %= 3600;
	timer.bMin = (BYTE)(lTime / 60);
	timer.bSec = (BYTE)(lTime % 60);
	return timer;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      _DATE结构的时间转换为BCD

@param      (i)_DATE scDate 需要转换的时间(YYMMDDhhmmss)

@retval     转换后的BCD编码

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE Date2BCD(_DATE scDate, LPBYTE lpBuf)
{
	int nDate = scDate.wYear * 10000 + scDate.biMonth * 100	+ scDate.biDay;
	int2BCD(nDate,(char*)lpBuf,4);					// 年月日    
	return lpBuf;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      _DATE_TIME结构的时间转换为BCD

@param      (i)_DATE_TIME scDate 需要转换的时间

@retval     转换后的BCD编码(YYMMDDHHSS)

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE TranDate2BCD(_DATE_TIME scDate, LPBYTE lpBuf)
{
	int nDate = scDate.wYear * 10000 + scDate.biMonth * 100	+ scDate.biDay;
	int2BCD(nDate,(char*)lpBuf,3);					// 年月日
	nDate = scDate.biHour * 100 + scDate.biMinute;
	int2BCD(nDate,(char*)(lpBuf + 4),2);           // 时分秒
	return lpBuf;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      BCD时间转换为_DATE_TIME结构

@param      (i)char* lpBuf 需要转换的BCD编码时间(YYYYMMDDhhmmss)

@retval     转换后的_DATE_TIME

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline _DATE_TIME BCD2Time(char* lpBuf)
{
	_DATE_TIME dateTime;

	dateTime.wYear = BCD2int(lpBuf, 2);
	dateTime.biMonth = BCD2int(lpBuf + 2, 1);
	dateTime.biDay = BCD2int(lpBuf + 3, 1);
	dateTime.biHour = BCD2int(lpBuf + 4, 1);
	dateTime.biMinute = BCD2int(lpBuf + 5, 1);
	dateTime.biSecond = BCD2int(lpBuf + 6, 1);

	return dateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCD时间转换为_DATE结构

@param      (i)char* lpBuf 需要转换的BCD编码时间(YYYYMMDD)

@retval     转换后的_DATE

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline _DATE BCD2Date(char* lpBuf)
{
	_DATE date;

	date.wYear = BCD2int(lpBuf, 2);
	date.biMonth = BCD2int(lpBuf + 2, 1);
	date.biDay = BCD2int(lpBuf + 3, 1);

	return date;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      SYSTIME结构的时间转换为BCD

@param      (i)SYSTEMTIME scDate 需要转换的时间
@param      (o)LPBYTE lpBuf      16位BYTE数组的指针

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE Systime2BCD(SYSTEMTIME scDate,LPBYTE lpBuf)
{
	//LPBYTE lpBuf = new BYTE[16];
	int nDate = 0;
	nDate = scDate.wYear * 10000 + scDate.wMonth * 100 + scDate.wDay;
	int2BCD(nDate,(char*)lpBuf,4);
	nDate = scDate.wHour * 10000 + scDate.wMinute * 100 + scDate.wSecond;
	int2BCD(nDate,(char*)(lpBuf+4),3);

	return lpBuf;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      计算CRC值

@param      (i)LPBYTE inputBuff     需要计算CRC的数据
@param      (i)int size             数据长度

@retval     BYTE 计算所得的CRC

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline BYTE CalcCRC(LPBYTE inputBuff,int size)
{
	BYTE crc = 0;
	for (int i=0;i<size;i++) {
		crc ^= *(inputBuff+i);
	}
	return crc;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    加密电文  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void encrypt( const unsigned char * plaintext, int len, char * ciphertext );

//////////////////////////////////////////////////////////////////////////
/*
@brief     解密电文 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void decrypt( const unsigned char * ciphertext, int len, char * plaintext);

//////////////////////////////////////////////////////////////////////////
/*
@brief    将PSTR将化成BCD形式 pSrc = '101028' len =6 -> pBCD={0x10,0x10,0x28};

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////

inline char* PSTR2BCD(PSTR  pSrc,int len, char* pBCD)
{
	for(int i=0;i<len/2;i++)
	{
		::sscanf_s(pSrc+2*i,"%02X",pBCD+i);
	}
	return pBCD;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   将BCD将化为PSTR pBCD={0x10,0x10,0x28} len = 3 pDes-> '101028'

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline PSTR BCD2PSTR(char* pBCD,int len,PSTR pDes)
{
	for(int i=0;i<len;i++)
	{
		sprintf(pDes+2*i,"%02X",(unsigned char)pBCD[i]);
	}
	return pDes;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   将BIN将化为PSTR 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline PSTR BIN2PSTR(BYTE* pBIN,int len,PSTR pDes)
{
	for(int i=0;i<len;i++)
	{
		::sprintf(pDes+2*i,"%02X",pBIN[i]);
	}
	return pDes;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断字符ch是否是十六进制的字母

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline BOOL IsNumeric(const char ch)
{
	return ((ch >= '0' && ch <= '9') ||
		(ch >= 'a' && ch <= 'f') ||
		(ch >= 'A' && ch <= 'F'));
}

//inline CString changeBuff2String(const char* buff, int len)
//{
//
//	if (!buff) return "";
//	if (!len) return "";
//
//	char data[4000];
//	::memset(data,0,sizeof(data));
//
//	for (int i=0; i<len; i++){
//
//		//::sprintf(data+2*i,"%02X",(unsigned char)buff[i]);
//		::sprintf_s(data+2*i,100,_T("%02X"),(unsigned char)buff[i]);
//	}
//	return data;
//}
//
//inline char* changeString2Char(CString data, char *buff, int len)
//{
//	if (data.GetLength()==0) return buff;
//
//	char* indata = data.GetBuffer(1);
//	for (int i=0;i<len/2;i++){
//		::sscanf_s(indata+2*i,_T("%02X"),buff+i);
//	}
//
//	return buff;
//
//}
//////////////////////////////////////////////////////////////////////////
/**
@brief      将tm结构体形式转换成日期类型
@param      tm *ptm   C样式时间结构体指针
long julienOffset   日期偏移量
@retval     long   日期序列
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
inline long DT_DateFromStructTm(tm *ptm, long julienOffset)
{

	long julianDays;
	long mon = ptm->tm_mon + 1;
	long year = ptm->tm_year + TM_YEAR_BASE;
	long day = ptm->tm_mday;
	julianDays = ( 1461 * ( year + 4800 + ( mon - 14 ) / 12 ) ) / 4 + ( 367 * ( mon - 2 - 12 * ( ( mon - 14 ) / 12 ) ) ) / 12 - 
		( 3 * ( ( year + 4900 + ( mon - 14 ) / 12 ) / 100 ) ) / 4 + day - 32075;
	julianDays -= julienOffset;
	return julianDays;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      DateC20_t类型转为_DATE
@param      无
@retval     无
@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
inline DateC20_t DateStructToDateC20(_DATE date)
{
	DateC20_t dateC20 = 0;
	tm stm;
	stm.tm_sec = 0;										/* seconds after the minute - [0,59] */
	stm.tm_min = 0;										/* minutes after the hour - [0,59] */
	stm.tm_hour = 0;									/* hours since midnight - [0,23] */
	stm.tm_mday = date.biDay;							/* day of the month - [1,31] */
	stm.tm_mon = date.biMonth - 1;						/* months since January - [0,11] */
	stm.tm_year = date.wYear - 1900;					/* years since 1900 */
	stm.tm_wday = 0;									/* days since Sunday - [0,6] */
	stm.tm_yday = 0;									/* days since January 1 - [0,365] */
	stm.tm_isdst = -1;									/* daylight savings time flag */
	dateC20 = (DateC20_t)DT_DateFromStructTm(&stm, JULIEN_OFFSET_FOR_DATEC20);
	return dateC20;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   取得路径中的文件名包括扩展名   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline CString GetFileNameFromFullPath(CString sPathAndName)
{
	CString strRet(_T(""));
	int nPos = sPathAndName.ReverseFind(_T('\\'));
	if (nPos != -1)	{
		strRet = sPathAndName.Mid(nPos+1);
	}
	else{
		nPos = sPathAndName.ReverseFind(_T('/'));
		if (nPos != -1)	{
			strRet = sPathAndName.Mid(nPos+1);
		}
		else{			
			strRet = sPathAndName;
		}
	}
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得路径中的文件名，无扩展名 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
inline CString GetFileNameWithoutExtentionFormFullPath(CString sPathAndName)
{
	CString fileName = GetFileNameFromFullPath(sPathAndName);
	int nPos = sPathAndName.ReverseFind(_T('.'));
	if (nPos != -1)	{
		return  fileName.Left(nPos);
	}
	return fileName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      在字符串左边追加0

@param      (i)const char* text  要变换的字符串
@param      (i)int strLen        变换后的字符串长度（全角）

@retval     CString              变换后的字符串

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//inline CString add_zero(const char* text,int strLen)
//{
//
//	CString strRet(text);
//	int size = strRet.GetLength();
//
//	// 做成所需追加的空格字符串
//	while (size < strLen)
//	{
//		// 在字符串左边追加"0"
//		strRet = _T("0") + strRet;
//		size++;
//	}
//
//	return strRet;
//}

// 可变数据的结构
typedef struct _variable_data{
	LPBYTE lpData;
	ULONG nLen;
	_variable_data(){
		lpData = NULL;
		nLen = 0;
	}
	void Clear()
	{
		delete [] lpData;
		lpData = NULL;
		nLen = 0;
	}
	~_variable_data(){
		if (lpData != NULL) {
			delete [] lpData;
			lpData = NULL;
		}
	}
}VARIABLE_DATA, *LPVARIABLE_DATA;

//////////////////////////////////////////////////////////////////////////
/*
@brief 杀死当前应用程序     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComKillApp();

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得当前程序exe所在路径  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
INT ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);

//////////////////////////////////////////////////////////////////////////
/*
@brief   判断文件是否存在   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckFileExist(LPCTSTR lpszFileName);

//////////////////////////////////////////////////////////////////////////
/*
@brief  判断目录是否存在    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckDirectory(LPCTSTR lpszDirName);

//////////////////////////////////////////////////////////////////////////
/*
@brief    判断目录是否存在，不存在就创建  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckAndCreateDirectory(LPCTSTR lpszDirName);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckAlphaNumericText(IN LPCTSTR lpszText, IN USHORT usTextLen);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComGetDestDir(CString sPath, CString sOrgPath, CString sDestDir, CString &sDest);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComSetCurrentDir(LPCTSTR lpszDirName, CString &sCurrentDir);

//////////////////////////////////////////////////////////////////////////
/*
@brief     是否是润年 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool IsLeapYear(int iYear);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDate(CString sYear, CString sMonth, CString sDay,CString sHour = _T("0"), CString sMinute = _T("0"),CString sSecond = _T("0"));

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateTimeValidSub(int iYear, int iMonth, int iDay,int iHour = 0, int iMinute = 0, int iSecond = 0);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateTimeValid(_DATE_TIME dateTime);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateValidSub(int iYear, int iMonth, int iDay);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateValid(_DATE date);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckIPEmpty(CString sIP1, CString sIP2, CString sIP3, CString sIP4, int *iErr);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComExecuteApp(LPCTSTR lpszAppPath, LPCTSTR lpszAppParameter);

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得ProcessId  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD   GetProcessIdFromName(LPCTSTR  name);

//////////////////////////////////////////////////////////////////////////
/*
@brief  杀死特定的应用程序    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComKillAppEx(LPCTSTR lpszBuf);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComIsBitSet(WORD value, int iBitNumber);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComGetFileExtention(LPCTSTR lpszFileName);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComStringToHex(LPTSTR lpszValue, LPTSTR lpszHex);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
time_t CreateTime(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond );



int ComfrHEX(const char* szData, BYTE* pBuff, int iBufSize);

//////////////////////////////////////////////////////////////////////////
/*
@brief       将一个字符串转换为8位的数组

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComConvertStrToArrEight(const TCHAR* szData, BYTE* pBuff);

//////////////////////////////////////////////////////////////////////////
/*
@brief      将8位的数组转换为字符串

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComConvertArrEightToStr(BYTE* pBuff);

//////////////////////////////////////////////////////////////////////////
/*
@brief     将UTF16格式文字转换成UTF8文字

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CHAR* ComUtf16ToUtf8(CString strUtf16);

//////////////////////////////////////////////////////////////////////////
/*
@brief     将UTF8格式的文字转化成UTF16格式 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComUtf8ToUtf16(CHAR* pUtf8);

//使用A2T或者T2A进行转换

////////////////////////////////////////////////////////////////////////////
///*
//@brief   将ANSI格式的文字转化成UTF16格式   
//
//@param
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CString ComAnsiToUtf16(CHAR* pAnsi);


////////////////////////////////////////////////////////////////////////////
///*
//@brief 将UTF16转换成ANSI格式的文字     
//
//@param
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CHAR* ComUtf16ToAnsi(CString strUtf16);

//////////////////////////////////////////////////////////////////////////
/*
@brief     按MOTOL序拼long 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long ComMakeLong(BYTE bHiByte1, BYTE bLoByte1, BYTE bHiByte2, BYTE bLoByte2);

//////////////////////////////////////////////////////////////////////////
/*
@brief    按MOTOL序拼WORD  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
WORD ComMakeWORD(BYTE bHiByte, BYTE bLoByte);
BYTE* ComPad(unsigned long ulDataLen);
void ComRevLong(long lValue, BYTE* p);
void ComRevWORD(WORD wValue, BYTE* p);
bool ComShutDownSystem(bool bReboot);
CString ComConvToHexStr(LPBYTE lpData, int iLen);
_DATE_TIME ComConvLongToDate(long lDateTime);

BOOL GetJobs(HANDLE hPrinter,        /* Handle to the printer. */
			 JOB_INFO_2 **ppJobInfo, /* Pointer to be filled.  */
			 int *pcJobs,            /* Count of jobs filled.  */
			 DWORD *pStatus);         /* Print Queue status.    */

// 追加内容
CString ComGetSysTimeText(TIME_FORMAT format);
int GetAactualCharacterWidth(CString);
int GetAactualCharacterMemarySize(CString text);
DurationUnit_t GetDurationType(Duration_t duration);
DurationValue_t GetDurationValue(Duration_t duration);
_DATE_TIME GetAfterCurTime(const Duration_t &duration);
_DATE_TIME GetAfterSetTime(_DATE_TIME dtSetTime,const Duration_t &duration);

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得当前时间

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME ComGetCurTime();

//////////////////////////////////////////////////////////////////////////
/*
@brief     取得当前日期 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE ComGetCurDate();

//////////////////////////////////////////////////////////////////////////
/*
@brief   加全角空格   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString add_wsp(const TCHAR* text,int strLen);

//////////////////////////////////////////////////////////////////////////
/*
@brief    加半角空格  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString add_hsp(const TCHAR* text,int strLen);

//////////////////////////////////////////////////////////////////////////
/*
@brief   在右边加半角空格   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString add_hsp_right(const TCHAR* text,int strLen);

bool IsHalfHZ(CString text, int pos);
CString ComPrintLineFormat(const CString& PrintStringLeft,const CString& PrintStringRight);
void ComPrintLineFormatForTemplate(CString PrintLine,CStringArray& printLines);

//////////////////////////////////////////////////////////////////////////
/*
@brief   将金额格式化为000,000.00格式   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComFormatAmount(long iAmount);

//////////////////////////////////////////////////////////////////////////
/*
@brief      从已经格式化好的金额格式中获得金额

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long ComGetAmount(CString sAmount);

//////////////////////////////////////////////////////////////////////////
/*
@brief  格式化数字000,000    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComFormatCount(int iCount);

//////////////////////////////////////////////////////////////////////////
/*
@brief    所有文字是否都是数字  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComIsAllNumericInString(const CString& str);

//////////////////////////////////////////////////////////////////////////
/*
@brief   检查字符是否全为数字并且在min和max之间   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CheckNumText(CString srcData,int min,int max);
bool GetAppPath(CString& sAppPath);

//////////////////////////////////////////////////////////////////////////
/*
@brief     按特定分格符划分字符 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void GetSplitStrArray(CString sData,CString sSplitFlag,CStringArray &array);
_DATE GetDayAfterTypicalDay(WORD publishDay,WORD expirationDay);

//////////////////////////////////////////////////////////////////////////
/*
@brief   取得特定日期后特定年数的日期   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetYearAfter(const _DATE& date, unsigned short year);

//////////////////////////////////////////////////////////////////////////
/*
@brief   取得指定日期后特定天数的日期   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayAfter(const _DATE& date, DWORD day);

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得指定时间后几天的时间  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetDateTimeAfter(const _DATE_TIME & datetime,DWORD day);

//////////////////////////////////////////////////////////////////////////
/*
@brief  取得指定日期的前几天的日期    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayBefore(const _DATE& date, DWORD day);

//////////////////////////////////////////////////////////////////////////
/*
@brief 取得两个时间之间的秒数     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD GetOffTimeSecond(const _DATE_TIME& offDate,const _DATE_TIME& baseDate);
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得两个日期之间的天数

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD GetOffTimeDay(const _DATE& offDate,const _DATE& baseDate);

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得特定时间后特定秒的时间

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetDateTimeAfterBySecond(const _DATE_TIME& baseDate, unsigned long offSeconds);

//////////////////////////////////////////////////////////////////////////
/*
@brief   比较两个时间的早晚   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CmpTwoDateTime(const _DATE_TIME& date1, const _DATE_TIME& date2);
//////////////////////////////////////////////////////////////////////////
/*
@brief   比较两个日期的早晚   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CmpTwoDate(const _DATE& date1, const _DATE& date2);

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得星期的文字表示  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComGetWeekdayNameCN(int weekday);

//////////////////////////////////////////////////////////////////////////
/*
@brief     读取二进制文件 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ReadBinaryFile(const TCHAR* sPathFileName,VARIABLE_DATA &bufferStruct);

//////////////////////////////////////////////////////////////////////////
/*
@brief     创建目录 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CreateFullDirectory(CString sPathName);        //创建目录

//////////////////////////////////////////////////////////////////////////
/*
@brief  取得当前时间（Time_t类型）

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Time_t ComGetCurTimeT();

//////////////////////////////////////////////////////////////////////////
/*
@brief     判断一个char是否是BCD 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckCharIsBCD(char s);

//////////////////////////////////////////////////////////////////////////
/*
@brief   判断给定的字符是否都是BCD   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckIsBCD(char *s, int len);

//////////////////////////////////////////////////////////////////////////
/*
@brief    将TimeT转换成_DATE_TIME形式  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME ComConvertTimeTToDateTime(TimeT offSeconds);

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
__int64 MkAbsTime(tm stm);

//////////////////////////////////////////////////////////////////////////
/*
@brief  Local time to utc time    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
__int64 ComConvertLocalTimeToUtcTime(__int64 localTime);

//////////////////////////////////////////////////////////////////////////
/*
@brief      utc time to local time

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
__int64 ComConvertUtcTimeToLocalTime(__int64 utcTime);

//////////////////////////////////////////////////////////////////////////
/*
@brief    根据条件，搜索文件  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void SearchFile(CString searchCondition,std::vector<CString>& result);
//////////////////////////////////////////////////////////////////////////
/*
@brief    十六进制转换到十进制  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Hex2Dec(int);

//////////////////////////////////////////////////////////////////////////
/*
@brief    十进制转换到十六进制  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Dec2Hex(int);

//////////////////////////////////////////////////////////////////////////
/*
@brief     算CRC 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
unsigned long Crc32(void *pStart, unsigned long length, unsigned long crc32);

//////////////////////////////////////////////////////////////////////////
/*
@brief   检查堆是否有错误，调试内存错误会用到   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CheckHeapHealth();

//////////////////////////////////////////////////////////////////////////
/*
@brief 检查身份证是否正确（仅针对中华人民共和国（大陆）地区居民身份证）     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCard(CString number);

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证身份证号码（澳门）

@param      无

@retval     true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCardMACAO(CString number);

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证身份证号码（香港）

@param      无

@retval     true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCardHongKong(CString number);

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证出生日期

@param      无

@retval     true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckBirthday(CString birthday);

//////////////////////////////////////////////////////////////////////////
/*
@brief    根据条件，搜索文件  

@param
@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CStringArray* SplitCString(CString str,CString delim);

//////////////////////////////////////////////////////////////////////////
/*
@brief  搜索一个文件    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString SearchOneFile(CString searchCondition);

//////////////////////////////////////////////////////////////////////////
/*
@brief     删除文件夹 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool DeleteFolder(CString sDirPath);

CString GetIniDataString(CString sFileName,LPCTSTR lpszSection, LPCTSTR lpszKey, CString sDefault);

#define COMMONLIB_API __declspec(dllimport)
#endif  // __MIMCUTILS_H_INCLUDED_