#pragma once

//TVM、BOM、ES公共UI定义
enum DOCKSTYLE
{
	NONE = 0x00,
	TOP  = 0x01,
	RIGHT = 0x02,
	BOTTOM = 0x03,
	LEFT = 0x04,
	FILL = 0x05,
	LEFTRIGHT = 0x06,
	TOPBOTTOM = 0x07,
};

typedef enum _btnTextLocationType
{
	BTN_TXT_LEFT,
	BTN_TXT_TOP,
	BTN_TXT_RIGHT,
	BTN_TXT_BOTTOM,
	BTN_TXT_CENTER,
}BTN_TEXT_LOCATION_TYPE;

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
	BUTTON_TYPE_OK,                 // 確認按钮
	BUTTON_TYPE_PRINT,              // 打印選擇按钮
	BUTTON_TYPE_SERVICE,            // 服务状态按钮
	BUTTON_TYPE_LANGUAGE,           // 语言选择按钮
	BUTTON_TYPE_BUTTENGROUP,        // 按纽组按钮
	BUTTON_TYPE_CANCLE,             // 取消按钮
	BUTTON_TYPE_LINE_AREA,          // 区域按钮
	// 以下是针对武汉界面，新增加风格
	BUTTON_TYPE_CLOSE,              // 关闭按钮(所有状态都是黄色)
	BUTTON_TYPE_BLUE,               // 全部线路显示按钮(所有状态都是蓝色)
	BUTTON_TYPE_BUTTONGROUP_BLUE,	// 按钮正常状态是浅蓝色，选中是绿色，无效是灰色
	BUTTON_TYPE_BUTTONGROUP_BROWN,	// 按钮正常状态是褐色，选中是绿色，无效是灰色
	BUTTON_TYPE_BUTTONGROUP_BLUE_SPEC,	// 按钮正常状态是浅蓝色，选中是绿色，无效是灰色(首尾按钮单独处理）
	BUTTON_TYPE_BUTTONGROUP_HOT,	// 热点车站组，所有状态都是同一图
	BUTTON_TYPE_CHARGE,				// 充值按钮
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
	BTN_TEXT_LOCATION_TYPE textLocationType;                // 按纽文言位置

	_btn_info(){
		btnID = 0;											// 按钮ID
		text = "";											// 显示文言1
		btnMsg = "";										// 按钮消息
		isVisible = true;									// 是否显示
		isEnable = true;									// 是否可用
		isSelected = false;									// 是否选中
		type = BUTTON_TYPE::BUTTON_TYPE_NORMAL;
		shortCutKey = 0;
		textLocationType = BTN_TEXT_LOCATION_TYPE::BTN_TXT_CENTER;
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
		this->textLocationType = another.textLocationType; 
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
		textLocationType = BTN_TEXT_LOCATION_TYPE::BTN_TXT_CENTER;
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
/////////////////////////////////////////////////颜色定义///////////////////////////////////////////////////////////////

#define BLUE						RGB(40, 94, 166)    //massageBoxTitle
#define SKYBLUE						RGB(235,241,222)
#define WHITE                       RGB(255,255,255)
#define BLACK                       RGB(0,0,0)
#define DARKGRAY                    RGB(187,187,187)
#define BLACKGRAY                   RGB(179,179,179)
#define RED                         RGB(255,0,0)
#define SOFTRED						RGB(200,0,0) 
#define SOFTGREEN                   RGB(0,200,0)
#define WHITE                       RGB(255,255,255)
#define ORANGE                      RGB(255,120,35)
#define GREEN						RGB(0,100,0)
#define YELLOW						RGB(238,180,34)//土黄色

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////大小定义////////////////////////////////////////////////////////////////
static const CSize DEFAULT_SIZE_EDIT	= CSize(100,23);
static const CSize DEFAULT_SIZE_STATIC  = CSize(200,80);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  DEFAULT_PADDINGS  PADDINGS(0,0,0,0)
static const COLORREF DEFAULT_COLOR_LABEL  = BLACK;
static const MARGINS DEFAULT_GROUPBOX_MARGINS = {5,2,5,2};
static const CPoint ROUND_POINT        =  CPoint(20,20);    // 圆角矩形的圆角弧度
static const int ROUNDRECT_BORDER      =      2;            // 区域边框线的粗细
static const int LINE_BORDER           =      3;            // 画面中蓝线的粗细
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////界面主区域定义//////////////////////////////////////////////////
static const CRect LOGO_FRAME_RECT				= CRect(0,0,262,100);			// 标志窗体对话框

static CRect const OUT_OF_RECEPTION_LOGO_RECT	= CRect(180,241,280,346);
static const CRect MENU_MAIN_RECT				= CRect(262,0,1024,100);		// 顶层菜单
static const CRect MENU_SECOND_RECT				= CRect(262,50,1024,100);		// 二级菜单对话框


static const CRect STS_FRAME_RECT				= CRect(0,730,1024,768);		// 状态栏对话框

static const CRect LEFT_1_FRAME_RECT			= CRect(0,100,262,520);			// 左一区域
static const CRect LEFT_2_FRAME_RECT			= CRect(0,520,262,730);			// 左二区域
static const CRect RIGHT_1_FRAME_RECT			= CRect(762,100,1024,310);		// 右一区域
static const CRect RIGHT_2_FRAME_RECT			= CRect(762,310,1024,610);		// 右二区域
static const CRect RIGHT_3_FRAME_RECT			= CRect(762,610,1024,730);		// 右三区域

//Common
static const CRect BTN_CHINESE_RECT = CRect(352,520,452,570);
static const CRect BTN_ENGLISH_RECT = CRect(462,520,562,570);
static const CRect BTN_PORTUGUESE_RECT = CRect(572,520,672,570);

// 维护界面
// 基本信息区域标题字体名称、大小、颜色
#define TITLE_FONT_NAME    theGUI_INFO.GetTitleFontName()
#define TITLE_FONT_SIZE	   theGUI_INFO.GetTitleFontSize()           
#define TITLE_FONT_COLOR   theGUI_INFO.GetTitleFontColor()
// 按钮字体大小（暂不使用）
#define BUTTON_FONT_SIZE   theGUI_INFO.GetBtnFontSize()			  
#define BUTTON_FONT_NAME   theGUI_INFO.GetBtnFontName()
// 基本信息区域普通文本字体名称、大小、颜色
#define NORMAL_FONT_SIZE   theGUI_INFO.GetNormalFontSize()            
#define NORMAL_FONT_NAME   theGUI_INFO.GetNormalFontName()
#define NORMAL_FONT_COLOR  theGUI_INFO.GetNormalFontColor()
// 导航框中文字体大小
#define GUIDE_FONT_SIZE	   theGUI_INFO.GetGuideFontSize()            
#define GUIDE_FONT_NAME    theGUI_INFO.GetGuideFontName()
// 初始化文字体大小
#define INI_FONT_SIZE	   theGUI_INFO.GetIniFontSize()            
#define INI_FONT_NAME	   theGUI_INFO.GetIniFontName()
#define INI_FONT_COLOR     theGUI_INFO.GetIniFontColor()
#define INI_RESULT_SUCCESS_COLOR theGUI_INFO.GetInitRsltSucceedFontColor() // 初始化结果成功字体颜色
#define INI_RESULT_FAILED_COLOR  theGUI_INFO.GetInitRsltFailedFontColor()  // 初始化结果失败字体颜色
#define INI_RESULT_UNKNOW_COLOR  theGUI_INFO.GetInitRsltUnknowFontColor()  // 初始化结果未知字体颜色
// 版本信息中文字体大小    
#define VERSION_FONT_SIZE  theGUI_INFO.GetVersionFontSize()  

// 乘客界面
// 基本信息区域正文字体名称、大小、颜色
#define TVM_PAS_BASE_INFO_FONT_NAME		theGUI_INFO.GetRecBaseInfoFontName()
#define TVM_PAS_BASE_INFO_FONT_SIZE		theGUI_INFO.GetRecBaseInfoFontSize()
#define TVM_PAS_BASE_INFO_FONT_COLOR	theGUI_INFO.GetRecBaseInfoFontColor()
// 基本信息区域标题字体名称、大小、颜色
#define TVM_PAS_BASE_TITLE_FONT_NAME	theGUI_INFO.GetRecTitleFontName()
#define TVM_PAS_BASE_TITLE_FONT_SIZE	theGUI_INFO.GetRecTitleFontSize()
#define TVM_PAS_BASE_TITLE_FONT_COLOR	theGUI_INFO.GetRecTitleFontColor()
// 向导区域正文字体名称、大小、颜色
#define TVM_PAS_GUIDE_FONT_NAME			theGUI_INFO.GetRecGuideFontName()
#define TVM_PAS_GUIDE_FONT_SIZE			theGUI_INFO.GetRecGuideFontSize()
#define TVM_PAS_GUIDE_FONT_COLOR		theGUI_INFO.GetRecGuideFontColor()

// 字符集   134:GB2312_CHARSET
#define CHARSET								theGUI_INFO.GetPassengerFontCodeType()
#define BOM_CHARSET					theGUI_INFO.GetFontCodeType()
#define ES_CHARSET					theGUI_INFO.GetFontCodeType()
#define OPERATORCHARSET				theGUI_INFO.GetOperatorFontCodeType()
#define DEFAULT_FONT       theGUI_INFO.GetDefaultFont()

static const int NORMAL_BUTTON_WIDTH        = 88;			 // 按钮的宽度
static const int NORMAL_BUTTON_HEIGHT       = 61;            // 按钮的高度

static const int LABEL_GROUP_ROWS_8 = 8;
static const int LABEL_GROUP_ROWS_11 = 11;
static const int LABEL_GROUP_ROWS_15 = 15;
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
#define BASE_INFO_RECT_RIGHT(len) (len+1)*9
#define BASE_INFO_RECT(row,len)   CRect(160,BASE_INFO_RECT_TOP(row),160+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+21)

static const int DEFAULT_TXT_DISPLAY_LEN = 10;
static const int DEFAULT_PASSWORD_LEN = 6;

/////////////////////////////////////////////////界面Group的ID定义///////////////////////////////////////////////////////////////
#define ID_GRP_NULL 0
#define MENU_GROUP_ID 1
#define	SECOND_MENU_GROUP_ID 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const int MAX_ISSUE_TICKET_COUNT     = 100;          // 最大发票张数
//static const int MAX_SBC_CASE_LEN		    = 8;		    // 左列全角字符最大长度
static const int MAX_SBC_CASE_LEN_YEAR      = 7;            // 左列全角字符最大长度(用于日期的年)
static const int MAX_SBC_CASE_LEN_DATE		= 5;		    // 左列全角字符最大长度(用于日期)
//static const int MAX_SBC_CASE_LEN_RIGHT     = 8;            // 右列全角字符最大长度
//static const int MAX_HALF_CASE_LEFT_LEN	    = 16;		    // 左列半角字符最大长度
static const int MAX_CASH_RIGHT_LEN         = 16;           // 现金信息右列最大字符

//以下是普通业务画面信息区域显示项目显示内容定义
static const TCHAR* const TXT_NOTHING					= _T("");		// 默认显示内容


// 以下是界面通用按钮显示文字代码定义
static const TCHAR* const STR_ENGLISH							= _T("English");
static const TCHAR* const STR_CHINESE							= _T("简体中文");
static const TCHAR* const STR_PORTUGUESE						= _T("ᠮᠣᠩᠭᠣᠯ ᠬᠡᠯᠡᠨ ᠦ");



/////////////////////////////////////////////////BUTTON消息文本定义///////////////////////////////////////
static const TCHAR* const WM_BTN_OK				= _T("WM_BTN_OK");								///< 确认按钮消息