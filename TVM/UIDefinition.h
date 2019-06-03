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
// ��ȷֱ���
#define RESOLUTION (theGUI_INFO.GetOperatorImageResolutionWidth() / 800)
#define HRESOLUTION (theGUI_INFO.GetOperatorImageResolutionHeight() / 600)

#define	CXSCREEN			theGUI_INFO.GetOperatorImageResolutionWidth()/*800*/
#define	CYSCREEN			theGUI_INFO.GetOperatorImageResolutionHeight()/*600*/

const int	CXCLIENTSCREEN =	1920;
const int	CYCLIENTSCREEN = 	1080;

const static double REC_RATIOWIDTH = theGUI_INFO.GetReceptionSolutionWidthRatio();
const static double REC_RATIOHEIGHT = theGUI_INFO.GetReceptionSolutionHeightRatio();



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////����ɫ����///////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////��С����////////////////////////////////////////////////////////////////
static const CSize DEFAULT_SIZE_BUTTON	= CSize(90,60);
static const CSize DEFAULT_SIZE_MENU_BUTTON	= CSize(165,72);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const COLORREF DEFAULT_COLOR_STATIC = GRAY;
static const MARGINS DEFAULT_MARGINS = {0,0,0,0};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////������������//////////////////////////////////////////////////
static CRect const LOGO_RECT                    = CRect(5,5,370,90);			// վԱ����LOGO����
static const CRect OPERATION_FRMAE_RECT			= CRect(262,150,762,730);		// �����Ի��򣨻�����Ϣ+Ǯ��Ϣ+��ť��
static const CRect INIT_FRAME_RECT		     	= CRect(0,100,1024,730);


//static const CRect KEYBOARD_FRAME_RECT			= CRect(762,467,1024,730);		// ��������
static const CRect GUIDE_FRAME_RECT				= CRect(262,100,762,150);		// �����Ի���
static const CRect BTN_OK_RECT = CRect(410,520,490,570);
static const CRect BTN_BACK_RECT = CRect(1120,10,1280,70);
static const CRect BTN_PRINT_RECT = CRect(410,520,490,570); // (210,520,290,570)
static const CRect BTN_RESET_RECT = CRect(10,520,90,570);
static const CPoint LEFTUP_AREA_POINT = CPoint(0,0);
static const CPoint LEFTDOWN_AREA_POINT = CPoint(0,350);

static const CRect OPERATION_BUTTON_RECT			= CRect(0,510,500,580);			// �ײ�������ť����
static const CRect MESSAGE_BOX_FRAME_RECT			= CRect(324,375,724,635);		// ��Ϣ��
static const CRect MESSAGE_PICTURE_BOX_FRAME_RECT	= CRect(240,212,1040,812);		// ��ϢͼƬ��
static const CRect  CASH_INFO_RECT					= CRect(163,86,240,107);		// �ֽ���Ϣ�����б༭�������

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// ����Ա����������
static const int status_h = 30;
static const int navi_h = 30;
static const CRect OPERATOR_STATUS_RECT = CRect(0,0,800, status_h);	// ״̬������
static const CRect OPERATOR_GUIDE_RECT = CRect(0,status_h, 800,status_h+30);	// ������������
static const CRect OPERATOR_BASE_RECT = CRect(0,status_h+30,800,600-navi_h);	// ������Ϣ����
static const CRect OPERATOR_NAVI_RECT = CRect(0,600-navi_h,800,600);	// ���ܵ���������
static const CRect OPERATOR_INIT_RECT = CRect(0,status_h,800,600-navi_h);	// ��ʼ�����򣨸�������������ͻ�����Ϣ����

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �˿ͽ���������
static const CRect REC_FRMAE_RECT		       = CRect(0,130,1920,1080-110);		    // �����Ի��򣨻�����Ϣ+�տ���Ϣ+ȷ�ϰ�ť��	
static const CRect REC_BUTTEN_GROUP_RECT	   = CRect(980,0,1920,844);		    // ��Ŧ����
static const CPoint BASEINFO_AREA_POINT        = CPoint(160,0);					// �˿ͻ�����Ϣ�������ʼ��
static const CPoint CASHINFO_AREA_POINT        = CPoint(160,175);				// �˿��ֽ���Ϣ�������ʼ��
static const CPoint NOTIFYINFO_AREA_POINT       = CPoint(160,740);				// �˿���ʾ��Ϣ�������ʼ��
static const CPoint GUIDEINFO_AREA_POINT       = CPoint(680,0);					// �˿���ʾ��Ϣ�������ʼ��
static const CPoint BTN_INFO_AREA_POINT        = CPoint(980,0);					// �˿Ͱ�Ŧ��Ϣ�������ʼ��

//static const CRect REC_HEAD_RECT			   = CRect(0,0,1920,80);
static const CRect REC_GUIDE_RECT              = CRect(CPoint(800,70),CSize(400,560));
static const CRect REC_BODY_RECT               = CRect(0,100,980,944);         
static const CRect REC_AREA_MAP_RECT           = CRect(0,100,980,944);   
static const CRect REC_BTN_OK_RECT             = /*CRect(720,760,940,820);*/CRect(CPoint(500,720),CSize(215,60));
static const CRect REC_BTN_OkAndCancel1_RECT   = CRect(535,820,755,880);
static const CRect REC_BTN_OkAndCancel2_RECT   = CRect(855,820,1075,880);
static const CRect REC_BTN_PRINT_RECT          = CRect(200,760,420,820);  
static const CRect REC_BTN_CANCELPRINT_RECT    = CRect(560,760,780,820);  
//// �ٰ�
static const int head_h = 130;
static const int memu_h = 110;
static const CRect REC_HEAD_RECT				= CRect(0, 0, CXCLIENTSCREEN, head_h);								// logo����վ����״̬��������
static const CRect RECT_OUTSVC					= CRect(0, head_h, CXCLIENTSCREEN, CYCLIENTSCREEN-memu_h);			// ҵ����Ϣ��ʾ��������	
static const CRect REC_MENU_MAIN_RECT			= CRect(0, CYCLIENTSCREEN-memu_h, CXCLIENTSCREEN, CYCLIENTSCREEN);	// ҵ��˵�

static const int border_h = 5;
static const int border_w = 5;
static const int kongbai_x = 7;

static const CSize left_number_size				= CSize(248,833);
static const CSize right_prices_size			= CSize(228,707);
static const CSize top_line_size				= CSize(1326,118);
static const CSize line_map_size				= CSize(1420,715);
static const CSize button_size					= CSize(165,72);
static const CSize cash_in_notic_size			= CSize(323,121);

static const CRect REC_LEFT_NUMBER_RECT			= CRect(CPoint(0, REC_HEAD_RECT.bottom+border_h), left_number_size);												// ��·ѡ��
static const CRect REC_AREA_TOP_RECT			= CRect(CPoint(left_number_size.cx + kongbai_x, REC_HEAD_RECT.bottom+kongbai_x), top_line_size);
static int line_map_posX = left_number_size.cx + kongbai_x;
static int line_map_posY = REC_AREA_TOP_RECT.bottom + border_h;
static const CRect REC_LINE_MAP_RECT			= CRect(CPoint(line_map_posX, line_map_posY+4), line_map_size);						// ��·վ����Ϣ
static const CRect REC_RIGHT_PRICES_RECT		= CRect(CPoint(left_number_size.cx+border_w+kongbai_x+line_map_size.cx,line_map_posY), right_prices_size);			// Ʊ��ѡ��
static const CRect REC_CASHIN_NOTICE_RECT		= CRect(CPoint(left_number_size.cx+top_line_size.cx+kongbai_x+border_h,border_h),cash_in_notic_size);
//static const CRect REC_RIGHT_NUMBER_RECT		= CRect(CPoint(RECT_OUTSVC.right-right_prices_size.cx-kongbai_x, REC_RIGHT_PRICES_RECT.bottom), right_number_size);	// ����ѡ��
//static const CRect REC_RIGHT_TOPSTATION_RECT	= CRect(CPoint(RECT_OUTSVC.right-right_prices_size.cx-kongbai_x, REC_RIGHT_NUMBER_RECT.bottom/*+border_h*/), right_topstation_size);// �ȵ㳵վ
//static const CRect REC_RIGHT_FUNC_RECT			= CRect(CPoint(RECT_OUTSVC.right-right_prices_size.cx-kongbai_x, REC_RIGHT_TOPSTATION_RECT.bottom/*+border_h*/), right_func_size);	// ����ѡ��
static const CRect WELCOME_FRAME_RECT		   = RECT_OUTSVC;//CRect(0,0,1920,1000);		    // ��ӭ��

static const CRect REC_ISSUE_FRAME_RECT		   = REC_LINE_MAP_RECT;		    // �����Ի��򣨻�����Ϣ+�տ���Ϣ+ȷ�ϰ�ť��	
static const CRect  RECT_BASE = CRect(CPoint(left_number_size.cx+kongbai_x, top_line_size.cy+border_h+2),CSize(REC_LINE_MAP_RECT.Width(),REC_LINE_MAP_RECT.Height()));

// h: 50 + 1180 + 50
// w: 140 + 1480 + 300

// logo����    
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
//	�˿ͽ���GUI����
//	������Ϣ����
#define GUEST_BASEINFO_FONT_SIZE			theGUI_INFO.GetRecBaseInfoFontSize()
#define GUEST_BASEINFO_FONT_NAME			theGUI_INFO.GetRecBaseInfoFontName()
#define GUEST_BASEINFO_FONT_COLOR			theGUI_INFO.GetRecBaseInfoFontColor()
//	״̬��
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
//	��ť
#define GUEST_FUNC_BTN_TITLE_FONT_SIZE		theGUI_INFO.GetFuncBtnGroupTitleFontSize()
#define GUEST_FUNC_BTN_TITLE_FONT_NAME		theGUI_INFO.GetFuncBtnGroupTitleFontName()
#define GUEST_FUNC_BTN_TITLE_FONT_COLOR		theGUI_INFO.GetFuncBtnGroupTitleFontColor()
#define GUEST_FUNC_BTN_FONT					theGUI_INFO.GetBtnFont()
#define GUEST_FUNC_BTN_FONT_COLOR			theGUI_INFO.GetFuncBtnFontColor()
#define GUEST_MENUE_BTN_FONT				theGUI_INFO.GetRecMenuBtnFont()
#define GUEST_MENUE_BTN_FONT_COLOR			theGUI_INFO.GetMenuBtnFontColor()
//	��Ϣ��
#define GUEST_MSGBOX_TITLE_FONT_SIZE		theGUI_INFO.GetRecMessageBoxTitleFontSize()
#define GUEST_MSGBOX_TXT_FONT_SIZE			theGUI_INFO.GetRecMessageBoxFontSize()
#define GUEST_MSGBOX_TITLE_FONT_NAME		theGUI_INFO.GetRecMessageBoxTitleFontName()
#define GUEST_MSGBOX_TXT_FONT_NAME			theGUI_INFO.GetRecMessageBoxFontName()
#define GUEST_MSGBOX_TITLE_FONT_COLOR		theGUI_INFO.GetRecMsgBoxTitleFontColor()
#define GUEST_MSGBOX_TXT_FONT_COLOR			theGUI_INFO.GetRecMsgBoxFontColor()

// �˵����
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

// �к�
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

// �к�
static const int LABEL_COL_1 = 0;
static const int LABEL_COL_2 = 1;
static const int LABEL_COL_3 = 2;
static const int LABEL_COL_4 = 3;
static const int LABEL_COL_5 = 4;
static const int LABEL_COL_6 = 5;
static const int LABEL_COL_7 = 6;

// ����������˵������������Ǳ༭����ʼ���ͨ��ƫ��İ���ֽ���
#define BASE_INFO_RECT_SP(row,len,leftStart)   CRect(160+9*leftStart,BASE_INFO_RECT_TOP(row),160+9*leftStart+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+25)

// 
#define BASE_INFO_RECT2_TOP(row) (4+(row-1)*34+row)
//#define BASE_INFO_RECT2_TOP(row) (6+(row-1)*(HRESOLUTION * 25)+row)

#define BASE_INFO_RECT2_WIDTH(len) ((len+1)*9)
#define BASE_INFO_RECT2(row,width,len) CRect(width+10,BASE_INFO_RECT2_TOP(row),width+BASE_INFO_RECT2_WIDTH(len),BASE_INFO_RECT2_TOP(row)+24)

// ��������󳤶�
static const int MONEY_MAX_LEN  = 4;

typedef enum _Menu_Item_ID{
	
	ISSUE_ID = 100,
	RECHARGE_ID = 200,
	ADDPRODUCT_ID = 300,
	ANALYZE_SVC_ID = 400,
	ISSUE_USE_SVT_ID = 500,
	LANGUAGE_SWITH_ID = 600,
	//CALL_HELP_SVC_ID = 500,

	MAINTENANCE_SVC_ID							= 2000, // ά��ҵ��
	TRANSACTION_HISTORY_ID						= 2100, // ����һ��
	DEVICE_STATUS_ID							= 2200, // �豸״̬ 
	SALE_STATISTIC_SVC_ID						= 2300, // ����ͳ��
	CARD_BOX_MANAGE_SVC_ID						= 2400, // Ʊ�����
	TOKEN_BOX_MANAGE_SVC_ID						= 2500, // Token�����
	BANKNOTE_BOX_MANAGE_SVC_ID					= 2600, // ֽ�������
		INSTALL_BANKNOTE_COLLECTION_BOX_SVC_ID					= 2601, // ��װֽ�һ�����
		REMOVE_BANKNOTE_COLLECTION_BOX_SVC_ID					= 2602,	// ж��ֽ�һ�����
		INSTALL_BANKNOTE_CHANGE_INVALID_BOX_SVC_ID				= 2603, // ��װֽ�����㣬�ϱ���
		REMOVE_BANKNOTE_CHANGE_INVALID_BOX_SVC_ID				= 2604,	// ж��ֽ�����㣬�ϱ���
		BANKNOTE_BOX_RECYCLE_CHANGE_MONEY_SVC_ID				= 2605,	// ֽ�һ��չ���
	COIN_BOX_MANAGE_SVC_ID						= 2700, // Ӳ�������
		ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC_ID			= 2701, // Ӳ��ר��������
		ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC_ID				= 2702, // Ӳ��ѭ��������
		ATTACH_COLLECTION_COIN_BOX_SVC_ID						= 2703, // ��װӲ�һ�����
		DETACH_COLLECTION_COIN_BOX_SVC_ID						= 2704,	// ж��Ӳ�һ�����
		//DETACH_ADDING_COIN_BOX_SVC_ID							= 2705, // ж��Ӳ�Ҳ�����
		COLLECTION_COIN_SVC_ID									= 2706,	// ����Ӳ��
		INSTALL_COIN_CHANGE_BOX_SVC_ID							= 2707,	// ��װӲ��������
		REMOVE_COIN_CHANGE_BOX_SVC_ID							= 2708,	// ж��Ӳ��������

	COLLECTION_MANAGE_SVC_ID					= 2800, // ���մ���
	OPERATION_CLOSE_SVC_ID						= 2900, // ҵ�����
	RUN_MODE_SETTING_SVC_ID						= 3000, // ����ģʽ�趨
		SERVICE_MODE_SETTING_SVC_ID								= 3001, // ����ģʽ�趨
		PAYMENT_MODE_SETTING_SVC_ID								= 3002, // ֧��ģʽ�趨
		CHANGE_MODE_SETTING_SVC_ID								= 3003, // ����ģʽ�趨
		PRINT_MODE_SETTING_SVC_ID								= 3004, // ��ӡģʽ�趨
	RECEIPT_REPRINT_SVC_ID						= 3100, // ƾ֤����
	SYSTEM_SET_SVC_ID							= 3200, // ϵͳ�趨
		STATION_SET_SVC_ID										= 3201, // ��վ����
		COMMUNICATION_SET_SVC_ID								= 3202, // ͨ�Ų�������
		MODE_SWITCH_SVC_ID										= 3203, // ģʽ�л�����
		SYSTEM_TIME_SET_SVC_ID									= 3204, // ϵͳʱ���趨
		UDSN_SET_SVC_ID											= 3205, // UDSN�趨
		SWITCH_OFF_SVC_ID										= 3206, // �����趨
		RFID_MODE_SET_SVC_ID									= 3207, // RFIDģʽ�趨
		SCREEN_BRIGHTNESS_SVC_ID								= 3208,	// ��Ļ�����趨
		DEVICE_COMPORT_SET_ID									= 3209,	// ���ں�����
	DATA_MANAGE_SVC_ID							= 3300, // ���ݹ���
		DATA_INPUT_SVC_ID										= 3301, // ���ݵ���
		DATA_OUTPUT_SVC_ID										= 3302, // ���ݵ���
		VERSION_INFO_SVC_ID										= 3303, // �汾��Ϣ
	DEVICE_TEST_SVC_ID							= 3400, // Ӳ���Լ�
		RW_TEST_SVC_ID											= 3401, // ��д������
		MONEYBOX_TEST_SVC_ID									= 3402, // Ӳ��ģ�����
		BANKNOTE_ACCEPT_MODULE_TEST_SVC_ID						= 3403, // ֽ�ҽ���ģ�����
		BANKNOTE_CHANGE_MODULE_TEST_SVC_ID						= 3404, // ֽ������ģ�����
		CARD_ISSUE_MODULE_TEST_SVC_ID							= 3405, // ����ģ�����
		TOKEN_MODULE_TEST_SVC_ID								= 3406, // Tokenģ�����
		RECEIPT_RINTER_TEST_SVC_ID								= 3407, // ƾ֤��ӡ������
		PIN_PAD_TEST_SVC_ID										= 3408, // �������ģ�����
		BANKCARD_MODULE_TEST_SVC_ID								= 3409, // ���п�����ģ�����
		IO_MODULE_TEST_SVC_ID									= 3410, // IOģ�����
		STATUS_DISPLAY_TEST_SVC_ID								= 3411, // ״̬��ʾ������
		NETWORK_TEST_SVC_ID										= 3412, // �������
		UPS_TEST_SVC_ID											= 3413, // UPS����
		ALL_DEVICE_STATUS_ID									= 3414,	// ����Ӳ���豸״̬
	REMOVE_EXCEPTION_SVC_ID						= 3500, // �쳣���
	SHUTDOWN_ID									= 3600, // �ػ�����
	    SHUTDOWN_COMPUTER_ID									= 3601, //�ػ�
		RESTART_COMPUTTER_ID									= 3602, //����
		EXIT_PROGRAM_ID											= 3603,	//�˳�
	ONE_KEY_SETTLEMENT_SVC_ID					= 3700,// һ���ջ�

} MENU_ITEM_ID;

/////////////////////////////////////////////////����Group��ID����///////////////////////////////////////////////////////////////
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

static const int MAX_SBC_CASE_LEN		    = 8;		    // ����ȫ���ַ���󳤶�
static const int MAX_SBC_CASE_LEN_RIGHT     = 8;            // ����ȫ���ַ���󳤶�
static const int MAX_HALF_CASE_LEFT_LEN	    = 16;		    // ���а���ַ���󳤶�
                                   
// ��λ���ƶ���
static const TCHAR* const TXT_YUAN_MARK					= _T("��");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////BUTTON��Ϣ�ı�����///////////////////////////////////////

static const TCHAR* const WM_BTN_PRINT			= _T("WM_BTN_PRINT");							///< ��ӡ��ť��Ϣ
static const TCHAR* const WM_BTN_NOPRINT	    = _T("WM_BTN_NOPRINT");							///< ����ӡ��ť��Ϣ
static const TCHAR* const WM_BTN_BACK			= _T("WM_BTN_BACK");							///< ���ذ�ť��Ϣ
static const TCHAR* const WM_BTN_RESET			= _T("WM_BTN_RESET");						    ///< ���ð�ť��Ϣ
static const TCHAR* const WM_BTN_ENGLISH	    = _T("WM_BTN_ENGLISH");							///< ��ӡ��ť��Ϣ
static const TCHAR* const WM_BTN_CHINESE	    = _T("WM_BTN_CHINESE");							///< ȷ�ϰ�ť��Ϣ
static const TCHAR* const WM_BTN_PORTUGUESE		= _T("WM_BTN_PORTUGUESE");						///< ���ذ�ť��Ϣ
static const TCHAR* const WM_BTN_SCAN		    = _T("WM_BTN_SCAN");							///< ɨ�谴ť��Ϣ
static const TCHAR* const WM_BTN_OkAndCancel1	= _T("WM_BTN_OkAndCancel1");					///< ȷ�ϰ�ť��Ϣ
static const TCHAR* const WM_BTN_OkAndCancel2   = _T("WM_BTN_OkAndCancel2");					///< ȡ����ť��Ϣ
static const TCHAR* const WM_BTN_SELECT_LANGUAGE= _T("WM_BTN_SELECT_LANGUZGE");					///< ѡ�����԰�ť
static const TCHAR* const WM_BTN_CHARGE			= _T("WM_BTN_CHARGE");							///< ��ֵ��ť
static const TCHAR* const WM_BTN_CASH_PAY		= _T("WM_BTN_CASH_PAY");							///< �ֽ�֧��
static const TCHAR* const WM_BTN_NET_PAY		= _T("WM_BTN_NET_PAY");							///< ɨ��֧��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


// ���·���Գ�������λ��Ϊ��׼
// GUI������
#define TVM_GUI_PATH					_T(".\\res\\gui\\")
#define TVM_GUI_IMAGE_CSV				_T(".\\res\\gui\\image.csv")
#define TVM_GUI_LAYOUT_CSV				_T(".\\res\\gui\\layout.csv")
#define TVM_GUI_BUTTON_CSV				_T(".\\res\\gui\\button.csv")
#define TVM_GUI_LAYOUT					_T(".\\res\\gui\\layout\\")

static const TCHAR* const AFC_MAP_ID_3085	= _T("TVM99.3085.*");					// ��ͼ��������ģ��

#define TVM_GUI_BTN_NORMAL				_T(".\\res\\gui\\btn\\station_btn_normal.png")
#define TVM_GUI_BTN_LEFT				_T(".\\res\\gui\\btn\\left.png")
#define TVM_GUI_BTN_TOP					_T(".\\res\\gui\\btn\\top.png")
#define TVM_GUI_BTN_RIGHT				_T(".\\res\\gui\\btn\\right.png")
#define TVM_GUI_BTN_BOTTOM				_T(".\\res\\gui\\btn\\bottom.png")

// ͼƬ����
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
// ��ʾ����ͼƬ
#define TVM_IMAGE_TIP_INSERT_COIN			_T(".\\res\\TVM_TIP_InsertCoins_Guide.gif")			// Ͷ��Ӳ��
#define TVM_IMAGE_TIP_INSERT_NOTE			_T(".\\res\\TVM_TIP_InsertBanknotes_Guide.gif")		// Ͷ��ֽ��
#define TVM_TIP_INSERT_CARD					_T(".\\res\\TVM_TIP_ReInsert_card.gif")
#define	TVM_TIP_RETURN_COIN					_T(".\\res\\TVM_TIP_Return.gif")					// �˻�Ӳ��
#define TVM_TIP_ISSUING						_T(".\\res\\TVM_TIP_ISSUING.gif")					// 
#define TVM_TIP_GET_RETURN_MONEY			_T(".\\res\\TVM_TIP_GET_RETURN_MONEY.gif")			// ��ȡ���ֽ�
#define TVM_TIP_GET_RETURN_CARD				_T(".\\res\\TVM_TIP_GET_RETURN_CARD.gif")			// ��ȡ���˿�
#define TVM_TIP_GET_RECEIPT					_T(".\\res\\TVM_TIP_GetReceipt.gif")				// ȡƾ��
#define TVM_IMAGE_TIP_RETURN_NOTE			_T(".\\res\\gui\\guide\\TVM_TIP_ReturnBanknotes_Guide.gif")		// �˻�ֽ��
#define TVM_IMAGE_TIP_INSERT_BANKCARD		_T(".\\res\\gui\\guide\\TVM_TIP_ReturnBanknotes_Guide.gif")		// �������п�
#define TVM_IMAGE_TIP_INSERT_EP				_T(".\\res\\gui\\guide\\TVM_TIP_ReturnBanknotes_Guide.gif")		// ʹ�õ���Ǯ��
#define TVM_IMAGE_TIP_ISSUE_UL				_T(".\\res\\gui\\guide\\TVM_TIP_ISSUE_UL.gif")					// ���ڷ���UL��
#define TVM_IMAGE_TIP_ISSUE_M1				_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// ���ڷ���M1��
#define TVM_TIP_ISSUE_TICKETS				_T(".\\res\\gui\\guide\\TVM_TIP_ISSUE_TICKETS.png")				// ���۵���Ʊ
#define TVM_IMAGE_CHARGING					_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// ���ڳ�ֵ��...
#define TVM_GIF_OPERATION_GUIDE				_T(".\\res\\gui\\guide\\TVM_TIP_Operation_Guide.gif")			// ��ʾѡ����
#define TVM_TIP_CONFIRM_NORMAL				_T(".\\res\\gui\\guide\\TVM_TIP_CONFIRM_NORMAL.gif")			// ����[Confirm]��
#define TVM_TIP_CONFIRM_BIG					_T(".\\res\\gui\\guide\\TVM_TIP_CONFIRM_BIG.gif")				// ����[Confirm]�� 270*600
#define TVM_TIP_WARINGS						_T(".\\res\\gui\\guide\\TVM_TIP_WARINGS.png")					// ����ͼ��
#define TVM_TIP_SWIPE_CARD					_T(".\\res\\gui\\guide\\TVM_TIP_SWIPE_CARD.png")				// ��ˢ��
#define TVM_TIP_GET_TOKEN_CHANGE			_T(".\\res\\gui\\guide\\TVM_TIP_GET_TOKEN_CHANGE.gif")			// ��ȡ��Token������
#define TVM_TIP_PRINTING					_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// ���ڴ�ӡ��...
#define TVM_TIP_PRINTED						_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// ��ӡ���
#define TVM_TIP_RECHARGE_FINISHED			_T(".\\res\\gui\\guide\\TVM_TIP_RECHARGING.gif")				// ��ֵ���
#define TVM_TIP_FORBID_INSERT_COINS			_T(".\\res\\gui\\guide\\TVM_TIP_FORBID_INSERT_COINS.png")		// ��ֹͶ��Ӳ��
#define TVM_TIP_FORBID_INSERT_BANKNOTES     _T(".\\res\\gui\\guide\\TVM_TIP_FORBID_INSERT_BANKNOTES.png")	// ��ֹͶ��ֽ��
#define TVM_TIP_SHOW_WARINGS				_T(".\\res\\gui\\guide\\TVM_TIP_WARINGS.png")					// ����ͼ

#define TVM_SYSTEM_IN_PROCESSING			_T(".\\res\\gui\\guide\\SYSTEM_IN_PROCESSING.gif")				// ��ʾϵͳ���ڴ�����