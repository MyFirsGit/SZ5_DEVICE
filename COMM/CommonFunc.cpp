#include "stdafx.h"
#include "CommonFunc.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief   �� ȡӪ������

@param      (i) unsigned short nBeforeDayNum       ��ǰ����֮ǰ������

@retval     _DATE  \n
Ӫ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
_DATE ComGetBusiDate(unsigned short nBeforeDayNum )
{
	__int64 ltime,lBusiStartTime,lBusiDate;
	struct tm tmTime;
	_DATE dtRetDate ;
	BYTE wHour = 0,wMinute = 0,wSecond = 0;

	//��ǰ������
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

	//ҵ��ʼʱ��
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
@brief     ����Ӫ����ȫ�ֱ�����ֵ

@param     BYTE wHour      ����������Ӫ���տ�ʼ��ʱ�䣨Сʱ��
@param     BYTE wMinute ������������ Ӫ���տ�ʼ��ʱ�䣨���ӣ�

@retval   _DATE  Ӫ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//Ӫ���ո����ʱ��
//BYTE g_wHour=0 ;   //Сʱ
//BYTE g_wMinute=0 ; //����

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
@brief      �õ�Ӫ�˿�ʼʱ��

@param      (i)unsigned short& Hour����Сʱ
@param      (i)unsigned short& Minute������
@param      (i)unsigned short& Second����

@retval      bool 
true:�ɹ��� false:ʧ��

@exception   ��
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
