#include "util.h"
#include "UIDefinition.h"
#pragma once

//������ʾ�ó�������
static const int MAX_ARRAY_LEN	= 9;            ///< ��ͨ��Ϣ�����������


// �Ӳ˵���ťλ��
#define  SUBMENU_BTN1_POINT          CPoint(36,489)			///< �Ӳ˵���һ����ť���Ͻ�����
#define  SUBMENU_BTN2_POINT          CPoint(143,489)		///< �Ӳ˵��ڶ�����ť���Ͻ�����
#define  SUBMENU_BTN3_POINT          CPoint(250,489)			///< �Ӳ˵���������ť���Ͻ�����
#define	SUBMENU_BTN4_POINT          CPoint(357,489)			///< �Ӳ˵����ĸ���ť���Ͻ�����
#define  SUBMENU_BTN5_POINT          CPoint(464,489)			///< �Ӳ˵��������ť���Ͻ�����
#define  PRINT_BTN_POINT             CPoint(571,489)			///< �Ӳ˵���ӡ��ť���Ͻ�����
#define  BACK_BTN_POINT              CPoint(678,489)			///< �Ӳ˵����ذ�ť���Ͻ�����
#define  OK_BTN_POINT               CPoint(785,489)			///< �Ӳ˵�ȷ����ť���Ͻ�����
#define  STANDARD_BTN_POINT         CPoint(NORMAL_BUTTON_WIDTH,NORMAL_BUTTON_HEIGHT)
#define   SUBMENU_BTN3_RECT           CRect(SUBMENU_BTN3_POINT,SUBMENU_BTN3_POINT+STANDARD_BTN_POINT)	///< �Ӳ˵���������ť��С
#define   SUBMENU_BTN4_RECT           CRect(SUBMENU_BTN4_POINT,SUBMENU_BTN4_POINT+STANDARD_BTN_POINT)	///< �Ӳ˵����ĸ���ť��С
#define   SUBMENU_BTN5_RECT           CRect(SUBMENU_BTN5_POINT,SUBMENU_BTN5_POINT+STANDARD_BTN_POINT)	///< �Ӳ˵��������ť��С

static const int DATAINPUT_MSG_POINT_X           =   26;
static const int DATAINPUT_DATATYPE_MSG_POINT_Y  =    8;
static const int DATAINPUT_INPUTTYPE_MSG_POINT_Y =  172;

static const int DEVICETEST_TESTTYPE_MSG_POINT_X     =   26;
static const int DEVICETEST_TESTTYPE_MSG_POINT_Y     =    8;
static const int DEVICETEST_TESTDETAIL_MSG_POINT_X   =   26;
static const int DEVICETEST_TESTDETAIL_MSG_POINT_Y   =  264;

static const int DEVICETEST_DETAILITEM_MSG_POINT_Y =  292;