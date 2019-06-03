#pragma once
#include "servicedefinition.h"
#include <vector>
#include "Bag.h"
#include "GUIInfo.h"
#include "LanguageMsg.h"
#include "CommonUIDefinition.h"

using namespace std;


#define IDCLOSE IDCANCEL+100
#define IDREST  IDCANCEL+101


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宽度分辨率
#define RESOLUTION (theGUI_INFO.GetOperatorImageResolutionWidth() / 800)
#define HRESOLUTION (theGUI_INFO.GetOperatorImageResolutionHeight() / 600)

#define	CXSCREEN			theGUI_INFO.GetOperatorImageResolutionWidth()/*800*/
#define	CYSCREEN			theGUI_INFO.GetOperatorImageResolutionHeight()/*600*/

const int	CXCLIENTSCREEN =	1920;
const int	CYCLIENTSCREEN = 	1080;

const static double REC_RATIOWIDTH = theGUI_INFO.GetReceptionSolutionWidthRatio();
const static double REC_RATIOHEIGHT = theGUI_INFO.GetReceptionSolutionHeightRatio();



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////背景色定义///////////////////////////////////////////////////////////////

#define LIGHTBLUE					theGUI_INFO.GetBaseBakColor()           
#define GRAY                        theGUI_INFO.GetBaseBakColor()       
#define OPERATOR_HEAD_BK_COLOR      theGUI_INFO.GetTitleBakColor() 
#define OPERATOR_STATUS_BK_COLOR    theGUI_INFO.GetStatusBakColor() 
#define OPERATOR_FUNC_BK_COLOR      theGUI_INFO.GetFunBakColor() 
#define RECEPTION_GUIDE_BK_COLOR    theGUI_INFO.GetStatusBakColor() 
#define RECEPTION_BK_COLOR          theGUI_INFO.GetBaseBakColor() 
#define RECEPTION_HEAD_BK_COLOR     theGUI_INFO.GetTitleBakColor() 

#define OPERATOR_GUIDE_BK_COLOR		theGUI_INFO.GetGuideAreaColor()
#define OPERATOR_BASE_BK_COLOR		theGUI_INFO.GetBaseInfoAreaColor()
#define OPERATOR_STATE_BK_COLOR		theGUI_INFO.GetStateAreaColor()
#define OPERATOR_GUIDE_FUNC_COLOR	theGUI_INFO.GetFuncAreaColor()

#define OERATOR_RESOLUTION_WIDTH_RAT	theGUI_INFO.GetOperatorImageResolutionWidth()
#define OERATOR_RESOLUTION_HEIGHT_RAT	theGUI_INFO.GetOperatorImageResolutionHeight()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////大小定义////////////////////////////////////////////////////////////////
static const CSize DEFAULT_SIZE_BUTTON	= CSize(90,60);
static const CSize DEFAULT_SIZE_MENU_BUTTON	= CSize(165,72);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const COLORREF DEFAULT_COLOR_STATIC = GRAY;
static const MARGINS DEFAULT_MARGINS = {0,0,0,0};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////界面主区域定义//////////////////////////////////////////////////
static CRect const LOGO_RECT                    = CRect(5,5,370,90);			// 站员界面LOGO区域
static const CRect OPERATION_FRMAE_RECT			= CRect(262,150,762,730);		// 操作对话框（基本信息+钱信息+按钮）
static const CRect INIT_FRAME_RECT		     	= CRect(0,100,1024,730);


//static const CRect KEYBOARD_FRAME_RECT			= CRect(762,467,1024,730);		// 键盘区域
static const CRect GUIDE_FRAME_RECT				= CRect(262,100,762,150);		// 导航对话框
static const CRect BTN_OK_RECT = CRect(410,520,490,570);
static const CRect BTN_BACK_RECT = CRect(1120,10,1280,70);
static const CRect BTN_PRINT_RECT = CRect(410,520,490,570); // (210,520,290,570)
static const CRect BTN_RESET_RECT = CRect(10,520,90,570);
static const CPoint LEFTUP_AREA_POINT = CPoint(0,0);
static const CPoint LEFTDOWN_AREA_POINT = CPoint(0,350);

static const CRect OPERATION_BUTTON_RECT			= CRect(0,510,500,580);			// 底部操作按钮区域
static const CRect MESSAGE_BOX_FRAME_RECT			= CRect(324,375,724,635);		// 消息框
static const CRect MESSAGE_PICTURE_BOX_FRAME_RECT	= CRect(240,212,1040,812);		// 消息图片框
static const CRect  CASH_INFO_RECT					= CRect(163,86,240,107);		// 现金信息区域中编辑框的坐标

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// 操作员界面区域定义
static const int status_h = 30;
static const int navi_h = 30;
static const CRect OPERATOR_STATUS_RECT = CRect(0,0,800, status_h);	// 状态栏区域
static const CRect OPERATOR_GUIDE_RECT = CRect(0,status_h, 800,status_h+30);	// 向导文言栏区域
static const CRect OPERATOR_BASE_RECT = CRect(0,status_h+30,800,600-navi_h);	// 基本信息区域
static const CRect OPERATOR_NAVI_RECT = CRect(0,600-navi_h,800,600);	// 功能导航栏区域
static const CRect OPERATOR_INIT_RECT = CRect(0,status_h,800,600-navi_h);	// 初始化区域（覆盖向导文言区域和基本信息区域）

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 乘客界面区域定义
static const CRect REC_FRMAE_RECT		       = CRect(0,130,1920,1080-110);		    // 操作对话框（基本信息+收款信息+确认按钮）	
static const CRect REC_BUTTEN_GROUP_RECT	   = CRect(980,0,1920,844);		    // 按纽区域
static const CPoint BASEINFO_AREA_POINT        = CPoint(160,0);					// 乘客基本信息区域的起始点
static const CPoint CASHINFO_AREA_POINT        = CPoint(160,175);				// 乘客现金信息区域的起始点
static const CPoint NOTIFYINFO_AREA_POINT       = CPoint(160,740);				// 乘客提示信息区域的起始点
static const CPoint GUIDEINFO_AREA_POINT       = CPoint(680,0);					// 乘客提示信息区域的起始点
static const CPoint BTN_INFO_AREA_POINT        = CPoint(980,0);					// 乘客按纽信息区域的起始点

//static const CRect REC_HEAD_RECT			   = CRect(0,0,1920,80);
static const CRect REC_GUIDE_RECT              = CRect(CPoint(800,70),CSize(400,560));
static const CRect REC_BODY_RECT               = CRect(0,100,980,944);         
static const CRect REC_AREA_MAP_RECT           = CRect(0,100,980,944);   
static const CRect REC_BTN_OK_RECT             = /*CRect(720,760,940,820);*/CRect(CPoint(500,720),CSize(215,60));
static const CRect REC_BTN_OkAndCancel1_RECT   = CRect(535,820,755,880);
static const CRect REC_BTN_OkAndCancel2_RECT   = CRect(855,820,1075,880);
static const CRect REC_BTN_PRINT_RECT          = CRect(200,760,420,820);  
static const CRect REC_BTN_CANCELPRINT_RECT    = CRect(560,760,780,820);  
//// 再版
static const int head_h = 130;
static const int memu_h = 110;
static const CRect REC_HEAD_RECT				= CRect(0, 0, CXCLIENTSCREEN, head_h);								// logo，车站所在状态窗口区域
static const CRect RECT_OUTSVC					= CRect(0, head_h, CXCLIENTSCREEN, CYCLIENTSCREEN-memu_h);			// 业务信息显示窗口区域	
static const CRect REC_MENU_MAIN_RECT			= CRect(0, CYCLIENTSCREEN-memu_h, CXCLIENTSCREEN, CYCLIENTSCREEN);	// 业务菜单

static const int border_h = 5;
static const int border_w = 5;
static const int kongbai_x = 7;

static const CSize left_number_size				= CSize(248,833);
static const CSize right_prices_size			= CSize(228,707);
static const CSize top_line_size				= CSize(1326,118);
static const CSize line_map_size				= CSize(1420,715);
static const CSize button_size					= CSize(165,72);
static const CSize cash_in_notic_size			= CSize(323,121);

static const CRect REC_LEFT_NUMBER_RECT			= CRect(CPoint(0, REC_HEAD_RECT.bottom+border_h), left_number_size);												// 线路选择
static const CRect REC_AREA_TOP_RECT			= CRect(CPoint(left_number_size.cx + kongbai_x, REC_HEAD_RECT.bottom+kongbai_x), top_line_size);
static int line_map_posX = left_number_size.cx + kongbai_x;
static int line_map_posY = REC_AREA_TOP_RECT.bottom + border_h;
static const CRect REC_LINE_MAP_RECT			= CRect(CPoint(line_map_posX, line_map_posY+4), line_map_size);						// 线路站点信息
static const CRect REC_RIGHT_PRICES_RECT		= CRect(CPoint(left_number_size.cx+border_w+kongbai_x+line_map_size.cx,line_map_posY), right_prices_size);			// 票价选择
static const CRect REC_CASHIN_NOTICE_RECT		= CRect(CPoint(left_number_size.cx+top_line_size.cx+kongbai_x+border_h,border_h),cash_in_notic_size);
//static const CRect REC_RIGHT_NUMBER_RECT		= CRect(CPoint(RECT_OUTSVC.right-right_prices_size.cx-kongbai_x, REC_RIGHT_PRICES_RECT.bottom), right_number_size);	// 张数选择
//static const CRect REC_RIGHT_TOPSTATION_RECT	= CRect(CPoint(RECT_OUTSVC.right-right_prices_size.cx-kongbai_x, REC_RIGHT_NUMBER_RECT.bottom/*+border_h*/), right_topstation_size);// 热点车站
//static const CRect REC_RIGHT_FUNC_RECT			= CRect(CPoint(RECT_OUTSVC.right-right_prices_size.cx-kongbai_x, REC_RIGHT_TOPSTATION_RECT.bottom/*+border_h*/), right_func_size);	// 功能选择
static const CRect WELCOME_FRAME_RECT		   = RECT_OUTSVC;//CRect(0,0,1920,1000);		    // 欢迎框

static const CRect REC_ISSUE_FRAME_RECT		   = REC_LINE_MAP_RECT;		    // 操作对话框（基本信息+收款信息+确认按钮）	
static const CRect  RECT_BASE = CRect(CPoint(left_number_size.cx+kongbai_x, top_line_size.cy+border_h+2),CSize(REC_LINE_MAP_RECT.Width(),REC_LINE_MAP_RECT.Height()));

// h: 50 + 1180 + 50
// w: 140 + 1480 + 300

// logo字体    
#define LOGO_FONT_NAME_CN                  theGUI_INFO.GetLogoFontNameCN()
#define LOGO_FONT_NAME_EN                  theGUI_INFO.GetLogoFontNameEN()
#define LOGO_FONT_SIZE_CN                  theGUI_INFO.GetLogoFontSizeCN()
#define LOGO_FONT_SIZE_EN                  theGUI_INFO.GetLogoFontSizeEN()


//#define REC_STATION_FONT_SIZE_CN         theGUI_INFO.GetRecStationFontSize(LANGUAGE_ID_CHINESE)
//#define REC_STATION_FONT_SIZE_EN         theGUI_INFO.GetRecStationFontSize(LANGUAGE_ID_ENGLISH) 
//#define REC_STATION_FONT_SIZE_PN         theGUI_INFO.GetRecStationFontSize(LANGUAGE_ID_PORTUGUESE) 
//#define REC_STATION_FONT_NAME_CN         theGUI_INFO.GetRecStationFontName(LANGUAGE_ID_CHINESE)
//#define REC_STATION_FONT_NAME_EN         theGUI_INFO.GetRecStationFontName(LANGUAGE_ID_ENGLISH) 
//#define REC_STATION_FONT_NAME_PN         theGUI_INFO.GetRecStationFontName(LANGUAGE_ID_PORTUGUESE) 

#define REC_TITLE_FONT_SIZE_CN         theGUI_INFO.GetRecTitleFontSize(LANGUAGE_ID_CHINESE)
#define REC_TITLE_FONT_SIZE_EN         theGUI_INFO.GetRecTitleFontSize(LANGUAGE_ID_ENGLISH) 
#define REC_TITLE_FONT_SIZE_PN         theGUI_INFO.GetRecTitleFontSize(LANGUAGE_ID_PORTUGUESE) 
#define REC_TITLE_FONT_NAME_CN         theGUI_INFO.GetRecTitleFontName(LANGUAGE_ID_CHINESE)
#define REC_TITLE_FONT_NAME_EN         theGUI_INFO.GetRecTitleFontName(LANGUAGE_ID_ENGLISH) 
#define REC_TITLE_FONT_NAME_PN         theGUI_INFO.GetRecTitleFontName(LANGUAGE_ID_PORTUGUESE) 

#define REC_DATE_FONT_SIZE_CN         theGUI_INFO.GetRecDateFontSize(LANGUAGE_ID_CHINESE)
#define REC_DATE_FONT_SIZE_EN         theGUI_INFO.GetRecDateFontSize(LANGUAGE_ID_ENGLISH) 
#define REC_DATE_FONT_SIZE_PN         theGUI_INFO.GetRecDateFontSize(LANGUAGE_ID_PORTUGUESE) 
#define REC_DATE_FONT_NAME_CN         theGUI_INFO.GetRecDateFontName(LANGUAGE_ID_CHINESE)
#define REC_DATE_FONT_NAME_EN         theGUI_INFO.GetRecDateFontName(LANGUAGE_ID_ENGLISH) 
#define REC_DATE_FONT_NAME_PN         theGUI_INFO.GetRecDateFontName(LANGUAGE_ID_PORTUGUESE)


#define REC_BASEINFO_FONT_SIZE_CN         theGUI_INFO.GetRecBaseInfoFontSize(LANGUAGE_ID_CHINESE)
#define REC_BASEINFO_FONT_SIZE_EN         theGUI_INFO.GetRecBaseInfoFontSize(LANGUAGE_ID_ENGLISH) 
#define REC_BASEINFO_FONT_SIZE_PN         theGUI_INFO.GetRecBaseInfoFontSize(LANGUAGE_ID_PORTUGUESE) 
#define REC_BASEINFO_FONT_NAME_CN         theGUI_INFO.GetRecBaseInfoFontName(LANGUAGE_ID_CHINESE)
#define REC_BASEINFO_FONT_NAME_EN         theGUI_INFO.GetRecBaseInfoFontName(LANGUAGE_ID_ENGLISH) 
#define REC_BASEINFO_FONT_NAME_PN         theGUI_INFO.GetRecBaseInfoFontName(LANGUAGE_ID_PORTUGUESE) 


#define REC_GUIDEINFO_FONT_SIZE_CN         theGUI_INFO.GetRecGuideFontSize(LANGUAGE_ID_CHINESE)
#define REC_GUIDEINFO_FONT_SIZE_EN         theGUI_INFO.GetRecGuideFontSize(LANGUAGE_ID_ENGLISH) 
#define REC_GUIDEINFO_FONT_SIZE_PN         theGUI_INFO.GetRecGuideFontSize(LANGUAGE_ID_PORTUGUESE) 
#define REC_GUIDEINFO_FONT_NAME_CN         theGUI_INFO.GetRecGuideFontName(LANGUAGE_ID_CHINESE)
#define REC_GUIDEINFO_FONT_NAME_EN         theGUI_INFO.GetRecGuideFontName(LANGUAGE_ID_ENGLISH) 
#define REC_GUIDEINFO_FONT_NAME_PN         theGUI_INFO.GetRecGuideFontName(LANGUAGE_ID_PORTUGUESE) 

#define REC_MESSAGEBOX_FONT_SIZE_CN         theGUI_INFO.GetRecMessageBoxFontSize(LANGUAGE_ID_CHINESE)
#define REC_MESSAGEBOX_FONT_SIZE_EN         theGUI_INFO.GetRecMessageBoxFontSize(LANGUAGE_ID_ENGLISH) 
#define REC_MESSAGEBOX_FONT_SIZE_PN         theGUI_INFO.GetRecMessageBoxFontSize(LANGUAGE_ID_PORTUGUESE) 
#define REC_MESSAGEBOX_FONT_NAME_CN         theGUI_INFO.GetRecMessageBoxFontName(LANGUAGE_ID_CHINESE)
#define REC_MESSAGEBOX_FONT_NAME_EN         theGUI_INFO.GetRecMessageBoxFontName(LANGUAGE_ID_ENGLISH) 
#define REC_MESSAGEBOX_FONT_NAME_PN         theGUI_INFO.GetRecMessageBoxFontName(LANGUAGE_ID_PORTUGUESE) 

#define REC_MESSAGEBOXTITLE_FONT_SIZE_CN    theGUI_INFO.GetRecMessageBoxTitleFontSize(LANGUAGE_ID_CHINESE)
#define REC_MESSAGEBOXTITLE_FONT_SIZE_EN    theGUI_INFO.GetRecMessageBoxTitleFontSize(LANGUAGE_ID_ENGLISH) 
#define REC_MESSAGEBOXTITLE_FONT_SIZE_PN    theGUI_INFO.GetRecMessageBoxTitleFontSize(LANGUAGE_ID_PORTUGUESE) 
#define REC_MESSAGEBOXTITLE_FONT_NAME_CN    theGUI_INFO.GetRecMessageBoxTitleFontName(LANGUAGE_ID_CHINESE)
#define REC_MESSAGEBOXTITLE_FONT_NAME_EN    theGUI_INFO.GetRecMessageBoxTitleFontName(LANGUAGE_ID_ENGLISH) 
#define REC_MESSAGEBOXTITLE_FONT_NAME_PN    theGUI_INFO.GetRecMessageBoxTitleFontName(LANGUAGE_ID_PORTUGUESE) 

#define OPERATOR_GUIDE_FONT_COLOR			theGUI_INFO.GetOperatorGuideFontColor()
//	乘客界面GUI配置
//	基本信息区域
#define GUEST_BASEINFO_FONT_SIZE			theGUI_INFO.GetRecBaseInfoFontSize()
#define GUEST_BASEINFO_FONT_NAME			theGUI_INFO.GetRecBaseInfoFontName()
#define GUEST_BASEINFO_FONT_COLOR			theGUI_INFO.GetRecBaseInfoFontColor()
//	状态栏
#define GUEST_STATE_STATION_FONT_SIZE		theGUI_INFO.GetRecStationFontSize()
#define GUEST_STATE_STATION_FONT_NAME		theGUI_INFO.GetRecStationFontName()
#define GUEST_STATE_STATION_FONT_COLOR		theGUI_INFO.GetRecStationFontColor()
#define GUEST_STATE_DATE_FONT_SIZE			theGUI_INFO.GetRecDateFontSize()
#define GUEST_STATE_DATE_FONT_NAME			theGUI_INFO.GetRecDateFontName()
#define GUEST_STATE_DATE_FONT_COLOR			theGUI_INFO.GetRecDateFontColor()
#define GUEST_STATE_SVC_FONT_SIZE			theGUI_INFO.GetSvcStateFontSize()
#define GUEST_STATE_SVC_FONT_NAME			theGUI_INFO.GetSvcStateFontName()
#define GUEST_STATE_SVC_NORMAL_FONT_COLOR	theGUI_INFO.GetNormalSvcStateFontColor()
#define GUEST_STATE_SVC_UNNORMAL_FONT_COLOR	theGUI_INFO.GetUnNormalSvcStateFontColor()
//	按钮
#define GUEST_FUNC_BTN_TITLE_FONT_SIZE		theGUI_INFO.GetFuncBtnGroupTitleFontSize()
#define GUEST_FUNC_BTN_TITLE_FONT_NAME		theGUI_INFO.GetFuncBtnGroupTitleFontName()
#define GUEST_FUNC_BTN_TITLE_FONT_COLOR		theGUI_INFO.GetFuncBtnGroupTitleFontColor()
#define GUEST_FUNC_BTN_FONT					theGUI_INFO.GetBtnFont()
#define GUEST_FUNC_BTN_FONT_COLOR			theGUI_INFO.GetFuncBtnFontColor()
#define GUEST_MENUE_BTN_FONT				theGUI_INFO.GetRecMenuBtnFont()
#define GUEST_MENUE_BTN_FONT_COLOR			theGUI_INFO.GetMenuBtnFontColor()
//	消息框
#define GUEST_MSGBOX_TITLE_FONT_SIZE		theGUI_INFO.GetRecMessageBoxTitleFontSize()
#define GUEST_MSGBOX_TXT_FONT_SIZE			theGUI_INFO.GetRecMessageBoxFontSize()
#define GUEST_MSGBOX_TITLE_FONT_NAME		theGUI_INFO.GetRecMessageBoxTitleFontName()
#define GUEST_MSGBOX_TXT_FONT_NAME			theGUI_INFO.GetRecMessageBoxFontName()
#define GUEST_MSGBOX_TITLE_FONT_COLOR		theGUI_INFO.GetRecMsgBoxTitleFontColor()
#define GUEST_MSGBOX_TXT_FONT_COLOR			theGUI_INFO.GetRecMsgBoxFontColor()

// 菜单编号
static const int MENU_CODE_0 = 0;
static const int MENU_CODE_1 = 1;
static const int MENU_CODE_2 = 2;
static const int MENU_CODE_3 = 3;
static const int MENU_CODE_4 = 4;
static const int MENU_CODE_5 = 5;
static const int MENU_CODE_6 = 6;
static const int MENU_CODE_7 = 7;
static const int MENU_CODE_8 = 8;
static const int MENU_CODE_9 = 9;
static const int MENU_CODE_10 = 10;
static const int MENU_CODE_11 = 11;
static const int MENU_CODE_12 = 12;
static const int MENU_CODE_13 = 13;
static const int MENU_CODE_14 = 14;
static const int MENU_CODE_15 = 15;
static const int MENU_CODE_16 = 16;
static const int MENU_CODE_17 = 17;
static const int MENU_CODE_18 = 18;
static const int MENU_CODE_19 = 19;
static const int MENU_CODE_20 = 20;
static const int MENU_CODE_31 = 31;
static const int MENU_CODE_32 = 32;
static const int MENU_CODE_33 = 33;
static const int MENU_CODE_41 = 41;
static const int MENU_CODE_42 = 42;
static const int MENU_CODE_43 = 43;
static const int MENU_CODE_100 = 100;
static const int MENU_CODE_1000 = 1000;
static const int MENU_CODE_10000 = 10000;

// 行号
static const int LABEL_ROW_1 = 0;
static const int LABEL_ROW_2 = 1;
static const int LABEL_ROW_3 = 2;
static const int LABEL_ROW_4 = 3;
static const int LABEL_ROW_5 = 4;
static const int LABEL_ROW_6 = 5;
static const int LABEL_ROW_7 = 6;
static const int LABEL_ROW_8 = 7;
static const int LABEL_ROW_9 = 8;
static const int LABEL_ROW_10 = 9;
static const int LABEL_ROW_11 = 10;
static const int LABEL_ROW_12 = 11;
static const int LABEL_ROW_13 = 12;
static const int LABEL_ROW_14 = 13;
static const int LABEL_ROW_15 = 14;

// 列号
static const int LABEL_COL_1 = 0;
static const int LABEL_COL_2 = 1;
static const int LABEL_COL_3 = 2;
static const int LABEL_COL_4 = 3;
static const int LABEL_COL_5 = 4;
static const int LABEL_COL_6 = 5;
static const int LABEL_COL_7 = 6;

// 下面这个多了第三个参数，是编辑框起始点比通常偏后的半角字节数
#define BASE_INFO_RECT_SP(row,len,leftStart)   CRect(160+9*leftStart,BASE_INFO_RECT_TOP(row),160+9*leftStart+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+25)

// 
#define BASE_INFO_RECT2_TOP(row) (4+(row-1)*34+row)
//#define BASE_INFO_RECT2_TOP(row) (6+(row-1)*(HRESOLUTION * 25)+row)

#define BASE_INFO_RECT2_WIDTH(len) ((len+1)*9)
#define BASE_INFO_RECT2(row,width,len) CRect(width+10,BASE_INFO_RECT2_TOP(row),width+BASE_INFO_RECT2_WIDTH(len),BASE_INFO_RECT2_TOP(row)+24)

// 定义金额最大长度
static const int MONEY_MAX_LEN  = 4;

typedef enum _Menu_Item_ID{
	
	ISSUE_ID = 100,
	RECHARGE_ID = 200,
	ADDPRODUCT_ID = 300,
	ANALYZE_SVC_ID = 400,
	ISSUE_USE_SVT_ID = 500,
	LANGUAGE_SWITH_ID = 600,
	//CALL_HELP_SVC_ID = 500,

	MAINTENANCE_SVC_ID							= 2000, // 维护业务
	TRANSACTION_HISTORY_ID						= 2100, // 交易一览
	DEVICE_STATUS_ID							= 2200, // 设备状态 
	SALE_STATISTIC_SVC_ID						= 2300, // 销售统计
	CARD_BOX_MANAGE_SVC_ID						= 2400, // 票箱管理
	TOKEN_BOX_MANAGE_SVC_ID						= 2500, // Token箱管理
	BANKNOTE_BOX_MANAGE_SVC_ID					= 2600, // 纸币箱管理
		INSTALL_BANKNOTE_COLLECTION_BOX_SVC_ID					= 2601, // 安装纸币回收箱
		REMOVE_BANKNOTE_COLLECTION_BOX_SVC_ID					= 2602,	// 卸载纸币回收箱
		INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC_ID				= 2603, // 安装纸币找零，废币箱
		REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC_ID				= 2604,	// 卸载纸币找零，废币箱
		BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC_ID				= 2605,	// 纸币回收功能
	COIN_BOX_MANAGE_SVC_ID						= 2700, // 硬币箱管理
		ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC_ID			= 2701, // 硬币专用找零箱
		ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC_ID				= 2702, // 硬币循环找零箱
		ATTACH_COLLECTION_COIN_BOX_SVC_ID						= 2703, // 安装硬币回收箱
		DETACH_COLLECTION_COIN_BOX_SVC_ID						= 2704,	// 卸载硬币回收箱
		//DETACH_ADDING_COIN_BOX_SVC_ID							= 2705, // 卸载硬币补充箱
		COLLECTION_COIN_SVC_ID									= 2706,	// 回收硬币
		INSTALL_COIN_CHANGE_BOX_SVC_ID							= 2707,	// 安装硬币找零箱
		REMOVE_COIN_CHANGE_BOX_SVC_ID							= 2708,	// 卸载硬币找零箱

	COLLECTION_MANAGE_SVC_ID					= 2800, // 回收处理
	OPERATION_CLOSE_SVC_ID						= 2900, // 业务结束
	RUN_MODE_SETTING_SVC_ID						= 3000, // 运行模式设定
		SERVICE_MODE_SETTING_SVC_ID								= 3001, // 服务模式设定
		PAYMENT_MODE_SETTING_SVC_ID								= 3002, // 支付模式设定
		CHANGE_MODE_SETTING_SVC_ID								= 3003, // 找零模式设定
		PRINT_MODE_SETTING_SVC_ID								= 3004, // 打印模式设定
	RECEIPT_REPRINT_SVC_ID						= 3100, // 凭证补打
	SYSTEM_SET_SVC_ID							= 3200, // 系统设定
		STATION_SET_SVC_ID										= 3201, // 机站设置
		COMMUNICATION_SET_SVC_ID								= 3202, // 通信参数设置
		MODE_SWITCH_SVC_ID										= 3203, // 模式切换设置
		SYSTEM_TIME_SET_SVC_ID									= 3204, // 系统时间设定
		UDSN_SET_SVC_ID											= 3205, // UDSN设定
		SWITCH_OFF_SVC_ID										= 3206, // 切离设定
		RFID_MODE_SET_SVC_ID									= 3207, // RFID模式设定
		SCREEN_BRIGHTNESS_SVC_ID								= 3208,	// 屏幕亮度设定
		DEVICE_COMPORT_SET_ID									= 3209,	// 串口号设置
	DATA_MANAGE_SVC_ID							= 3300, // 数据管理
		DATA_INPUT_SVC_ID										= 3301, // 数据导入
		DATA_OUTPUT_SVC_ID										= 3302, // 数据导出
		VERSION_INFO_SVC_ID										= 3303, // 版本信息
	DEVICE_TEST_SVC_ID							= 3400, // 硬件自检
		RW_TEST_SVC_ID											= 3401, // 读写器测试
		MONEYBOX_TEST_SVC_ID									= 3402, // 硬币模块测试
		BANKNOTE_ACCEPT_MODULE_TEST_SVC_ID						= 3403, // 纸币接收模块测试
		BANKNOTE_CHANGE_MODULE_TEST_SVC_ID						= 3404, // 纸币找零模块测试
		CARD_ISSUE_MODULE_TEST_SVC_ID							= 3405, // 发卡模块测试
		TOKEN_MODULE_TEST_SVC_ID								= 3406, // Token模块测试
		RECEIPT_RINTER_TEST_SVC_ID								= 3407, // 凭证打印机测试
		PIN_PAD_TEST_SVC_ID										= 3408, // 密码键盘模块测试
		BANKCARD_MODULE_TEST_SVC_ID								= 3409, // 银行卡处理模块测试
		IO_MODULE_TEST_SVC_ID									= 3410, // IO模块测试
		STATUS_DISPLAY_TEST_SVC_ID								= 3411, // 状态显示屏测试
		NETWORK_TEST_SVC_ID										= 3412, // 网络测试
		UPS_TEST_SVC_ID											= 3413, // UPS测试
		ALL_DEVICE_STATUS_ID									= 3414,	// 所有硬件设备状态
	REMOVE_EXCEPTION_SVC_ID						= 3500, // 异常解除
	SHUTDOWN_ID									= 3600, // 关机重启
	    SHUTDOWN_COMPUTER_ID									= 3601, //关机
		RESTART_COMPUTTER_ID									= 3602, //重启
		EXIT_PROGRAM_ID											= 3603,	//退出
	ONE_KEY_SETTLEMENT_SVC_ID					= 3700,// 一键收机

} MENU_ITEM_ID;

/////////////////////////////////////////////////界面Group的ID定义///////////////////////////////////////////////////////////////
#define TOP_1_GROUP_ID 2
#define LEFT_1_GROUP_ID 3
#define LEFT_2_GROUP_ID 4
#define RIGHT_1_GROUP_ID 5
#define RIGHT_2_GROUP_ID 6
#define RIGHT_3_GROUP_ID 7
#define RIGHT_4_GROUP_ID 8
#define OPERATION_BUTTON_GROUP_ID 9
#define KEYBOARD_GROUP_ID 10
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int MAX_SBC_CASE_LEN		    = 8;		    // 左列全角字符最大长度
static const int MAX_SBC_CASE_LEN_RIGHT     = 8;            // 右列全角字符最大长度
static const int MAX_HALF_CASE_LEFT_LEN	    = 16;		    // 左列半角字符最大长度
                                   
// 单位名称定义
static const TCHAR* const TXT_YUAN_MARK					= _T("￥");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////BUTTON消息文本定义///////////////////////////////////////

static const TCHAR* const WM_BTN_PRINT			= _T("WM_BTN_PRINT");							///< 打印按钮消息
static const TCHAR* const WM_BTN_NOPRINT	    = _T("WM_BTN_NOPRINT");							///< 不打印按钮消息
static const TCHAR* const WM_BTN_BACK			= _T("WM_BTN_BACK");							///< 返回按钮消息
static const TCHAR* const WM_BTN_RESET			= _T("WM_BTN_RESET");						    ///< 重置按钮消息
static const TCHAR* const WM_BTN_ENGLISH	    = _T("WM_BTN_ENGLISH");							///< 打印按钮消息
static const TCHAR* const WM_BTN_CHINESE	    = _T("WM_BTN_CHINESE");							///< 确认按钮消息
static const TCHAR* const WM_BTN_PORTUGUESE		= _T("WM_BTN_PORTUGUESE");						///< 返回按钮消息
static const TCHAR* const WM_BTN_SCAN		    = _T("WM_BTN_SCAN");							///< 扫描按钮消息
static const TCHAR* const WM_BTN_OkAndCancel1	= _T("WM_BTN_OkAndCancel1");					///< 确认按钮消息
static const TCHAR* const WM_BTN_OkAndCancel2   = _T("WM_BTN_OkAndCancel2");					///< 取消按钮消息
static const TCHAR* const WM_BTN_SELECT_LANGUAGE= _T("WM_BTN_SELECT_LANGUZGE");					///< 选择语言按钮
static const TCHAR* const WM_BTN_CHARGE			= _T("WM_BTN_CHARGE");							///< 充值按钮
static const TCHAR* const WM_BTN_CASH_PAY		= _T("WM_BTN_CASH_PAY");							///< 现金支付
static const TCHAR* const WM_BTN_NET_PAY		= _T("WM_BTN_NET_PAY");							///< 扫码支付
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


// 相对路径以程序所在位置为基准
// GUI包定义
#define TVM_GUI_PATH					_T(".\\res\\gui\\")
#define TVM_GUI_IMAGE_CSV				_T(".\\res\\gui\\image.csv")
#define TVM_GUI_LAYOUT_CSV				_T(".\\res\\gui\\layout.csv")
#define TVM_GUI_BUTTON_CSV				_T(".\\res\\gui\\button.csv")
#define TVM_GUI_LAYOUT					_T(".\\res\\gui\\layout\\")

static const TCHAR* const AFC_MAP_ID_3085	= _T("TVM99.3085.*");					// 底图解析名称模板

#define TVM_GUI_BTN_NORMAL				_T(".\\res\\gui\\btn\\station_btn_normal.png")
#define TVM_GUI_BTN_LEFT				_T(".\\res\\gui\\btn\\left.png")
#define TVM_GUI_BTN_TOP					_T(".\\res\\gui\\btn\\top.png")
#define TVM_GUI_BTN_RIGHT				_T(".\\res\\gui\\btn\\right.png")
#define TVM_GUI_BTN_BOTTOM				_T(".\\res\\gui\\btn\\bottom.png")

// 图片定义
#define TVM_IMAGE_LOGO					_T(".\\res\\TVM_logo.png")
#define TVM_IMAGE_HEAD_BKG				_T(".\\res\\TVM_head_bkg.png")
#define TVM_IMAGE_STATUS_BKG			_T(".\\res\\TVM_status_bkg.png")
#define TVM_IMAGE_MODULE_STATUS_RED		_T(".\\res\\TVM_status_module_red.png")
#define TVM_IMAGE_MODULE_STATUS_GREEN	_T(".\\res\\TVM_status_module_green.png")
#define TVM_IMAGE_MODULE_STATUS_YELLOW	_T(".\\res\\TVM_status_module_yellow.png")
#define TVM_IMAGE_MODULE_STATUS_GRAY	_T(".\\res\\TVM_status_module_gray.png")
#define TVM_IMAGE_SERVICE_NORMAL		_T(".\\res\\TVM_bg_StatusNormal.png")
#define TVM_IMAGE_SERVICE_ABNORMAL		_T(".\\res\\TVM_bg_StatusAbnormal.png")
#define TVM_IMAGE_FUNC_BTN_BKG			_T(".\\res\\TVM_func_btn_bkg.png")

#define TVM_IMAGE_BTN_NORMAL			_T(".\\res\\TVM_btn_Purchase_Ok.png")
#define TVM_IMAGE_BTN_DOWN				_T(".\\res\\TVM_btn_Print_Confirm.png")

#define TVM_IMAGE_OK_NORMAL				_T(".\\res\\TVM_btn_Purchase_Ok_Normal.png")
#define TVM_IMAGE_OK_DOWN				_T(".\\res\\TVM_btn_Purchase_Ok_Pressed.png")

#define TVM_IMAGE_STATION_BTN_NORMAL	_T(".\\res\\TVM_btn_station_on.png")
#define TVM_IMAGE_STATION_BTN_DOWN		_T(".\\res\\TVM_btn_station_off.bmp")

#define TVM_IMAGE_CANCLE_BTN_NORMAL		_T(".\\res\\TVM_btn_Cancel_Default.png")

#define TVM_IMAGE_RIGHT_BTN_NORMAL		_T(".\\res\\TVM_btn_price_on.png")
#define TVM_IMAGE_RIGHT_BTN_DOWN		_T(".\\res\\TVM_btn_price_off.png")

#define TVM_IMAGE_CHARGE_BTN_NORMAL		_T(".\\res\\TVM_btn_Charge_Default.png")
#define TVM_IMAGE_CHARGE_BTN_DOWN		_T(".\\res\\TVM_btn_Charge_Pressed.png")

#define TVM_IMAGE_LANG_BTN_NORMAL		_T(".\\res\\TVM_btn_Group_Blue_Normal.png")//_T(".\\res\\TVM_btn_Language_Default.png")
#define TVM_IMAGE_LANG_BTN_DOWN			_T(".\\res\\TVM_btn_Group_Blue_Normal.png")//_T(".\\res\\TVM_btn_Language_Pressed.png")

#define TVM_IMAGE_GROUP_BLUE_NORMAL		_T(".\\res\\TVM_btn_Group_Blue_Normal.png")
#define TVM_IMAGE_GROUP_BLUE_DOWN		_T(".\\res\\TVM_btn_Group_Blue_Pressed.png")
#define TVM_IMAGE_GROUP_BLUE_DISABLE	_T(".\\res\\TVM_btn_Group_Blue_Disabled.png")

#define TVM_IMAGE_GROUP_BROWN_NORMAL	_T(".\\res\\TVM_btn_Group_Brown_Normal.png")
#define TVM_IMAGE_GROUP_BROWN_DOWN		_T(".\\res\\TVM_btn_Group_Brown_Pressed.png")
#define TVM_IMAGE_GROUP_BROWN_DISABLE	_T(".\\res\\TVM_btn_Group_Brown_Disabled.png")

#define TVM_IMAGE_GROUP_BTN_YELLOW		_T(".\\res\\TVM_btn_Group_Btn_Yellow.png")
#define TVM_IMAGE_GROUP_BTN_ORANGE		_T(".\\res\\TVM_btn_Group_Btn_Orange.png")
#define TVM_IMAGE_GROUP_BTN_BLUE		_T(".\\res\\TVM_btn_Group_Btn_Blue.png")
#define TVM_IMAGE_GROUP_BTN_HOT			_T(".\\res\\TVM_btn_Group_Btn_Hot.png")

#define  TVM_IMAGE_BOX_PICTURE			_T(".\\res\\TVM_SJT_issue.png")
// 提示区域图片
#define TVM_IMAGE_TIP_INSERT_COIN			_T(".\\res\\TVM_TIP_InsertCoins_Guide.gif")			// 投入硬币
#define TVM_IMAGE_TIP_INSERT_NOTE			_T(".\\res\\TVM_TIP_InsertBanknotes_Guide.gif")		// 投入纸币
#define TVM_TIP_INSERT_CARD					_T(".\\res\\TVM_TIP_ReInsert_card.gif")
#define	TVM_TIP_RETURN_COIN					_T(".\\res\\TVM_TIP_Return.gif")					// 退还硬币
#define TVM_TIP_ISSUING						_T(".\\res\\TVM_TIP_ISSUING.gif")					// 
#define TVM_TIP_GET_RETURN_MONEY			_T(".\\res\\TVM_TIP_GET_RETURN_MONEY.gif")			// 请取走现金
#define TVM_TIP_GET_RETURN_CARD				_T(".\\res\\TVM_TIP_GET_RETURN_CARD.gif")			// 请取走退卡
#define TVM_TIP_GET_RECEIPT					_T(".\\res\\TVM_TIP_GetReceipt.gif")				// 取凭条
#define TVM_IMAGE_TIP_RETURN_NOTE			_T(".\\res\\gui\\guide\\TVM_TIP_ReturnBanknotes_Guide.gif")		// 退回纸币
#define TVM_IMAGE_TIP_INSERT_BANKCARD		_T(".\\res\\gui\\guide\\TVM_TIP_ReturnBanknotes_Guide.gif")		// 插入银行卡
#define TVM_IMAGE_TIP_INSERT_EP				_T(".\\res\\gui\\guide\\TVM_TIP_ReturnBanknotes_Guide.gif")		// 使用电子钱包
#define TVM_IMAGE_TIP_ISSUE_UL				_T(".\\res\\gui\\guide\\TVM_TIP_ISSUE_UL.gif")					// 正在发售UL卡
#define TVM_IMAGE_TIP_ISSUE_M1				_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// 正在发售M1卡
#define TVM_TIP_ISSUE_TICKETS				_T(".\\res\\gui\\guide\\TVM_TIP_ISSUE_TICKETS.png")				// 发售单程票
#define TVM_IMAGE_CHARGING					_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// 正在充值中...
#define TVM_GIF_OPERATION_GUIDE				_T(".\\res\\gui\\guide\\TVM_TIP_Operation_Guide.gif")			// 提示选择功能
#define TVM_TIP_CONFIRM_NORMAL				_T(".\\res\\gui\\guide\\TVM_TIP_CONFIRM_NORMAL.gif")			// 请点击[Confirm]键
#define TVM_TIP_CONFIRM_BIG					_T(".\\res\\gui\\guide\\TVM_TIP_CONFIRM_BIG.gif")				// 请点击[Confirm]键 270*600
#define TVM_TIP_WARINGS						_T(".\\res\\gui\\guide\\TVM_TIP_WARINGS.png")					// 警告图标
#define TVM_TIP_SWIPE_CARD					_T(".\\res\\gui\\guide\\TVM_TIP_SWIPE_CARD.png")				// 请刷卡
#define TVM_TIP_GET_TOKEN_CHANGE			_T(".\\res\\gui\\guide\\TVM_TIP_GET_TOKEN_CHANGE.gif")			// 请取走Token和找零
#define TVM_TIP_PRINTING					_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// 正在打印中...
#define TVM_TIP_PRINTED						_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// 打印完成
#define TVM_TIP_RECHARGE_FINISHED			_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// 充值完成
#define TVM_TIP_FORBID_INSERT_COINS			_T(".\\res\\gui\\guide\\TVM_TIP_FORBID_INSERT_COINS.png")		// 禁止投入硬币
#define TVM_TIP_FORBID_INSERT_BANKNOTES     _T(".\\res\\gui\\guide\\TVM_TIP_FORBID_INSERT_BANKNOTES.png")	// 禁止投入纸币
#define TVM_TIP_SHOW_WARINGS				_T(".\\res\\gui\\guide\\TVM_TIP_WARINGS.png")					// 警告图

#define TVM_SYSTEM_IN_PROCESSING			_T(".\\res\\gui\\guide\\SYSTEM_IN_PROCESSING.gif")				// 提示系统正在处理中