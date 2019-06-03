
#pragma once
#include "servicedefinition.h"
#include <vector>
#include "Bag.h"

using namespace std;

enum DOCKSTYLE
{
	NONE = 0x00,
	TOP  = 0x01,
	RIGHT = 0x02,
	BOTTOM = 0x03,
	LEFT = 0x04,
	FILL = 0x05
};

typedef struct _PADDINGS
{
	int cyTopHeight;
	int cxRightWidth;
	int cyBottomHeight;
	int cxLeftWidth;
	_PADDINGS()
	{
		this->cxLeftWidth = 0;
		this->cyTopHeight = 0;
		this->cxRightWidth = 0;
		this->cyBottomHeight = 0;
	}
	_PADDINGS(int cxLeftWidth,int cyTopHeight, int cxRightWidth,int cyBottomHeight)
	{
		this->cxLeftWidth = cxLeftWidth;
		this->cyTopHeight = cyTopHeight;
		this->cxRightWidth = cxRightWidth;
		this->cyBottomHeight = cyBottomHeight;
	}
	bool operator == (const _PADDINGS& another) const
	{
		return cyTopHeight == another.cyTopHeight && cxRightWidth == another.cxRightWidth && cyBottomHeight == another.cyBottomHeight && cxLeftWidth == another.cxLeftWidth;
	}

} PADDINGS,*PPADDINGS;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////按钮显示相关定义//////////////////////////////////////////////
enum BUTTON_TYPE {
	BUTTON_TYPE_NORMAL,             // 正常按钮
	BUTTON_TYPE_NOSELECT,           // 无选中状态的按钮
};


typedef struct _btn_info {
	int btnID;												// 按钮ID(0开始)
	CString text;											// 显示文言1
	CString btnMsg;											// 按钮消息
	bool isVisible;											// 是否显示
	bool isEnable;											// 是否可用
	bool isSelected;										// 是否选中
	UINT shortCutKey;										// 快捷键
	BUTTON_TYPE type;										// 按钮类型

	_btn_info(){
		btnID = 0;											// 按钮ID
		text = _T("");											// 显示文言1
		btnMsg = _T("");										// 按钮消息
		isVisible = true;									// 是否显示
		isEnable = true;									// 是否可用
		isSelected = false;									// 是否选中
		type = BUTTON_TYPE::BUTTON_TYPE_NORMAL;
		shortCutKey = 0;
	}
	_btn_info(const _btn_info& another)
	{
		this->btnID = another.btnID;
		this->text = another.text;
		this->btnMsg = another.btnMsg;
		this->isVisible = another.isVisible;
		this->isEnable = another.isEnable;
		this->isSelected = another.isSelected;
		this->type = another.type;
		this->shortCutKey = another.shortCutKey;
	}
	void _btn_info::initialize(){
		btnID = 0;											// 按钮ID
		text = _T("");											// 显示文言1
		btnMsg = _T("");										// 按钮消息
		isVisible = true;									// 是否显示
		isEnable = true;									// 是否可用
		isSelected = false;									// 是否选中
		type = BUTTON_TYPE::BUTTON_TYPE_NORMAL;	
		shortCutKey = 0;
	}
}BTN_INFO;


typedef struct _Menu_Item:public BTN_INFO
{
	SERVICE_ID serviceID;
	_Menu_Item* pParent;
	vector<_Menu_Item*>  children;
	CBag*	bag;
	_Menu_Item():BTN_INFO()
	{
		serviceID = UNKNOWN_SVC;
		pParent = NULL;
		bag = new CBag();
	};
	~_Menu_Item(){
		children.clear();
		if(bag!=NULL)
		{
			delete bag;
			bag = NULL;
		}
	}
} Menu_Item,*PMenu_Item;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	CXSCREENREC			1024
#define	CYSCREENREC			768

#define	CXSCREEN			1920
#define	CYSCREEN			1080

#define	CXSCREENRATIO			1920/1024
#define	CYSCREENRATIO			1080/768

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////颜色定义///////////////////////////////////////////////////////////////

#define BLUE						RGB(40, 94, 166)
#define LIGHTBLUE					RGB(81,157,219)
#define SOFTBLUE                    RGB(128,174,203)
#define WATERBLUE                   RGB(122,158,187)
#define YELLOW                      RGB(255,255,0)
#define SKYBLUE						RGB(235,241,222)
#define WHITE                       RGB(255,255,255)
#define BLACK                       RGB(0,0,0)
#define DARKGRAY                    RGB(187,187,187)
#define GRAY                        RGB(236, 240, 241)
#define BLACKGRAY                   RGB(179,179,179)
#define RED                         RGB(255,0,0)
#define PINK                        RGB(167,168,207)
#define SOFTGREEN                   RGB(0,200,0)
#define RECEPTION_BK_COLOR          RGB(236,240,241)
#define RECEPTION_LINE_COLOR        RGB(192,192,192)
#define RECEPTION_ROUND_COLOR       RGB(215,211,204)
#define RECEPTION_HEAD_BK_COLOR     RGB(0,53,103)
#define RECEPTION_HEAD_ROUND_COLOR  RGB(128,174,203)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////大小定义////////////////////////////////////////////////////////////////
static const CSize DEFAULT_SIZE_EDIT	= CSize(100*CXSCREENRATIO,23*CYSCREENRATIO);
static const CSize DEFAULT_SIZE_BUTTON	= CSize(80*CXSCREENRATIO,40*CYSCREENRATIO);
static const CSize DEFAULT_SIZE_STATIC = CSize(200*CXSCREENRATIO,80*CYSCREENRATIO);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  DEFAULT_PADDINGS  PADDINGS(0,0,0,0)
static const COLORREF DEFAULT_COLOR_LABEL  = BLACK;
static const COLORREF DEFAULT_COLOR_STATIC = GRAY;
static const MARGINS DEFAULT_MARGINS = {0,0,0,0};
static const MARGINS DEFAULT_GROUPBOX_MARGINS = {5,2,5,2};
static const CPoint ROUND_POINT        =  CPoint(20,20);    // 圆角矩形的圆角弧度
static const int ROUNDRECT_BORDER      =      2;            // 区域边框线的粗细
static const int LINE_BORDER           =      3;            // 画面中蓝线的粗细
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////界面主区域定义//////////////////////////////////////////////////
//站员屏幕分辨率由1024*768变为1920*1080，乘客屏幕分辨率为1024*768
static const CRect LOGO_FRAME_RECT				= CRect(0,0,262*CXSCREENRATIO,100*CYSCREENRATIO);//CRect(0,0,262,100)			// 标志窗体对话框
static CRect const LOGO_RECT					= CRect(20*CXSCREENRATIO,34*CYSCREENRATIO,110*CXSCREENRATIO,105*CYSCREENRATIO);			// 站员界面LOGO区域
static CRect const RECEPTION_HEAD_LOGO_RECT		= CRect(10*CXSCREENRATIO,380*CYSCREENRATIO,100*CXSCREENRATIO,465*CYSCREENRATIO);			// 乘客业务界面LOGO区域
//static CRect const OUT_OF_RECEPTION_LOGO_RECT	= CRect(10,176,100,261);
//static CRect const WELCOME_LOGO_RECT			= CRect(10,241,110,346);
static CRect const WELCOME_LOGO_RECT			= CRect(40,210,180,360);		// 乘客欢迎光临界面LOGO区域
static CRect const WELCOME_LOGO_RECT2         = CRect(40+CXSCREENREC,210,180+CXSCREENREC,360);

static CRect const OUT_OF_RECEPTION_LOGO_RECT	= CRect(180*CXSCREENRATIO,241*CYSCREENRATIO,280*CXSCREENRATIO,346*CYSCREENRATIO);
static const CRect MENU_MAIN_RECT				= CRect(262*CXSCREENRATIO,0,1024*CXSCREENRATIO,100*CYSCREENRATIO);		// 顶层菜单
static const CRect MENU_SECOND_RECT				= CRect(262*CXSCREENRATIO,50*CYSCREENRATIO,1024*CXSCREENRATIO,100*CYSCREENRATIO);		// 二级菜单对话框
static const CRect OPERATION_FRMAE_RECT			= CRect(262*CXSCREENRATIO,150*CYSCREENRATIO,762*CXSCREENRATIO,730*CYSCREENRATIO);		// 操作对话框（基本信息+钱信息+按钮）
static const CRect INIT_FRAME_RECT				= CRect(0,100*CYSCREENRATIO,1024*CXSCREENRATIO,730*CYSCREENRATIO);
static const CRect STS_FRAME_RECT				= CRect(0,730*CYSCREENRATIO,1024*CXSCREENRATIO,768*CYSCREENRATIO);		// 状态栏对话框

static const CRect LEFT_1_FRAME_RECT			= CRect(0,100*CYSCREENRATIO,262*CXSCREENRATIO,520*CYSCREENRATIO);			// 左一区域
static const CRect LEFT_2_FRAME_RECT			= CRect(0,520*CYSCREENRATIO,262*CXSCREENRATIO,730*CYSCREENRATIO);			// 左二区域
static const CRect RIGHT_1_FRAME_RECT			= CRect(762*CXSCREENRATIO,100*CYSCREENRATIO,1024*CXSCREENRATIO,310*CYSCREENRATIO);		// 右一区域
static const CRect RIGHT_2_FRAME_RECT			= CRect(762*CXSCREENRATIO,310*CYSCREENRATIO,1024*CXSCREENRATIO,467*CYSCREENRATIO);		// 右二区域
static const CRect KEYBOARD_FRAME_RECT			= CRect(762*CXSCREENRATIO,467*CYSCREENRATIO,1024*CXSCREENRATIO,730*CYSCREENRATIO);		// 键盘区域
static const CRect GUIDE_FRAME_RECT				= CRect(262*CXSCREENRATIO,100*CYSCREENRATIO,762*CXSCREENRATIO,150*CYSCREENRATIO);		// 导航对话框
static const CRect BTN_OK_RECT = CRect(410*CXSCREENRATIO,520*CYSCREENRATIO,490*CXSCREENRATIO,570*CYSCREENRATIO);
static const CRect BTN_BACK_RECT = CRect(310*CXSCREENRATIO,520*CYSCREENRATIO,390*CXSCREENRATIO,570*CYSCREENRATIO);
static const CRect BTN_PRINT_RECT = CRect(110*CXSCREENRATIO,520*CYSCREENRATIO,290*CXSCREENRATIO,570*CYSCREENRATIO);
static const CRect BTN_RESET_RECT = CRect(10*CXSCREENRATIO,520*CYSCREENRATIO,90*CXSCREENRATIO,570*CYSCREENRATIO);


static const CRect OPERATION_BUTTON_RECT		= CRect(0,510*CYSCREENRATIO,500*CXSCREENRATIO,580*CYSCREENRATIO);			// 底部操作按钮区域
static const CRect MESSAGE_BOX_FRMAE_RECT		= CRect(315,365,733,645);//CRect(324,375,724,635)		// 消息框
static const CRect  CASH_INFO_RECT				= CRect(163*CXSCREENRATIO,86,400,111);		// 现金信息区域中编辑框的坐标
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const CPoint LEFTUP_AREA_POINT          = CPoint(0,0);					// 左上角区域的起始点
static const CPoint LEFTDOWN_AREA_POINT        = CPoint(0,350*CYSCREENRATIO);					// 左下角区域的起始点
static const CRect REC_HEAD_RECT			   = CRect(0,0,2048,100);
static const CRect REC_GUIDE_RECT              = CRect(0,680,2048,768);
static const CRect REC_BODY_RECT               = CRect(0,100,2048,680);
static const CRect BUSINESS_AREA_RECT          = CRect(5,5,645,575);


//static const int REC_BODY_L = 0;												// 乘客对话框左上横坐标
//static const int REC_BODY_U = 136;											// 乘客对话框左上纵坐标
//static const int REC_BODY_R = 1024;											// 乘客对话框右下横坐标
//static const int REC_BODY_D = 768;											// 乘客对话框右下纵坐标
static const CRect  REC_GUIDE_MSG_RECT         = CRect(15,6,1009,66);			// 乘客导航框区域
static const CRect WELCOME_FRAME_RECT		   = CRect(0,0,2048,768);		    // 欢迎框

static const TCHAR* const REC_CN_FONT_NAME= _T("黑体");
static const UINT const REC_CN_FONT_SIZE = 24;
static const UINT const REC_CN_FONT_SIZE2 = 18;
static const UINT const REC_CN_FONT_SIZE3 = 15;
static const TCHAR* const REC_EN_FONT_NAME = _T("黑体");
static const UINT const REC_EN_FONT_SIZE = 12;
static const UINT const REC_EN_FONT_SIZE2 = 10;

static const LOGFONT DEFAULT_FONT =
{
	18,
	0,
	0,
	0,
	FW_NORMAL,
	0,
	0,
	0,
	GB2312_CHARSET,
	OUT_TT_PRECIS,
	CLIP_DEFAULT_PRECIS|CLIP_LH_ANGLES,
	PROOF_QUALITY,
	DEFAULT_PITCH|FF_DONTCARE,
	_T("黑体")
};

static const TCHAR* const TITLE_FONT         = _T( "黑体");
static const int TITLE_FONT_SIZE			=  16;           // 按钮组的标题字体大小
static const int BUTTON_FONT_SIZE           =  14;    //编辑框中文字、行列内容字体大小
static const int NORMAL_FONT_SIZE	        =  14;
static const TCHAR* const NORMAL_FONT	    = _T("黑体");

static const int NORMAL_BUTTON_WIDTH        = 88*CXSCREENRATIO;			 // 按钮的宽度
static const int NORMAL_BUTTON_HEIGHT       = 61*CYSCREENRATIO;            // 按钮的高度

static const int GUIDE_FONT_SIZE_CN    =      18;            // 导航框中文字体大小
static const int GUIDE_FONT_SIZE_EN    =      10;            // 导航框英文字体大小

static const int LABEL_GROUP_ROWS_8      =   8;
static const int LABEL_GROUP_ROWS_11     =   11;
static const int LABEL_GROUP_EDIT_ROWS_0 =   0;
static const int LABEL_GROUP_EDIT_ROWS_1 =   1;
static const int LABEL_GROUP_EDIT_ROWS_2 =   2;
static const int LABEL_GROUP_EDIT_ROWS_3 =   3;
static const int LABEL_GROUP_EDIT_ROWS_4 =   4;
static const int LABEL_GROUP_EDIT_ROWS_5 =   5;
static const int LABEL_GROUP_EDIT_ROWS_6 =   6;
static const int LABEL_GROUP_EDIT_ROWS_7 =   7;

static const int LABEL_GROUP_EDIT_ROWS_8 =   8;
static const int LABEL_GROUP_EDIT_ROWS_9 =   9;
static const int LABEL_GROUP_EDIT_ROWS_10 = 10;
// 基本区域的编辑框坐标，第一个参数是行数，从1开始，第二个参数是编辑框内可输数字的长度，以半角字符计数
#define BASE_INFO_RECT_TOP(row)   49+(row-1)*37
#define BASE_INFO_RECT_RIGHT(len) (len+1)*12
#define BASE_INFO_RECT(row,len)   CRect(160*CXSCREENRATIO,BASE_INFO_RECT_TOP(row),160*CXSCREENRATIO+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+26)
// 下面这个多了第三个参数，是编辑框起始点比通常偏后的半角字节数
#define BASE_INFO_RECT_SP(row,len,leftStart)   CRect(160*CXSCREENRATIO+11.5*leftStart,BASE_INFO_RECT_TOP(row),160*CXSCREENRATIO+11.5*leftStart+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+26)

static const int DEFAULT_TXT_DISPLAY_LEN = 10;
static const int DEFAULT_PASSWORD_LEN = 6;

// 定义金额最大长度
static const int MONEY_MAX_LEN  = 4;

typedef enum _Menu_Item_ID{
		END_SVC_ID = 100,
		SHUTDOWN_SVC_ID = 200,
		LOGIN_SVC_ID = 600,
		REMOVE_INIT_EXCEPTION_SVC_ID = 700,
		ISSUE_MAIN_ID = 800,
			ISSUE_SINGLE_RIDE_ID = 801,
			ISSUE_EXIT_ONLY_ID = 802,
			ISSUE_COMMEMORATICE_ID = 803,
			ISSUE_WELFARE_ID = 804,
			ISSUE_ECT_ID				= 805,
		RECHARGE_MAIN_ID = 900,
			RECHARGE_SVC_ID = 901,
			ADDTIMES_SVC_ID = 902,
		ANALYZE_SVC_ID = 1000,
		ADJUST_MAIN_ID = 1100,
			ADJUST_PAIDAREA_ID = 1101,
			ADJUST_NOPAIDAREA_ID = 1102,
		REFUND_MAIN_ID = 1200,
			SURRENDER_SVC_ID = 1201,
			REFUND_SVC_ID = 1202,
			REFUND_BASE_SVC_ID = 1203,
			REFUND_APPLY_SVC_ID = 1204,
			REFUND_QUERY_SVC_ID = 1205,
		
		PRESELL_CANCEL_TICKET_MAIN_ID		= 1300,
			PRESELL_TICKET_ID	= 1301,
			COUNTERACT_SVC_ID = 1302,
		OTHER_MAIN_ID = 1400,
			TVMFAILURE_REFUND_SVC_ID = 1401,
			NOBUSINESS_INCOME_SVC_ID = 1402,
			SIGNCARD_APPLY_SVC_ID = 1404,
			SIGNCARD_REPORT_LOSS_SVC_ID = 1405,
			SIGNCARD_CANCEL_REPORT_LOSS_SVC_ID = 1406,
			CARD_SUSPEND_AND_RESUME_SVC_ID = 1407,
			TICKETBOX_EXCHANGE_SVC_ID = 1408,
			ADMINISTRATION_PROCESS_SVC_ID = 1409,
			LOCK_SVC_ID = 1410,
			RECEIPT_PRINT_SVC_ID = 1411,
		ASSIST_MAIN_ID = 1500,
			CHANGE_PASSWORD_SVC_ID = 1501,
			SALE_STATIC_SVC_ID = 1502,
			LOGOFF_SVC_ID = 1503,
			EXIT_BOM_SVC_ID = 1504,
		SYSTEM_SET_MAIN_ID = 1600,
			STATION_SET_SVC_ID = 1601,
			COMMUNICATION_SET_SVC_ID = 1602,
			MODE_SWITCH_SVC_ID = 1603,
			SYSTEM_TIME_SET_SVC_ID = 1604,
			UDSN_SET_SVC_ID = 1605,
			TH_MODE_SVC_ID = 1606,
			RFID_MODE_SVC_ID = 1607,
		DATA_MANAGE_MAIN_ID = 1700,
			DATA_INPUT_SVC_ID	= 1701,
			DATA_OUTPUT_SVC_ID = 1702,
		DEVICE_TEST_SVC_ID = 1900,
			OUTRW_TEST_SVC_ID = 1901,
			PRINTER_TEST_SVC_ID = 1902,
			MONEYBOX_TEST_SVC_ID = 1903,
			NETWORK_TEST_SVC_ID = 1904,
			UPS_INFO_SVC_ID = 1905,
			TH_TEST_SVC_ID = 1906,
		REMOVE_EXCEPTION_SVC_ID = 2000,
		EXCHANGE_SVC_ID = 2100,
		REFRESH_MAIN_ID = 2200,
			REFRESH_SVC_ID = 2210,
			DEFER_SVC_ID = 2220,
} MENU_ITEM_ID;

/////////////////////////////////////////////////界面Group的ID定义///////////////////////////////////////////////////////////////
#define ID_GRP_NULL 0
#define MENU_GROUP_ID 1
#define	SECOND_MENU_GROUP_ID 2
#define LEFT_1_GROUP_ID 3
#define LEFT_2_GROUP_ID 4
#define RIGHT_1_GROUP_ID 5
#define RIGHT_2_GROUP_ID 6
//#define RIGHT_3_GROUP_ID 7
#define OPERATION_BUTTON_GROUP_ID 8
#define KEYBOARD_GROUP_ID 9
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int MAX_ISSUE_TICKET_COUNT     = 100;          // 最大发票张数
static const int MAX_SBC_CASE_LEN		    = 8;		    // 左列全角字符最大长度
static const int MAX_SBC_CASE_LEN_YEAR      = 7;            // 左列全角字符最大长度(用于日期的年)
static const int MAX_SBC_CASE_LEN_DATE		= 5;		    // 左列全角字符最大长度(用于日期)
static const int MAX_SBC_CASE_LEN_RIGHT     = 4;            // 右列全角字符最大长度
static const int MAX_CASH_RIGHT_LEN         = 16;           // 现金信息右列最大字符

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//以下是普通业务画面按钮区域及信息区域标题定义
static const TCHAR* const TITLE_CASH_INFO					   = _T("现金信息");
static const TCHAR* const TITLE_CASH_INFO_EN					   = _T("Cash Information");
static const TCHAR* const TITLE_TICKET_INFO					   = _T("车票信息");
static const TCHAR* const TITLE_TICKET_INFO_EN				   = _T("Ticket Information");
static const TCHAR* const TITLE_ISSUE_INFO					   = _T("售票信息");
static const TCHAR* const TITLE_ISSUE_INFO_EN				   = _T("Issue Information");
static const TCHAR* const TITLE_CHARGE_INFO					   = _T("充值信息");
static const TCHAR* const TITLE_CHARGE_INFO_EN                  = _T("CHARge Information");
static const TCHAR* const TITLE_UPDATE_INFO					   = _T("补票信息");
static const TCHAR* const TITLE_UPDATE_INFO_EN				   = _T("Update Information");
static const TCHAR* const TITLE_REFUND_INFO					   = _T("退款信息");
static const TCHAR* const TITLE_REFUND_INFO_EN                  = _T("Refund Information");
static const TCHAR* const TITLE_EXCHANGE_INFO				   = _T("换卡信息");
static const TCHAR* const TITLE_EXCHANGE_INFO_EN				   = _T("Exchange Information");
static const TCHAR* const TITLE_REFRESH_INFO					   = _T("激活信息");
static const TCHAR* const TITLE_REFRESH_INFO_EN				   = _T("Refresh Information");
static const TCHAR* const TITLE_TRANSACTION_RECORDS			   = _T("交易记录");
static const TCHAR* const TITLE_NO_BUSINESS_INCOME			   =  _T("非正常收支");
static const TCHAR* const TITLE_ADMIN_PROCESS				   =  _T("行政处理信息");

//以下是维护业务画面按钮区域及信息区域标题定义
static const TCHAR* const TITLE_STATION_INFO                    =  _T("车站信息");
static const TCHAR* const TITLE_FAILURE_TITLENAME			   =  _T("故障信息");
static const TCHAR* const TITLE_COMMUNICATION_PARA_INFO         =  _T("通信参数信息");
static const TCHAR* const TITLE_MODE_SWITCH_INFO                =  _T("设备模式信息");
static const TCHAR* const TITLE_DATATYPE                        =  _T("数据种类");
static const TCHAR* const TITLE_DATAIMPORT_INFO                 =  _T("数据输入信息");
static const TCHAR* const TITLE_IMPORTTYPE                      =  _T("输入方式");
static const TCHAR* const TITLE_EXPORTTYPE                      =  _T("输出方式");
static const TCHAR* const TITLE_DATAEXPORT_INFO                 =  _T("数据输出信息");
static const TCHAR* const TITLE_VERSION_INFO                    =  _T("版本信息");
static const TCHAR* const TITLE_DATA_AUDIT_INFO                 =  _T("可结算数据信息");
static const TCHAR* const TTILE_SYSTEMTYPEOVER                  =  _T("程序更新信息");
static const TCHAR* const TITLE_TH_TEST_INFO                    =  _T("出票机测试");
static const TCHAR* const TITLE_TH_UNITTEST_INFO                =  _T("单体测试");
static const TCHAR* const TITLE_TH_UNITTEST_DETAIL              =  _T("详细测试");
static const TCHAR* const TITLE_ICRW_TEST_INFO                  =  _T("外部IC读写器测试项目");
static const TCHAR* const TITLE_PRINT_TEST_INFO                 =  _T("打印机测试项目");
static const TCHAR* const TITLE_MONEYBOX_TEST_INFO              =  _T("钱柜测试项目");
static const TCHAR* const TITLE_NETWORK_TEST_INFO               =  _T("上位测试项目");
static const TCHAR* const TTILE_SYSTEMTYPE                      =  _T("程序种类");
static const TCHAR* const TTILE_YKTPARA                         =  _T("一卡通参数信息");
static const TCHAR* const TITLE_AUTHORIZE_TYPE                  =  _T("认证种类");
static const TCHAR* const TITLE_AUTHORIZE_MANNER                =  _T("认证方式");
static const TCHAR* const TITLE_AUTHORIZE_MANNER_OVER           =  _T("认证方式信息");
static const TCHAR* const TITLE_SN_SETTING_OVER                 =  _T("序列号设定");
static const TCHAR* const TITLE_UDSN_INFO					   =  _T("流水号信息");
static const TCHAR* const TITLE_TH_MODE_INFO					   =  _T("出票模式信息");
static const TCHAR* const TITLE_UPS_INFO						   =  _T("UPS信息");

//以下是普通业务画面信息区域显示项目显示内容定义
static const TCHAR* const TXT_COLON						= _T("：");
static const TCHAR* const TXT_NOTHING					= _T("--");		// 默认显示内容
// 单位名称定义
static const TCHAR* const TXT_YUAN_MARK					= _T("￥");
static const TCHAR* const MONEY_UNIT						= _T("元");
static const TCHAR* const MONEY_UNIT_EN					= _T("yuan");
static const TCHAR* const TICKET_UNIT					= _T("张");
static const TCHAR* const TICKET_UNIT_EN					= _T("piece(s)");
static const TCHAR* const TIMES_UNIT						= _T("次");
static const TCHAR* const TIMES_UNIT_EN					= _T("time(s)");
static const TCHAR* const YEAR_UNIT						= _T("年");
static const TCHAR* const YEAR_UNIT_EN					= _T("year(s)");
static const TCHAR* const MONTH_UNIT						= _T("月");
static const TCHAR* const MONTH_CNT_UNIT					= _T("个月");
static const TCHAR* const MONTH_CNT_UNIT_EN				= _T("month(s)");
static const TCHAR* const DATE_UNIT						= _T("天");
static const TCHAR* const DAY_UNIT						= _T("日");
static const TCHAR* const DAY_UNIT_EN					= _T("day(s)");
static const TCHAR* const HOUR_UNIT						= _T("时");
static const TCHAR* const MINUTE_UNIT					= _T("分");

static const TCHAR* const MALE							= _T("男");
static const TCHAR* const FEMALE						= _T("女");

static const TCHAR* const TXT_REFUND_REASON_PASSAGER = _T("乘客原因");
static const TCHAR* const TXT_REFUND_REASON_SUBWAY = _T("地铁原因");
static const TCHAR* const TXT_APPLY_REASON_BAD_CARD = _T("票卡损坏");
static const TCHAR* const TXT_APPLY_REASON_LARGE_CARD = _T("大额卡");
static const TCHAR* const TXT_APPLY_REASON_OTHER = _T("其他");

static const TCHAR* const TXT_CERTIFICATE_ID_CARD_MAINLAND = _T("身份证");
static const TCHAR* const TXT_CERTIFICATE_STUDENT = _T("学生证");
static const TCHAR* const TXT_CERTIFICATE_MILITARY = _T("军官证");
static const TCHAR* const TXT_CERTIFICATE_FOREIGNER = _T("外国人永久居留证");

// 公用定义
static const TCHAR* const TXT_UNDEFINE = _T("--");
static const TCHAR* const TXT_TICKET_NUMBER				= _T("车票编号");
static const TCHAR* const TXT_TICKET_NUMBER_EN 		    = _T("Ticket Number");
static const TCHAR* const TXT_TICKET_KIND				= _T("车票种类");
static const TCHAR* const TXT_TICKET_KIND_EN	            = _T("Ticket Type");
static const TCHAR* const TXT_TICKET_VALIDATE_DATE		= _T("有效期限");
static const TCHAR* const TXT_TICKET_VALIDATE_DATE_EN	= _T("Validity Period");
static const TCHAR* const TXT_TICKET_BALANCE				= _T("票内余额");
static const TCHAR* const TXT_TICKET_RIDES				= _T("票内余次");
static const TCHAR* const TXT_TICKET_RIDES_EN			= _T("The Balance");
static const TCHAR* const TXT_TICKET_BALANCE_EN          = _T("The Balance");

static const TCHAR* const TXT_TICKET_DEPOSIT				= _T("车票押金");
static const TCHAR* const TXT_TICKET_DEPOSIT_EN          = _T("Deposit");
static const TCHAR* const TXT_TOTAL_VALUE			    = _T("合计金额");
static const TCHAR* const TXT_TOTAL_VALUE_EN			    = _T("Total Amount");

static const TCHAR* const TXT_TOTAL     					= _T("应收");
static const TCHAR* const TXT_INCOME					    = _T("实收");
static const TCHAR* const TXT_CHANGE					    = _T("找零");

static const TCHAR* const TXT_TOTAL_RECEPTION     			= _T("应收金额");
static const TCHAR* const TXT_TOTAL_RECEPTION_EN	        = _T("Amount Due");
static const TCHAR* const TXT_INCOME_RECEPTION				= _T("已付金额");
static const TCHAR* const TXT_INCOME_RECEPTION_EN		    = _T("Paid");
static const TCHAR* const TXT_CHANGE_RECEPTION				= _T("找零金额");
static const TCHAR* const TXT_CHANGE_RECEPTION_EN		    = _T("Change");

// 售票
static const TCHAR* const TXT_TICKET_AMOUNT				= _T("车票单价");
static const TCHAR* const TXT_TICKET_AMOUNT_EN			= _T("Unit Fare");
static const TCHAR* const TXT_TICKET_COUNT				= _T("售票张数");
static const TCHAR* const TXT_TICKET_COUNT_EN			= _T("No. of Ticket");
static const TCHAR* const TXT_START_STATION				= _T("起始车站");
static const TCHAR* const TXT_START_STATION_EN			= _T("Starting Station");
static const TCHAR* const TXT_END_STATION				= _T("目的车站");
static const TCHAR* const TXT_END_STATION_EN				= _T("Destination");
static const TCHAR* const TXT_ISSUE_DATE_ONLY			= _T("当日有效");
static const TCHAR* const TXT_ISSUE_DATE_ONLY_EN			= _T("Issued Date Only");
static const TCHAR* const TXT_ANY_STATION				= _T("任意车站");
static const TCHAR* const TXT_USED_STATION				= _T("有效车站");
static const TCHAR* const TXT_USED_STATION_EN			= _T("Validity Station");
static const TCHAR* const TXT_VALID_RIDES = _T("有效乘次");
static const TCHAR* const TXT_VALID_RIDES_EN = _T("Rides");
static const TCHAR* const TXT_TICKET_SALE_AMOUNT = _T("售票金额");
static const TCHAR* const TXT_TICKET_SALE_AMOUNT_EN = _T("Fare");

// 充值
static const TCHAR* const TXT_CHARGEOVER_AMOUNT			= _T("充值后余额");
static const TCHAR* const TXT_CHARGEOVER_AMOUNT_EN       = _T("The Balance");
static const TCHAR* const TXT_CHARGE_AMOUNT				= _T("充值金额");
static const TCHAR* const TXT_CHARGE_AMOUNT_EN			= _T("Add Value");
static const TCHAR* const TXT_CHARGE_RIDE				= _T("充值次数");
static const TCHAR* const TXT_CHARGE_RIDE_EN				= _T("Add Rides");
static const TCHAR* const TXT_CHARGEOVER_RIDE			= _T("充值后余次");
static const TCHAR* const TXT_CHARGEOVER_RIDE_EN			= _T("The Balance");
static const TCHAR* const TXT_CHARGE_PRICE_TIME			= _T("收费金额");
static const TCHAR* const TXT_CHARGE_PRICE_TIME_EN		= _T("Fee");

// 分析
static const TCHAR* const TXT_TICKET_STATUS				= _T("车票状态");
static const TCHAR* const TXT_TICKET_STATUS_EN		    = _T("Ticket Status");
static const TCHAR* const TXT_SUGGESTION					= _T("处理建议");
static const TCHAR* const TXT_SUGGESTION_EN				= _T("Suggestion");
static const TCHAR* const TXT_ENTER_STATION				= _T("进站车站");
static const TCHAR* const TXT_ENTER_STATION_EN           = _T("Entry Station");
static const TCHAR* const TXT_ENTER_STATION_TIME			= _T("进站时间");
static const TCHAR* const TXT_ENTER_STATION_TIME_EN      = _T("Entry Time");
static const TCHAR* const TXT_EXIT_STATION				= _T("出站车站");
static const TCHAR* const TXT_EXIT_STATION_EN            = _T("Exit Station");
static const TCHAR* const TXT_EXIT_STATION_TIME			= _T("出站时间");
static const TCHAR* const TXT_EXIT_STATION_TIME_EN		= _T("Exit Time");
static const TCHAR* const TXT_TRANSACTION_TIME			= _T("交易时间");
static const TCHAR* const TXT_TRANSACTION_TIME_EN        = _T("Transaction Time");
static const TCHAR* const TXT_TRANSACTION_LOCATION	    = _T("交易地点");
static const TCHAR* const TXT_TRANSACTION_LOCATION_EN    = _T("Transaction Location");
static const TCHAR* const TXT_TRANSACTION_TYPE			= _T("交易类型");
static const TCHAR* const TXT_TRANSACTION_TYPE_EN        = _T("Transaction Type");
static const TCHAR* const TXT_TRANSACTION_AMOUNT			= _T("交易金额");
static const TCHAR* const TXT_TRANSACTION_AMOUNT_EN      = _T("Transaction Amount");
static const TCHAR* const TXT_TRANSCATION_TIMES          =_T("交易次数");
static const TCHAR* const TXT_RECENTLY_USE_TIME			= _T("最近使用时间");
static const TCHAR* const TXT_RECENTLY_USE_STATUS		= _T("最近使用车站");
static const TCHAR* const TXT_JOURNEYSTATUS_STATUS		= _T("旅程状态");
static const TCHAR* const TXT_ACTIVE_STATUS = _T("激活状态");
static const TCHAR* const TXT_HOLDER_NAME = _T("姓名");
static const TCHAR* const TXT_CERTIFATE_TYPE = _T("证件类型");
static const TCHAR* const TXT_CERTIFATE_NUMBER = _T("证件号码");

static const TCHAR* const TXT_UNKNOW_STATUS				= _T("未知");
static const TCHAR* const TXT_ENTERED_STATUS				= _T("进站");
static const TCHAR* const TXT_EXITED_STATUS				= _T("出站");
static const TCHAR* const TXT_UNKNOW_STATUS_EN			= _T("Unknow");
static const TCHAR* const TXT_TICKET_REGION = _T("票卡类型");
static const TCHAR* const TXT_TICKET_LOCK_STATUS = _T("锁卡状态");
static const TCHAR* const TXT_TICKET_START_DATE = _T("启用日期");


// 补票/更新
static const TCHAR* const TXT_ADJUST_TYPE				= _T("补票方式");
static const TCHAR* const TXT_ADJUST_TYPE_EN				= _T("Adjust Type");
static const TCHAR* const TXT_ADJUST_AMOUNT				= _T("补票金额");
static const TCHAR* const TXT_ADJUST_AMOUNT_EN           = _T("Adjust Amount");
static const TCHAR* const TXT_ACTUAL_ADJUST_AMOUNT       = _T("实际补票金额");
static const TCHAR* const TXT_ADJUST_RIDES				= _T("补票次数");
static const TCHAR* const TXT_ACTUAL_ADJUST_RIDES		= _T("实际补票次数");
static const TCHAR* const TXT_ADJUST_RIDES_EN            = _T("Adjust Rides");
static const TCHAR* const TXT_UPDATE_REASON				= _T("补票原因");
static const TCHAR* const TXT_UPDATE_REASON_EN           = _T("Reason for Updating");
static const TCHAR* const TXT_AMOUNT_DUE_VALUE			= _T("应付金额");
static const TCHAR* const TXT_AMOUNT_DUE_VALUE_EN		= _T("Actual Amount");
static const TCHAR* const TXT_AMOUNT_DUE_RIDES			= _T("应补次数");
static const TCHAR* const TXT_AMOUNT_DUE_RIDES_EN		= _T("Actual Rides");

//激活
static const TCHAR* const TXT_OLD_VALIDATE = _T("原有效期");
static const TCHAR* const TXT_OLD_VALIDATE_EN = _T("Original Validity Period");
static const TCHAR* const TXT_NEW_VALIDATE = _T("新有效期");
static const TCHAR* const TXT_NEW_VALIDATE_EN = _T("New Validity Period");

//延期
static const TCHAR* const TXT_DEFER_DAYS = _T("延长时间");
static const TCHAR* const TXT_DEFER_DAYS_EN = _T("Defer Days");
static const TCHAR* const TXT_SEVICE_FEE = _T("手续费");

// 退款 && TVM退款金额
static const TCHAR* const TXT_DEVICE_NUMBER				= _T("设备编号");
static const TCHAR* const TXT_FAILURE_NUMBER			    = _T("退款凭条号");
static const TCHAR* const TXT_REFUND_MONEY				= _T("退款金额");
static const TCHAR* const TXT_REFUND_MONEY_EN			= _T("Refund Amount");
static const TCHAR* const TXT_REFUND_TYPE				= _T("退款类型");
static const TCHAR* const TXT_REFUND_REASON				= _T("退款原因");

// 非营业性收支
static const TCHAR* const TXT_PROCESS_TYPE				= _T("处理类别：");
static const TCHAR* const TXT_INCOME_TYPE				= _T("收支类型：");
static const TCHAR* const TXT_INCOME_REASON				= _T("收支原因：");
static const TCHAR* const TXT_INCOME_VALUE				= _T("收支金额：");
static const TCHAR* const TXT_INCOME_NOTE				= _T("备注：");

// 销售统计
static const TCHAR* const TXT_BUSINESS					= _T("业务");
static const TCHAR* const TXT_COUNT						= _T("张数");
static const TCHAR* const TXT_INCOME_AMOUNT				= _T("金额");

// 记名卡申请
static const TCHAR* const TXT_FIRST_APPLY				    = _T("记名卡申请");
static const TCHAR* const TXT_UPDATA_APPLY				    = _T("持卡人信息修改");
static const TCHAR* const TXT_CHECK_APPLY				    = _T("状态查询");
static const TCHAR* const TXT_USER_NAME				    	= _T("姓名");
static const TCHAR* const TXT_USER_SEX						= _T("性别");
static const TCHAR* const TXT_USER_CERTIFICATE_TYPE			= _T("证件类型");
static const TCHAR* const TXT_USER_CERTIFICATE_NUM			= _T("证件编码");
static const TCHAR* const TXT_USER_CURRENT_STATE 			= _T("当前状态");
static const TCHAR* const TXT_USER_CERTIFICATE_VALIDATE		= _T("证件有效期");
static const TCHAR* const TXT_USER_PHONE						= _T("电话");
static const TCHAR* const TXT_USER_EMAIL						= _T("Email");
static const TCHAR* const TXT_USER_FIRST_ADDR				= _T("地址1");
static const TCHAR* const TXT_USER_SECND_ADDE				= _T("地址2");


//static const TCHAR* const TXT_PERIOD_RECEPTION_EN		        =	"Period");
//static const TCHAR* const TXT_ZONE_RECEPTION_EN                  =  _T("Zone");
//static const TCHAR* const TXT_TICKET_REGION_EN                   =  _T("Ticket Region");
//static const TCHAR* const TXT_THE_USE_TIMES_RECEPTION_EN         =  _T("Use Times");
//static const TCHAR* const TXT_TCHARGE_THE_BALANCE_RECEPTION_EN    =  _T("Before Reload");
//static const TCHAR* const TXT_TOTAL_RIDE_RECEPTION_EN            =  _T("Value");
//static const TCHAR* const TXT_TOTAL_EXPIRY_DATE_RECEPTION_EN     =  _T("Total Expiry Date");
//static const TCHAR* const TXT_TOTAL_AMOUNT_RECEPTION_EN          =  _T("Value");
//
//static const TCHAR* const TXT_CARDHOLDERFEE_RECEPTION_EN         =  _T("Cardholder Fee");
//static const TCHAR* const TXT_NUMBEROFRIDES_EN			=  _T("Number of Rides");
//static const TCHAR* const TXT_REFRESHING_PERIOD_RECEPTION_EN     =  _T("Refreshing Period");
//static const TCHAR* const TXT_NEW_EXPITY_DATE_RECEPTION_EN       =  _T("New Validity Period");
//static const TCHAR* const TXT_TCHARGE_FOR_SERVICE_RECEPTION_EN    =  _T("TCHARge for Service");
//static const TCHAR* const TXT_TICKET_NUMBER_RECEPTION_EN         =  _T("Ticket Number");
//static const TCHAR* const TXT_REFRESH_AMOUNT_RECEPTION_EN        =  _T("Refresh Amount");
//static const TCHAR* const TXT_REFUND_INFO_TITLE_EN	            = 	"Refund Information");
//
//static const TCHAR* const TXT_REFUND_AMOUNT_RECEPTION_EN         =  _T("Total Amount");
//static const TCHAR* const TXT_TICKET_COST_RECEPTION_EN           =  _T("Ticket Cost");
//static const TCHAR* const TXT_UPDATE_AMOUNT_RECEPTION_EN         =  _T("Update Amount");

//static const TCHAR* const TXT_EXCHANGE_AMOUNT_RECEPTION_EN       =  _T("Exchange Amount");
//static const TCHAR* const TXT_PASSENGERTYPE_EN                   =  _T("Passenger Type");
//static const TCHAR* const TXT_LOCK_STATUS_EN                     =  _T("Lock Status");
//
//
//static const TCHAR* const TXT_ADJUST_OVER_TIME_EN                =  _T("OverTime Amount");
//static const TCHAR* const TXT_ADJUST_FINE_EN                     =  _T("Fine");
//static const TCHAR* const TXT_ADJUST_COSTPRICE_EN                =  _T("Cost Price");
//static const TCHAR* const TXT_TCHARGE_FOR_SERVICE_EN              =  _T("TCHARge for Service");
//static const TCHAR* const TXT_ACTUAL_AMOUNT_EN		            =  _T("Actual Amount");
//static const TCHAR* const TXT_TICKET_PRICE_TITLE_EN              =  _T("Unit Fare");
//static const TCHAR* const TXT_EXCHANGE_REASON_TITLE_EN           =  _T("Reason for Exchange");
//static const TCHAR* const TXT_STATION_MODE_EN                    =  _T("Station Operation Mode");
//static const TCHAR* const TXT_OVERRIDE_FEE_EN                    =  _T("Override Fee");
//static const TCHAR* const TXT_REVERSE_INFO_RECEPTION_EN          =  _T("Reverse Information");
//static const TCHAR* const TXT_REVERSE_AMOUNT_RECEPTION_EN        =  _T("Reverse Amount");
//static const TCHAR* const TXT_REVERSE_TIMES_RECEPTION_EN         =  _T("Reverse Time(s)");
//static const TCHAR* const TXT_CARD_FEE_RECEPTION_EN              =  _T("Card Fee");
//
//static const TCHAR* const TXT_TICK_STATE_EN                      =  _T("Ticket State");
//static const TCHAR* const TXT_MULTIRIDE_PRODUCT_STATE_EN         =  _T("Multiride Product State");
//static const TCHAR* const TXT_PERIOD_PRODUCT_STATE_EN            =  _T("Period Product State");
//static const TCHAR* const TXT_PURSE_PRODUCT_STATE_EN             =  _T("Purse Product State");
//static const TCHAR* const TXT_PASSENGERTYPE_RECEPTION_EN         =  _T("Passenger Type");
//static const TCHAR* const TXT_JOURNEYSTATUS_RECEPTION_EN         =  _T("Journey Status");
//static const TCHAR* const TXT_LASTTIME_RECEPTION_EN              =    "Last Date Time");
//static const TCHAR* const TXT_LASTSTATION_RECEPTION_EN           =    "Last Station");
//
//static const TCHAR* const TXT_ANALYZE_DATE_EN                    =    "Date");
//static const TCHAR* const TXT_ANALYZE_TICKET_KIND_EN             =    "Ticket Type");
//
//static const TCHAR* const TXT_ANALYZE_BALANCE_EN                 =    "Balance");
//
//static const TCHAR* const TXT_YKTANALYZE_DATE_EN                    =    "Date");
//static const TCHAR* const TXT_YKTANALYZE_TICKET_KIND_EN             =    "System");
//static const TCHAR* const TXT_YKTANALYZE_TRANSACTION_LOCATION_EN    =    "Station Involved");
//
//static const TCHAR* const TXT_YKTANALYZE_BALANCE_EN                 =    "Origin Value");
//
//static const TCHAR* const TXT_MULTIRIDE_CARD_TYPE_EN                =    "multiride card type");
//static const TCHAR* const TXT_MULTIRIDE_CARD_VALIDITYPERIOD_EN      =    "multiride card validityperiod");
//static const TCHAR* const TXT_PERIOD_CARD_TYPE_EN                   =    "period card type");
//static const TCHAR* const TXT_PERIOD_CARD_VALIDITYPERIOD_EN         =    "period card validityperiod");
//static const TCHAR* const TXT_TRANSACTION_TIMES_EN                   =    "transaction times");
//static const TCHAR* const TXT_OLD_TIMES_EN                           =    "old times");
//
//static const TCHAR* const TXT_ANALYZE_DATE_YKT_PRO_EN			   =    "Date");
//static const TCHAR* const TXT_ANALYZE_TRANSACTION_TYPE_YKT_PRO_EN   =    "Transaction Type");
//static const TCHAR* const TXT_ANALYZE_TRANSACTION_AMOUNT_YKT_PRO_EN =    "Transaction Amount");
//static const TCHAR* const TXT_ANALYZE_BALANCE_YKT_PRO_EN			   =    "Balance");
//static const TCHAR* const TXT_ANALYZE_STAFFID_YKT_PRO_EN 		   =    "StaffID");
//static const TCHAR* const TXT_ANALYZE_TCHARGE_YKT_PRO_EN			   =    "DeviceID");

// 以下是普通业务画面按钮显示名称
static const TCHAR* const STR_OK									= _T("确认");
static const TCHAR* const STR_PRINT								= _T("打印");
static const TCHAR* const STR_BACK								= _T("返回");
static const TCHAR* const STR_RESET								= _T("重置");
static const TCHAR* const TXT_END_SVC                            = _T("业务结束");
static const TCHAR* const TXT_SHUTDOWN_SVC                       = _T("关机");
static const TCHAR* const TXT_LOGIN_SVC                          = _T("登录");
static const TCHAR* const TXT_REMOVE_INIT_EXCEPTION_SVC			= _T("异常解除");
static const TCHAR* const TXT_ISSUE_SVC                          = _T("售票");
static const TCHAR* const TXT_SINGLE_RIDE_TICKET				    = _T("单程票");
static const TCHAR* const TXT_EXIT_ONLY_TICKET					= _T("出站票");
static const TCHAR* const TXT_COMMEMORATICE_TICKET				= _T("纪念票");
static const TCHAR* const TXT_WELFARE_TICKET					    = _T("福利票");
static const TCHAR* const TXT_ECT_TICKET							= _T("一卡通");
static const TCHAR* const TXT_CHARGE_SVC                         = _T("充值");
static const TCHAR* const TXT_RECHARGE_SVC                       = _T("充资");
static const TCHAR* const TXT_ADDTIMES_SVC                       = _T("充次");
static const TCHAR* const TXT_ANALYZE_SVC						= _T("分析");
static const TCHAR* const TXT_ADJUST_SVC                         = _T("补票");
static const TCHAR* const TXT_ADJUST_SVC_PAIDAREA				= _T("付费区");
static const TCHAR* const TXT_ADJUST_SVC_NOPAIDAREA				= _T("非付费区");
static const TCHAR* const TXT_REFUND_MAIN						= _T("退款");
static const TCHAR* const TXT_SURRENDER_CARD						= _T("退卡");
static const TCHAR* const TXT_REFUND_SVC                         = _T("退资");
static const TCHAR* const TXT_REFUND_BASE_SVC					= _T("即时退卡");
static const TCHAR* const TXT_REFUND_APPLY_SVC					= _T("退卡申请");
static const TCHAR* const TXT_REFUND_QUERY_SVC					= _T("申退查询");
static const TCHAR* const TXT_EXCHANGE_SVC                       = _T("替换");
static const TCHAR* const TXT_PRESELL_CANCEL_TICKET				= _T("预售抵销");
static const TCHAR* const TXT_PRESELL							= _T("预售");
static const TCHAR* const TXT_CANCEL_READ_TICKET_SVC             = _T("抵消");
static const TCHAR* const TXT_OTHERS_SVC                         = _T("其他");
static const TCHAR* const TXT_TVMFAILURE_REFUND_SVC              = _T("TVM故障退款");
static const TCHAR* const TXT_NOBUSINESS_INCOME_SVC				= _T("非营业性收支");
static const TCHAR* const TXT_REFRESH_SVC						= _T("激活");
static const TCHAR* const TXT_DEFER_SVC							= _T("延期");
static const TCHAR* const TXT_SIGNCARD_APPLY_SVC					= _T("记名卡业务");
static const TCHAR* const TXT_SIGNCARD_REPORT_LOSS_SVC           = _T("记名卡挂失");
static const TCHAR* const TXT_SIGNCARD_CANCEL_REPORT_LOSS_SVC	= _T("记名卡解挂");
static const TCHAR* const TXT_CARD_SUSPEND_AND_RESUME_SVC		= _T("挂失/解挂");
static const TCHAR* const TXT_TICKETBOX_EXCHANGE_SVC = _T("票箱更换");
static const TCHAR* const TXT_ADMINISTRATION_PROCESS_SVC = _T("行政处理");
static const TCHAR* const TXT_LOCK_SVC = _T("锁定/解锁");
static const TCHAR* const TXT_ASSIST_MAIN						= _T("辅助");
static const TCHAR* const TXT_TH_MODE_SET_SVC					= _T("出票模式设定");
static const TCHAR* const TXT_CHANGE_PASSWORD_SVC                = _T("修改密码");
static const TCHAR* const TXT_SALE_STATIC_SVC					= _T("销售统计");
static const TCHAR* const TXT_RECEIPT_PRINT_SVC					= _T("打印单据");
static const TCHAR* const TXT_LOGOFF_SVC                         = _T("登出");
static const TCHAR* const TXT_EXIT_BOM_SVC						= _T("退出程序");

// 以下是维护业务按钮显示名称(中文)
static const TCHAR* const TXT_SYSTEM_SET_SVC                     = _T("系统设定");
static const TCHAR* const TXT_STATION_SET_SVC					= _T("当前车站设定");
static const TCHAR* const TXT_COMMUNICATION_PARAMETER_SET_SVC	= _T("通信参数设定");
static const TCHAR* const TXT_MODE_SWITCH_SVC					= _T("设备模式切换");
static const TCHAR* const TXT_SYSTEM_TIME_SET_SVC				= _T("时间设定")  ;
static const TCHAR* const TXT_RFID_MODE_SET_SVC					= _T("RFID模式设定")  ;
static const TCHAR* const TXT_UDSN_SET_SVC						= _T("UDSN设定");

static const TCHAR* const TXT_DATA_MANAGE_SVC					= _T("数据管理");
static const TCHAR* const TXT_DATA_INPUT_SVC						= _T("数据导入");
static const TCHAR* const TXT_DATA_OUTPUT_SVC					= _T("数据导出");
static const TCHAR* const TXT_DEVICE_TEST_SVC                    = _T("硬件自检");
static const TCHAR* const TXT_OUTRW_TEST_SVC						= _T("读写器测试");
static const TCHAR* const TXT_PRINTER_TEST_SVC					= _T("打印机测试");
static const TCHAR* const TXT_MONEYBOX_TEST_SVC					= _T("钱箱测试");
static const TCHAR* const TXT_NETWORK_TEST_SVC					= _T("网络测试");
static const TCHAR* const TXT_UPS_INFO_SVC						= _T("UPS信息");
static const TCHAR* const TXT_TH_TEST_SVC						= _T("出票机测试");
static const TCHAR* const TXT_REMOVE_EXCEPTION_SVC				= _T("异常解除");

// 以下是普通业务中文名称定义
static const TCHAR* const TXT_END_SVC_CN                            = _T("业务结束");
static const TCHAR* const TXT_SHUTDOWN_SVC_CN                       = _T("关机");
static const TCHAR* const TXT_LOGIN_SVC_CN                          = _T("登录");
static const TCHAR* const TXT_REMOVE_INIT_EXCEPTION_SVC_CN		   = _T("异常解除");
static const TCHAR* const TXT_ISSUE_SVC_CN                          = _T("售票");
static const TCHAR* const TXT_ISSUE_SVC_EN                          = _T("ISSUE");
static const TCHAR* const TXT_CHARGE_SVC_CN                         = _T("充值");
static const TCHAR* const TXT_RECHARGE_SVC_CN                       = _T("充资");
static const TCHAR* const TXT_ADDTIMES_SVC_CN                       = _T("充次");
static const TCHAR* const TXT_CHARGE_SVC_EN                         = _T("TCHARGE");
static const TCHAR* const TXT_ANALYZE_SVC_CN						   = _T("分析");
static const TCHAR* const TXT_ANALYZE_SVC_EN						   = _T("ANALYZE");
static const TCHAR* const TXT_ADJUST_SVC_CN                         = _T("补票");
static const TCHAR* const TXT_ADJUST_SVC_PAIDAREA_CN				   = _T("付费区");
static const TCHAR* const TXT_ADJUST_SVC_NOPAIDAREA_CN			   = _T("非付费区");
static const TCHAR* const TXT_ADJUST_SVC_EN						   = _T("UPDATE");
static const TCHAR* const TXT_REFUND_MAIN_CN						   = _T("退款");
//static const TCHAR* const TXT_SURRENDER_CARD_CN					   = _T("退卡");
//static const TCHAR* const TXT_REFUND_SVC_CN                         = _T("退资");
static const TCHAR* const TXT_REFUND_SVC_EN                         = _T("REFUND");
static const TCHAR* const TXT_EXCHANGE_SVC_CN                       = _T("换卡");
static const TCHAR* const TXT_EXCHANGE_SVC_EN                       = _T("EXCHANGE");
static const TCHAR* const TXT_PRESELL_CANCEL_TICKET_CN			   = _T("预售抵销");
static const TCHAR* const TXT_PRESELL_CN							   = _T("预售");
static const TCHAR* const TXT_CANCEL_READ_TICKET_SVC_CN             = _T("抵消");
static const TCHAR* const TXT_OTHERS_SVC_CN                         = _T("其他");
static const TCHAR* const TXT_TVMFAILURE_REFUND_SVC_CN              = _T("TVM故障退款");
static const TCHAR* const TXT_NOBUSINESS_INCOME_SVC_CN			   = _T("非营业性收支");
static const TCHAR* const TXT_REFRESH_SVC_CN						   = _T("年检");
static const TCHAR* const TXT_SIGNCARD_APPLY_SVC_CN				   = _T("申请人信息");
static const TCHAR* const TXT_SIGNCARD_REPORT_LOSS_SVC_CN           = _T("记名卡挂失");
static const TCHAR* const TXT_SUSPEND_AND_RESUME_SVC_CN			   = _T("持卡人信息");
static const TCHAR* const TXT_CHANGE_PASSWORD_SVC_CN                = _T("修改密码");
static const TCHAR* const TXT_SALE_STATIC_SVC_CN					   = _T("销售统计");
static const TCHAR* const TXT_LOGOFF_SVC_CN                         = _T("登出");

// 以下是维护业务名称定义
static const TCHAR* const TXT_SYSTEM_SET_SVC_CN						= _T("系统设定");
static const TCHAR* const TXT_STATION_SET_SVC_CN						= _T("当前车站设定");
static const TCHAR* const TXT_COMMUNICATION_PARAMETER_SET_SVC_CN		= _T("通信参数设定");
static const TCHAR* const TXT_MODE_SWITCH_SVC_CN						= _T("设备模式切换");
static const TCHAR* const TXT_SYSTEM_TIME_SET_SVC_CN					= _T("时间设定")  ;
static const TCHAR* const TXT_RFID_MODE_SET_SVC_CN					= _T("RFID模式设定")  ;
static const TCHAR* const TXT_UDSN_SET_SVC_CN						= _T("UDSN设定");
static const TCHAR* const TXT_DATA_MANAGE_SVC_CN						= _T("数据管理");
static const TCHAR* const TXT_DATA_INPUT_SVC_CN						= _T("数据导入");
static const TCHAR* const TXT_DATA_OUTPUT_SVC_CN						= _T("数据导出");
static const TCHAR* const TXT_DEVICE_TEST_SVC_CN						= _T("硬件自检");
static const TCHAR* const TXT_RW_TEST_SVC_CN							= _T("读写器测试");
static const TCHAR* const TXT_PRINTER_TEST_SVC_CN					= _T("打印机测试");
static const TCHAR* const TXT_MONEYBOX_TEST_SVC_CN					= _T("钱箱测试");
static const TCHAR* const TXT_NETWORK_TEST_SVC_CN					= _T("网络测试");
static const TCHAR* const TXT_TH_TEST_SVC_CN							= _T("出票机测试");
static const TCHAR* const TXT_REMOVE_EXCEPTION_SVC_CN				= _T("异常解除");
static const TCHAR* const TXT_RESTART_SVC_CN							= _T("重启");


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////导航文言定义///////////////////////////////////////////////////////////////
typedef enum _maintenance_msg{
	// Ｆｉｇ２０１０１
	GUIDE_SYSTEM_SET                                    = 20101001,     // （黑）请选择要进行的处理
	// 设置内容选择
	//Ｆｉｇ２０１１１	
	GUIDE_MACHINE_PARAMETER_SET_SELECT                  = 20111001, 	// （黑）请选择要设定的内容

	// 车站设置
	//Ｆｉｇ２０１１２	
	GUIDE_STATION_SET_INFO_INPUT                        = 20112001, 	// （黑）请输入车站信息，并按确认键
	GUIDE_STATION_SET_INPUT_ERROR                       = 20112002, 	// （红）输入的内容有错误，请核对后重新输入
	// 车站设置结束
	//Ｆｉｇ２０１１３	
	GUIDE_STATION_SET_CONFIRM                           = 20113001, 	// （黑）信息确认无误后，按确认键进行修改
	GUIDE_STATION_SET_FINISH                            = 20113002, 	// （黑）车站信息设置正常结束
	GUIDE_STATION_SET_FARE_ERROR                        = 20113003, 	// （红）设置成功，但所设车站不在票价表中

	// UDSN设置
	//Fig20119
	GUIDE_UDSN_SET_INFO_INPUT                           = 20119001,		// （黑）请输入流水号信息，并按确认键
	GUIDE_UDSN_SET_INPUT_ERROR                          = 20119002, 	// （红）输入的内容有错误，请核对后重新输入
	GUIDE_USDN_SET_FINISH                               = 20119003, 	// （黑）流水号信息设置正常结束

	// 出票模式设定
	GUIDE_TH_MODE_SET_INFO_INPUT                        = 20120001, 	// （黑）请选择出票模式种类，并按确认键
	GUIDE_TH_MODE_SET_INPUT_ERROR                       = 20120002, 	// （红）存在未送数据，不能进行TH模式切换处理
	GUIDE_TH_MODE_SET_FINISH                            = 20120003, 	// （黑）出票模式切换处理正常结束

	// RFID写入模式设定
	GUIDE_RFID_MODE_SET_INFO_INPUT                        = 20122001, 	// （黑）请选择RFID写入模式种类，并按确认键
	GUIDE_RFID_MODE_SET_INPUT_ERROR                       = 20122002, 	// （红）RFID写入模式切换处理异常
	GUIDE_RFID_MODE_SET_FINISH                            = 20122003, 	// （黑）RFID写入模式切换处理正常结束

	// 通信设置
	//Ｆｉｇ２０１１４	
	GUIDE_COMMUNICATION_SET_INFO_INPUT                  = 20114001, 	// （黑）请输入以下各参数
	GUIDE_COMMUNICATION_SET_INPUT_ERROR                 = 20114002, 	// （红）输入的内容有错误，请核对后重新输入

	// 通信设置结束
	//Ｆｉｇ２０１１５	
	GUIDE_COMMUNICATION_SET_CONFIRM                     = 20115001, 	// （黑）信息确认无误后，请按确认键进行修改
	GUIDE_COMMUNICATION_SET_FINISH                      = 20115002, 	// （黑）通信参数设置正常结束
	GUIDE_COMMUNICATION_SET_IP_FAIL                     = 20115003, 	// （红）修改本机IP地址处理失败
	GUIDE_COMMUNICATION_SET_ADAPTER_NOT_MATCH           = 20115004, 	// （红）机器设置参数与实际使用网卡不符，本次设置处理失败

	// 设备模式切换
	//Ｆｉｇ２０１１６	
	GUIDE_MODE_SWITCH_SELECT                            = 20116001, 	// （黑）请选择设备模式种类，并按确认键
	GUIDE_MODE_SWITCH_DATA_EXIST_ERROR                  = 20116002, 	// （红）存在未送数据，不能进行设备模式切换处理
	GUIDE_MODE_SWITCH_CONFIRM_OR_BACK                   = 20116003, 	// （黑）信息确认后，请按确认键，如需修改，请按返回键
	GUIDE_MODE_SWITCH_FINISH                            = 20116004, 	// （黑）设备模式切换处理正常结束

	// RFID可写状态设定
	//Ｆｉｇ２０１１７	
	GUIDE_RFID_STATUS_SWITCH_SELECT                     = 20117001, 	// （黑）请选择RFID可写状态，并按确认键进行RFID可写状态设定
	GUIDE_RFID_STATUS_SWITCH_CONFIRM_BACK               = 20117002, 	// （黑）信息确认后，请按确认键，如需修改，请按返回键
	GUIDE_RFID_STATUS_SWITCH_FINISH                     = 20117003, 	// （黑）RFID可写状态设定正常结束

	// 一卡通参数设置
	//Ｆｉｇ２０１１８
	GUIDE_YKT_PARA_SET_SELECT                           = 20118001, 	// （黑）请输入以下各参数
	GUIDE_YKT_PARA_SET_ERROR                            = 20118002, 	// （红）输入的内容有错误，请核对后重新输入
	GUIDE_YKT_PARA_SET_CONFIRM                          = 20118003, 	// （黑）信息确认无误后，请按确认键进行修改
	GUIDE_YKT_PARA_SET_OVER                             = 20118004,     // （黑）一卡通参数信息设置正常结束


	// 数据输入
	//Ｆｉｇ２０１２１	
	GUIDE_DATA_INPUT_SELECT                             = 20121001, 	// （黑）请确认要导入的数据，按确认键开始
	//Ｆｉｇ２０１２２	
	GUIDE_DATA_INPUTING                                 = 20122001, 	// （黑）数据正在输入中，请稍候……
	//Ｆｉｇ２０１２３	
	GUIDE_DATA_INPUT_FINISH                             = 20123001, 	// （黑）数据输入正常结束
	GUIDE_DATA_INPUT_COMMUNICATION_FAIL                 = 20123002, 	// （红）上位通信失败，无法进行数据输入处理
	GUIDE_DATA_INPUT_NOT_INPUT                          = 20123003, 	// （红）本地数据已经是最新版本，无须进行数据输入处理
	GUIDE_DATA_INPUT_MDDIR_NOT_FIND                     = 20123004, 	// （红）移动介质不存在，请确认移动介质连接正确
	GUIDE_DATA_INPUT_DATA_ERROR                         = 20123005, 	// （红）数据不存在或已损坏，无法进行数据输入处理
	GUIDE_DATA_INPUT_PATH_GET_ERROR                     = 20123006, 	// （红）无法正常取得系统路径
	DUIDE_DATA_INPUT_FAIL                               = 20123007, 	// （红）数据输入处理失败
	DUIDE_DATA_INPUT_FINISH_ACC                         = 20123008, 	// （黑）数据输入正常结束
	GUIDE_DATA_INPUT_PARA_INDEX_NOT_EXIST				= 20123009,		// （红）参数索引文件不存在
	GUIDE_DATA_INPUT_PARA_INDEX_TYPE_ERROR				= 20123010,		// （红）参数索引文件类型不正确
	GUIDE_DATA_INPUT_PARA_INDEX_OTHER_ERROR				= 20123011,		// （红）未知错误，不能进行数据导入

	// 数据输出
	//Ｆｉｇ２０１３１	
	GUIDE_DATA_OUTPUT_SELECT                            = 20131001, 	// （黑）请选择要输出的数据种类和输出方式
	//Ｆｉｇ２０１３２	
	GUIDE_DATA_OUTPUTING                                = 20132001, 	// （黑）数据正在输出中，请稍候……
	//Ｆｉｇ２０１３３	
	GUIDE_DATA_OUTPUT_FINISH                            = 20133001, 	// （黑）数据输出正常结束
	GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND                    = 20133002, 	// （红）移动介质不存在，请确认移动介质连接正确
	GUIDE_DATA_OUTPUT_PATH_GET_ERROR                    = 20133003, 	// （红）无法正常取得系统路径
	GUIDE_DATA_OUTPUT_FAIL                              = 20133004, 	// （红）数据输出处理失败
	GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL					= 20133005,		// （红）创建导出目录失败

	// 版本信息
	//Ｆｉｇ２０１４１	
	GUIDE_VERSION_CONFIRM                               = 20141001, 	// （黑）信息确认后，如需打印，请按打印键
	GUIDE_VERSION_CONFIRM_FINISH                        = 20141002, 	// （黑）版本信息确认结束

	// 认证方式设置
	//Ｆｉｇ２０１５１	
	GUIDE_AUTHORIZE_MANNER_SEL                          = 20151001, 	// （黑）请选择要设置的认证种类和认证方式
	//Ｆｉｇ２０１５２	
	GUIDE_AUTHORIZE_MANNER_OVER                         = 20151002, 	// （黑）认证方式设置正常结束

	// 序列号设定
	//Ｆｉｇ２０１６１
	GUIDE_SN_SET_SEL                                    = 20161001, 	// （黑）请分别输入各种序列号，并按确认键
	//Ｆｉｇ２０１６３
	GUIDE_SN_SET_OVER                                   = 20161003, 	// （黑）序列号设定正常结束

	// 手动机具领用
	//Ｆｉｇ２０１７１
	GUIDE_SLE_RECEIVE_SET                               = 20171001, 	// （黑）请输入以下各参数
	GUIDE_SLE_RECEIVE_ERROR                             = 20171002, 	// （红）输入的内容有错误，请核对后重新输入
	GUIDE_SLE_RECEIVE_ING                               = 20171004,		// （黑）手动机具领用正在发送中，请稍候……
	GUIDE_SLE_RECEIVE_OVER                              = 20171005,		// （黑）手动机具领用正常结束
	GUIDE_SLE_RECEIVE_FAIL                              = 20171006, 	// （红）手动机具领用发送失败

	// 手动机具变更
	//Ｆｉｇ２０１８１
	GUIDE_SLE_EXCHANGE_SET_OLD                            = 20181001,	// （黑）请输入以下原机具设备各参数
	GUIDE_SLE_EXCHANGE_ERROR_OLD                          = 20181002, 	// （红）输入的内容有错误，请核对后重新输入
	//Ｆｉｇ２０１８２
	GUIDE_SLE_EXCHANGE_SET_NEW                            = 20182001,     // （黑）请输入以下新机具设备各参数
	GUIDE_SLE_EXCHANGE_ERROR_NEW                          = 20182002,     // （红）输入的内容有错误，请核对后重新输入
	GUIDE_SLE_EXCHANGE_ING                                = 20182004,     // （黑）手动机具更换正在发送中，请稍候……
	GUIDE_SLE_EXCHANGE_OVER                               = 20182005,     //  （黑）手动机具更换正常结束
	GUIDE_SLE_EXCHANGE_FAIL                               = 20182006,     //  （红）手动机具更换发送失败
	// 程序更新
	//Ｆｉｇ２０２０１	
	GUIDE_SYSTEM_UPDATE_FIRST_SELECT                    = 20201001, 	// （黑）请选择需要更新的程序
	//Ｆｉｇ２０２１１	
	GUIDE_SYSTEM_UPDATE_SORT_METHOD_SELECT              = 20211001, 	// （黑）请选择要更新的程序种类和输入方式
	//Ｆｉｇ２０２１２	
	GUIDE_SYSTEM_UPDATING                               = 20212001, 	// （黑）正在更新中，请稍候……
	//Ｆｉｇ２０２１３	
	GUIDE_SYSTEM_UPDATE_FINISH                          = 20213001, 	// （黑）程序更新正常结束
	GUIDE_SYSTEM_UPDATE_COMMUNICATION_FAIL              = 20213002, 	// （红）上位通信失败，无法进行程序更新处理
	GUIDE_SYSTEM_UPDATE_NOT_UPDATE                      = 20213003, 	// （红）本地程序已经是最新版本，无须进行程序更新处理
	GUIDE_SYSTEM_UPDATE_MDDIR_NOT_FIND                  = 20213004, 	// （红）移动介质不存在，请确认移动介质连接正确
	GUIDE_SYSTEM_UPDATE_FILE_ERROR                      = 20213005, 	// （红）程序文件不存在或已损坏，无法进行数据输入处理
	GUIDE_SYSTEM_UPDATE_PATH_NOT_FIND                   = 20213006, 	// （红）无法正常取得系统路径
	GUIDE_SYSTEM_UPDATE_FAIL                            = 20213007, 	// （红）程序更新处理失败
	//Ｆｉｇ２０２２１	
	GUIDE_SYSTEM_UPDATE_CONFIRM                         = 20221001, 	// （黑）信息确认后，如需打印，请按打印键
	GUIDE_SYSTEM_UPDATE_CONFIRM_FINISH                  = 20221002, 	// （黑）版本信息确认结束

	// 数据结算
	//Ｆｉｇ２０３０１	
	GUIDE_SETTLEMENT_SELECT                             = 20301001, 	// （黑）请选择处理方式
	GUIDE_SETTLEMENT_OUTPUT_WITHOUT_DATA                = 20301002, 	// （黑）不存在可输出的结算数据
	//Ｆｉｇ２０３０２	
	GUIDE_SETTLEMENT_DATA_OUTPUTING                     = 20302001, 	// （黑）数据正在输出中，请稍候……
	//Ｆｉｇ２０３０３	
	GUIDE_SETTLEMENT_OUTPUT_FINISH                      = 20303001, 	// （黑）数据输出正常结束
	GUIDE_SETTLEMENT_CLEAR_FINISH                       = 20303002, 	// （黑）数据清除正常结束
	GUIDE_SETTLEMENT_CLEARING                           = 20303003, 	// （黑）数据清除中，请稍候……
	GUIDE_SETTLEMENT_MDDIR_NOT_FIND                     = 20303004, 	// （红）移动介质不存在，请确认移动介质连接正确
	GUIDE_SETTLEMENT_OUTPUT_FAIL                        = 20303005,	    // （红）数据输出处理失败
	GUIDE_SETTLEMENT_CLEAR_FAIL                         = 20303006, 	// （红）清除数据处理失败
	//Ｆｉｇ２０３０４	
	GUIDE_SETTLEMENT_CLEAR_SELECT                       = 20304001, 	// （黑）请选择处理方式

	// 日志管理
	//Ｆｉｇ２０４０１	
	GUIDE_LOG_MANAGER_SELECT                            = 20401001, 	// （黑）请选择日志种类和处理方式
	//Ｆｉｇ２０４０２	
	GUIDE_LOG_MANAGER_OUTPUTING                         = 20402001, 	// （黑）数据正在输出中，请稍候……
	//Ｆｉｇ２０４０３	
	GUIDE_LOG_MANAGER_OUTPUT_FINISH                     = 20403001, 	// （黑）日志输出正常结束
	GUIDE_LOG_MANAGER_CLEAR_FINISH                      = 20403002, 	// （黑）日志清除正常结束
	GUIDE_LOG_MANAGER_NO_OUTPUT_FILE                    = 20403003, 	// （红）没有可以输出的日志文件
	GUIDE_LOG_MANAGER_MDDIR_NOT_FIND                    = 20403004, 	// （红）移动介质不存在，请确认移动介质连接正确
	GUIDE_LOG_MANAGER_PATH_GET_ERROR                    = 20403005, 	// （红）无法正常得到系统路径
	GUIDE_LOG_MANAGER_OUTPUT_FAIL                       = 20403006, 	// （红）日志输出到移动介质处理失败
	GUIDE_LOG_MANAGER_PRINT_FAIL                        = 20403007, 	// （红）日志输出到打印机处理失败
	GUIDE_LOG_MANAGER_CLEAR_FAIL                        = 20403008, 	// （红）日志清除处理失败
	//Ｆｉｇ２０４０４	
	GUIDE_LOG_MANAGER_CLEAR_SELECT                      = 20404001, 	// （黑）请选择日志种类和处理方式

	// 硬件自检
	//Ｆｉｇ２０５０１	
	GUIDE_DEVICE_TEST_CHOOSE_HARDWARE                   = 20501001, 	// （黑）请选择需要自检的硬件

	// 出票机
	//Ｆｉｇ２０５１１	
	GUIDE_TH_SELECT_ITEM                                = 20511001, 	// （黑）请选择需要测试的项目，按确认键开始
	GUIDE_TH_LOG_OUTPUTING                              = 20511002, 	// （黑）工作日志输出中，请稍候……
	GUIDE_TH_LOG_OUTPUT_FINISH                          = 20511003, 	// （黑）工作日志输出结束，请选择需要测试的项目，按确认键开始
	GUIDE_TH_MOVE_DISK_NOT_HAVE                         = 20511004, 	// （红）移动介质不存在，请确认移动介质连接正确
	GUIDE_TH_LOG_CLEARED_CONFIRM                        = 20511005, 	// （黑）全部工作日志删除结束，请选择需要测试的项目，按确认键开始
	GUIDE_TH_LOG_OUTPUT_ERROR                           = 20511006, 	// （黑）工作日志输出处理失败
	//Ｆｉｇ２０５１２	
	GUIDE_TH_UNIT_TEST_CHOOSE_ITEM                      = 20512001, 	// （黑）请选择需要测试的项目，按确认键开始
	//Ｆｉｇ２０５１３	
	GUIDE_TH_INPUT_SERIAL_NUMBER                        = 20513001, 	// （黑）请输入需要删除的工作日志编号
	GUIDE_TH_INPUT_ERROR                                = 20513002, 	// （红）输入的内容有错误，请核对后重新输入
	GUIDE_TH_LOG_DELETING                               = 20513003, 	// （黑）指定工作日志删除中，请稍候……
	GUIDE_TH_LOG_DELETE_FINISH                          = 20513004, 	// （黑）指定工作日志删除结束
	//Ｆｉｇ２０５１４	
	GUIDE_TH_ALL_LOG_CHOOSE_ITEM                        = 20514001, 	// （黑）请选择需要测试的项目，按确认键开始
	GUIDE_TH_ALL_LOG_DELETING                           = 20514002, 	// （黑）工作日志删除中，请稍候……
	//Ｆｉｇ２０５１５	
	GUIDE_TH_TESTING                                    = 20515001, 	// （黑）测试中，请稍候……
	GUIDE_TH_OPEN                                       = 20515002, 	// （黑）请打开出票机
	GUIDE_TH_TEST_SUCCESS                               = 20515003, 	// （黑）选择的出票机项目测试成功
	
	GUIDE_TH_TEST_CHOOSE_ITEM							= 20515004, 	// （黑）请选择需要测试的项目，按确认键开始
	GUIDE_TH_TEST_FAIL									= 20515005, 	// （红）项目测试失败
	GUIDE_TH_TEST_SUCCESS_NEW							= 20515006, 	// （黑）项目测试成功

	// 外部IC读写器
	//Ｆｉｇ２０５２１	
	GUIDE_IC_PUT_CARD									= 20521001, 	// （黑）请将卡片放在读卡器上
	GUIDE_IC_RESET_CHOOSE_ITEM                          = 20521002, 	// （黑）读写器重置成功，请选择需要测试的项目，按确认键开始
	GUIDE_IC_RESET_ERROR                                = 20521003, 	// （红）读写器重置测试失败
	//Ｆｉｇ２０５２２	
	GUIDE_IC_READ_WAITING                               = 20522001, 	// （黑）等待读取中……
	GUIDE_IC_WRITE_SUCCEED								= 20522002,		// （黑）卡写入测试成功
	GUIDE_IC_WRITE_FAIL									= 20522003,		// （红）卡写入测试失败
	//Ｆｉｇ２０５２３	
	GUIDE_IC_READ_SUCCESS                               = 20523001, 	// （黑）卡读取测试成功
	GUIDE_IC_READ_FAIL                                  = 20523002, 	// （红）卡读取测试失败
	GUIDE_IC_UNKNOWN_CARD								= 20523003,		// （红）不能识别的票卡

	GUIDE_RW_TEST_CHOOSE_ITEM							= 20524001,		// （黑）请选择需要测试的项目，按确认键开始
	GUIDE_RW_TEST_SUCCESS								= 20524002,		// （黑）测试完成，请确认测试结果
	GUIDE_RW_TEST_FAIL									= 20524003,		// （红）测试失败，请检查原因

	// 打印机
	//Ｆｉｇ２０５３１	
	GUIDE_PRINTER_CHOOSE_ITEM                           = 20531001, 	// （黑）请按确认键开始测试
	GUIDE_PRINTER_TESTING                               = 20531002, 	// （黑）测试中，请稍候……
	GUIDE_PRINTER_TEST_FINISH                           = 20531003, 	// （黑）打印测试结束，请确认设备动作

	// 钱柜
	//Ｆｉｇ２０５４１	
	GUIDE_CASH_BOX_CHOOSE_ITEM                          = 20541001, 	// （黑）请按确认键开始测试
	GUIDE_CASH_BOX_TESTING                              = 20541002, 	// （黑）测试中，请稍候……
	GUIDE_CASH_BOX_TEST_FINISH                          = 20541003, 	// （黑）钱柜打开测试结束，请确认设备动作

	// 网络
	//Ｆｉｇ２０５５１	
	GUIDE_NET_TEST_CHOOSE_ITEM                          = 20551001, 	// （黑）请选择需要测试的项目，按确认键开始
	GUIDE_NET_TESTING                                   = 20551002, 	// （黑）测试中，请稍候……
	GUIDE_SC_INFO_CONFIRM                               = 20551003, 	// （黑）请确认SC联机测试信息
	GUIDE_FTP_DOWNLOAD_SUCCESS                          = 20551004, 	// （黑）FTP下载测试成功
	GUIDE_FTP_UPLOAD_SUCCESS                            = 20551005, 	// （黑）FTP上传测试成功
	GUIDE_FTP_CONNECT_FAIL                              = 20551006, 	// （红）FTP连接失败，请检查网络连接设置
	GUIDE_FTP_UPLOAD_FAIL                               = 20551007, 	// （红）FTP上传测试失败，请检查相关参数设定
	GUIDE_FTP_DOWNLOAD_FAIL                             = 20551008, 	// （红）FTP下载测试失败，请检查相关参数设定
	GUIDE_CONNECTION_CLOSE_FAIL                         = 20551009, 	// （红）FTP关闭连接失败，请检查网络连接设置
	GUIDE_PASSWORD_FAIL                                 = 20551010, 	// （红）FTP用户名或密码错误，请重新下载参数

	// 触摸屏
	//Ｆｉｇ２０５６１	
	GUIDE_TOUCH_SCREEN_CLICK                            = 20561001, 	// （黑）请按下图中所示按钮后，按确认键进行确认
	GUIDE_TOUCH_SCREEN_UPLEFT_ERROR                     = 20561002, 	// （黑）触摸屏左上侧按钮区域点击有误，请继续测试其它区域
	GUIDE_TOUCH_SCREEN_UPRIGHT_ERROR                    = 20561003, 	// （黑）触摸屏右上侧按钮区域点击有误，请继续测试其它区域
	GUIDE_TOUCH_SCREEN_CENTER_ERROR                     = 20561004, 	// （黑）触摸屏中央按钮区域点击有误，请继续测试其它区域
	GUIDE_TOUCH_SCREEN_DOWNLEFT_ERROR                   = 20561005, 	// （黑）触摸屏左下侧按钮区域点击有误，请继续测试其它区域
	GUIDE_TOUCH_SCREEN_DOWNRIGHT_ERROR                  = 20561006, 	// （黑）触摸屏右下侧按钮区域点击有误，触摸屏测试结束
	GUIDE_TOUCH_SCREEN_TEST_FINISH                      = 20561007, 	// （黑）触摸屏测试结束

	// UPS	
	GUIDE_UPS_INFO_READ_WAITING							= 20571001, 	// （黑）等待读取中……
	GUIDE_UPS_INFO_READ_FAIL							= 20571002, 	// （红）UPS信息读取失败
	GUIDE_UPS_INFO_READ_SUCCESS							= 20571003, 	// （黑）UPS信息读取成功

	// 外部媒体业务
	//Ｆｉｇ２０６０１	
	GUIDE_OUT_DEVICE_SELECT                             = 20601001, 	// （黑）请选择您要拔取的外部媒体
	GUIDE_USB_REMOVE_SUCCESS                            = 20601002, 	// （黑）USB设备安全移除，可以拔取
	GUIDE_CF_REMOVE_SUCCESS                             = 20601003, 	// （黑）CF卡安全移除
	GUIDE_USB_REMOVE_ERROR                              = 20601004, 	// （黑）未能安全移除USB设备
	GUIDE_CF_REMOVE_ERROR                               = 20601005,  	// （黑）未能安全移除CF卡
	GUIDE_CF_FORMAT_SUCCESS                             = 20601006,     // （黑）格式化CF卡正常结束

	// 异常解除业务
	//Ｆｉｇ２０７０１	
	GUIDE_REMOVE_EXCEPTION                              = 20701001, 	// （黑）请确认以下异常信息，按确认键解除异常

	// 数据修复业务
	// Ｆｉｇ２０８０１
	GUIDE_REPAIR_DATABASE_SELECT                        = 20801001,     //  (黑)请选择要进行的处理,修复或导入数据库会中断与上位的连接
	GUIDE_DB_OUTPUT_NOFILE                              = 20801002,     //  (红)数据库文件不存在或已损坏，无法进行数据库导出处理
	GUIDE_DB_OUTPUTING                                  = 20801003,     //  (黑)数据库导出中，请稍候…… 
	GUIDE_DB_OUTPUT_SUCCESS                             = 20801004,     //  (黑)数据库导出正常结束
	GUIDE_DB_OUTPUT_FAIL                                = 20801005,     //  (红)数据库导出失败
	GUIDE_DB_REPAIRING                                  = 20801006,     //  (黑)数据库修复中，请稍候……
	GUIDE_DB_REPAIR_SUCCESS                             = 20801007,     //  (黑)数据库修复正常结束，请按确认键重新启动
	GUIDE_DB_REPAIR_FAIL                                = 20801008,     //  (红)数据库修复失败，请联系维修人员
	GUIDE_DB_INPUT_NOFILE                               = 20801009,     //  (红)数据库文件不存在或已损坏，无法进行数据库导入处理
	GUIDE_DB_INPUTING                                   = 20801010,     //  (黑)数据库导入中，请稍候……
	GUIDE_DB_INPUT_SUCCESS                              = 20801011,     //  (黑)数据库导入正常结束，请按确认键重新启动
	GUIDE_DB_INPUT_FAIL                                 = 20801012,     //  (红)数据库导入失败,请联系维修人员
	GUIDE_DB_OUTPUT_FIRST                               = 20801013,     // （红)主数据库文件尚未导出，不能进行修复或导入数据库处理 
	GUIDE_DB_REPAIR_SUCCESS_NO_RESTART                  = 20801014,     //  (黑)数据库修复成功，与上位连接处于断开状态
	GUIDE_DB_INPUT_SUCCESS_NO_RESTART                   = 20801015,     //  (黑)导入数据库成功，与上位连接处于断开状态
	GUIDE_DB_CF_CARD_ERROR                              = 20801016,     // （红)备份数据库处理失败，请检查CF卡
	//Ｆｉｇ２０９０１	
	GUIDE_MAINTENANCE_LOGOUT                            = 20901001 	    // （黑）正在登出，请稍候……

}MAINTENANCE_MSG;

typedef enum _passenger_msg{
	// 售票
	// Ｆｉｇ１０１０２
	GUIDE_BASE_CONFIRM                                  = 10102001,     // （黑）请确认以上所示信息
	GUIDE_BASE_CONFIRM_UL                               = 10102002,     // （黑）请确认以上所示信息    
	// Ｆｉｇ１０１０３	
	GUIDE_ISSUING_PASSENGER                             = 10103001,  	// （黑）车票发行中，请稍候……
	// Ｆｉｇ１０１０４	
	GUIDE_ISSUE_FINISH_PASSENGER                        = 10104001,  	// （黑）车票发行结束，谢谢光临
	GUIDE_ISSUE_ERROR_PASSENGER                         = 10104002,     // （黑）该车票不能进行发行处理
	GUIDE_ISSUE_CARD_NOTFOUND_PASSENGER                 = 10104003,     // （黑）车票发行结束

	// 补票
	// Ｆｉｇ１０２０２	
	GUIDE_ADJUST_CONFIRM_PASSENGER                      = 10202001, 	// （黑）请确认以上所示信息
	// Ｆｉｇ１０２０３	
	GUIDE_ADJUST_PAIDAREA_FINISH_PASSENGER              = 10203001, 	// （黑）补票处理结束，请从检票机出站，谢谢光临
	GUIDE_ADJUST_UNPAIDAREA_FINISH_PASSENGER            = 10203002, 	// （黑）补票处理结束，谢谢光临
	GUIDE_ADJUST_ERROR_PASSENGER                        = 10203003, 	// （黑）该车票不能进行补票处理
	GUIDE_ADJUST_CARD_NOTFOUND_PASSENGER                = 10203004,     // （黑）补票处理结束

	// 充值
	// Ｆｉｇ１０３０２	
	GUIDE_CHARGE_CONFIRM_PASSENGER                      = 10302001, 	 // （黑）请确认以上所示信息。
	// Ｆｉｇ１０３０３	
	GUIDE_CHARGE_FINISH_PASSENGER                       = 10303001, 	 // （黑）充值处理结束，谢谢光临
	GUIDE_CHARGE_NOT_PASSENGER                          = 10303002, 	 // （黑）该车票不能进行充值处理
	GUIDE_CHARGE_CARD_NOTFOUND_PASSENGER                = 10303003,      // （黑）充值处理结束
	
	// 替换
	// Ｆｉｇ１０５０２	
	GUIDE_EXCHANGE_CONFIRM_PASSENGER                    = 10502001,     // （黑）请确认以上所示信息
	// Ｆｉｇ１０５０３
	GUIDE_EXCHANGE_FINISH_PASSENGER                     = 10503001,     // （黑）替换处理结束，谢谢光临
	GUIDE_EXCHANGE_NOT_PASSENGER                        = 10503002,     // （黑）该车票不能进行替换处理
	GUIDE_EXCHANGE_CARD_NOREAD_PASSENGER                = 10503003,     // （黑）替换处理结束

	// 退款
	// Ｆｉｇ１０６０２	
	GUIDE_REFUND_CONFIRM_PASSENGER                      = 10602001,     // （黑）请确认以下所示信息
	// Ｆｉｇ１０６０３	
	GUIDE_REFUND_FINISH_PASSENGER                       = 10603001,     // （黑）退款处理结束，谢谢光临
	GUIDE_REFUNDING_PASSENGER							= 10603002,		// （黑）退款处理中，请稍候……
	GUIDE_REFUND_NOT_PASSENGER                          = 10603003,     // （黑）该车票不能进行退款处理
	GUIDE_REFUND_CARD_NOTFOUND_PASSENGER                = 10603004,     // （黑）退款处理结束
	GUIDE_REFUND_CPU_NOT_PASSENGER                      = 10603005,     // （黑）该车票不能进行即时退款处理
	GUIDE_REFUND_ERROR_PASSENGER						= 10603006,     // （黑）退款处理发生错误，退款失败

	// 激活
	// Ｆｉｇ１０７０２	
	GUIDE_REFRESH_CONFIRM_PASSENGER                     = 10702001,     // （黑）请确认以上所示信息
	// Ｆｉｇ１０７０３	
	GUIDE_REFRESH_FINISH_PASSENGER                      = 10703001, 	// （黑）激活处理结束，谢谢光临
	GUIDE_REFRESH_NOT_PASSENGER                         = 10703002, 	// （黑）该车票不能进行激活处理
	GUIDE_REFRESH_CARD_NOTFOUND_PASSENGER               = 10703003,     // （黑）激活处理结束

	// 分析
	// Ｆｉｇ１０８０２
	GUIDE_ANALYZE_CONFIRM_PASSENGER                     = 10802001, 	// （黑）请确认以上所示信息
	GUIDE_ANALYZE_FAILED_PASSENGER                      = 10802002,     // （黑）该车票不能进行分析处理
	GUIDE_ANALYZE_CARD_NOTFOUND_PASSENGER               = 10802003,     // （黑）分析处理结束
	// Ｆｉｇ１０８０３	
	GUIDE_ANALYZE_HISTORY_CONFIRM_PASSENGER             = 10803001,	    // （黑）请确认以上所示信息
	
	// 回退
	// Ｆｉｇ１１２０２	
	GUIDE_REVERSE_CONFIRM_PASSENGER                     = 11202001,	    // （黑）请确认以上所示信息
	// Ｆｉｇ１１２０３	
	GUIDE_REVERSE_FINISH_PASSENGER                      = 11203001, 	// （黑）回退处理结束，谢谢光临
	GUIDE_REVERSE_ERROR_PASSENGER                       = 11203002,     // （黑）该车票不能进行回退处理

	// 锁定
	// Ｆｉｇ１１３０２	
	GUIDE_BLOCK_CONFIRM_PASSENGER                       = 11302001, 	// （黑）请确认以上所示信息
	// Ｆｉｇ１１３０３	
	GUIDE_BLOCK_FINISH_PASSENGER                        = 11303001,	    // （黑）锁定处理结束，谢谢光临
	GUIDE_BLOCK_ERROR_PASSENGER                         = 11303002,     // （黑）该车票不能进行锁定处理
	GUIDE_BLOCK_CARD_NOTFOUND_PASSENGER                 = 11303003,     // （黑）锁定处理结束

	// 产品删除
	// Ｆｉｇ１１５０２	
	GUIDE_PRODUCT_DELETE_CONFIRM_PASSENGER              = 11502001, 	// （黑）请确认以上所示信息
	// Ｆｉｇ１１５０３	
	GUIDE_PRODUCT_DELETE_FINISH_PASSENGER               = 11503001, 	// （黑）产品删除处理结束，谢谢光临
	GUIDE_PRODUCT_DELETE_NOT_PASSENGER                  = 11503002,     // （黑）该车票不能进行删除处理

	// TVM故障退款
	// Ｆｉｇ１１６０１	
	GUIDE_TVMFAILURE_CONFIRM_PASSENGER                  = 11601001, 	// （黑）请确认以上所示信息
	// Ｆｉｇ１１６０２	
	GUIDE_TVMFAILURE_REFUND_FINISH_PASSENGER            = 11602001,      // （黑）退款处理结束，请在收据上签名，谢谢光临

	// 激活
	GUIDE_DEFER_CONFIRM_PASSENGER                     = 11702001,     // （黑）请确认以上所示信息
	GUIDE_DEFER_FINISH_PASSENGER                      = 11703001, 	// （黑）激活处理结束，谢谢光临
	GUIDE_DEFER_NOT_PASSENGER                         = 11703002, 	// （黑）该车票不能进行激活处理
	GUIDE_DEFER_CARD_NOTFOUND_PASSENGER               = 11703003,     // （黑）激活处理结束

	GUIDE_CANCEL             						  = 4000000,//（黑）操作已取消
	GUIDE_BLACK	            						  = 4000001,//（红）黑名单卡，已锁定！

}PASSENGER_MSG;

typedef enum _operation_msg{
	// 空白文言
	GUIDE_NULL                                          = 1000,       
	// 初期启动
	// Ｆｉｇ００００１
	GUIDE_INIT_WAIT                                     = 1001,         // （黑）正在初始化系统，请稍候…… 
	GUIDE_INIT_NEW_PARAM                                = 1002,         // （黑）系统参数已更新，重启动后生效
	GUIDE_INIT_ERROR                                    = 1003,         // （红）初期启动失败，关机后重新启动或与维修人员联系
	GUIDE_INIT_ACCREDIT_ERROR                           = 1004,         // （红）初期化认证失败
	// 操作员登录
	// Ｆｉｇ００００２ 
	GUIDE_LOGIN_INPUT_ID_AND_PWD                        = 2001,         // （黑）请输入操作员ID和密码并选择登录业务
	GUIDE_LOGIN_LOGINNING	                            = 2002,         // （黑）正在登录，请稍候……
	GUIDE_LOGIN_PARAM_SYNING                            = 2004,         // （黑）正在进行参数同步，请稍候……
	GUIDE_LOGIN_UNAUTHORIZED                            = 2003,         // （红）该操作员没有取得授权，请核对后重新输入
	GUIDE_LOGIN_PWD_FAIL                                = 2005,         // （红）密码输入错误，请核对后重新输入
	GUIDE_LOGIN_FAIL                                    = 2006,         // （红）ＸＸＸＸ异常，请重启或登录维护处理
	GUIDE_LOGIN_MAINTAIN_FAIL                           = 2007,         // （红）ＸＸＸＸ异常，关机后重新启动
	GUIDE_LOGIN_REST                                    = 2008,         // （黑）休息中
	GUIDE_LOGIN_EMERGENCY                               = 2009,         // （红）当前模式不能登录，请稍候再登录
	GUIDE_LOGIN_ALREADY_LOGIN                           = 2010,         // （红）当前操作员已在其他设备登录
	GUIDE_LOGIN_PWD_FAIL_MAX                            = 2011,         // （红）输入密码连续错误次数已经超过规定次数
	GUIDE_LOGIN_CLOSE                                   = 2012,         // （黑）正在进行业务结束处理，请稍候……
	GUIDE_LOGIN_SHUTDOWN                                = 2013,         // （黑）正在关机，请稍候……
	GUIDE_LOGIN_UPDATE_HARDWARE                         = 2014,         // （黑）程序更新中，请稍候……
	GUIDE_LOGIN_MACHINE_ERROR                           = 2015,         // （红）设备初始化异常，关机后重新启动
	GUIDE_LOGIN_UPDATE_SUCCESS                          = 2016,         // （黑）程序更新结束
	GUIDE_LOGIN_UPDATE_FAIL                             = 2017,         // （红）ＸＸＸＸ程序更新失败
	GUIDE_LOGIN_REDIT_FAIL                              = 2018,         // （红）认证失败（ＸＸＸＸ）
	GUIDE_LOGIN_REDIT_SUCCESS                           = 2019,         // （黑）认证成功
	GUIDE_LOGIN_REDITING                                = 2020,         // （黑）正在认证，请稍候……
	GUIDE_LOGIN_DEVICEREGISTER_FAIL                     = 2021,         // （红）机具领用失败
	GUIDE_LOGIN_DEVICEEXCHANGE_FAIL                     = 2022,         // （红）机具变更失败
	GUIDE_LOGIN_NO_SAM                                  = 2023,         // （红）SAM卡不存在，不能进行认证处理
	GUIDE_LOGIN_UPDATE_PARAMETER                        = 2024,         // （红）参数更新中，请稍候再登录
	GUIDE_LOGIN_ACC_UPDATE_FAIL                         = 2025,         // （红）ＡＣＣ参数更新失败
	GUIDE_LOGIN_SC_NO_RESPONSE							=  2026,		 // （红）网络忙，请重试
	GUIDE_LOGIN_EXITBOM									= 2027,			// （黑）正在退出程序，请稍候
	
	GUIDE_LOGIN_UNKNOW_RESPONSE							=  2031,		 // （红）未知错误
	GUIDE_LOGIN_REQUEST_SERVER_FAILED					=  2032,		 // （红）请求服务失败
	GUIDE_LOGIN_HAS_EXCEPTION 							=  2033,		 // （红）异常中，禁止登陆
	GUIDE_LOGIN_CANNOT_USE_SUPEROPERATOR				=  2034,		 // （红）超级用户权限没有放开。
	GUIDE_LOGIN_OK										=  2035,		 // （黑）允许登录
	GUIDE_LOGIN_ID_PASSWORD_NOT_EXITS					=  2036,		 // （红）用户名不存在或密码错误
	GUIDE_LOGIN_REPEAT_LOGIN							=  2037,		 // （红）拒绝登录（重复登录）
	GUIDE_LOGIN_OK_PASSWORD_WILL_EXPIRED				=  2038,		 // （黑）允许登录（密码即将过期）
	GUIDE_LOGIN_OK_CHANGE_PASSWORD						=  2039,		 // （黑）允许登录（强制修改密码）
	GUIDE_LOGOUT_OK										=  2040,		 // （黑）允许登出
	GUIDE_LOGOUT_REFUSE									=  2041,		 // （红）拒绝登出
	GUIDE_LOGIN_ID_LOCKED								=  2042,		 // （红）拒绝登录（账户锁定）
	GUIDE_LOGIN_PASSWORD_STOPED							=  2043,		 // （红）拒绝登录（密码终止）
	GUIDE_LOGIN_ID_STOPED								=  2044,		 // （红）拒绝登录（账户信用）
	GUIDE_LOGIN_ID_PASSWORD_EMPTY						=  2045,		 // （红）用户名或密码为空
	// 售票
	// Ｆｉｇ００１０１
	GUIDE_ISSUE_NEW_CARD_SELET_INFO                     = 101001,       // （黑）请选择需要购买的车票信息                       
	// Ｆｉｇ００１０２
	GUIDE_ISSUE_OLD_CARD_SELET_INFO                     = 102001,       // （黑）请选择需要购买的车票信息
	// Ｆｉｇ００１０３
	GUIDE_ISSUE_PUT_CARD                                = 103001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	GUIDE_ISSUE_PRINT                                   = 103002, 	    // （黑）如需凭证，请输入收款金额后按打印键
	GUIDE_ISSUE_INPUT_CASH_ERROR                        = 103003,       // （红）输入的内容有错误，请核对后重新输入
	GUIDE_ISSUE_ECT_MONEY_NOT_ENOUGH                    = 103004, 	    // （红）首次充值金额不足，请核对后重新输入
	GUIDE_ISSUE_ECT_MONEY_NOT_BASE_AMOUNT               = 103005, 	    // （红）充值金额非法，请输入整数倍的充值金额
	GUIDE_ISSUE_ECT_MONEY_OVER_MAX                      = 103006,       // （红）充值金额超过最大限额，请核对后重新输入
	GUIDE_ISSUE_ECT_BALANCE_OVER_MAX                    = 103007, 	    // （红）充值后金额将超过最大允许卡内余额，请核对后重新输入
	GUIDE_ISSUE_ECT_AUTHORITY_MONEY_NOT_ENOUGH          = 103008,       // （红）授权余额不足，请核对后重新输入或进行签到签退
	GUIDE_ISSUE_CONFIRM_INFO                            = 103009,       // （黑）请确认以下所示信息
	// Ｆｉｇ００１０４
	GUIDE_ISSUING                                       = 104001,       // （黑）车票发行中，请稍候……
	GUIDE_WRITE_CARD_SUCCESS                            = 104002,       // （黑）车票写入成功，请稍候……
	// Ｆｉｇ００１０５
	GUIDE_ISSUE_FINISH                                  = 105001,       // （黑）车票发行正常结束
	GUIDE_ISSUE_ERROR                                   = 105002, 	    // （红）ＸＸＸＸＸＸ，不能进行发行处理
	GUIDE_ISSUE_MAGAZINE_NOT_ENOUGH                     = 105003,       // （红）出票机票箱剩余张数不足，不能发行指定张数的车票
	GUIDE_ISSUE_COLBOX_FULL                             = 105004,       // （红）出票机废票箱已满，不能进行发行处理
	GUIDE_ISSUE_CARD_NOT_FOUND                          = 105005,       // （黑）没有可以处理的车票，发行处理结束
	// Ｆｉｇ００１０６
	GUIDE_ISSUE_SELET_PRODUCT                           = 106001,    	// （黑）请选择要购买的衍生产品
	// Ｆｉｇ００１０７
	GUIDE_ISSUE_SELECT_START                            = 107001,    	// （黑）请选择起点
	GUIDE_ISSUE_SELECT_END                              = 107002,    	// （黑）请选择终点
	GUIDE_ISSUE_PRICE_ERROR								= 107003,       // （红）票价非法，请重新输入
	// Ｆｉｇ００１０８
	GUIDE_ISSUE_INPUT_COUNT                             = 108001,       // （黑）请输入要购买的张数
	GUIDE_ISSUE_INPUT_COUNT_ERROR                       = 108002,       // （红）输入的内容有错误，请核对后重新输入
	// Ｆｉｇ００１０９
	GUIDE_ISSUE_INPUT_DATE                              = 109001, 	    // （黑）请输入年月日
	GUIDE_ISSUE_INPUT_DATE_ERROR                        = 109002, 	    // （红）输入的内容有错误，请核对后重新输入

	// 补票
	// Ｆｉｇ００２０１
	GUIDE_ADJUST_PUT_CARD                               = 201001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ００２０２
	GUIDE_ADJUST_SELECT_ADJUST_TYPE                     = 202001,       // （黑）请选择补票的类型
	GUIDE_ADJUST_PAYMATHOD_ERROR                        = 202002,       // （红）该车票只能进行现金补票
	//GUIDE_ADJUST_PEAKEXIT_STATION_LEAVE                 = 202003,    	// （红）该车票可能在出站免检模式下出站，如需补票请继续操作
	//GUIDE_ADJUST_PEAKENTRY_STATION_ENTER                = 202004,    	// （红）该车票可能在进站免检模式下进站，如需补票请继续操作
	GUIDE_ADJUST_NOT_NEED_INSIDE                        = 202005,       // （黑）该车票还未进站，请发行出站
	GUIDE_ADJUST_NOT_NEED_OUTSIDE                       = 202006,       // （黑）该车票旅程状态正常，无需进行补票处理
	GUIDE_ADJUST_CODE_ERROR						        = 202007,       // （红）补票建议代码出错

	// Ｆｉｇ００２０３
	GUIDE_ADJUST_PRINT                                  = 203001, 	    // （黑）请输入以下相关信息
	GUIDE_ADJUST_CONFORM                                = 203003, 	    // （黑）请确认以下相关信息
	GUIDE_ADJUST_INPUT_ERROR                            = 203002,    	// （红）输入的内容有错误，请核对后重新输入
	// Ｆｉｇ００２０４    
	GUIDE_ADJUST_WAITING                                = 204001, 	    // （黑）正在进行补票处理，请稍候……
	GUIDE_ADJUST_END                                    = 204002,       // （黑）补票处理正常结束
	GUIDE_ADJUST_ERROR                                  = 204003, 	    // （红）ＸＸＸＸＸＸ，不能进行补票处理
	GUIDE_ADJUST_NOT                                    = 204004,    	// （红）该车票不能进行补票处理
	GUIDE_ADJUST_MAGAZINE_NOT_ENOUGH                    = 204005, 	    // （红）出票机票箱剩余张数不足，不能发行指定张数的车票
	GUIDE_ADJUST_CARD_NOT_FOUND                         = 204006,       // （黑）没有可以处理的车票，补票处理结束
	GUIDE_ADJUST_WRITE_PURSE_END                        = 204007,       // （黑）写卡时移卡但钱包写入成功，补票处理正常结束

	// Ｆｉｇ００２０５
	GUIDE_ADJUST_SELECT_ENTER_STATION                   = 205001,    	// （黑）请选择进站线路和车站
	GUIDE_ADJUST_SELECT_LEAVE_STATION	                = 205002, 	    // （黑）请选择出站线路和车站
	//GUIDE_ADJUST_SELECT_OVERRIDE                        = 205003,       // （红）车站超程，请重新选择车站或发出站票
	GUIDE_ADJUST_SELECT_FARE_ERR                        = 205004,       // （红）票价无效，请选择其他车站或发行出站票
	GUIDE_ADJUST_REPAIR_NO_FARE_INSIDE				    = 205005,		// （红）已修复无法取得有效票价的车票，请发行出站票出站
	GUIDE_ADJUST_REPAIR_OVERRIDE_INSIDE                 = 205006,		// （红）已修复超程的车票，请发行出站票出站
	GUIDE_ADJUST_STAFF_INSIDE							= 205007,		// （黑）车票已修复，请发行出站票出站

	// Ｆｉｇ００２０６
	GUIDE_ADJUST_SELECT_STATION_MODE                    =  206001, 	    // （黑）请选择发生过的系统运营模式
	GUIDE_ADJUST_STATION_MODE_MISMATCH                  =  206002,      // （红）选择的系统运营模式不能进行组合
	// 充值
	// Ｆｉｇ００３０１
	GUIDE_CHARGE_PUT_CARD                               = 301001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ００３０２
	GUIDE_CHARGE_SVTICKET                               = 302001, 	    // （黑）请输入充值金额，并按确认键
	GUIDE_CHARGE_PRINT                                  = 302002, 	    // （黑）如需凭证，请输入收款金额后按打印键
	GUIDE_CHARGE_INPUT_ERROR                            = 302003, 	    // （红）输入的内容有错误，请核对后重新输入
	GUIDE_CHARGE_ECT_MONEY_NOT_BASE_AMOUNT              = 302004, 	    // （红）充值金额非法，请输入整数倍的充值金额
	GUIDE_CHARGE_ECT_MONEY_OVER_MAX                     = 302005, 	    // （红）充值金额超过最大限额，请核对后重新输入
	GUIDE_CHARGE_ECT_BALANCE_OVER_MAX                   = 302006, 	    // （红）充值后金额将超过最大允许卡内余额，请核对后重新输入
	GUIDE_CHARGE_ECT_AUTHORITY_MONEY_NOT_ENOUGH         = 302007, 	    // （红）授权余额不足，请核对后重新输入或进行签到签退
	GUIDE_CHARGE_NOT_SAME_CARD_PASSENGER                = 302008,       // （红）卡片加值操作失败，请使用同一张卡
	GUIDE_CHARGE_CHARGE_ABNORMAL_PASSENGER              = 302009,       // （红）可疑记录转为收入
	// Ｆｉｇ００３０３
	GUIDE_CHARGE_WAIT                                   = 303001, 	    // （黑）正在进行充值处理，请稍候……
	GUIDE_CHARGE_FINISH                                 = 303002, 	    // （黑）充值处理正常结束
	GUIDE_CHARGE_ERROR                                  = 303003, 	    // （红）ＸＸＸＸＸＸ，不能进行充值处理
	GUIDE_CHARGE_NOT                                    = 303004,    	// （红）该车票不能进行充值处理
	GUIDE_CHARGE_CARD_NOT_FOUND                         = 303005, 	    // （黑）没有可以处理的车票，充值处理结束
	GUIDE_TIME_LIMIT_CHARGE_NOT                         = 303006, 	    // （红）已超过一卡通授权截止时间，不能进行充值处理
	GUIDE_CHARGE_WRITE_PURSE_FINISH	                    = 303007, 	    // （黑）写卡时移卡但钱包写入成功，充值处理正常结束
	GUIDE_CHARGE_BLACK_CARD							= 303008,					//（红）黑名单卡，已锁定
	// 替换
	// Ｆｉｇ００５０１
	GUIDE_EXCHANGE_PUT_CARD                             = 501001,       // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ００５０２
	GUIDE_EXCHANGE_CONFIRM                              = 502001,       // （黑）请确认车票信息，按确认键前请勿移走旧车票
	GUIDE_EXCHANGE_BACKUP                               = 502002,       // （红）上次替换处理没有结束，继续处理请按确认键，取消处理请按返回键 
	// Ｆｉｇ００５０３
	GUIDE_EXCHANGE_PUT_NEW_CARD                         = 503001,       // （黑）请将新车票放置在外部IC读写器的感应区内
	GUIDE_EXCHANGE_PUT_OTHER_CARD                       = 503002,       // （红）ＸＸＸＸＸＸ，请另外放置车票在外部IC读写器的感应区内
	GUIDE_EXCHANGING                                    = 503003,       // （黑）正在进行替换处理，请稍候……
	// Ｆｉｇ００５０４
	GUIDE_EXCHANGE_FINISH                               = 504001,       // （黑）替换处理正常结束
	GUIDE_EXCHANGE_NOT                                  = 504003,       // （红）该车票不能进行替换处理
	GUIDE_EXCHANGE_ERROR                                = 504002,       // （红）ＸＸＸＸＸＸ，不能进行替换处理
	GUIDE_EXCHANGE_CARD_NOREAD                          = 504004,       // （黑）没有可以处理的车票，替换处理结束

	// 退款
	// Ｆｉｇ００６０１
	GUIDE_REFUND_PUT_CARD                               = 601001,       // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ００６０２
	GUIDE_REFUND_INPUT_SERIAL_NUMBER                    = 602001, 	    // （黑）请输入车票编号
	GUIDE_REFUND_CONNECTING                             = 602002,       // （黑）上位通信处理中，请稍候……
	GUIDE_REFUND_INPUT_ERROR                            = 602003,       // （红）输入的内容有错误，请核对后重新输入
	// Ｆｉｇ００６０３
	GUIDE_REFUND_SELECT_MTD_CONFIRM                     = 603001,       // （黑）确认无误后请按确认键
	GUIDE_REFUND_APPLING_ECT							= 603002,       // （黑）正在进行一卡通延期退款申请处理,请稍候……
	// Ｆｉｇ００６０４
	GUIDE_REFUNDING                                     = 604001,       // （黑）正在进行退款处理，请稍候……
	GUIDE_REFUND_FINISH									= 604002,       // （黑）退款处理正常结束
	GUIDE_REFUND_ERROR                                  = 604004,       // （红）ＸＸＸＸＸＸ，不能进行退款处理
	GUIDE_REFUND_NOT                                    = 604005,       // （红）该车票不能进行退款处理
	GUIDE_REFUND_CARD_NOT_FOUND                         = 604006,       // （黑）没有可以处理的车票，退款处理结束
	GUIDE_REFUND_WRITE_PURSE_FINISH						= 604007,       // （黑）写卡时移卡但钱包写入成功，退款处理正常结束
	GUIDE_REFUND_CPU_NOT                                = 604008,       // （红）该车票不能进行即时退款处理
	GUIDE_REFUNDING_APPLY                               = 604009,       // （黑）正在进行退款申请，请稍候……
	GUIDE_REFUND_APPLY_NOT								= 604010,       // （红）该车票不能进行退款申请
	GUIDE_REFUND_APPLY_ERROR                            = 604011,       // （红）退款申请发生错误，不能进行退款处理
	GUIDE_REFUNDING_QUERY                               = 604012,       // （黑）正在进行退款查询，请稍候……
	GUIDE_REFUND_QUERY_NOT								= 604013,       // （红）该车票不能进行退款查询
	GUIDE_REFUND_QUERY_ERROR                            = 604014,       // （红）退款查询发生错误，不能进行退款处理

	// Ｆｉｇ００６０５
	GUIDE_REFUND_INPUT_CENTER_SERIAL_NUMBER             = 605001, 	    // （黑）请输入中心流水号
	GUIDE_REFUND_CONNECTING_ECT							= 605002,		// （黑）正在进行补票处理，请稍候……
	GUIDE_REFUND_INPUT_ERROR_ECT                        = 605003,       // （红）输入的内容有错误，请核对后重新输入

	// 激活
	// Ｆｉｇ００７０１
	GUIDE_REFRESH_PUT_CARD                              = 701001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ００７０２
	GUIDE_REFRESH_CONFIRM_PRESS_OKBTN                   = 702001,    	// （黑）车票信息确认无误后，按确认键进行激活
	GUIDE_REFRESH_INPUT_ERROR                           = 702002, 	    // （红）输入的内容有错误，请核对后重新输入
	GUIDE_REFRESH_ENTER_DATE                            = 702003, 	    // （黑）请输入车票的新有效期
	// Ｆｉｇ００７０３
	GUIDE_REFRESH_WAIT                                  = 703001, 	    // （黑）正在进行激活处理，请稍候……
	GUIDE_REFRESH_FINISH                                = 703002,    	// （黑）激活处理正常结束
	GUIDE_REFRESH_ERROR                                 = 703003, 	    // （红）ＸＸＸＸＸＸ，不能进行激活处理
	GUIDE_REFRESH_NOT                                   = 703004, 	    // （红）该车票不能进行激活处理
	GUIDE_REFRESH_IN_PERIOD_OF_VALIDITY                 = 703005, 	    // （红）车票还在有效期内，无需进行激活处理
	GUIDE_REFRESH_CARD_NOT_FOUND                        = 703006,       // （黑）没有可以处理的车票，激活处理结束

	// 分析
	// Ｆｉｇ００８０１	
	GUIDE_ANALYZE_PUT_CARD                              = 801001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ００８０２	
	GUIDE_ANALYZE_SELET_OPERATE                         = 802001,    	// （黑）请确认车票信息
	GUIDE_ANALYZE_ERROR                                 = 802002, 	    // （红）ＸＸＸＸＸＸ，不能进行分析处理
	GUIDE_ANALYZE_CARD_NOT_FOUND                        = 802003,       // （黑）没有可以处理的车票，分析处理结束

	// Ｆｉｇ００８０３	
	GUIDE_ANALYZE_PRINT_HISTORY                         = 803001, 	    // （黑）请确认以下所示信息
	// Ｆｉｇ００８０４	
	GUIDE_ANALYZE_COMMUNICATING                         = 804001, 	    // （黑）上位通信处理中，请稍候……
	GUIDE_ANALYZE_PRINT                                 = 804002,    	// （黑）请确认以下所示信息
	GUIDE_ANALYZE_SC_NOTFOUND                           = 804003,       // （黑）该车票为无效车票，无法查询所需信息
	GUIDE_ANALYZE_SC_FAILED                             = 804004,       // （红）上位通信处理失败，无法查询所需信息
	GUIDE_ANALYZE_BLACKLIST_CARD_BLOCKED                = 804005,       // （红）黑名单票卡，已锁定


	// 其他业务
	// Ｆｉｇ００９０１	
	GUIDE_OTHERS_CHOOSE_SERVICE                         = 901001,    	// （黑）请选择业务种类
	GUIDE_OTHERS_FAIL                                   = 901002,    	// （红）ＸＸＸＸ异常，请按确认键或与维修人员联系

	// 挂失
	// Ｆｉｇ０１００１	
	GUIDE_REPORTLOSS_INPUT_SERIAL_NUMBER                = 1001001, 	    // （黑）请输入车票编号
	GUIDE_REPORTLOSS_INPUT_ERROR                        = 1001002,   	// （红）输入的内容有错误，请核对后重新输入
	// Ｆｉｇ０１００２	
	GUIDE_REPORTLOSS_CONFIRM                            = 1002001, 	    // （黑）车票信息确认无误后，请按确认键挂失
	// Ｆｉｇ０１００３	
	GUIDE_REPORTLOSS_FINISH                             = 1003001, 	    // （黑）挂失处理正常结束

	// 抵消
	// Ｆｉｇ０１１０１	
	GUIDE_CANCEL_PUT_CARD                               = 1101001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ０１１０２	
	GUIDE_CANCEL_CONFIRM                                = 1102001, 	    // （黑）车票信息确认无误后，请按确认键抵消
	GUIDE_CANCEL_ERROR                                  = 1102002, 	    // （红）ＸＸＸＸＸＸ，不能进行抵消处理
	GUIDE_CANCEL_NOT                                    = 1102003, 	    // （红）该车票不能进行抵消处理
	GUIDE_CANCEL_CARD_NOT_FOUND                         = 1102004,      // （黑）没有可以处理的车票，抵消处理结束
	GUIDE_CANCEL_NOT_PRE_SELL									= 1102005,	//（红）该车票不是预售票
	// 回退
	// Ｆｉｇ０１２０１	
	GUIDE_REVERSE_SELECT_SORT_AND_QUOMODO               = 1201001, 	    // （黑）请选择业务种类和处理方式
	// Ｆｉｇ０１２０２	
	GUIDE_REVERSE_PUT_CARD                              = 1202001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ０１２０３	
	GUIDE_REVERSE_CONFIRM                               = 1203001, 	    // （黑）车票信息确认无误后，请按确认键进行回退处理
	// Ｆｉｇ０１２０４	
	GUIDE_REVERSE_FINISH                                = 1204001, 	    // （黑）回退处理正常结束
	GUIDE_REVERSE_ERROR                                 = 1204002, 	    // （红）ＸＸＸＸＸＸ，不能进行回退处理
	GUIDE_REVERSE_NOT                                   = 1204003, 	    // （红）该车票不能进行回退处理

	// 锁定
	// Ｆｉｇ０１３０１	
	GUIDE_BLOCK_CHOOSE_SORT                             = 1301001,   	// （黑）请选择业务种类
	// Ｆｉｇ０１３０２	
	GUIDE_BLOCK_PUT_CARD                                = 1302001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ０１３０３	
	GUIDE_BLOCK_SELECT_REASON                           = 1303001,  	// （黑）请选择锁定的原因
	GUIDE_BLOCK_SELECT_PRODUCT_AND_SEASON               = 1303002,      // （黑）请选择要锁定的产品和原因
	// Ｆｉｇ０１３０４	
	GUIDE_BLOCK_FINISH                                  = 1304001, 	    // （黑）锁定处理正常结束
	GUIDE_BLOCK_ERROR                                   = 1304002, 	    // （红）ＸＸＸＸＸＸ，不能进行锁定处理
	GUIDE_BLOCK_CARD_NOT_FOUND                          = 1304003,      // （黑）没有可以处理的车票，锁定处理结束

	// 退卡
	// Ｆｉｇ０１４０１	
	GUIDE_SURRENDER_PUT_CARD                            = 1401001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ０１４０２	
	GUIDE_SURRENDER_CONFIRM                             = 1402002, 	    // （黑）车票信息确认无误后，请按确认键进行退卡处理
	// Ｆｉｇ０１４０３	
	GUIDE_SURRENDER_FINISH                              = 1403001, 	    // （黑）退卡处理正常结束
	GUIDE_SURRENDER_ERROR                               = 1403002,  	// （红）ＸＸＸＸＸＸ，不能进行退卡处理

	// 产品删除
	// Ｆｉｇ０１５０１	
	GUIDE_PRODUCT_DELETE_PUT_CARD                       = 1501001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ０１５０２	
	GUIDE_PRODUCT_DELETE_CONFIRM                        = 1502001, 	    // （黑）车票信息确认无误后，请按确认键进行产品删除处理
	// Ｆｉｇ０１５０３	
	GUIDE_PRODUCT_DELETE_FINISH                         = 1503001,   	// （黑）产品删除处理正常结束
	GUIDE_PRODUCT_DELETE_ERROR                          = 1503002, 	    // （红）ＸＸＸＸＸＸ，不能进行产品删除处理
	GUIDE_PRODUCT_DELETE_NOT                            = 1503003, 	    // （红）该车票不能进行产品删除处理

	// TVM故障退款
	GUIDE_TVMFAILURE_CONFIRM                            = 1601001, 	    // （黑）输入信息确认无误后，请按确认键
	GUIDE_TVMFAILURE_INPUT_ERROR                        = 1601002, 	    // （红）输入的内容有错误，请核对后重新输入
	GUIDE_TVMFAILURE_PLEASE_INPUT_DEVICE_NO				= 1601003,		// （红）请输入设备编号
	GUIDE_TVMFAILURE_PLEASE_INPUT_TICKET_NO				= 1601004,		// （红）请输入退款凭条号
	GUIDE_TVMFAILURE_PLEASE_INPUT_MONEY					= 1601005,		// （红）请输入退款金额	
	GUIDE_TICKET_MUST_BE_NUMBER							= 1601006,		// （红）退款凭条号必须为10位数字
	GUIDE_DEVICE_MUST_BE_NUMBER							= 1601007,		// （红）设备编号必须为8位数字
	GUIDE_TVMFAILURE_ERROR								= 1601008,		// （红）已退款，不能重复退款
	GUIDE_TVMFAILURE_FINISH                             = 1602001, 	    // （黑）TVM故障退款处理成功
	GUIDE_TVM_FAILURE_REFUND_FAIL						= 1602002,		// （红）TVM故障退款处理失败

	// 销售统计
	// Ｆｉｇ０１７０１	
	GUIDE_AUDIT_PRINT                                   = 1701001, 	    // （黑）请确认销售统计信息
	GUIDE_AUDIT_FINISH                                  = 1701002, 	    // （黑）销售统计处理正常结束

	// 时间设定
	// Ｆｉｇ０１８０１	
	GUIDE_TIMESET_INPUT_AND_CONFIRM                     = 1801001, 	    // （黑）请分别输入年月日时分，并按确认键
	GUIDE_TIMESET_INPUT_ERROR                           = 1801002, 	    // （红）输入的内容有错误，请核对后重新输入
	GUIDE_TIMESET_CONFIRM_OR_BACK                       = 1801003, 	    // （黑）信息确认后，请按确认键，如需修改，请按返回键
	GUIDE_TIMESET_FINISH                                = 1801004, 	    // （黑）时间设定成功

	// 修改密码
	// Ｆｉｇ０１９０１	
	GUIDE_MODIFY_PWD_PUT_CARD                           = 1901001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	// Ｆｉｇ０１９０２	
	GUIDE_MODIFY_PWD_CONFIRM                            = 1902001, 	    // （黑）请输入画面所示的各项信息，确认无误后，按确认键进行修改
	GUIDE_MODIFY_PWD_READ_ERROR                         = 1902002, 	    // （红）该操作员没有取得授权，请核对后请重新输入
	GUIDE_MODIFY_PWD_INPUT_ERROR                        = 1902003, 	    // （红）输入的内容有错误，请核对后重新输入
	GUIDE_MODIFY_PWD_OLD_PASSWORD_ERROR                 = 1902004, 	    // （红）旧密码输入错误，请核对后重新输入
	GUIDE_MODIFY_PWD_NEW_PASSWORD_ERROR                 = 1902005, 	    // （红）新密码两次输入不一致，请核对后重新输入
	GUIDE_MODIFY_PWD_PASSWORD_SAME                      = 1902006, 	    // （红）新密码与旧密码相同，请核对后重新输入
	GUIDE_MODIFY_PWD_PASSWORD_OVER_TIMES                = 1902007, 	    // （红）输入密码连续错误次数已经超过规定次数
	GUIDE_MODIFY_PWD_FINISH                             = 1902008, 	    // （黑）修改密码处理正常结束
	//GUIDE_MODIFY_PWD_NOT                                = 1902009, 	    // （红）该车票不能进行修改密码处理

	// 票价表
	// Ｆｉｇ０２００１	
	GUIDE_FARE_TABLE_PRINT                              = 2001001, 	    // （黑）请确认票价表信息，如需打印，请按打印键
	GUIDE_FARE_TABLE_FINISH                             = 2001002, 	    // （黑）票价表信息确认处理正常结束

	// 模式切换
	// Ｆｉｇ０２１０１	
	GUIDE_OPERATE_SWITCH_SELECT                          = 2101001,   	// （黑）请选择业务模式种类，并按确认键进行业务模式切换
	GUIDE_OPERATE_SWITCH_CONFIRM                         = 2101002, 	// （黑）信息确认后，请按确认键，如需修改，请按返回键
	GUIDE_OPERATE_SWITCH_FINISH                          = 2101003, 	// （黑）模式切换处理正常结束

	// 票箱更换
	// Ｆｉｇ０２２０１	
	GUIDE_TICKETBOX_CHANGE_START                        = 2201001, 	    // （黑）请按更换开始键及确认键后准备更换票箱
	GUIDE_TICKETBOX_CHANGE_A_ERROR                      = 2201002, 	    // （红）票箱A信息异常
	GUIDE_TICKETBOX_CHANGE_B_ERROR                      = 2201003, 	    // （红）票箱B信息异常
	// Ｆｉｇ０２２０２	
	GUIDE_TICKETBOX_CHANGE_FINISH                       = 2202001, 	    // （黑）请更换票箱，更换结束后请按更换结束键及确认键
	// Ｆｉｇ０２２０３	
	GUIDE_TICKETBOX_BUSIMESS_FINISH                     = 2203001, 	    // （黑）票箱更换处理正常结束
	GUIDE_TICKETBOX_BUSIMESS_A_ERROR                    = 2203002, 	    // （红）票箱A信息异常
	GUIDE_TICKETBOX_BUSIMESS_B_ERROR                    = 2203003, 	    // （红）票箱B信息异常
	GUIDE_TICKETBOX_CHANGE_A_B_ERROR                    = 2203004, 	    // （红）票箱A、B信息异常

	// Ｆｉｇ０２３０１	
	GUIDE_OTHERS_REST                                   = 2301001, 	    // （黑）休息中

	// Ｆｉｇ０２４０１	
	GUIDE_OTHERS_LOGOUT                                 = 2401001, 	    // （黑）正在登出，请稍候……

	// 前回帐票打印
	GUIDE_REPRINT_CONFIRM								= 2501001, 	    // （黑）请确认前回登录信息，如需打印，请按打印键
	GUIDE_REPRINT_FINISH								= 2501002, 	    // （黑）前回帐票打印处理正常结束

	// 充值丢失电文
	GUIDE_CHARGE_LOST_CONFIRM							= 2601001, 	    // （黑）请确认丢失电文信息，选择需集计的电文后按确认键

	// 非营业性收支/行政处理
	GUIDE_NON_BUISNESS_SELECT							= 2701001,		// （黑）请选择行政处理类别后按确认键
	GUIDE_NON_BUISNESS_SUCCEED							= 2701002,		// （黑）行政处理业务处理成功
	GUIDE_NON_BUISNESS_FAIL								= 2701003,		// （红）行政处理业务处理失败
	GUIDE_NON_BUISNESS_INPUT_NULL						= 2701004,		// （红）请输入收支金额
	GUIDE_NON_BUISNESS_INPUT_ERROR						= 2701005,		// （红）输入的内容有错误，请核对后重新输入

	// 记名卡申请
	GUIDE_APPLY_SELECT									= 2801001,		// （黑）请输入申请人信息后按确认键
	GUIDE_APPLY_SUCCEED									= 2801002,		// （黑）记名卡申请业务处理成功
	GUIDE_APPLY_FAIL									= 2801003,		// （红）记名卡申请状态查询失败
	GUIDE_APPLY_CHECKING								= 2801004,		// （黑）正在查询，请稍后……
	GUIDE_APPLY_INPUT_ERROR					        	= 2801005,		// （红）输入的内容有错误，请核对后重新输入
	GUIDE_APPLY_UPDATA_SUCCEED							= 2801006,		// （黑）记名卡持卡人信息修改处理正常结束
	GUIDE_APPLY_CHECK_SUCCEED							= 2801007,		// （黑）记名卡申请状态查询成功
	GUIDE_APPLY_CARDINFO_ERROR							= 2801008,		// （红）该车票不能进行记名卡业务


	//挂失解挂
	GUIDE_SUSPEND_AND_RESUME_SELECT						= 2901001,		// （黑）请输入持卡人信息后按确认键
	GUIDE_SUSPEND_SUCCESS								= 2901002,		// （黑）挂失业务处理成功
	GUIDE_SUSPEND_FAIL									= 2901003,		// （红）挂失业务处理失败
	GUIDE_RESUME_SUCCESS								= 2901004,		// （黑）解挂业务处理成功
	GUIDE_RESUME_FAIL									= 2901005,		// （红）解挂业务处理失败
	//延期
	GUIDE_DEFER_PUT_CARD                              = 3001001, 	    // （黑）请将车票放置在外部IC读写器的感应区内
	GUIDE_DEFER_CONFIRM_PRESS_OKBTN                   = 3002001,    	// （黑）车票信息确认无误后，按确认键进行激活
	GUIDE_DEFER_WAIT                                  = 3002002, 	    // （黑）正在进行延期处理，请稍候……
	GUIDE_DEFER_DAYS_ERROR				= 3002003,		// （红）延期时间不正确
	GUIDE_DEFER_CASH_ERROR				= 3002004,		// （红）已付金额不正确
	GUIDE_DEFER_FINISH                                = 3003001,    	// （黑）延期处理正常结束
	GUIDE_DEFER_ERROR                                 = 3003002, 	    // （红）ＸＸＸＸＸＸ，不能进行延期处理
	GUIDE_DEFER_NOT                                   = 3003003, 	    // （红）该车票不能进行延期处理

}OPERATION_MSG;


//
//// 结果编码
//typedef enum _result_code_msg{
//	RESULT_CODE_OK                                                 = 0,
//	RESULT_CODE_NOT_OK                                             = 1,
//	RESULT_CODE_FALSE							                   = 2,
//	RESULT_CODE_YIKATONG_CARD                                      = 11,
//	RESULT_CODE_CARD_CAPTURED                                      = 12,
//	RESULT_CODE_CARD_NOT_CAPTURED                                  = 13,
//	RESULT_CODE_DEVICE_MODE_INVALID                                = 14,
//	RESULT_CODE_DECRYPTION_ERROR                                   = 15,
//	RESULT_CODE_CARD_BLOCKED                                       = 16,
//	RESULT_CODE_CARD_OK                                            = 17,
//	RESULT_CODE_CARD_CORRUPT                                       = 18,
//	RESULT_CODE_CARD_OPERATION_FAILED                              = 19,
//	RESULT_CODE_CARD_MAC_INVALID                                   = 20,
//	RESULT_CODE_CARD_READ_FAILED                                   = 21,
//	RESULT_CODE_CARD_VERSION                                       = 22,
//	RESULT_CODE_JOURNEY_INVALID_STATE                              = 25,
//	RESULT_CODE_BLACKLISTED                                        = 26,
//	RESULT_CODE_TEST_MODE_MISMATCH                                 = 27,
//	RESULT_CODE_CARD_EXPIRED                                       = 31,
//	RESULT_CODE_CARD_ALREADY_BLOCKED                               = 32,
//	RESULT_CODE_CARD_NOT_INITIALISED                               = 33,
//	RESULT_CODE_CARD_ALREADY_INITIALISED                           = 34,
//	RESULT_CODE_CARD_INVALID_ISSUER                                = 35,
//	RESULT_CODE_CARD_NOT_ISSUED                                    = 36,
//	RESULT_CODE_CARD_ALREADY_ISSUED                                = 37,
//	RESULT_CODE_CARD_MAX_LIFECYCLE_REACHED                         = 38,
//	RESULT_CODE_CARD_NOT_BLOCKED_NOT_ISSUER_DEVICE                 = 39,    
//	RESULT_CODE_CARD_WITHDRAW_REQUIRED                             = 41,
//	RESULT_CODE_CARD_ALREADY_CONTAINS_PRODUCTS                     = 42,
//	RESULT_CODE_CARD_PERSONALISED                                  = 43,
//	RESULT_CODE_CARD_NOT_PERSONALISED                              = 44,
//	RESULT_CODE_CARD_TYPE_UNKNOWN                                  = 45,
//	RESULT_CODE_PRODUCT_NOT_BLOCKED                                = 50,
//	RESULT_CODE_NO_PRODUCTS                                        = 51,
//	RESULT_CODE_PRODUCT_BLOCKED                                    = 52,
//	RESULT_CODE_PRODUCT_ALREADY_BLOCKED                            = 53,
//	RESULT_CODE_PRODUCT_NOT_ISSUED                                 = 54,
//	RESULT_CODE_PRODUCT_ALREADY_ISSUED                             = 55,
//	RESULT_CODE_PRODUCT_NOT_STARTED                                = 56,
//	RESULT_CODE_PRODUCT_EXPIRED                                    = 57,
//	RESULT_CODE_PRODUCT_NOT_VALID_FOR_ENTRY                        = 58,
//	RESULT_CODE_PRODUCT_DESTINATION_INVALID                        = 59,
//	RESULT_CODE_PRODUCT_NO_REMAINING_RIDES                         = 60,
//	RESULT_CODE_PRODUCT_NO_REMAINING_VALUE                         = 61,
//	RESULT_CODE_PRODUCT_HAS_REMAINING_VALUE                        = 62,
//	RESULT_CODE_PRODUCT_NOT_ACTIVATED                              = 63,
//	RESULT_CODE_PRODUCT_NOT_VALID_FOR_CARD                         = 64,
//	RESULT_CODE_PRODUCT_MAY_NOT_BE_ACTIVATED                       = 65,
//	RESULT_CODE_PRODUCT_HAS_BEEN_USED                              = 66,
//	RESULT_CODE_PRODUCT_INSUFFICIENT_VALUE                         = 67,
//	RESULT_CODE_PRODUCT_NOT_USED                                   = 68,
//	RESULT_CODE_PRODUCT_MAY_NOT_BE_DELETED                         = 69,
//	RESULT_CODE_PRODUCT_INVALID_ADD_VALUE_RIDES                    = 70,
//	RESULT_CODE_PRODUCT_DEFERRED_REFUND_REQUIRED                   = 71,
//	RESULT_CODE_PRODUCT_ADD_VALUE_MAY_NOT_BE_REVERSED              = 72,
//	RESULT_CODE_PRODUCT_INVALID_ADD_VALUE_AUTOLOAD                 = 73,
//	RESULT_CODE_PRODUCT_NOT_YET_VALID                              = 74,
//	RESULT_CODE_NO_RIDES_REMAINING                                 = 81,
//	RESULT_CODE_INVALID_ENTRY_LOCATION                             = 82,
//	RESULT_CODE_INVALID_EXIT_LOCATION                              = 83,
//	RESULT_CODE_NO_TRANSACTION_GATE_OPEN                           = 84,
//	RESULT_CODE_TRAVEL_OVERTIME                                    = 85,
//	RESULT_CODE_TRANSFER_OVERTIME                                  = 86,
//	RESULT_CODE_TOO_MANY_TRANSFERS                                 = 87,
//	RESULT_CODE_FARE_PAID_INSUFFICIENT                             = 88,
//	RESULT_CODE_CARD_NOT_USED_TODAY                                = 89,
//	RESULT_CODE_EXIT_OVERTIME                                      = 90,
//	RESULT_CODE_CARD_INVALID_STATE                                 = 91,
//	RESULT_CODE_TICKET_VALIDITY_TO_BE_UPDATED                      = 92,
//	RESULT_CODE_WRONG_CARD_TYPE                                    = 93,
//	RESULT_CODE_CARD_NOT_PRESENT                                   = 94,
//	RESULT_CODE_DEGRADED_MODE_OUTSIDE_SPECIFIED_PERIOD             = 95,
//	RESULT_CODE_PASSBACK                                           = 96,
//	RESULT_CODE_WRONG_PRODUCT_CATEGORY                             = 97,
//	RESULT_CODE_TOO_MANY_PRODUCTS                                  = 98,
//	RESULT_CODE_NOT_THE_SAME_DEVICE                                = 99,
//	RESULT_CODE_WRONG_CARD_PRESENTED                               = 100,
//	RESULT_CODE_MAY_NOT_BE_UNBLOCKED                               = 101,
//	RESULT_CODE_APPLICATION_NOT_PRESENT                            = 102,
//	RESULT_CODE_PRODUCT_NOT_PRESENT                                = 103,
//	RESULT_CODE_ERROR_CREATING_DELAYED_ENTRY_RECORD                = 111,
//	RESULT_CODE_ERROR_CREATING_DELAYED_EXIT_RECORD                 = 112,
//	RESULT_CODE_CARD_COMMIT_FAILED                                 = 113,
//	RESULT_CODE_CARD_COMMIT_UNCONFIRMED                            = 114,
//	RESULT_CODE_NO_ACTION_SEQUENCE_NUMBER_MATCH                    = 115,
//	RESULT_CODE_NO_ACTION_SEQUENCE_CARD_NUMBER_MATCH               = 116,
//	RESULT_CODE_PRODUCT_AUTOLOAD_ALREADY_ENABLED                   = 117,
//	RESULT_CODE_PRODUCT_AUTOLOAD_ALREADY_DISABLED                  = 118,
//	RESULT_CODE_COMMUNICATIONS_ERROR                               = 119,
//	RESULT_CODE_REFUND_NOT_PERMITTED                               = 120,
//	RESULT_CODE_REFUND_ENQUIRY_RESULT_ERROR						   = 121,
//	RESULT_CODE_CARD_WRITE_FAILED                                  = 501,
//	RESULT_CODE_INVALIDCARD                                        = 502,
//	ECT_CODE_AUTHORITY_TIME_EXPIRED                                = 601,
//	ECT_CODE_NOT_ECT_CARD										   = 602,
//	ECT_CODE_TWO_MODES_MISMATCH                                    = 603,
//	ECT_CODE_CARD_STATUS_INVALIDITY                                = 604,
//	ECT_CODE_ISSUE_DATE_INVALIDITY                                 = 605,
//	ECT_CODE_CARD_BLOCKED										   = 606,
//	ECT_CODE_BALANCE_NOT_ZERO									   = 607,
//	ECT_CODE_DEPOSIT_INVALIDITY									   = 608,
//	ECT_CODE_CARD_TYPE_MISMATCH                                    = 609,
//	ECT_CODE_CARD_TYPE_NOT_ISSUE                                   = 610,
//	ECT_CODE_NOT_EXTEND_EXPIRY_DATE								   = 611,
//	ECT_CODE_CARD_TYPE_NOT_CHARGE								   = 612,
//	ECT_CODE_BALANCE_OVER_LAVVALUE                                 = 613,
//	ECT_CODE_PARAMETER_ERROR									   = 614,
//	ECT_CODE_NOT_INVALIDITY                                        = 615,
//	ECT_CODE_FARE_OVER_RIDE                                        = 620,
//	ECT_CODE_GET_FARE_ERROR                                        = 621,
//	ECT_CODE_GET_PARAM_ERROR                                       = 622,
//	ECT_CODE_PRODUCT_EXPIRED                                       = 623,
//	ECT_CODE_PRODUCT_NOT_ISSUE                                     = 624,
//	ECT_CODE_PRODUCT_INVALIDITY_CHANGE                             = 625,
//	ECT_CODE_BALANCE_NOT_ENOUGH	                                   = 626,
//	ECT_CODE_ISSUE_STATUS_INVALIDITY                               = 627,
//	ECT_CODE_PURSE_ERROR										   = 628,
//	ECT_CODE_CARD_NO_EXIT_RECORD								   = 641,
//	ECT_CODE_GET_BMAC_INFO_FAILED                                  = 642,
//	ECT_CODE_REFUND_APPLY_FAILED								   = 643,
//	ECT_CODE_BLOCK_CARD_FAILED                                     = 644,
//	ECT_CODE_BUS_STATUS_FAILED                                     = 645,
//	ECT_CODE_VALID_DATE_INVALIDITY								   = 646,
//	ECT_CODE_STAFF_DATE_INVALIDITY                                 = 647,
//	ECT_CODE_CRAD_REMOVE                                           = 648,
//	ECT_CODE_CRAD_CHARGE_LOST									   = 649,
//}RESULT_CODE_MSG;

//typedef RESULT_CODE_MSG     ResultCode_t;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////BUTTON消息文本定义///////////////////////////////////////

static const TCHAR* const WM_BTN_PRINT			= _T("print");							///< 打印按钮消息
static const TCHAR* const WM_BTN_OK				= _T("ok");								///< 确认按钮消息
static const TCHAR* const WM_BTN_BACK			= _T("back");							///< 返回按钮消息
static const TCHAR* const WM_BTN_RESET       = _T("reset");							///< 重置按钮消息
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


