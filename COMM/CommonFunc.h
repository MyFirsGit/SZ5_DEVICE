#pragma once
#include "stdafx.h"
#include "Util.h"


_DATE ComGetBusiDate(unsigned short nBeforeDayNum=0 );
void ComSetBusiDateSwitchTime(BYTE wHour ,BYTE wMinute,BYTE wSecond);
bool GetBusiDateSwitchTime(unsigned char& Hour, unsigned char& Minute, unsigned char& Second);


// ��ӡ��ֽһ���������İ���ַ���
static const short PRINT_MAX_WIDTH = 38;

// �������뻭�水ť����Ͱ�ť����
static const CPoint DATAINPUT_DATATYPE_BTNGROUP_POINT	= CPoint(120,34);
static const CPoint DATAINPUT_INPUTTYPE_BTNGROUP_POINT	= CPoint(120,194);
static const CRect  DATAINPUT_DATATYPE_RECT				= CRect(15,2,886,122);
static const CRect  DATAINPUT_INPUTTYPE_RECT			= CRect(15,166,886,282);

static const CRect  DEVICETEST_TESTTYPE_RECT            = CRect(15,2,886,122);      // �豸�Լ� ��������
static const CRect  DEVICETEST_DETAILITEM_RECT          = CRect(15,286,886,122);    // �豸�Լ� ��ϸ��Ŀ

static unsigned char    m_uHour;
static unsigned char    m_uMinute;
static unsigned char    m_uSecond;
