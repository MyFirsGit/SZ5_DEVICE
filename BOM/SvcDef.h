#include "util.h"
#include "UIDefinition.h"
#pragma once

//画面显示用常量定义
static const int MAX_ARRAY_LEN	= 9;            ///< 普通信息区域最大行数


// 子菜单按钮位置
#define  SUBMENU_BTN1_POINT          CPoint(36,489)			///< 子菜单第一个按钮左上角坐标
#define  SUBMENU_BTN2_POINT          CPoint(143,489)		///< 子菜单第二个按钮左上角坐标
#define  SUBMENU_BTN3_POINT          CPoint(250,489)			///< 子菜单第三个按钮左上角坐标
#define	SUBMENU_BTN4_POINT          CPoint(357,489)			///< 子菜单第四个按钮左上角坐标
#define  SUBMENU_BTN5_POINT          CPoint(464,489)			///< 子菜单第五个按钮左上角坐标
#define  PRINT_BTN_POINT             CPoint(571,489)			///< 子菜单打印按钮左上角坐标
#define  BACK_BTN_POINT              CPoint(678,489)			///< 子菜单返回按钮左上角坐标
#define  OK_BTN_POINT               CPoint(785,489)			///< 子菜单确定按钮左上角坐标
#define  STANDARD_BTN_POINT         CPoint(NORMAL_BUTTON_WIDTH,NORMAL_BUTTON_HEIGHT)
#define   SUBMENU_BTN3_RECT           CRect(SUBMENU_BTN3_POINT,SUBMENU_BTN3_POINT+STANDARD_BTN_POINT)	///< 子菜单第三个按钮大小
#define   SUBMENU_BTN4_RECT           CRect(SUBMENU_BTN4_POINT,SUBMENU_BTN4_POINT+STANDARD_BTN_POINT)	///< 子菜单第四个按钮大小
#define   SUBMENU_BTN5_RECT           CRect(SUBMENU_BTN5_POINT,SUBMENU_BTN5_POINT+STANDARD_BTN_POINT)	///< 子菜单第五个按钮大小

static const int DATAINPUT_MSG_POINT_X           =   26;
static const int DATAINPUT_DATATYPE_MSG_POINT_Y  =    8;
static const int DATAINPUT_INPUTTYPE_MSG_POINT_Y =  172;

static const int DEVICETEST_TESTTYPE_MSG_POINT_X     =   26;
static const int DEVICETEST_TESTTYPE_MSG_POINT_Y     =    8;
static const int DEVICETEST_TESTDETAIL_MSG_POINT_X   =   26;
static const int DEVICETEST_TESTDETAIL_MSG_POINT_Y   =  264;

static const int DEVICETEST_DETAILITEM_MSG_POINT_Y =  292;