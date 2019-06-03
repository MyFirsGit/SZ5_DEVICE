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
typedef unsigned long       DateC19_t;                  // ������1799��12��31������0=��δ�趨����1=1800��1��1��
typedef unsigned short      DateC20_t;                  // ������1899��12��31������0=��δ�趨����1=1900��1��1��
typedef unsigned short      Duration_t;   	            // ͨ����λ��ֵ����ʾ����ʱ�䡣DurationUnit_t ռ������Ч��4 λ��DurationValue_t ռ�������Ч��12 λ��
typedef unsigned long       DurationValue_t;            // ָ��ĳ����ĳ����ڼ䣨���磬��Ʒ�ĳ����ڣ���


static const short          SECONDS_IN_A_MINUTE                                 = 60;
static const short          SECONDS_IN_A_HOUR                                   = 3600;
static const short          MINUTES_IN_A_DAY                                    = 1440;
static const long           SECONDS_24_HOURS                                    = MINUTES_IN_A_DAY * 60;
static const long           JULIEN_OFFSET_FOR_DATEC26                           = 2453737;
static const long           JULIEN_OFFSET_FOR_DATEC20                           = 2415020;
static const short          TM_YEAR_BASE                                        = 1900;
static const short          TM_YEAR_2006                                        = 2006;

static const CString  W_SPACE = _T("��");
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
//�����Ʒ��λ�ĳ����ڼ�
typedef enum _durationUnit_t{  
	DURATION_UNIT_SECOND                = 1,        // ����Ϊ��λ���塣
	DURATION_UNIT_MINUTE                = 2,        // �Է���Ϊ��λ����
	DURATION_UNIT_HOUR                  = 3,        // ��СʱΪ��λ���塣
	DURATION_UNIT_DAY                   = 4,        // ����Ϊ��λ���塣
	DURATION_UNIT_WEEK                  = 5,        // ����Ϊ��λ���塣
	DURATION_UNIT_CALENDAR_MONTH        = 6,        // ����Ϊ��λ���塣
	DURATION_UNIT_YEAR                  = 7,        // ����Ϊ��λ���塣
	DURATION_UNIT_UNSPECIFIED           = 0xF       // ����ʱ��ĵ�λδ����
}DurationUnit_t;
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ַ�����Ч�ԣ�����26����Сд��ĸ��0~9��_-.@����
@param      (i)char c ��Ҫ ����char����

@retval     bool     true;��ȷ false:����

@exception  ��

*/
//////////////////////////////////////////////////////////////////////////
inline bool IsValidChar(TCHAR ch)
{
	if( (ch>=97) && (ch<=122) ) //26��Сд��ĸ
		return TRUE;
	if( (ch>=65) && (ch<=90) ) //26����д��ĸ
		return TRUE;
	if((ch>=48) && (ch<=57)) //0~9
		return TRUE;
	if( ch==95 || ch==45 || ch==46 || ch==64 ) //_-.@
		return TRUE;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������Ч��

@param      (i)CString email ����

@retval     bool     true;��ȷ false:����

@exception  ��

sample      

*/
//////////////////////////////////////////////////////////////////////////
inline bool checkEmailValidate(CString& strEmail)
{ 
	if(strEmail.GetLength()<12){ //a@b.cn
		 return FALSE;
	 }
	TCHAR ch = strEmail[0];
	//1. ���ַ���������ĸ������
	if( ((ch>=97) && (ch<=122)) || ((ch>=65) && (ch<=90)) ||(ch>=48) && (ch<=57)){
		int atCount =0;
		int atPos = 0;
		int dotCount = 0;
		int subPos=0;
		int underlinePos=0;
		for(int i=1;i<strEmail.GetLength();i++){ //0�Ѿ��жϹ��ˣ���1��ʼ
			ch = strEmail[i];
			if(IsValidChar(ch)){
				if( ch==64 ){ //"@"
					atCount ++;
					atPos = i;
				}
				else if( ch==46 ){//@���ź��"."��
					if (atCount>0){
						dotCount ++;
					}else{
						return FALSE;
					}
				}
				else if( ch==95 ){//_���ź��"_"��
					if (underlinePos>0&&underlinePos+1!=i){
						return FALSE;
					}
					underlinePos=i;
				}
				else if( ch==45 ){
					if (subPos>0&&subPos+1!=i){//-���ź��"-"��
						return FALSE;
					}
					subPos =i;
				}
			}
		}
		//5. ��β�������ַ���@�����ߡ�.��,"-","_"
		if( ch==95 || ch==45 || ch==46 || ch==64  )
			return FALSE;
		//2. �������һ������ֻ��һ�����š�@��
		//3. @������������һ�������������š�.��
		if( (atCount!=1) || (dotCount<1) || (dotCount>3) )
		return FALSE;
		// @�󲻰���"_","-"
		if(subPos>atPos||underlinePos>atPos){
			return FALSE;
		}
		//4. ��������֡�@.������.@���ߡ�..��,"..."��"-"
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
@brief      charת��ΪBCD

@param      (i)char c ��Ҫת����char����

@retval     char      ת�����BCD����

@exception  ��

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
@brief      BYTE(unsigned char)ת��ΪBCD

@param      (i)BYTE c ��Ҫת����BYTE����

@retval     BYTE      ת�����BCD����

@exception  ��

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
@brief      intת��ΪBCD

@param      (i)char  s   ��Ҫת������
@param      (o)char* p   ת�����BCD
@param      (i)int   len ��Ҫת���ĳ���

@retval     char*        ת�����BCD

@exception  ��

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
@brief      BCDת��Ϊint

@param      (i)char* s   ��Ҫש����BCD
@param      (i)int   len ��Ҫת���ĳ���

@retval     int          ת�����int��

@exception  ��

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


// ʱ��ṹ��ʱ���룩
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

// ���ڽṹ��������ʱ���룩
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
		int2BCD(nDate,(char*)lpByte,4);					// ������
		nDate = biHour * 10000 + biMinute * 100 + biSecond;
		int2BCD(nDate,(char*)(lpByte + 4),3);           // ʱ����
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

// ���ڽṹ�������գ�
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
		int2BCD(nDate,(char*)lpByte,4);					// ������
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
@brief      ת��2�ֽ��������ݵĴ��˳��(����ʹ��)

@param      (i)short v ��Ҫת��������

@retval     short      ת���������

@exception  ��
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
@brief      ת��4�ֽ��������ݵĴ��˳��(����ʹ��)

@param      (i)long l ��Ҫת��������

@retval     long      ת���������

@exception  ��
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

@param      (i)short s ��Ҫת��������

@retval     short      ת���������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
inline short stoms(short s)
{ 
	return srev(s); 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      motorola short -> intel short

@param      (i)short s ��Ҫת��������

@retval     short      ת���������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
inline short mstos(short s)
{ 
	return srev(s); 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      intel long -> motorola long

@param      (i)short s ��Ҫת��������

@retval     short      ת���������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
inline long ltoml(long s)
{ 
	return lrev(s); 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      motorola long -> intel long

@param      (i)short s ��Ҫת��������

@retval     short      ת���������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
inline long mltol(long s)
{ return lrev(s); }

// ʱ���ʽ
typedef enum _time_format								///< ����ʱ���ʽ
{
	YYYYMMDD,											///< 9999��99��99��
	YYYYMMDDHHMM_TYPE1,                                 ///< 9999��99��99��  99ʱ99��
	YYYYMMDDHHMMSS_TYPE1,                                 ///< 9999��99��99��  99ʱ99��99��
	YYYYMMDDHHMM_TYPE2,                                 ///< 9999/99/99 99:99
	YYYYMMDDHHMMSS_TYPE2,                                 ///< 9999/99/99 99:99:99
	YYYYMMDDHHMM_TYPE3,                                 ///< 9999��99��99�� 99:99
	YYYYMMDDHHMMSS_TYPE3,                                 ///< 9999��99��99�� 99:99:99
	YYYYMMDDHHMM_TYPE4,                                 ///< 99:99 9999-99-99
	YYYYMMDDHHMMSS_TYPE4,                                 ///< 99:99:99 9999-99-99
	YYYYMMDDHHMMSS_TYPE5,								///< 9999-99-99 99:99:99
	DDMMYYYYHHMM_TYPE6,									///< 99-99-9999 99:99
	WEEKDAY_CN,                                         ///< ����һ�����ڶ���
	WEEKDAY_EN,                                         ///< Monday, Tuesday,,
} TIME_FORMAT;


//////////////////////////////////////////////////////////////////////////
/**
@brief      _DATE_TIME�ṹ��ʱ��ת��ΪBCD

@param      (i)_DATE_TIME scDate ��Ҫת����ʱ��

@retval     ת�����BCD����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE Time2BCD(_DATE_TIME scDate, LPBYTE lpBuf)
{
	int nDate = scDate.wYear * 10000 + scDate.biMonth * 100	+ scDate.biDay;
	int2BCD(nDate,(char*)lpBuf,4);					// ������
	nDate = scDate.biHour * 10000 + scDate.biMinute * 100 + scDate.biSecond;
	int2BCD(nDate,(char*)(lpBuf + 4),3);           // ʱ����
	return lpBuf;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     longת��Ϊ_TIME

@param      (i)long lTime ��Ҫת��������

@retval     ת�����ʱ��

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
@brief      _DATE�ṹ��ʱ��ת��ΪBCD

@param      (i)_DATE scDate ��Ҫת����ʱ��(YYMMDDhhmmss)

@retval     ת�����BCD����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE Date2BCD(_DATE scDate, LPBYTE lpBuf)
{
	int nDate = scDate.wYear * 10000 + scDate.biMonth * 100	+ scDate.biDay;
	int2BCD(nDate,(char*)lpBuf,4);					// ������    
	return lpBuf;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      _DATE_TIME�ṹ��ʱ��ת��ΪBCD

@param      (i)_DATE_TIME scDate ��Ҫת����ʱ��

@retval     ת�����BCD����(YYMMDDHHSS)

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline LPBYTE TranDate2BCD(_DATE_TIME scDate, LPBYTE lpBuf)
{
	int nDate = scDate.wYear * 10000 + scDate.biMonth * 100	+ scDate.biDay;
	int2BCD(nDate,(char*)lpBuf,3);					// ������
	nDate = scDate.biHour * 100 + scDate.biMinute;
	int2BCD(nDate,(char*)(lpBuf + 4),2);           // ʱ����
	return lpBuf;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      BCDʱ��ת��Ϊ_DATE_TIME�ṹ

@param      (i)char* lpBuf ��Ҫת����BCD����ʱ��(YYYYMMDDhhmmss)

@retval     ת�����_DATE_TIME

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
@brief      BCDʱ��ת��Ϊ_DATE�ṹ

@param      (i)char* lpBuf ��Ҫת����BCD����ʱ��(YYYYMMDD)

@retval     ת�����_DATE

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
@brief      SYSTIME�ṹ��ʱ��ת��ΪBCD

@param      (i)SYSTEMTIME scDate ��Ҫת����ʱ��
@param      (o)LPBYTE lpBuf      16λBYTE�����ָ��

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
@brief      ����CRCֵ

@param      (i)LPBYTE inputBuff     ��Ҫ����CRC������
@param      (i)int size             ���ݳ���

@retval     BYTE �������õ�CRC

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
@brief    ���ܵ���  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void encrypt( const unsigned char * plaintext, int len, char * ciphertext );

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���ܵ��� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void decrypt( const unsigned char * ciphertext, int len, char * plaintext);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��PSTR������BCD��ʽ pSrc = '101028' len =6 -> pBCD={0x10,0x10,0x28};

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
@brief   ��BCD����ΪPSTR pBCD={0x10,0x10,0x28} len = 3 pDes-> '101028'

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
@brief   ��BIN����ΪPSTR 

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
@brief      �ж��ַ�ch�Ƿ���ʮ�����Ƶ���ĸ

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
@brief      ��tm�ṹ����ʽת������������
@param      tm *ptm   C��ʽʱ��ṹ��ָ��
long julienOffset   ����ƫ����
@retval     long   ��������
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
@brief      DateC20_t����תΪ_DATE
@param      ��
@retval     ��
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
@brief   ȡ��·���е��ļ���������չ��   

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
@brief     ȡ��·���е��ļ���������չ�� 

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
@brief      ���ַ������׷��0

@param      (i)const char* text  Ҫ�任���ַ���
@param      (i)int strLen        �任����ַ������ȣ�ȫ�ǣ�

@retval     CString              �任����ַ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//inline CString add_zero(const char* text,int strLen)
//{
//
//	CString strRet(text);
//	int size = strRet.GetLength();
//
//	// ��������׷�ӵĿո��ַ���
//	while (size < strLen)
//	{
//		// ���ַ������׷��"0"
//		strRet = _T("0") + strRet;
//		size++;
//	}
//
//	return strRet;
//}

// �ɱ����ݵĽṹ
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
@brief ɱ����ǰӦ�ó���     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComKillApp();

//////////////////////////////////////////////////////////////////////////
/*
@brief    ȡ�õ�ǰ����exe����·��  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
INT ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);

//////////////////////////////////////////////////////////////////////////
/*
@brief   �ж��ļ��Ƿ����   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckFileExist(LPCTSTR lpszFileName);

//////////////////////////////////////////////////////////////////////////
/*
@brief  �ж�Ŀ¼�Ƿ����    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckDirectory(LPCTSTR lpszDirName);

//////////////////////////////////////////////////////////////////////////
/*
@brief    �ж�Ŀ¼�Ƿ���ڣ������ھʹ���  

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
@brief     �Ƿ������� 

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
@brief    ȡ��ProcessId  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD   GetProcessIdFromName(LPCTSTR  name);

//////////////////////////////////////////////////////////////////////////
/*
@brief  ɱ���ض���Ӧ�ó���    

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
@brief       ��һ���ַ���ת��Ϊ8λ������

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComConvertStrToArrEight(const TCHAR* szData, BYTE* pBuff);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��8λ������ת��Ϊ�ַ���

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComConvertArrEightToStr(BYTE* pBuff);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��UTF16��ʽ����ת����UTF8����

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CHAR* ComUtf16ToUtf8(CString strUtf16);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��UTF8��ʽ������ת����UTF16��ʽ 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComUtf8ToUtf16(CHAR* pUtf8);

//ʹ��A2T����T2A����ת��

////////////////////////////////////////////////////////////////////////////
///*
//@brief   ��ANSI��ʽ������ת����UTF16��ʽ   
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
//@brief ��UTF16ת����ANSI��ʽ������     
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
@brief     ��MOTOL��ƴlong 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long ComMakeLong(BYTE bHiByte1, BYTE bLoByte1, BYTE bHiByte2, BYTE bLoByte2);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��MOTOL��ƴWORD  

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

// ׷������
CString ComGetSysTimeText(TIME_FORMAT format);
int GetAactualCharacterWidth(CString);
int GetAactualCharacterMemarySize(CString text);
DurationUnit_t GetDurationType(Duration_t duration);
DurationValue_t GetDurationValue(Duration_t duration);
_DATE_TIME GetAfterCurTime(const Duration_t &duration);
_DATE_TIME GetAfterSetTime(_DATE_TIME dtSetTime,const Duration_t &duration);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�õ�ǰʱ��

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME ComGetCurTime();

//////////////////////////////////////////////////////////////////////////
/*
@brief     ȡ�õ�ǰ���� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE ComGetCurDate();

//////////////////////////////////////////////////////////////////////////
/*
@brief   ��ȫ�ǿո�   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString add_wsp(const TCHAR* text,int strLen);

//////////////////////////////////////////////////////////////////////////
/*
@brief    �Ӱ�ǿո�  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString add_hsp(const TCHAR* text,int strLen);

//////////////////////////////////////////////////////////////////////////
/*
@brief   ���ұ߼Ӱ�ǿո�   

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
@brief   ������ʽ��Ϊ000,000.00��ʽ   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComFormatAmount(long iAmount);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���Ѿ���ʽ���õĽ���ʽ�л�ý��

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long ComGetAmount(CString sAmount);

//////////////////////////////////////////////////////////////////////////
/*
@brief  ��ʽ������000,000    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComFormatCount(int iCount);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ���������Ƿ�������  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComIsAllNumericInString(const CString& str);

//////////////////////////////////////////////////////////////////////////
/*
@brief   ����ַ��Ƿ�ȫΪ���ֲ�����min��max֮��   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CheckNumText(CString srcData,int min,int max);
bool GetAppPath(CString& sAppPath);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���ض��ָ�������ַ� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void GetSplitStrArray(CString sData,CString sSplitFlag,CStringArray &array);
_DATE GetDayAfterTypicalDay(WORD publishDay,WORD expirationDay);

//////////////////////////////////////////////////////////////////////////
/*
@brief   ȡ���ض����ں��ض�����������   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetYearAfter(const _DATE& date, unsigned short year);

//////////////////////////////////////////////////////////////////////////
/*
@brief   ȡ��ָ�����ں��ض�����������   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayAfter(const _DATE& date, DWORD day);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ȡ��ָ��ʱ������ʱ��  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetDateTimeAfter(const _DATE_TIME & datetime,DWORD day);

//////////////////////////////////////////////////////////////////////////
/*
@brief  ȡ��ָ�����ڵ�ǰ���������    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayBefore(const _DATE& date, DWORD day);

//////////////////////////////////////////////////////////////////////////
/*
@brief ȡ������ʱ��֮�������     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD GetOffTimeSecond(const _DATE_TIME& offDate,const _DATE_TIME& baseDate);
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ����������֮�������

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD GetOffTimeDay(const _DATE& offDate,const _DATE& baseDate);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ���ض�ʱ����ض����ʱ��

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetDateTimeAfterBySecond(const _DATE_TIME& baseDate, unsigned long offSeconds);

//////////////////////////////////////////////////////////////////////////
/*
@brief   �Ƚ�����ʱ�������   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CmpTwoDateTime(const _DATE_TIME& date1, const _DATE_TIME& date2);
//////////////////////////////////////////////////////////////////////////
/*
@brief   �Ƚ��������ڵ�����   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CmpTwoDate(const _DATE& date1, const _DATE& date2);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ȡ�����ڵ����ֱ�ʾ  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComGetWeekdayNameCN(int weekday);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��ȡ�������ļ� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ReadBinaryFile(const TCHAR* sPathFileName,VARIABLE_DATA &bufferStruct);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ����Ŀ¼ 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CreateFullDirectory(CString sPathName);        //����Ŀ¼

//////////////////////////////////////////////////////////////////////////
/*
@brief  ȡ�õ�ǰʱ�䣨Time_t���ͣ�

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Time_t ComGetCurTimeT();

//////////////////////////////////////////////////////////////////////////
/*
@brief     �ж�һ��char�Ƿ���BCD 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckCharIsBCD(char s);

//////////////////////////////////////////////////////////////////////////
/*
@brief   �жϸ������ַ��Ƿ���BCD   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckIsBCD(char *s, int len);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��TimeTת����_DATE_TIME��ʽ  

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
@brief    ���������������ļ�  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void SearchFile(CString searchCondition,std::vector<CString>& result);
//////////////////////////////////////////////////////////////////////////
/*
@brief    ʮ������ת����ʮ����  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Hex2Dec(int);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ʮ����ת����ʮ������  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Dec2Hex(int);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��CRC 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
unsigned long Crc32(void *pStart, unsigned long length, unsigned long crc32);

//////////////////////////////////////////////////////////////////////////
/*
@brief   �����Ƿ��д��󣬵����ڴ������õ�   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CheckHeapHealth();

//////////////////////////////////////////////////////////////////////////
/*
@brief ������֤�Ƿ���ȷ��������л����񹲺͹�����½�������������֤��     

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCard(CString number);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��֤���֤���루���ţ�

@param      ��

@retval     true:�ɹ�  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCardMACAO(CString number);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��֤���֤���루��ۣ�

@param      ��

@retval     true:�ɹ�  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCardHongKong(CString number);

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��֤��������

@param      ��

@retval     true:�ɹ�  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CheckBirthday(CString birthday);

//////////////////////////////////////////////////////////////////////////
/*
@brief    ���������������ļ�  

@param
@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CStringArray* SplitCString(CString str,CString delim);

//////////////////////////////////////////////////////////////////////////
/*
@brief  ����һ���ļ�    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString SearchOneFile(CString searchCondition);

//////////////////////////////////////////////////////////////////////////
/*
@brief     ɾ���ļ��� 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool DeleteFolder(CString sDirPath);

CString GetIniDataString(CString sFileName,LPCTSTR lpszSection, LPCTSTR lpszKey, CString sDefault);

#define COMMONLIB_API __declspec(dllimport)
#endif  // __MIMCUTILS_H_INCLUDED_