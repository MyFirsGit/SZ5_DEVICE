
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
/////////////////////////////////////////////////��ť��ʾ��ض���//////////////////////////////////////////////
enum BUTTON_TYPE {
	BUTTON_TYPE_NORMAL,             // ������ť
	BUTTON_TYPE_NOSELECT,           // ��ѡ��״̬�İ�ť
};


typedef struct _btn_info {
	int btnID;												// ��ťID(0��ʼ)
	CString text;											// ��ʾ����1
	CString btnMsg;											// ��ť��Ϣ
	bool isVisible;											// �Ƿ���ʾ
	bool isEnable;											// �Ƿ����
	bool isSelected;										// �Ƿ�ѡ��
	UINT shortCutKey;										// ��ݼ�
	BUTTON_TYPE type;										// ��ť����

	_btn_info(){
		btnID = 0;											// ��ťID
		text = _T("");											// ��ʾ����1
		btnMsg = _T("");										// ��ť��Ϣ
		isVisible = true;									// �Ƿ���ʾ
		isEnable = true;									// �Ƿ����
		isSelected = false;									// �Ƿ�ѡ��
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
		btnID = 0;											// ��ťID
		text = _T("");											// ��ʾ����1
		btnMsg = _T("");										// ��ť��Ϣ
		isVisible = true;									// �Ƿ���ʾ
		isEnable = true;									// �Ƿ����
		isSelected = false;									// �Ƿ�ѡ��
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
/////////////////////////////////////////////////��ɫ����///////////////////////////////////////////////////////////////

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
/////////////////////////////////////////////////��С����////////////////////////////////////////////////////////////////
static const CSize DEFAULT_SIZE_EDIT	= CSize(100*CXSCREENRATIO,23*CYSCREENRATIO);
static const CSize DEFAULT_SIZE_BUTTON	= CSize(80*CXSCREENRATIO,40*CYSCREENRATIO);
static const CSize DEFAULT_SIZE_STATIC = CSize(200*CXSCREENRATIO,80*CYSCREENRATIO);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  DEFAULT_PADDINGS  PADDINGS(0,0,0,0)
static const COLORREF DEFAULT_COLOR_LABEL  = BLACK;
static const COLORREF DEFAULT_COLOR_STATIC = GRAY;
static const MARGINS DEFAULT_MARGINS = {0,0,0,0};
static const MARGINS DEFAULT_GROUPBOX_MARGINS = {5,2,5,2};
static const CPoint ROUND_POINT        =  CPoint(20,20);    // Բ�Ǿ��ε�Բ�ǻ���
static const int ROUNDRECT_BORDER      =      2;            // ����߿��ߵĴ�ϸ
static const int LINE_BORDER           =      3;            // ���������ߵĴ�ϸ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////������������//////////////////////////////////////////////////
//վԱ��Ļ�ֱ�����1024*768��Ϊ1920*1080���˿���Ļ�ֱ���Ϊ1024*768
static const CRect LOGO_FRAME_RECT				= CRect(0,0,262*CXSCREENRATIO,100*CYSCREENRATIO);//CRect(0,0,262,100)			// ��־����Ի���
static CRect const LOGO_RECT					= CRect(20*CXSCREENRATIO,34*CYSCREENRATIO,110*CXSCREENRATIO,105*CYSCREENRATIO);			// վԱ����LOGO����
static CRect const RECEPTION_HEAD_LOGO_RECT		= CRect(10*CXSCREENRATIO,380*CYSCREENRATIO,100*CXSCREENRATIO,465*CYSCREENRATIO);			// �˿�ҵ�����LOGO����
//static CRect const OUT_OF_RECEPTION_LOGO_RECT	= CRect(10,176,100,261);
//static CRect const WELCOME_LOGO_RECT			= CRect(10,241,110,346);
static CRect const WELCOME_LOGO_RECT			= CRect(40,210,180,360);		// �˿ͻ�ӭ���ٽ���LOGO����
static CRect const WELCOME_LOGO_RECT2         = CRect(40+CXSCREENREC,210,180+CXSCREENREC,360);

static CRect const OUT_OF_RECEPTION_LOGO_RECT	= CRect(180*CXSCREENRATIO,241*CYSCREENRATIO,280*CXSCREENRATIO,346*CYSCREENRATIO);
static const CRect MENU_MAIN_RECT				= CRect(262*CXSCREENRATIO,0,1024*CXSCREENRATIO,100*CYSCREENRATIO);		// ����˵�
static const CRect MENU_SECOND_RECT				= CRect(262*CXSCREENRATIO,50*CYSCREENRATIO,1024*CXSCREENRATIO,100*CYSCREENRATIO);		// �����˵��Ի���
static const CRect OPERATION_FRMAE_RECT			= CRect(262*CXSCREENRATIO,150*CYSCREENRATIO,762*CXSCREENRATIO,730*CYSCREENRATIO);		// �����Ի��򣨻�����Ϣ+Ǯ��Ϣ+��ť��
static const CRect INIT_FRAME_RECT				= CRect(0,100*CYSCREENRATIO,1024*CXSCREENRATIO,730*CYSCREENRATIO);
static const CRect STS_FRAME_RECT				= CRect(0,730*CYSCREENRATIO,1024*CXSCREENRATIO,768*CYSCREENRATIO);		// ״̬���Ի���

static const CRect LEFT_1_FRAME_RECT			= CRect(0,100*CYSCREENRATIO,262*CXSCREENRATIO,520*CYSCREENRATIO);			// ��һ����
static const CRect LEFT_2_FRAME_RECT			= CRect(0,520*CYSCREENRATIO,262*CXSCREENRATIO,730*CYSCREENRATIO);			// �������
static const CRect RIGHT_1_FRAME_RECT			= CRect(762*CXSCREENRATIO,100*CYSCREENRATIO,1024*CXSCREENRATIO,310*CYSCREENRATIO);		// ��һ����
static const CRect RIGHT_2_FRAME_RECT			= CRect(762*CXSCREENRATIO,310*CYSCREENRATIO,1024*CXSCREENRATIO,467*CYSCREENRATIO);		// �Ҷ�����
static const CRect KEYBOARD_FRAME_RECT			= CRect(762*CXSCREENRATIO,467*CYSCREENRATIO,1024*CXSCREENRATIO,730*CYSCREENRATIO);		// ��������
static const CRect GUIDE_FRAME_RECT				= CRect(262*CXSCREENRATIO,100*CYSCREENRATIO,762*CXSCREENRATIO,150*CYSCREENRATIO);		// �����Ի���
static const CRect BTN_OK_RECT = CRect(410*CXSCREENRATIO,520*CYSCREENRATIO,490*CXSCREENRATIO,570*CYSCREENRATIO);
static const CRect BTN_BACK_RECT = CRect(310*CXSCREENRATIO,520*CYSCREENRATIO,390*CXSCREENRATIO,570*CYSCREENRATIO);
static const CRect BTN_PRINT_RECT = CRect(110*CXSCREENRATIO,520*CYSCREENRATIO,290*CXSCREENRATIO,570*CYSCREENRATIO);
static const CRect BTN_RESET_RECT = CRect(10*CXSCREENRATIO,520*CYSCREENRATIO,90*CXSCREENRATIO,570*CYSCREENRATIO);


static const CRect OPERATION_BUTTON_RECT		= CRect(0,510*CYSCREENRATIO,500*CXSCREENRATIO,580*CYSCREENRATIO);			// �ײ�������ť����
static const CRect MESSAGE_BOX_FRMAE_RECT		= CRect(315,365,733,645);//CRect(324,375,724,635)		// ��Ϣ��
static const CRect  CASH_INFO_RECT				= CRect(163*CXSCREENRATIO,86,400,111);		// �ֽ���Ϣ�����б༭�������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const CPoint LEFTUP_AREA_POINT          = CPoint(0,0);					// ���Ͻ��������ʼ��
static const CPoint LEFTDOWN_AREA_POINT        = CPoint(0,350*CYSCREENRATIO);					// ���½��������ʼ��
static const CRect REC_HEAD_RECT			   = CRect(0,0,2048,100);
static const CRect REC_GUIDE_RECT              = CRect(0,680,2048,768);
static const CRect REC_BODY_RECT               = CRect(0,100,2048,680);
static const CRect BUSINESS_AREA_RECT          = CRect(5,5,645,575);


//static const int REC_BODY_L = 0;												// �˿ͶԻ������Ϻ�����
//static const int REC_BODY_U = 136;											// �˿ͶԻ�������������
//static const int REC_BODY_R = 1024;											// �˿ͶԻ������º�����
//static const int REC_BODY_D = 768;											// �˿ͶԻ�������������
static const CRect  REC_GUIDE_MSG_RECT         = CRect(15,6,1009,66);			// �˿͵���������
static const CRect WELCOME_FRAME_RECT		   = CRect(0,0,2048,768);		    // ��ӭ��

static const TCHAR* const REC_CN_FONT_NAME= _T("����");
static const UINT const REC_CN_FONT_SIZE = 24;
static const UINT const REC_CN_FONT_SIZE2 = 18;
static const UINT const REC_CN_FONT_SIZE3 = 15;
static const TCHAR* const REC_EN_FONT_NAME = _T("����");
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
	_T("����")
};

static const TCHAR* const TITLE_FONT         = _T( "����");
static const int TITLE_FONT_SIZE			=  16;           // ��ť��ı��������С
static const int BUTTON_FONT_SIZE           =  14;    //�༭�������֡��������������С
static const int NORMAL_FONT_SIZE	        =  14;
static const TCHAR* const NORMAL_FONT	    = _T("����");

static const int NORMAL_BUTTON_WIDTH        = 88*CXSCREENRATIO;			 // ��ť�Ŀ��
static const int NORMAL_BUTTON_HEIGHT       = 61*CYSCREENRATIO;            // ��ť�ĸ߶�

static const int GUIDE_FONT_SIZE_CN    =      18;            // ���������������С
static const int GUIDE_FONT_SIZE_EN    =      10;            // ������Ӣ�������С

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
// ��������ı༭�����꣬��һ����������������1��ʼ���ڶ��������Ǳ༭���ڿ������ֵĳ��ȣ��԰���ַ�����
#define BASE_INFO_RECT_TOP(row)   49+(row-1)*37
#define BASE_INFO_RECT_RIGHT(len) (len+1)*12
#define BASE_INFO_RECT(row,len)   CRect(160*CXSCREENRATIO,BASE_INFO_RECT_TOP(row),160*CXSCREENRATIO+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+26)
// ����������˵������������Ǳ༭����ʼ���ͨ��ƫ��İ���ֽ���
#define BASE_INFO_RECT_SP(row,len,leftStart)   CRect(160*CXSCREENRATIO+11.5*leftStart,BASE_INFO_RECT_TOP(row),160*CXSCREENRATIO+11.5*leftStart+BASE_INFO_RECT_RIGHT(len),BASE_INFO_RECT_TOP(row)+26)

static const int DEFAULT_TXT_DISPLAY_LEN = 10;
static const int DEFAULT_PASSWORD_LEN = 6;

// ��������󳤶�
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

/////////////////////////////////////////////////����Group��ID����///////////////////////////////////////////////////////////////
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

static const int MAX_ISSUE_TICKET_COUNT     = 100;          // ���Ʊ����
static const int MAX_SBC_CASE_LEN		    = 8;		    // ����ȫ���ַ���󳤶�
static const int MAX_SBC_CASE_LEN_YEAR      = 7;            // ����ȫ���ַ���󳤶�(�������ڵ���)
static const int MAX_SBC_CASE_LEN_DATE		= 5;		    // ����ȫ���ַ���󳤶�(��������)
static const int MAX_SBC_CASE_LEN_RIGHT     = 4;            // ����ȫ���ַ���󳤶�
static const int MAX_CASH_RIGHT_LEN         = 16;           // �ֽ���Ϣ��������ַ�

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��������ͨҵ���水ť������Ϣ������ⶨ��
static const TCHAR* const TITLE_CASH_INFO					   = _T("�ֽ���Ϣ");
static const TCHAR* const TITLE_CASH_INFO_EN					   = _T("Cash Information");
static const TCHAR* const TITLE_TICKET_INFO					   = _T("��Ʊ��Ϣ");
static const TCHAR* const TITLE_TICKET_INFO_EN				   = _T("Ticket Information");
static const TCHAR* const TITLE_ISSUE_INFO					   = _T("��Ʊ��Ϣ");
static const TCHAR* const TITLE_ISSUE_INFO_EN				   = _T("Issue Information");
static const TCHAR* const TITLE_CHARGE_INFO					   = _T("��ֵ��Ϣ");
static const TCHAR* const TITLE_CHARGE_INFO_EN                  = _T("CHARge Information");
static const TCHAR* const TITLE_UPDATE_INFO					   = _T("��Ʊ��Ϣ");
static const TCHAR* const TITLE_UPDATE_INFO_EN				   = _T("Update Information");
static const TCHAR* const TITLE_REFUND_INFO					   = _T("�˿���Ϣ");
static const TCHAR* const TITLE_REFUND_INFO_EN                  = _T("Refund Information");
static const TCHAR* const TITLE_EXCHANGE_INFO				   = _T("������Ϣ");
static const TCHAR* const TITLE_EXCHANGE_INFO_EN				   = _T("Exchange Information");
static const TCHAR* const TITLE_REFRESH_INFO					   = _T("������Ϣ");
static const TCHAR* const TITLE_REFRESH_INFO_EN				   = _T("Refresh Information");
static const TCHAR* const TITLE_TRANSACTION_RECORDS			   = _T("���׼�¼");
static const TCHAR* const TITLE_NO_BUSINESS_INCOME			   =  _T("��������֧");
static const TCHAR* const TITLE_ADMIN_PROCESS				   =  _T("����������Ϣ");

//������ά��ҵ���水ť������Ϣ������ⶨ��
static const TCHAR* const TITLE_STATION_INFO                    =  _T("��վ��Ϣ");
static const TCHAR* const TITLE_FAILURE_TITLENAME			   =  _T("������Ϣ");
static const TCHAR* const TITLE_COMMUNICATION_PARA_INFO         =  _T("ͨ�Ų�����Ϣ");
static const TCHAR* const TITLE_MODE_SWITCH_INFO                =  _T("�豸ģʽ��Ϣ");
static const TCHAR* const TITLE_DATATYPE                        =  _T("��������");
static const TCHAR* const TITLE_DATAIMPORT_INFO                 =  _T("����������Ϣ");
static const TCHAR* const TITLE_IMPORTTYPE                      =  _T("���뷽ʽ");
static const TCHAR* const TITLE_EXPORTTYPE                      =  _T("�����ʽ");
static const TCHAR* const TITLE_DATAEXPORT_INFO                 =  _T("���������Ϣ");
static const TCHAR* const TITLE_VERSION_INFO                    =  _T("�汾��Ϣ");
static const TCHAR* const TITLE_DATA_AUDIT_INFO                 =  _T("�ɽ���������Ϣ");
static const TCHAR* const TTILE_SYSTEMTYPEOVER                  =  _T("���������Ϣ");
static const TCHAR* const TITLE_TH_TEST_INFO                    =  _T("��Ʊ������");
static const TCHAR* const TITLE_TH_UNITTEST_INFO                =  _T("�������");
static const TCHAR* const TITLE_TH_UNITTEST_DETAIL              =  _T("��ϸ����");
static const TCHAR* const TITLE_ICRW_TEST_INFO                  =  _T("�ⲿIC��д��������Ŀ");
static const TCHAR* const TITLE_PRINT_TEST_INFO                 =  _T("��ӡ��������Ŀ");
static const TCHAR* const TITLE_MONEYBOX_TEST_INFO              =  _T("Ǯ�������Ŀ");
static const TCHAR* const TITLE_NETWORK_TEST_INFO               =  _T("��λ������Ŀ");
static const TCHAR* const TTILE_SYSTEMTYPE                      =  _T("��������");
static const TCHAR* const TTILE_YKTPARA                         =  _T("һ��ͨ������Ϣ");
static const TCHAR* const TITLE_AUTHORIZE_TYPE                  =  _T("��֤����");
static const TCHAR* const TITLE_AUTHORIZE_MANNER                =  _T("��֤��ʽ");
static const TCHAR* const TITLE_AUTHORIZE_MANNER_OVER           =  _T("��֤��ʽ��Ϣ");
static const TCHAR* const TITLE_SN_SETTING_OVER                 =  _T("���к��趨");
static const TCHAR* const TITLE_UDSN_INFO					   =  _T("��ˮ����Ϣ");
static const TCHAR* const TITLE_TH_MODE_INFO					   =  _T("��Ʊģʽ��Ϣ");
static const TCHAR* const TITLE_UPS_INFO						   =  _T("UPS��Ϣ");

//��������ͨҵ������Ϣ������ʾ��Ŀ��ʾ���ݶ���
static const TCHAR* const TXT_COLON						= _T("��");
static const TCHAR* const TXT_NOTHING					= _T("--");		// Ĭ����ʾ����
// ��λ���ƶ���
static const TCHAR* const TXT_YUAN_MARK					= _T("��");
static const TCHAR* const MONEY_UNIT						= _T("Ԫ");
static const TCHAR* const MONEY_UNIT_EN					= _T("yuan");
static const TCHAR* const TICKET_UNIT					= _T("��");
static const TCHAR* const TICKET_UNIT_EN					= _T("piece(s)");
static const TCHAR* const TIMES_UNIT						= _T("��");
static const TCHAR* const TIMES_UNIT_EN					= _T("time(s)");
static const TCHAR* const YEAR_UNIT						= _T("��");
static const TCHAR* const YEAR_UNIT_EN					= _T("year(s)");
static const TCHAR* const MONTH_UNIT						= _T("��");
static const TCHAR* const MONTH_CNT_UNIT					= _T("����");
static const TCHAR* const MONTH_CNT_UNIT_EN				= _T("month(s)");
static const TCHAR* const DATE_UNIT						= _T("��");
static const TCHAR* const DAY_UNIT						= _T("��");
static const TCHAR* const DAY_UNIT_EN					= _T("day(s)");
static const TCHAR* const HOUR_UNIT						= _T("ʱ");
static const TCHAR* const MINUTE_UNIT					= _T("��");

static const TCHAR* const MALE							= _T("��");
static const TCHAR* const FEMALE						= _T("Ů");

static const TCHAR* const TXT_REFUND_REASON_PASSAGER = _T("�˿�ԭ��");
static const TCHAR* const TXT_REFUND_REASON_SUBWAY = _T("����ԭ��");
static const TCHAR* const TXT_APPLY_REASON_BAD_CARD = _T("Ʊ����");
static const TCHAR* const TXT_APPLY_REASON_LARGE_CARD = _T("��");
static const TCHAR* const TXT_APPLY_REASON_OTHER = _T("����");

static const TCHAR* const TXT_CERTIFICATE_ID_CARD_MAINLAND = _T("���֤");
static const TCHAR* const TXT_CERTIFICATE_STUDENT = _T("ѧ��֤");
static const TCHAR* const TXT_CERTIFICATE_MILITARY = _T("����֤");
static const TCHAR* const TXT_CERTIFICATE_FOREIGNER = _T("��������þ���֤");

// ���ö���
static const TCHAR* const TXT_UNDEFINE = _T("--");
static const TCHAR* const TXT_TICKET_NUMBER				= _T("��Ʊ���");
static const TCHAR* const TXT_TICKET_NUMBER_EN 		    = _T("Ticket Number");
static const TCHAR* const TXT_TICKET_KIND				= _T("��Ʊ����");
static const TCHAR* const TXT_TICKET_KIND_EN	            = _T("Ticket Type");
static const TCHAR* const TXT_TICKET_VALIDATE_DATE		= _T("��Ч����");
static const TCHAR* const TXT_TICKET_VALIDATE_DATE_EN	= _T("Validity Period");
static const TCHAR* const TXT_TICKET_BALANCE				= _T("Ʊ�����");
static const TCHAR* const TXT_TICKET_RIDES				= _T("Ʊ�����");
static const TCHAR* const TXT_TICKET_RIDES_EN			= _T("The Balance");
static const TCHAR* const TXT_TICKET_BALANCE_EN          = _T("The Balance");

static const TCHAR* const TXT_TICKET_DEPOSIT				= _T("��ƱѺ��");
static const TCHAR* const TXT_TICKET_DEPOSIT_EN          = _T("Deposit");
static const TCHAR* const TXT_TOTAL_VALUE			    = _T("�ϼƽ��");
static const TCHAR* const TXT_TOTAL_VALUE_EN			    = _T("Total Amount");

static const TCHAR* const TXT_TOTAL     					= _T("Ӧ��");
static const TCHAR* const TXT_INCOME					    = _T("ʵ��");
static const TCHAR* const TXT_CHANGE					    = _T("����");

static const TCHAR* const TXT_TOTAL_RECEPTION     			= _T("Ӧ�ս��");
static const TCHAR* const TXT_TOTAL_RECEPTION_EN	        = _T("Amount Due");
static const TCHAR* const TXT_INCOME_RECEPTION				= _T("�Ѹ����");
static const TCHAR* const TXT_INCOME_RECEPTION_EN		    = _T("Paid");
static const TCHAR* const TXT_CHANGE_RECEPTION				= _T("������");
static const TCHAR* const TXT_CHANGE_RECEPTION_EN		    = _T("Change");

// ��Ʊ
static const TCHAR* const TXT_TICKET_AMOUNT				= _T("��Ʊ����");
static const TCHAR* const TXT_TICKET_AMOUNT_EN			= _T("Unit Fare");
static const TCHAR* const TXT_TICKET_COUNT				= _T("��Ʊ����");
static const TCHAR* const TXT_TICKET_COUNT_EN			= _T("No. of Ticket");
static const TCHAR* const TXT_START_STATION				= _T("��ʼ��վ");
static const TCHAR* const TXT_START_STATION_EN			= _T("Starting Station");
static const TCHAR* const TXT_END_STATION				= _T("Ŀ�ĳ�վ");
static const TCHAR* const TXT_END_STATION_EN				= _T("Destination");
static const TCHAR* const TXT_ISSUE_DATE_ONLY			= _T("������Ч");
static const TCHAR* const TXT_ISSUE_DATE_ONLY_EN			= _T("Issued Date Only");
static const TCHAR* const TXT_ANY_STATION				= _T("���⳵վ");
static const TCHAR* const TXT_USED_STATION				= _T("��Ч��վ");
static const TCHAR* const TXT_USED_STATION_EN			= _T("Validity Station");
static const TCHAR* const TXT_VALID_RIDES = _T("��Ч�˴�");
static const TCHAR* const TXT_VALID_RIDES_EN = _T("Rides");
static const TCHAR* const TXT_TICKET_SALE_AMOUNT = _T("��Ʊ���");
static const TCHAR* const TXT_TICKET_SALE_AMOUNT_EN = _T("Fare");

// ��ֵ
static const TCHAR* const TXT_CHARGEOVER_AMOUNT			= _T("��ֵ�����");
static const TCHAR* const TXT_CHARGEOVER_AMOUNT_EN       = _T("The Balance");
static const TCHAR* const TXT_CHARGE_AMOUNT				= _T("��ֵ���");
static const TCHAR* const TXT_CHARGE_AMOUNT_EN			= _T("Add Value");
static const TCHAR* const TXT_CHARGE_RIDE				= _T("��ֵ����");
static const TCHAR* const TXT_CHARGE_RIDE_EN				= _T("Add Rides");
static const TCHAR* const TXT_CHARGEOVER_RIDE			= _T("��ֵ�����");
static const TCHAR* const TXT_CHARGEOVER_RIDE_EN			= _T("The Balance");
static const TCHAR* const TXT_CHARGE_PRICE_TIME			= _T("�շѽ��");
static const TCHAR* const TXT_CHARGE_PRICE_TIME_EN		= _T("Fee");

// ����
static const TCHAR* const TXT_TICKET_STATUS				= _T("��Ʊ״̬");
static const TCHAR* const TXT_TICKET_STATUS_EN		    = _T("Ticket Status");
static const TCHAR* const TXT_SUGGESTION					= _T("������");
static const TCHAR* const TXT_SUGGESTION_EN				= _T("Suggestion");
static const TCHAR* const TXT_ENTER_STATION				= _T("��վ��վ");
static const TCHAR* const TXT_ENTER_STATION_EN           = _T("Entry Station");
static const TCHAR* const TXT_ENTER_STATION_TIME			= _T("��վʱ��");
static const TCHAR* const TXT_ENTER_STATION_TIME_EN      = _T("Entry Time");
static const TCHAR* const TXT_EXIT_STATION				= _T("��վ��վ");
static const TCHAR* const TXT_EXIT_STATION_EN            = _T("Exit Station");
static const TCHAR* const TXT_EXIT_STATION_TIME			= _T("��վʱ��");
static const TCHAR* const TXT_EXIT_STATION_TIME_EN		= _T("Exit Time");
static const TCHAR* const TXT_TRANSACTION_TIME			= _T("����ʱ��");
static const TCHAR* const TXT_TRANSACTION_TIME_EN        = _T("Transaction Time");
static const TCHAR* const TXT_TRANSACTION_LOCATION	    = _T("���׵ص�");
static const TCHAR* const TXT_TRANSACTION_LOCATION_EN    = _T("Transaction Location");
static const TCHAR* const TXT_TRANSACTION_TYPE			= _T("��������");
static const TCHAR* const TXT_TRANSACTION_TYPE_EN        = _T("Transaction Type");
static const TCHAR* const TXT_TRANSACTION_AMOUNT			= _T("���׽��");
static const TCHAR* const TXT_TRANSACTION_AMOUNT_EN      = _T("Transaction Amount");
static const TCHAR* const TXT_TRANSCATION_TIMES          =_T("���״���");
static const TCHAR* const TXT_RECENTLY_USE_TIME			= _T("���ʹ��ʱ��");
static const TCHAR* const TXT_RECENTLY_USE_STATUS		= _T("���ʹ�ó�վ");
static const TCHAR* const TXT_JOURNEYSTATUS_STATUS		= _T("�ó�״̬");
static const TCHAR* const TXT_ACTIVE_STATUS = _T("����״̬");
static const TCHAR* const TXT_HOLDER_NAME = _T("����");
static const TCHAR* const TXT_CERTIFATE_TYPE = _T("֤������");
static const TCHAR* const TXT_CERTIFATE_NUMBER = _T("֤������");

static const TCHAR* const TXT_UNKNOW_STATUS				= _T("δ֪");
static const TCHAR* const TXT_ENTERED_STATUS				= _T("��վ");
static const TCHAR* const TXT_EXITED_STATUS				= _T("��վ");
static const TCHAR* const TXT_UNKNOW_STATUS_EN			= _T("Unknow");
static const TCHAR* const TXT_TICKET_REGION = _T("Ʊ������");
static const TCHAR* const TXT_TICKET_LOCK_STATUS = _T("����״̬");
static const TCHAR* const TXT_TICKET_START_DATE = _T("��������");


// ��Ʊ/����
static const TCHAR* const TXT_ADJUST_TYPE				= _T("��Ʊ��ʽ");
static const TCHAR* const TXT_ADJUST_TYPE_EN				= _T("Adjust Type");
static const TCHAR* const TXT_ADJUST_AMOUNT				= _T("��Ʊ���");
static const TCHAR* const TXT_ADJUST_AMOUNT_EN           = _T("Adjust Amount");
static const TCHAR* const TXT_ACTUAL_ADJUST_AMOUNT       = _T("ʵ�ʲ�Ʊ���");
static const TCHAR* const TXT_ADJUST_RIDES				= _T("��Ʊ����");
static const TCHAR* const TXT_ACTUAL_ADJUST_RIDES		= _T("ʵ�ʲ�Ʊ����");
static const TCHAR* const TXT_ADJUST_RIDES_EN            = _T("Adjust Rides");
static const TCHAR* const TXT_UPDATE_REASON				= _T("��Ʊԭ��");
static const TCHAR* const TXT_UPDATE_REASON_EN           = _T("Reason for Updating");
static const TCHAR* const TXT_AMOUNT_DUE_VALUE			= _T("Ӧ�����");
static const TCHAR* const TXT_AMOUNT_DUE_VALUE_EN		= _T("Actual Amount");
static const TCHAR* const TXT_AMOUNT_DUE_RIDES			= _T("Ӧ������");
static const TCHAR* const TXT_AMOUNT_DUE_RIDES_EN		= _T("Actual Rides");

//����
static const TCHAR* const TXT_OLD_VALIDATE = _T("ԭ��Ч��");
static const TCHAR* const TXT_OLD_VALIDATE_EN = _T("Original Validity Period");
static const TCHAR* const TXT_NEW_VALIDATE = _T("����Ч��");
static const TCHAR* const TXT_NEW_VALIDATE_EN = _T("New Validity Period");

//����
static const TCHAR* const TXT_DEFER_DAYS = _T("�ӳ�ʱ��");
static const TCHAR* const TXT_DEFER_DAYS_EN = _T("Defer Days");
static const TCHAR* const TXT_SEVICE_FEE = _T("������");

// �˿� && TVM�˿���
static const TCHAR* const TXT_DEVICE_NUMBER				= _T("�豸���");
static const TCHAR* const TXT_FAILURE_NUMBER			    = _T("�˿�ƾ����");
static const TCHAR* const TXT_REFUND_MONEY				= _T("�˿���");
static const TCHAR* const TXT_REFUND_MONEY_EN			= _T("Refund Amount");
static const TCHAR* const TXT_REFUND_TYPE				= _T("�˿�����");
static const TCHAR* const TXT_REFUND_REASON				= _T("�˿�ԭ��");

// ��Ӫҵ����֧
static const TCHAR* const TXT_PROCESS_TYPE				= _T("�������");
static const TCHAR* const TXT_INCOME_TYPE				= _T("��֧���ͣ�");
static const TCHAR* const TXT_INCOME_REASON				= _T("��֧ԭ��");
static const TCHAR* const TXT_INCOME_VALUE				= _T("��֧��");
static const TCHAR* const TXT_INCOME_NOTE				= _T("��ע��");

// ����ͳ��
static const TCHAR* const TXT_BUSINESS					= _T("ҵ��");
static const TCHAR* const TXT_COUNT						= _T("����");
static const TCHAR* const TXT_INCOME_AMOUNT				= _T("���");

// ����������
static const TCHAR* const TXT_FIRST_APPLY				    = _T("����������");
static const TCHAR* const TXT_UPDATA_APPLY				    = _T("�ֿ�����Ϣ�޸�");
static const TCHAR* const TXT_CHECK_APPLY				    = _T("״̬��ѯ");
static const TCHAR* const TXT_USER_NAME				    	= _T("����");
static const TCHAR* const TXT_USER_SEX						= _T("�Ա�");
static const TCHAR* const TXT_USER_CERTIFICATE_TYPE			= _T("֤������");
static const TCHAR* const TXT_USER_CERTIFICATE_NUM			= _T("֤������");
static const TCHAR* const TXT_USER_CURRENT_STATE 			= _T("��ǰ״̬");
static const TCHAR* const TXT_USER_CERTIFICATE_VALIDATE		= _T("֤����Ч��");
static const TCHAR* const TXT_USER_PHONE						= _T("�绰");
static const TCHAR* const TXT_USER_EMAIL						= _T("Email");
static const TCHAR* const TXT_USER_FIRST_ADDR				= _T("��ַ1");
static const TCHAR* const TXT_USER_SECND_ADDE				= _T("��ַ2");


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

// ��������ͨҵ���水ť��ʾ����
static const TCHAR* const STR_OK									= _T("ȷ��");
static const TCHAR* const STR_PRINT								= _T("��ӡ");
static const TCHAR* const STR_BACK								= _T("����");
static const TCHAR* const STR_RESET								= _T("����");
static const TCHAR* const TXT_END_SVC                            = _T("ҵ�����");
static const TCHAR* const TXT_SHUTDOWN_SVC                       = _T("�ػ�");
static const TCHAR* const TXT_LOGIN_SVC                          = _T("��¼");
static const TCHAR* const TXT_REMOVE_INIT_EXCEPTION_SVC			= _T("�쳣���");
static const TCHAR* const TXT_ISSUE_SVC                          = _T("��Ʊ");
static const TCHAR* const TXT_SINGLE_RIDE_TICKET				    = _T("����Ʊ");
static const TCHAR* const TXT_EXIT_ONLY_TICKET					= _T("��վƱ");
static const TCHAR* const TXT_COMMEMORATICE_TICKET				= _T("����Ʊ");
static const TCHAR* const TXT_WELFARE_TICKET					    = _T("����Ʊ");
static const TCHAR* const TXT_ECT_TICKET							= _T("һ��ͨ");
static const TCHAR* const TXT_CHARGE_SVC                         = _T("��ֵ");
static const TCHAR* const TXT_RECHARGE_SVC                       = _T("����");
static const TCHAR* const TXT_ADDTIMES_SVC                       = _T("���");
static const TCHAR* const TXT_ANALYZE_SVC						= _T("����");
static const TCHAR* const TXT_ADJUST_SVC                         = _T("��Ʊ");
static const TCHAR* const TXT_ADJUST_SVC_PAIDAREA				= _T("������");
static const TCHAR* const TXT_ADJUST_SVC_NOPAIDAREA				= _T("�Ǹ�����");
static const TCHAR* const TXT_REFUND_MAIN						= _T("�˿�");
static const TCHAR* const TXT_SURRENDER_CARD						= _T("�˿�");
static const TCHAR* const TXT_REFUND_SVC                         = _T("����");
static const TCHAR* const TXT_REFUND_BASE_SVC					= _T("��ʱ�˿�");
static const TCHAR* const TXT_REFUND_APPLY_SVC					= _T("�˿�����");
static const TCHAR* const TXT_REFUND_QUERY_SVC					= _T("���˲�ѯ");
static const TCHAR* const TXT_EXCHANGE_SVC                       = _T("�滻");
static const TCHAR* const TXT_PRESELL_CANCEL_TICKET				= _T("Ԥ�۵���");
static const TCHAR* const TXT_PRESELL							= _T("Ԥ��");
static const TCHAR* const TXT_CANCEL_READ_TICKET_SVC             = _T("����");
static const TCHAR* const TXT_OTHERS_SVC                         = _T("����");
static const TCHAR* const TXT_TVMFAILURE_REFUND_SVC              = _T("TVM�����˿�");
static const TCHAR* const TXT_NOBUSINESS_INCOME_SVC				= _T("��Ӫҵ����֧");
static const TCHAR* const TXT_REFRESH_SVC						= _T("����");
static const TCHAR* const TXT_DEFER_SVC							= _T("����");
static const TCHAR* const TXT_SIGNCARD_APPLY_SVC					= _T("������ҵ��");
static const TCHAR* const TXT_SIGNCARD_REPORT_LOSS_SVC           = _T("��������ʧ");
static const TCHAR* const TXT_SIGNCARD_CANCEL_REPORT_LOSS_SVC	= _T("���������");
static const TCHAR* const TXT_CARD_SUSPEND_AND_RESUME_SVC		= _T("��ʧ/���");
static const TCHAR* const TXT_TICKETBOX_EXCHANGE_SVC = _T("Ʊ�����");
static const TCHAR* const TXT_ADMINISTRATION_PROCESS_SVC = _T("��������");
static const TCHAR* const TXT_LOCK_SVC = _T("����/����");
static const TCHAR* const TXT_ASSIST_MAIN						= _T("����");
static const TCHAR* const TXT_TH_MODE_SET_SVC					= _T("��Ʊģʽ�趨");
static const TCHAR* const TXT_CHANGE_PASSWORD_SVC                = _T("�޸�����");
static const TCHAR* const TXT_SALE_STATIC_SVC					= _T("����ͳ��");
static const TCHAR* const TXT_RECEIPT_PRINT_SVC					= _T("��ӡ����");
static const TCHAR* const TXT_LOGOFF_SVC                         = _T("�ǳ�");
static const TCHAR* const TXT_EXIT_BOM_SVC						= _T("�˳�����");

// ������ά��ҵ��ť��ʾ����(����)
static const TCHAR* const TXT_SYSTEM_SET_SVC                     = _T("ϵͳ�趨");
static const TCHAR* const TXT_STATION_SET_SVC					= _T("��ǰ��վ�趨");
static const TCHAR* const TXT_COMMUNICATION_PARAMETER_SET_SVC	= _T("ͨ�Ų����趨");
static const TCHAR* const TXT_MODE_SWITCH_SVC					= _T("�豸ģʽ�л�");
static const TCHAR* const TXT_SYSTEM_TIME_SET_SVC				= _T("ʱ���趨")  ;
static const TCHAR* const TXT_RFID_MODE_SET_SVC					= _T("RFIDģʽ�趨")  ;
static const TCHAR* const TXT_UDSN_SET_SVC						= _T("UDSN�趨");

static const TCHAR* const TXT_DATA_MANAGE_SVC					= _T("���ݹ���");
static const TCHAR* const TXT_DATA_INPUT_SVC						= _T("���ݵ���");
static const TCHAR* const TXT_DATA_OUTPUT_SVC					= _T("���ݵ���");
static const TCHAR* const TXT_DEVICE_TEST_SVC                    = _T("Ӳ���Լ�");
static const TCHAR* const TXT_OUTRW_TEST_SVC						= _T("��д������");
static const TCHAR* const TXT_PRINTER_TEST_SVC					= _T("��ӡ������");
static const TCHAR* const TXT_MONEYBOX_TEST_SVC					= _T("Ǯ�����");
static const TCHAR* const TXT_NETWORK_TEST_SVC					= _T("�������");
static const TCHAR* const TXT_UPS_INFO_SVC						= _T("UPS��Ϣ");
static const TCHAR* const TXT_TH_TEST_SVC						= _T("��Ʊ������");
static const TCHAR* const TXT_REMOVE_EXCEPTION_SVC				= _T("�쳣���");

// ��������ͨҵ���������ƶ���
static const TCHAR* const TXT_END_SVC_CN                            = _T("ҵ�����");
static const TCHAR* const TXT_SHUTDOWN_SVC_CN                       = _T("�ػ�");
static const TCHAR* const TXT_LOGIN_SVC_CN                          = _T("��¼");
static const TCHAR* const TXT_REMOVE_INIT_EXCEPTION_SVC_CN		   = _T("�쳣���");
static const TCHAR* const TXT_ISSUE_SVC_CN                          = _T("��Ʊ");
static const TCHAR* const TXT_ISSUE_SVC_EN                          = _T("ISSUE");
static const TCHAR* const TXT_CHARGE_SVC_CN                         = _T("��ֵ");
static const TCHAR* const TXT_RECHARGE_SVC_CN                       = _T("����");
static const TCHAR* const TXT_ADDTIMES_SVC_CN                       = _T("���");
static const TCHAR* const TXT_CHARGE_SVC_EN                         = _T("TCHARGE");
static const TCHAR* const TXT_ANALYZE_SVC_CN						   = _T("����");
static const TCHAR* const TXT_ANALYZE_SVC_EN						   = _T("ANALYZE");
static const TCHAR* const TXT_ADJUST_SVC_CN                         = _T("��Ʊ");
static const TCHAR* const TXT_ADJUST_SVC_PAIDAREA_CN				   = _T("������");
static const TCHAR* const TXT_ADJUST_SVC_NOPAIDAREA_CN			   = _T("�Ǹ�����");
static const TCHAR* const TXT_ADJUST_SVC_EN						   = _T("UPDATE");
static const TCHAR* const TXT_REFUND_MAIN_CN						   = _T("�˿�");
//static const TCHAR* const TXT_SURRENDER_CARD_CN					   = _T("�˿�");
//static const TCHAR* const TXT_REFUND_SVC_CN                         = _T("����");
static const TCHAR* const TXT_REFUND_SVC_EN                         = _T("REFUND");
static const TCHAR* const TXT_EXCHANGE_SVC_CN                       = _T("����");
static const TCHAR* const TXT_EXCHANGE_SVC_EN                       = _T("EXCHANGE");
static const TCHAR* const TXT_PRESELL_CANCEL_TICKET_CN			   = _T("Ԥ�۵���");
static const TCHAR* const TXT_PRESELL_CN							   = _T("Ԥ��");
static const TCHAR* const TXT_CANCEL_READ_TICKET_SVC_CN             = _T("����");
static const TCHAR* const TXT_OTHERS_SVC_CN                         = _T("����");
static const TCHAR* const TXT_TVMFAILURE_REFUND_SVC_CN              = _T("TVM�����˿�");
static const TCHAR* const TXT_NOBUSINESS_INCOME_SVC_CN			   = _T("��Ӫҵ����֧");
static const TCHAR* const TXT_REFRESH_SVC_CN						   = _T("���");
static const TCHAR* const TXT_SIGNCARD_APPLY_SVC_CN				   = _T("��������Ϣ");
static const TCHAR* const TXT_SIGNCARD_REPORT_LOSS_SVC_CN           = _T("��������ʧ");
static const TCHAR* const TXT_SUSPEND_AND_RESUME_SVC_CN			   = _T("�ֿ�����Ϣ");
static const TCHAR* const TXT_CHANGE_PASSWORD_SVC_CN                = _T("�޸�����");
static const TCHAR* const TXT_SALE_STATIC_SVC_CN					   = _T("����ͳ��");
static const TCHAR* const TXT_LOGOFF_SVC_CN                         = _T("�ǳ�");

// ������ά��ҵ�����ƶ���
static const TCHAR* const TXT_SYSTEM_SET_SVC_CN						= _T("ϵͳ�趨");
static const TCHAR* const TXT_STATION_SET_SVC_CN						= _T("��ǰ��վ�趨");
static const TCHAR* const TXT_COMMUNICATION_PARAMETER_SET_SVC_CN		= _T("ͨ�Ų����趨");
static const TCHAR* const TXT_MODE_SWITCH_SVC_CN						= _T("�豸ģʽ�л�");
static const TCHAR* const TXT_SYSTEM_TIME_SET_SVC_CN					= _T("ʱ���趨")  ;
static const TCHAR* const TXT_RFID_MODE_SET_SVC_CN					= _T("RFIDģʽ�趨")  ;
static const TCHAR* const TXT_UDSN_SET_SVC_CN						= _T("UDSN�趨");
static const TCHAR* const TXT_DATA_MANAGE_SVC_CN						= _T("���ݹ���");
static const TCHAR* const TXT_DATA_INPUT_SVC_CN						= _T("���ݵ���");
static const TCHAR* const TXT_DATA_OUTPUT_SVC_CN						= _T("���ݵ���");
static const TCHAR* const TXT_DEVICE_TEST_SVC_CN						= _T("Ӳ���Լ�");
static const TCHAR* const TXT_RW_TEST_SVC_CN							= _T("��д������");
static const TCHAR* const TXT_PRINTER_TEST_SVC_CN					= _T("��ӡ������");
static const TCHAR* const TXT_MONEYBOX_TEST_SVC_CN					= _T("Ǯ�����");
static const TCHAR* const TXT_NETWORK_TEST_SVC_CN					= _T("�������");
static const TCHAR* const TXT_TH_TEST_SVC_CN							= _T("��Ʊ������");
static const TCHAR* const TXT_REMOVE_EXCEPTION_SVC_CN				= _T("�쳣���");
static const TCHAR* const TXT_RESTART_SVC_CN							= _T("����");


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////�������Զ���///////////////////////////////////////////////////////////////
typedef enum _maintenance_msg{
	// �ƣ�磲��������
	GUIDE_SYSTEM_SET                                    = 20101001,     // ���ڣ���ѡ��Ҫ���еĴ���
	// ��������ѡ��
	//�ƣ�磲��������	
	GUIDE_MACHINE_PARAMETER_SET_SELECT                  = 20111001, 	// ���ڣ���ѡ��Ҫ�趨������

	// ��վ����
	//�ƣ�磲��������	
	GUIDE_STATION_SET_INFO_INPUT                        = 20112001, 	// ���ڣ������복վ��Ϣ������ȷ�ϼ�
	GUIDE_STATION_SET_INPUT_ERROR                       = 20112002, 	// ���죩����������д�����˶Ժ���������
	// ��վ���ý���
	//�ƣ�磲��������	
	GUIDE_STATION_SET_CONFIRM                           = 20113001, 	// ���ڣ���Ϣȷ������󣬰�ȷ�ϼ������޸�
	GUIDE_STATION_SET_FINISH                            = 20113002, 	// ���ڣ���վ��Ϣ������������
	GUIDE_STATION_SET_FARE_ERROR                        = 20113003, 	// ���죩���óɹ��������賵վ����Ʊ�۱���

	// UDSN����
	//Fig20119
	GUIDE_UDSN_SET_INFO_INPUT                           = 20119001,		// ���ڣ���������ˮ����Ϣ������ȷ�ϼ�
	GUIDE_UDSN_SET_INPUT_ERROR                          = 20119002, 	// ���죩����������д�����˶Ժ���������
	GUIDE_USDN_SET_FINISH                               = 20119003, 	// ���ڣ���ˮ����Ϣ������������

	// ��Ʊģʽ�趨
	GUIDE_TH_MODE_SET_INFO_INPUT                        = 20120001, 	// ���ڣ���ѡ���Ʊģʽ���࣬����ȷ�ϼ�
	GUIDE_TH_MODE_SET_INPUT_ERROR                       = 20120002, 	// ���죩����δ�����ݣ����ܽ���THģʽ�л�����
	GUIDE_TH_MODE_SET_FINISH                            = 20120003, 	// ���ڣ���Ʊģʽ�л�������������

	// RFIDд��ģʽ�趨
	GUIDE_RFID_MODE_SET_INFO_INPUT                        = 20122001, 	// ���ڣ���ѡ��RFIDд��ģʽ���࣬����ȷ�ϼ�
	GUIDE_RFID_MODE_SET_INPUT_ERROR                       = 20122002, 	// ���죩RFIDд��ģʽ�л������쳣
	GUIDE_RFID_MODE_SET_FINISH                            = 20122003, 	// ���ڣ�RFIDд��ģʽ�л�������������

	// ͨ������
	//�ƣ�磲��������	
	GUIDE_COMMUNICATION_SET_INFO_INPUT                  = 20114001, 	// ���ڣ����������¸�����
	GUIDE_COMMUNICATION_SET_INPUT_ERROR                 = 20114002, 	// ���죩����������д�����˶Ժ���������

	// ͨ�����ý���
	//�ƣ�磲��������	
	GUIDE_COMMUNICATION_SET_CONFIRM                     = 20115001, 	// ���ڣ���Ϣȷ��������밴ȷ�ϼ������޸�
	GUIDE_COMMUNICATION_SET_FINISH                      = 20115002, 	// ���ڣ�ͨ�Ų���������������
	GUIDE_COMMUNICATION_SET_IP_FAIL                     = 20115003, 	// ���죩�޸ı���IP��ַ����ʧ��
	GUIDE_COMMUNICATION_SET_ADAPTER_NOT_MATCH           = 20115004, 	// ���죩�������ò�����ʵ��ʹ�������������������ô���ʧ��

	// �豸ģʽ�л�
	//�ƣ�磲��������	
	GUIDE_MODE_SWITCH_SELECT                            = 20116001, 	// ���ڣ���ѡ���豸ģʽ���࣬����ȷ�ϼ�
	GUIDE_MODE_SWITCH_DATA_EXIST_ERROR                  = 20116002, 	// ���죩����δ�����ݣ����ܽ����豸ģʽ�л�����
	GUIDE_MODE_SWITCH_CONFIRM_OR_BACK                   = 20116003, 	// ���ڣ���Ϣȷ�Ϻ��밴ȷ�ϼ��������޸ģ��밴���ؼ�
	GUIDE_MODE_SWITCH_FINISH                            = 20116004, 	// ���ڣ��豸ģʽ�л�������������

	// RFID��д״̬�趨
	//�ƣ�磲��������	
	GUIDE_RFID_STATUS_SWITCH_SELECT                     = 20117001, 	// ���ڣ���ѡ��RFID��д״̬������ȷ�ϼ�����RFID��д״̬�趨
	GUIDE_RFID_STATUS_SWITCH_CONFIRM_BACK               = 20117002, 	// ���ڣ���Ϣȷ�Ϻ��밴ȷ�ϼ��������޸ģ��밴���ؼ�
	GUIDE_RFID_STATUS_SWITCH_FINISH                     = 20117003, 	// ���ڣ�RFID��д״̬�趨��������

	// һ��ͨ��������
	//�ƣ�磲��������
	GUIDE_YKT_PARA_SET_SELECT                           = 20118001, 	// ���ڣ����������¸�����
	GUIDE_YKT_PARA_SET_ERROR                            = 20118002, 	// ���죩����������д�����˶Ժ���������
	GUIDE_YKT_PARA_SET_CONFIRM                          = 20118003, 	// ���ڣ���Ϣȷ��������밴ȷ�ϼ������޸�
	GUIDE_YKT_PARA_SET_OVER                             = 20118004,     // ���ڣ�һ��ͨ������Ϣ������������


	// ��������
	//�ƣ�磲��������	
	GUIDE_DATA_INPUT_SELECT                             = 20121001, 	// ���ڣ���ȷ��Ҫ��������ݣ���ȷ�ϼ���ʼ
	//�ƣ�磲��������	
	GUIDE_DATA_INPUTING                                 = 20122001, 	// ���ڣ��������������У����Ժ򡭡�
	//�ƣ�磲��������	
	GUIDE_DATA_INPUT_FINISH                             = 20123001, 	// ���ڣ�����������������
	GUIDE_DATA_INPUT_COMMUNICATION_FAIL                 = 20123002, 	// ���죩��λͨ��ʧ�ܣ��޷������������봦��
	GUIDE_DATA_INPUT_NOT_INPUT                          = 20123003, 	// ���죩���������Ѿ������°汾����������������봦��
	GUIDE_DATA_INPUT_MDDIR_NOT_FIND                     = 20123004, 	// ���죩�ƶ����ʲ����ڣ���ȷ���ƶ�����������ȷ
	GUIDE_DATA_INPUT_DATA_ERROR                         = 20123005, 	// ���죩���ݲ����ڻ����𻵣��޷������������봦��
	GUIDE_DATA_INPUT_PATH_GET_ERROR                     = 20123006, 	// ���죩�޷�����ȡ��ϵͳ·��
	DUIDE_DATA_INPUT_FAIL                               = 20123007, 	// ���죩�������봦��ʧ��
	DUIDE_DATA_INPUT_FINISH_ACC                         = 20123008, 	// ���ڣ�����������������
	GUIDE_DATA_INPUT_PARA_INDEX_NOT_EXIST				= 20123009,		// ���죩���������ļ�������
	GUIDE_DATA_INPUT_PARA_INDEX_TYPE_ERROR				= 20123010,		// ���죩���������ļ����Ͳ���ȷ
	GUIDE_DATA_INPUT_PARA_INDEX_OTHER_ERROR				= 20123011,		// ���죩δ֪���󣬲��ܽ������ݵ���

	// �������
	//�ƣ�磲��������	
	GUIDE_DATA_OUTPUT_SELECT                            = 20131001, 	// ���ڣ���ѡ��Ҫ�������������������ʽ
	//�ƣ�磲��������	
	GUIDE_DATA_OUTPUTING                                = 20132001, 	// ���ڣ�������������У����Ժ򡭡�
	//�ƣ�磲��������	
	GUIDE_DATA_OUTPUT_FINISH                            = 20133001, 	// ���ڣ����������������
	GUIDE_DATA_OUTPUT_MDDIR_NOT_FIND                    = 20133002, 	// ���죩�ƶ����ʲ����ڣ���ȷ���ƶ�����������ȷ
	GUIDE_DATA_OUTPUT_PATH_GET_ERROR                    = 20133003, 	// ���죩�޷�����ȡ��ϵͳ·��
	GUIDE_DATA_OUTPUT_FAIL                              = 20133004, 	// ���죩�����������ʧ��
	GUIDE_DATA_OUTPUT_CREATE_PATH_FAIL					= 20133005,		// ���죩��������Ŀ¼ʧ��

	// �汾��Ϣ
	//�ƣ�磲��������	
	GUIDE_VERSION_CONFIRM                               = 20141001, 	// ���ڣ���Ϣȷ�Ϻ������ӡ���밴��ӡ��
	GUIDE_VERSION_CONFIRM_FINISH                        = 20141002, 	// ���ڣ��汾��Ϣȷ�Ͻ���

	// ��֤��ʽ����
	//�ƣ�磲��������	
	GUIDE_AUTHORIZE_MANNER_SEL                          = 20151001, 	// ���ڣ���ѡ��Ҫ���õ���֤�������֤��ʽ
	//�ƣ�磲��������	
	GUIDE_AUTHORIZE_MANNER_OVER                         = 20151002, 	// ���ڣ���֤��ʽ������������

	// ���к��趨
	//�ƣ�磲��������
	GUIDE_SN_SET_SEL                                    = 20161001, 	// ���ڣ���ֱ�����������кţ�����ȷ�ϼ�
	//�ƣ�磲��������
	GUIDE_SN_SET_OVER                                   = 20161003, 	// ���ڣ����к��趨��������

	// �ֶ���������
	//�ƣ�磲��������
	GUIDE_SLE_RECEIVE_SET                               = 20171001, 	// ���ڣ����������¸�����
	GUIDE_SLE_RECEIVE_ERROR                             = 20171002, 	// ���죩����������д�����˶Ժ���������
	GUIDE_SLE_RECEIVE_ING                               = 20171004,		// ���ڣ��ֶ������������ڷ����У����Ժ򡭡�
	GUIDE_SLE_RECEIVE_OVER                              = 20171005,		// ���ڣ��ֶ�����������������
	GUIDE_SLE_RECEIVE_FAIL                              = 20171006, 	// ���죩�ֶ��������÷���ʧ��

	// �ֶ����߱��
	//�ƣ�磲��������
	GUIDE_SLE_EXCHANGE_SET_OLD                            = 20181001,	// ���ڣ�����������ԭ�����豸������
	GUIDE_SLE_EXCHANGE_ERROR_OLD                          = 20181002, 	// ���죩����������д�����˶Ժ���������
	//�ƣ�磲��������
	GUIDE_SLE_EXCHANGE_SET_NEW                            = 20182001,     // ���ڣ������������»����豸������
	GUIDE_SLE_EXCHANGE_ERROR_NEW                          = 20182002,     // ���죩����������д�����˶Ժ���������
	GUIDE_SLE_EXCHANGE_ING                                = 20182004,     // ���ڣ��ֶ����߸������ڷ����У����Ժ򡭡�
	GUIDE_SLE_EXCHANGE_OVER                               = 20182005,     //  ���ڣ��ֶ����߸�����������
	GUIDE_SLE_EXCHANGE_FAIL                               = 20182006,     //  ���죩�ֶ����߸�������ʧ��
	// �������
	//�ƣ�磲��������	
	GUIDE_SYSTEM_UPDATE_FIRST_SELECT                    = 20201001, 	// ���ڣ���ѡ����Ҫ���µĳ���
	//�ƣ�磲��������	
	GUIDE_SYSTEM_UPDATE_SORT_METHOD_SELECT              = 20211001, 	// ���ڣ���ѡ��Ҫ���µĳ�����������뷽ʽ
	//�ƣ�磲��������	
	GUIDE_SYSTEM_UPDATING                               = 20212001, 	// ���ڣ����ڸ����У����Ժ򡭡�
	//�ƣ�磲��������	
	GUIDE_SYSTEM_UPDATE_FINISH                          = 20213001, 	// ���ڣ����������������
	GUIDE_SYSTEM_UPDATE_COMMUNICATION_FAIL              = 20213002, 	// ���죩��λͨ��ʧ�ܣ��޷����г�����´���
	GUIDE_SYSTEM_UPDATE_NOT_UPDATE                      = 20213003, 	// ���죩���س����Ѿ������°汾��������г�����´���
	GUIDE_SYSTEM_UPDATE_MDDIR_NOT_FIND                  = 20213004, 	// ���죩�ƶ����ʲ����ڣ���ȷ���ƶ�����������ȷ
	GUIDE_SYSTEM_UPDATE_FILE_ERROR                      = 20213005, 	// ���죩�����ļ������ڻ����𻵣��޷������������봦��
	GUIDE_SYSTEM_UPDATE_PATH_NOT_FIND                   = 20213006, 	// ���죩�޷�����ȡ��ϵͳ·��
	GUIDE_SYSTEM_UPDATE_FAIL                            = 20213007, 	// ���죩������´���ʧ��
	//�ƣ�磲��������	
	GUIDE_SYSTEM_UPDATE_CONFIRM                         = 20221001, 	// ���ڣ���Ϣȷ�Ϻ������ӡ���밴��ӡ��
	GUIDE_SYSTEM_UPDATE_CONFIRM_FINISH                  = 20221002, 	// ���ڣ��汾��Ϣȷ�Ͻ���

	// ���ݽ���
	//�ƣ�磲��������	
	GUIDE_SETTLEMENT_SELECT                             = 20301001, 	// ���ڣ���ѡ����ʽ
	GUIDE_SETTLEMENT_OUTPUT_WITHOUT_DATA                = 20301002, 	// ���ڣ������ڿ�����Ľ�������
	//�ƣ�磲��������	
	GUIDE_SETTLEMENT_DATA_OUTPUTING                     = 20302001, 	// ���ڣ�������������У����Ժ򡭡�
	//�ƣ�磲��������	
	GUIDE_SETTLEMENT_OUTPUT_FINISH                      = 20303001, 	// ���ڣ����������������
	GUIDE_SETTLEMENT_CLEAR_FINISH                       = 20303002, 	// ���ڣ����������������
	GUIDE_SETTLEMENT_CLEARING                           = 20303003, 	// ���ڣ���������У����Ժ򡭡�
	GUIDE_SETTLEMENT_MDDIR_NOT_FIND                     = 20303004, 	// ���죩�ƶ����ʲ����ڣ���ȷ���ƶ�����������ȷ
	GUIDE_SETTLEMENT_OUTPUT_FAIL                        = 20303005,	    // ���죩�����������ʧ��
	GUIDE_SETTLEMENT_CLEAR_FAIL                         = 20303006, 	// ���죩������ݴ���ʧ��
	//�ƣ�磲��������	
	GUIDE_SETTLEMENT_CLEAR_SELECT                       = 20304001, 	// ���ڣ���ѡ����ʽ

	// ��־����
	//�ƣ�磲��������	
	GUIDE_LOG_MANAGER_SELECT                            = 20401001, 	// ���ڣ���ѡ����־����ʹ���ʽ
	//�ƣ�磲��������	
	GUIDE_LOG_MANAGER_OUTPUTING                         = 20402001, 	// ���ڣ�������������У����Ժ򡭡�
	//�ƣ�磲��������	
	GUIDE_LOG_MANAGER_OUTPUT_FINISH                     = 20403001, 	// ���ڣ���־�����������
	GUIDE_LOG_MANAGER_CLEAR_FINISH                      = 20403002, 	// ���ڣ���־�����������
	GUIDE_LOG_MANAGER_NO_OUTPUT_FILE                    = 20403003, 	// ���죩û�п����������־�ļ�
	GUIDE_LOG_MANAGER_MDDIR_NOT_FIND                    = 20403004, 	// ���죩�ƶ����ʲ����ڣ���ȷ���ƶ�����������ȷ
	GUIDE_LOG_MANAGER_PATH_GET_ERROR                    = 20403005, 	// ���죩�޷������õ�ϵͳ·��
	GUIDE_LOG_MANAGER_OUTPUT_FAIL                       = 20403006, 	// ���죩��־������ƶ����ʴ���ʧ��
	GUIDE_LOG_MANAGER_PRINT_FAIL                        = 20403007, 	// ���죩��־�������ӡ������ʧ��
	GUIDE_LOG_MANAGER_CLEAR_FAIL                        = 20403008, 	// ���죩��־�������ʧ��
	//�ƣ�磲��������	
	GUIDE_LOG_MANAGER_CLEAR_SELECT                      = 20404001, 	// ���ڣ���ѡ����־����ʹ���ʽ

	// Ӳ���Լ�
	//�ƣ�磲��������	
	GUIDE_DEVICE_TEST_CHOOSE_HARDWARE                   = 20501001, 	// ���ڣ���ѡ����Ҫ�Լ��Ӳ��

	// ��Ʊ��
	//�ƣ�磲��������	
	GUIDE_TH_SELECT_ITEM                                = 20511001, 	// ���ڣ���ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_TH_LOG_OUTPUTING                              = 20511002, 	// ���ڣ�������־����У����Ժ򡭡�
	GUIDE_TH_LOG_OUTPUT_FINISH                          = 20511003, 	// ���ڣ�������־�����������ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_TH_MOVE_DISK_NOT_HAVE                         = 20511004, 	// ���죩�ƶ����ʲ����ڣ���ȷ���ƶ�����������ȷ
	GUIDE_TH_LOG_CLEARED_CONFIRM                        = 20511005, 	// ���ڣ�ȫ��������־ɾ����������ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_TH_LOG_OUTPUT_ERROR                           = 20511006, 	// ���ڣ�������־�������ʧ��
	//�ƣ�磲��������	
	GUIDE_TH_UNIT_TEST_CHOOSE_ITEM                      = 20512001, 	// ���ڣ���ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	//�ƣ�磲��������	
	GUIDE_TH_INPUT_SERIAL_NUMBER                        = 20513001, 	// ���ڣ���������Ҫɾ���Ĺ�����־���
	GUIDE_TH_INPUT_ERROR                                = 20513002, 	// ���죩����������д�����˶Ժ���������
	GUIDE_TH_LOG_DELETING                               = 20513003, 	// ���ڣ�ָ��������־ɾ���У����Ժ򡭡�
	GUIDE_TH_LOG_DELETE_FINISH                          = 20513004, 	// ���ڣ�ָ��������־ɾ������
	//�ƣ�磲��������	
	GUIDE_TH_ALL_LOG_CHOOSE_ITEM                        = 20514001, 	// ���ڣ���ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_TH_ALL_LOG_DELETING                           = 20514002, 	// ���ڣ�������־ɾ���У����Ժ򡭡�
	//�ƣ�磲��������	
	GUIDE_TH_TESTING                                    = 20515001, 	// ���ڣ������У����Ժ򡭡�
	GUIDE_TH_OPEN                                       = 20515002, 	// ���ڣ���򿪳�Ʊ��
	GUIDE_TH_TEST_SUCCESS                               = 20515003, 	// ���ڣ�ѡ��ĳ�Ʊ����Ŀ���Գɹ�
	
	GUIDE_TH_TEST_CHOOSE_ITEM							= 20515004, 	// ���ڣ���ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_TH_TEST_FAIL									= 20515005, 	// ���죩��Ŀ����ʧ��
	GUIDE_TH_TEST_SUCCESS_NEW							= 20515006, 	// ���ڣ���Ŀ���Գɹ�

	// �ⲿIC��д��
	//�ƣ�磲��������	
	GUIDE_IC_PUT_CARD									= 20521001, 	// ���ڣ��뽫��Ƭ���ڶ�������
	GUIDE_IC_RESET_CHOOSE_ITEM                          = 20521002, 	// ���ڣ���д�����óɹ�����ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_IC_RESET_ERROR                                = 20521003, 	// ���죩��д�����ò���ʧ��
	//�ƣ�磲��������	
	GUIDE_IC_READ_WAITING                               = 20522001, 	// ���ڣ��ȴ���ȡ�С���
	GUIDE_IC_WRITE_SUCCEED								= 20522002,		// ���ڣ���д����Գɹ�
	GUIDE_IC_WRITE_FAIL									= 20522003,		// ���죩��д�����ʧ��
	//�ƣ�磲��������	
	GUIDE_IC_READ_SUCCESS                               = 20523001, 	// ���ڣ�����ȡ���Գɹ�
	GUIDE_IC_READ_FAIL                                  = 20523002, 	// ���죩����ȡ����ʧ��
	GUIDE_IC_UNKNOWN_CARD								= 20523003,		// ���죩����ʶ���Ʊ��

	GUIDE_RW_TEST_CHOOSE_ITEM							= 20524001,		// ���ڣ���ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_RW_TEST_SUCCESS								= 20524002,		// ���ڣ�������ɣ���ȷ�ϲ��Խ��
	GUIDE_RW_TEST_FAIL									= 20524003,		// ���죩����ʧ�ܣ�����ԭ��

	// ��ӡ��
	//�ƣ�磲��������	
	GUIDE_PRINTER_CHOOSE_ITEM                           = 20531001, 	// ���ڣ��밴ȷ�ϼ���ʼ����
	GUIDE_PRINTER_TESTING                               = 20531002, 	// ���ڣ������У����Ժ򡭡�
	GUIDE_PRINTER_TEST_FINISH                           = 20531003, 	// ���ڣ���ӡ���Խ�������ȷ���豸����

	// Ǯ��
	//�ƣ�磲��������	
	GUIDE_CASH_BOX_CHOOSE_ITEM                          = 20541001, 	// ���ڣ��밴ȷ�ϼ���ʼ����
	GUIDE_CASH_BOX_TESTING                              = 20541002, 	// ���ڣ������У����Ժ򡭡�
	GUIDE_CASH_BOX_TEST_FINISH                          = 20541003, 	// ���ڣ�Ǯ��򿪲��Խ�������ȷ���豸����

	// ����
	//�ƣ�磲��������	
	GUIDE_NET_TEST_CHOOSE_ITEM                          = 20551001, 	// ���ڣ���ѡ����Ҫ���Ե���Ŀ����ȷ�ϼ���ʼ
	GUIDE_NET_TESTING                                   = 20551002, 	// ���ڣ������У����Ժ򡭡�
	GUIDE_SC_INFO_CONFIRM                               = 20551003, 	// ���ڣ���ȷ��SC����������Ϣ
	GUIDE_FTP_DOWNLOAD_SUCCESS                          = 20551004, 	// ���ڣ�FTP���ز��Գɹ�
	GUIDE_FTP_UPLOAD_SUCCESS                            = 20551005, 	// ���ڣ�FTP�ϴ����Գɹ�
	GUIDE_FTP_CONNECT_FAIL                              = 20551006, 	// ���죩FTP����ʧ�ܣ�����������������
	GUIDE_FTP_UPLOAD_FAIL                               = 20551007, 	// ���죩FTP�ϴ�����ʧ�ܣ�������ز����趨
	GUIDE_FTP_DOWNLOAD_FAIL                             = 20551008, 	// ���죩FTP���ز���ʧ�ܣ�������ز����趨
	GUIDE_CONNECTION_CLOSE_FAIL                         = 20551009, 	// ���죩FTP�ر�����ʧ�ܣ�����������������
	GUIDE_PASSWORD_FAIL                                 = 20551010, 	// ���죩FTP�û���������������������ز���

	// ������
	//�ƣ�磲��������	
	GUIDE_TOUCH_SCREEN_CLICK                            = 20561001, 	// ���ڣ��밴��ͼ����ʾ��ť�󣬰�ȷ�ϼ�����ȷ��
	GUIDE_TOUCH_SCREEN_UPLEFT_ERROR                     = 20561002, 	// ���ڣ����������ϲఴť���������������������������
	GUIDE_TOUCH_SCREEN_UPRIGHT_ERROR                    = 20561003, 	// ���ڣ����������ϲఴť���������������������������
	GUIDE_TOUCH_SCREEN_CENTER_ERROR                     = 20561004, 	// ���ڣ����������밴ť���������������������������
	GUIDE_TOUCH_SCREEN_DOWNLEFT_ERROR                   = 20561005, 	// ���ڣ����������²ఴť���������������������������
	GUIDE_TOUCH_SCREEN_DOWNRIGHT_ERROR                  = 20561006, 	// ���ڣ����������²ఴť���������󣬴��������Խ���
	GUIDE_TOUCH_SCREEN_TEST_FINISH                      = 20561007, 	// ���ڣ����������Խ���

	// UPS	
	GUIDE_UPS_INFO_READ_WAITING							= 20571001, 	// ���ڣ��ȴ���ȡ�С���
	GUIDE_UPS_INFO_READ_FAIL							= 20571002, 	// ���죩UPS��Ϣ��ȡʧ��
	GUIDE_UPS_INFO_READ_SUCCESS							= 20571003, 	// ���ڣ�UPS��Ϣ��ȡ�ɹ�

	// �ⲿý��ҵ��
	//�ƣ�磲��������	
	GUIDE_OUT_DEVICE_SELECT                             = 20601001, 	// ���ڣ���ѡ����Ҫ��ȡ���ⲿý��
	GUIDE_USB_REMOVE_SUCCESS                            = 20601002, 	// ���ڣ�USB�豸��ȫ�Ƴ������԰�ȡ
	GUIDE_CF_REMOVE_SUCCESS                             = 20601003, 	// ���ڣ�CF����ȫ�Ƴ�
	GUIDE_USB_REMOVE_ERROR                              = 20601004, 	// ���ڣ�δ�ܰ�ȫ�Ƴ�USB�豸
	GUIDE_CF_REMOVE_ERROR                               = 20601005,  	// ���ڣ�δ�ܰ�ȫ�Ƴ�CF��
	GUIDE_CF_FORMAT_SUCCESS                             = 20601006,     // ���ڣ���ʽ��CF����������

	// �쳣���ҵ��
	//�ƣ�磲��������	
	GUIDE_REMOVE_EXCEPTION                              = 20701001, 	// ���ڣ���ȷ�������쳣��Ϣ����ȷ�ϼ�����쳣

	// �����޸�ҵ��
	// �ƣ�磲��������
	GUIDE_REPAIR_DATABASE_SELECT                        = 20801001,     //  (��)��ѡ��Ҫ���еĴ���,�޸��������ݿ���ж�����λ������
	GUIDE_DB_OUTPUT_NOFILE                              = 20801002,     //  (��)���ݿ��ļ������ڻ����𻵣��޷��������ݿ⵼������
	GUIDE_DB_OUTPUTING                                  = 20801003,     //  (��)���ݿ⵼���У����Ժ򡭡� 
	GUIDE_DB_OUTPUT_SUCCESS                             = 20801004,     //  (��)���ݿ⵼����������
	GUIDE_DB_OUTPUT_FAIL                                = 20801005,     //  (��)���ݿ⵼��ʧ��
	GUIDE_DB_REPAIRING                                  = 20801006,     //  (��)���ݿ��޸��У����Ժ򡭡�
	GUIDE_DB_REPAIR_SUCCESS                             = 20801007,     //  (��)���ݿ��޸������������밴ȷ�ϼ���������
	GUIDE_DB_REPAIR_FAIL                                = 20801008,     //  (��)���ݿ��޸�ʧ�ܣ�����ϵά����Ա
	GUIDE_DB_INPUT_NOFILE                               = 20801009,     //  (��)���ݿ��ļ������ڻ����𻵣��޷��������ݿ⵼�봦��
	GUIDE_DB_INPUTING                                   = 20801010,     //  (��)���ݿ⵼���У����Ժ򡭡�
	GUIDE_DB_INPUT_SUCCESS                              = 20801011,     //  (��)���ݿ⵼�������������밴ȷ�ϼ���������
	GUIDE_DB_INPUT_FAIL                                 = 20801012,     //  (��)���ݿ⵼��ʧ��,����ϵά����Ա
	GUIDE_DB_OUTPUT_FIRST                               = 20801013,     // ����)�����ݿ��ļ���δ���������ܽ����޸��������ݿ⴦�� 
	GUIDE_DB_REPAIR_SUCCESS_NO_RESTART                  = 20801014,     //  (��)���ݿ��޸��ɹ�������λ���Ӵ��ڶϿ�״̬
	GUIDE_DB_INPUT_SUCCESS_NO_RESTART                   = 20801015,     //  (��)�������ݿ�ɹ�������λ���Ӵ��ڶϿ�״̬
	GUIDE_DB_CF_CARD_ERROR                              = 20801016,     // ����)�������ݿ⴦��ʧ�ܣ�����CF��
	//�ƣ�磲��������	
	GUIDE_MAINTENANCE_LOGOUT                            = 20901001 	    // ���ڣ����ڵǳ������Ժ򡭡�

}MAINTENANCE_MSG;

typedef enum _passenger_msg{
	// ��Ʊ
	// �ƣ�磱��������
	GUIDE_BASE_CONFIRM                                  = 10102001,     // ���ڣ���ȷ��������ʾ��Ϣ
	GUIDE_BASE_CONFIRM_UL                               = 10102002,     // ���ڣ���ȷ��������ʾ��Ϣ    
	// �ƣ�磱��������	
	GUIDE_ISSUING_PASSENGER                             = 10103001,  	// ���ڣ���Ʊ�����У����Ժ򡭡�
	// �ƣ�磱��������	
	GUIDE_ISSUE_FINISH_PASSENGER                        = 10104001,  	// ���ڣ���Ʊ���н�����лл����
	GUIDE_ISSUE_ERROR_PASSENGER                         = 10104002,     // ���ڣ��ó�Ʊ���ܽ��з��д���
	GUIDE_ISSUE_CARD_NOTFOUND_PASSENGER                 = 10104003,     // ���ڣ���Ʊ���н���

	// ��Ʊ
	// �ƣ�磱��������	
	GUIDE_ADJUST_CONFIRM_PASSENGER                      = 10202001, 	// ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_ADJUST_PAIDAREA_FINISH_PASSENGER              = 10203001, 	// ���ڣ���Ʊ�����������Ӽ�Ʊ����վ��лл����
	GUIDE_ADJUST_UNPAIDAREA_FINISH_PASSENGER            = 10203002, 	// ���ڣ���Ʊ���������лл����
	GUIDE_ADJUST_ERROR_PASSENGER                        = 10203003, 	// ���ڣ��ó�Ʊ���ܽ��в�Ʊ����
	GUIDE_ADJUST_CARD_NOTFOUND_PASSENGER                = 10203004,     // ���ڣ���Ʊ�������

	// ��ֵ
	// �ƣ�磱��������	
	GUIDE_CHARGE_CONFIRM_PASSENGER                      = 10302001, 	 // ���ڣ���ȷ��������ʾ��Ϣ��
	// �ƣ�磱��������	
	GUIDE_CHARGE_FINISH_PASSENGER                       = 10303001, 	 // ���ڣ���ֵ���������лл����
	GUIDE_CHARGE_NOT_PASSENGER                          = 10303002, 	 // ���ڣ��ó�Ʊ���ܽ��г�ֵ����
	GUIDE_CHARGE_CARD_NOTFOUND_PASSENGER                = 10303003,      // ���ڣ���ֵ�������
	
	// �滻
	// �ƣ�磱��������	
	GUIDE_EXCHANGE_CONFIRM_PASSENGER                    = 10502001,     // ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������
	GUIDE_EXCHANGE_FINISH_PASSENGER                     = 10503001,     // ���ڣ��滻���������лл����
	GUIDE_EXCHANGE_NOT_PASSENGER                        = 10503002,     // ���ڣ��ó�Ʊ���ܽ����滻����
	GUIDE_EXCHANGE_CARD_NOREAD_PASSENGER                = 10503003,     // ���ڣ��滻�������

	// �˿�
	// �ƣ�磱��������	
	GUIDE_REFUND_CONFIRM_PASSENGER                      = 10602001,     // ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_REFUND_FINISH_PASSENGER                       = 10603001,     // ���ڣ��˿�������лл����
	GUIDE_REFUNDING_PASSENGER							= 10603002,		// ���ڣ��˿���У����Ժ򡭡�
	GUIDE_REFUND_NOT_PASSENGER                          = 10603003,     // ���ڣ��ó�Ʊ���ܽ����˿��
	GUIDE_REFUND_CARD_NOTFOUND_PASSENGER                = 10603004,     // ���ڣ��˿�����
	GUIDE_REFUND_CPU_NOT_PASSENGER                      = 10603005,     // ���ڣ��ó�Ʊ���ܽ��м�ʱ�˿��
	GUIDE_REFUND_ERROR_PASSENGER						= 10603006,     // ���ڣ��˿���������˿�ʧ��

	// ����
	// �ƣ�磱��������	
	GUIDE_REFRESH_CONFIRM_PASSENGER                     = 10702001,     // ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_REFRESH_FINISH_PASSENGER                      = 10703001, 	// ���ڣ�����������лл����
	GUIDE_REFRESH_NOT_PASSENGER                         = 10703002, 	// ���ڣ��ó�Ʊ���ܽ��м����
	GUIDE_REFRESH_CARD_NOTFOUND_PASSENGER               = 10703003,     // ���ڣ���������

	// ����
	// �ƣ�磱��������
	GUIDE_ANALYZE_CONFIRM_PASSENGER                     = 10802001, 	// ���ڣ���ȷ��������ʾ��Ϣ
	GUIDE_ANALYZE_FAILED_PASSENGER                      = 10802002,     // ���ڣ��ó�Ʊ���ܽ��з�������
	GUIDE_ANALYZE_CARD_NOTFOUND_PASSENGER               = 10802003,     // ���ڣ������������
	// �ƣ�磱��������	
	GUIDE_ANALYZE_HISTORY_CONFIRM_PASSENGER             = 10803001,	    // ���ڣ���ȷ��������ʾ��Ϣ
	
	// ����
	// �ƣ�磱��������	
	GUIDE_REVERSE_CONFIRM_PASSENGER                     = 11202001,	    // ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_REVERSE_FINISH_PASSENGER                      = 11203001, 	// ���ڣ����˴��������лл����
	GUIDE_REVERSE_ERROR_PASSENGER                       = 11203002,     // ���ڣ��ó�Ʊ���ܽ��л��˴���

	// ����
	// �ƣ�磱��������	
	GUIDE_BLOCK_CONFIRM_PASSENGER                       = 11302001, 	// ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_BLOCK_FINISH_PASSENGER                        = 11303001,	    // ���ڣ��������������лл����
	GUIDE_BLOCK_ERROR_PASSENGER                         = 11303002,     // ���ڣ��ó�Ʊ���ܽ�����������
	GUIDE_BLOCK_CARD_NOTFOUND_PASSENGER                 = 11303003,     // ���ڣ������������

	// ��Ʒɾ��
	// �ƣ�磱��������	
	GUIDE_PRODUCT_DELETE_CONFIRM_PASSENGER              = 11502001, 	// ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_PRODUCT_DELETE_FINISH_PASSENGER               = 11503001, 	// ���ڣ���Ʒɾ�����������лл����
	GUIDE_PRODUCT_DELETE_NOT_PASSENGER                  = 11503002,     // ���ڣ��ó�Ʊ���ܽ���ɾ������

	// TVM�����˿�
	// �ƣ�磱��������	
	GUIDE_TVMFAILURE_CONFIRM_PASSENGER                  = 11601001, 	// ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磱��������	
	GUIDE_TVMFAILURE_REFUND_FINISH_PASSENGER            = 11602001,      // ���ڣ��˿������������վ���ǩ����лл����

	// ����
	GUIDE_DEFER_CONFIRM_PASSENGER                     = 11702001,     // ���ڣ���ȷ��������ʾ��Ϣ
	GUIDE_DEFER_FINISH_PASSENGER                      = 11703001, 	// ���ڣ�����������лл����
	GUIDE_DEFER_NOT_PASSENGER                         = 11703002, 	// ���ڣ��ó�Ʊ���ܽ��м����
	GUIDE_DEFER_CARD_NOTFOUND_PASSENGER               = 11703003,     // ���ڣ���������

	GUIDE_CANCEL             						  = 4000000,//���ڣ�������ȡ��
	GUIDE_BLACK	            						  = 4000001,//���죩������������������

}PASSENGER_MSG;

typedef enum _operation_msg{
	// �հ�����
	GUIDE_NULL                                          = 1000,       
	// ��������
	// �ƣ�磰��������
	GUIDE_INIT_WAIT                                     = 1001,         // ���ڣ����ڳ�ʼ��ϵͳ�����Ժ򡭡� 
	GUIDE_INIT_NEW_PARAM                                = 1002,         // ���ڣ�ϵͳ�����Ѹ��£�����������Ч
	GUIDE_INIT_ERROR                                    = 1003,         // ���죩��������ʧ�ܣ��ػ���������������ά����Ա��ϵ
	GUIDE_INIT_ACCREDIT_ERROR                           = 1004,         // ���죩���ڻ���֤ʧ��
	// ����Ա��¼
	// �ƣ�磰�������� 
	GUIDE_LOGIN_INPUT_ID_AND_PWD                        = 2001,         // ���ڣ����������ԱID�����벢ѡ���¼ҵ��
	GUIDE_LOGIN_LOGINNING	                            = 2002,         // ���ڣ����ڵ�¼�����Ժ򡭡�
	GUIDE_LOGIN_PARAM_SYNING                            = 2004,         // ���ڣ����ڽ��в���ͬ�������Ժ򡭡�
	GUIDE_LOGIN_UNAUTHORIZED                            = 2003,         // ���죩�ò���Աû��ȡ����Ȩ����˶Ժ���������
	GUIDE_LOGIN_PWD_FAIL                                = 2005,         // ���죩�������������˶Ժ���������
	GUIDE_LOGIN_FAIL                                    = 2006,         // ���죩�أأأ��쳣�����������¼ά������
	GUIDE_LOGIN_MAINTAIN_FAIL                           = 2007,         // ���죩�أأأ��쳣���ػ�����������
	GUIDE_LOGIN_REST                                    = 2008,         // ���ڣ���Ϣ��
	GUIDE_LOGIN_EMERGENCY                               = 2009,         // ���죩��ǰģʽ���ܵ�¼�����Ժ��ٵ�¼
	GUIDE_LOGIN_ALREADY_LOGIN                           = 2010,         // ���죩��ǰ����Ա���������豸��¼
	GUIDE_LOGIN_PWD_FAIL_MAX                            = 2011,         // ���죩��������������������Ѿ������涨����
	GUIDE_LOGIN_CLOSE                                   = 2012,         // ���ڣ����ڽ���ҵ������������Ժ򡭡�
	GUIDE_LOGIN_SHUTDOWN                                = 2013,         // ���ڣ����ڹػ������Ժ򡭡�
	GUIDE_LOGIN_UPDATE_HARDWARE                         = 2014,         // ���ڣ���������У����Ժ򡭡�
	GUIDE_LOGIN_MACHINE_ERROR                           = 2015,         // ���죩�豸��ʼ���쳣���ػ�����������
	GUIDE_LOGIN_UPDATE_SUCCESS                          = 2016,         // ���ڣ�������½���
	GUIDE_LOGIN_UPDATE_FAIL                             = 2017,         // ���죩�أأأس������ʧ��
	GUIDE_LOGIN_REDIT_FAIL                              = 2018,         // ���죩��֤ʧ�ܣ��أأأأ�
	GUIDE_LOGIN_REDIT_SUCCESS                           = 2019,         // ���ڣ���֤�ɹ�
	GUIDE_LOGIN_REDITING                                = 2020,         // ���ڣ�������֤�����Ժ򡭡�
	GUIDE_LOGIN_DEVICEREGISTER_FAIL                     = 2021,         // ���죩��������ʧ��
	GUIDE_LOGIN_DEVICEEXCHANGE_FAIL                     = 2022,         // ���죩���߱��ʧ��
	GUIDE_LOGIN_NO_SAM                                  = 2023,         // ���죩SAM�������ڣ����ܽ�����֤����
	GUIDE_LOGIN_UPDATE_PARAMETER                        = 2024,         // ���죩���������У����Ժ��ٵ�¼
	GUIDE_LOGIN_ACC_UPDATE_FAIL                         = 2025,         // ���죩���ãò�������ʧ��
	GUIDE_LOGIN_SC_NO_RESPONSE							=  2026,		 // ���죩����æ��������
	GUIDE_LOGIN_EXITBOM									= 2027,			// ���ڣ������˳��������Ժ�
	
	GUIDE_LOGIN_UNKNOW_RESPONSE							=  2031,		 // ���죩δ֪����
	GUIDE_LOGIN_REQUEST_SERVER_FAILED					=  2032,		 // ���죩�������ʧ��
	GUIDE_LOGIN_HAS_EXCEPTION 							=  2033,		 // ���죩�쳣�У���ֹ��½
	GUIDE_LOGIN_CANNOT_USE_SUPEROPERATOR				=  2034,		 // ���죩�����û�Ȩ��û�зſ���
	GUIDE_LOGIN_OK										=  2035,		 // ���ڣ������¼
	GUIDE_LOGIN_ID_PASSWORD_NOT_EXITS					=  2036,		 // ���죩�û��������ڻ��������
	GUIDE_LOGIN_REPEAT_LOGIN							=  2037,		 // ���죩�ܾ���¼���ظ���¼��
	GUIDE_LOGIN_OK_PASSWORD_WILL_EXPIRED				=  2038,		 // ���ڣ������¼�����뼴�����ڣ�
	GUIDE_LOGIN_OK_CHANGE_PASSWORD						=  2039,		 // ���ڣ������¼��ǿ���޸����룩
	GUIDE_LOGOUT_OK										=  2040,		 // ���ڣ�����ǳ�
	GUIDE_LOGOUT_REFUSE									=  2041,		 // ���죩�ܾ��ǳ�
	GUIDE_LOGIN_ID_LOCKED								=  2042,		 // ���죩�ܾ���¼���˻�������
	GUIDE_LOGIN_PASSWORD_STOPED							=  2043,		 // ���죩�ܾ���¼��������ֹ��
	GUIDE_LOGIN_ID_STOPED								=  2044,		 // ���죩�ܾ���¼���˻����ã�
	GUIDE_LOGIN_ID_PASSWORD_EMPTY						=  2045,		 // ���죩�û���������Ϊ��
	// ��Ʊ
	// �ƣ�磰��������
	GUIDE_ISSUE_NEW_CARD_SELET_INFO                     = 101001,       // ���ڣ���ѡ����Ҫ����ĳ�Ʊ��Ϣ                       
	// �ƣ�磰��������
	GUIDE_ISSUE_OLD_CARD_SELET_INFO                     = 102001,       // ���ڣ���ѡ����Ҫ����ĳ�Ʊ��Ϣ
	// �ƣ�磰��������
	GUIDE_ISSUE_PUT_CARD                                = 103001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	GUIDE_ISSUE_PRINT                                   = 103002, 	    // ���ڣ�����ƾ֤���������տ���󰴴�ӡ��
	GUIDE_ISSUE_INPUT_CASH_ERROR                        = 103003,       // ���죩����������д�����˶Ժ���������
	GUIDE_ISSUE_ECT_MONEY_NOT_ENOUGH                    = 103004, 	    // ���죩�״γ�ֵ���㣬��˶Ժ���������
	GUIDE_ISSUE_ECT_MONEY_NOT_BASE_AMOUNT               = 103005, 	    // ���죩��ֵ���Ƿ����������������ĳ�ֵ���
	GUIDE_ISSUE_ECT_MONEY_OVER_MAX                      = 103006,       // ���죩��ֵ��������޶��˶Ժ���������
	GUIDE_ISSUE_ECT_BALANCE_OVER_MAX                    = 103007, 	    // ���죩��ֵ��������������������˶Ժ���������
	GUIDE_ISSUE_ECT_AUTHORITY_MONEY_NOT_ENOUGH          = 103008,       // ���죩��Ȩ���㣬��˶Ժ�������������ǩ��ǩ��
	GUIDE_ISSUE_CONFIRM_INFO                            = 103009,       // ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磰��������
	GUIDE_ISSUING                                       = 104001,       // ���ڣ���Ʊ�����У����Ժ򡭡�
	GUIDE_WRITE_CARD_SUCCESS                            = 104002,       // ���ڣ���Ʊд��ɹ������Ժ򡭡�
	// �ƣ�磰��������
	GUIDE_ISSUE_FINISH                                  = 105001,       // ���ڣ���Ʊ������������
	GUIDE_ISSUE_ERROR                                   = 105002, 	    // ���죩�أأأأأأ����ܽ��з��д���
	GUIDE_ISSUE_MAGAZINE_NOT_ENOUGH                     = 105003,       // ���죩��Ʊ��Ʊ��ʣ���������㣬���ܷ���ָ�������ĳ�Ʊ
	GUIDE_ISSUE_COLBOX_FULL                             = 105004,       // ���죩��Ʊ����Ʊ�����������ܽ��з��д���
	GUIDE_ISSUE_CARD_NOT_FOUND                          = 105005,       // ���ڣ�û�п��Դ���ĳ�Ʊ�����д������
	// �ƣ�磰��������
	GUIDE_ISSUE_SELET_PRODUCT                           = 106001,    	// ���ڣ���ѡ��Ҫ�����������Ʒ
	// �ƣ�磰��������
	GUIDE_ISSUE_SELECT_START                            = 107001,    	// ���ڣ���ѡ�����
	GUIDE_ISSUE_SELECT_END                              = 107002,    	// ���ڣ���ѡ���յ�
	GUIDE_ISSUE_PRICE_ERROR								= 107003,       // ���죩Ʊ�۷Ƿ�������������
	// �ƣ�磰��������
	GUIDE_ISSUE_INPUT_COUNT                             = 108001,       // ���ڣ�������Ҫ���������
	GUIDE_ISSUE_INPUT_COUNT_ERROR                       = 108002,       // ���죩����������д�����˶Ժ���������
	// �ƣ�磰��������
	GUIDE_ISSUE_INPUT_DATE                              = 109001, 	    // ���ڣ�������������
	GUIDE_ISSUE_INPUT_DATE_ERROR                        = 109002, 	    // ���죩����������д�����˶Ժ���������

	// ��Ʊ
	// �ƣ�磰��������
	GUIDE_ADJUST_PUT_CARD                               = 201001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������
	GUIDE_ADJUST_SELECT_ADJUST_TYPE                     = 202001,       // ���ڣ���ѡ��Ʊ������
	GUIDE_ADJUST_PAYMATHOD_ERROR                        = 202002,       // ���죩�ó�Ʊֻ�ܽ����ֽ�Ʊ
	//GUIDE_ADJUST_PEAKEXIT_STATION_LEAVE                 = 202003,    	// ���죩�ó�Ʊ�����ڳ�վ���ģʽ�³�վ�����貹Ʊ���������
	//GUIDE_ADJUST_PEAKENTRY_STATION_ENTER                = 202004,    	// ���죩�ó�Ʊ�����ڽ�վ���ģʽ�½�վ�����貹Ʊ���������
	GUIDE_ADJUST_NOT_NEED_INSIDE                        = 202005,       // ���ڣ��ó�Ʊ��δ��վ���뷢�г�վ
	GUIDE_ADJUST_NOT_NEED_OUTSIDE                       = 202006,       // ���ڣ��ó�Ʊ�ó�״̬������������в�Ʊ����
	GUIDE_ADJUST_CODE_ERROR						        = 202007,       // ���죩��Ʊ����������

	// �ƣ�磰��������
	GUIDE_ADJUST_PRINT                                  = 203001, 	    // ���ڣ����������������Ϣ
	GUIDE_ADJUST_CONFORM                                = 203003, 	    // ���ڣ���ȷ�����������Ϣ
	GUIDE_ADJUST_INPUT_ERROR                            = 203002,    	// ���죩����������д�����˶Ժ���������
	// �ƣ�磰��������    
	GUIDE_ADJUST_WAITING                                = 204001, 	    // ���ڣ����ڽ��в�Ʊ�������Ժ򡭡�
	GUIDE_ADJUST_END                                    = 204002,       // ���ڣ���Ʊ������������
	GUIDE_ADJUST_ERROR                                  = 204003, 	    // ���죩�أأأأأأ����ܽ��в�Ʊ����
	GUIDE_ADJUST_NOT                                    = 204004,    	// ���죩�ó�Ʊ���ܽ��в�Ʊ����
	GUIDE_ADJUST_MAGAZINE_NOT_ENOUGH                    = 204005, 	    // ���죩��Ʊ��Ʊ��ʣ���������㣬���ܷ���ָ�������ĳ�Ʊ
	GUIDE_ADJUST_CARD_NOT_FOUND                         = 204006,       // ���ڣ�û�п��Դ���ĳ�Ʊ����Ʊ�������
	GUIDE_ADJUST_WRITE_PURSE_END                        = 204007,       // ���ڣ�д��ʱ�ƿ���Ǯ��д��ɹ�����Ʊ������������

	// �ƣ�磰��������
	GUIDE_ADJUST_SELECT_ENTER_STATION                   = 205001,    	// ���ڣ���ѡ���վ��·�ͳ�վ
	GUIDE_ADJUST_SELECT_LEAVE_STATION	                = 205002, 	    // ���ڣ���ѡ���վ��·�ͳ�վ
	//GUIDE_ADJUST_SELECT_OVERRIDE                        = 205003,       // ���죩��վ���̣�������ѡ��վ�򷢳�վƱ
	GUIDE_ADJUST_SELECT_FARE_ERR                        = 205004,       // ���죩Ʊ����Ч����ѡ��������վ���г�վƱ
	GUIDE_ADJUST_REPAIR_NO_FARE_INSIDE				    = 205005,		// ���죩���޸��޷�ȡ����ЧƱ�۵ĳ�Ʊ���뷢�г�վƱ��վ
	GUIDE_ADJUST_REPAIR_OVERRIDE_INSIDE                 = 205006,		// ���죩���޸����̵ĳ�Ʊ���뷢�г�վƱ��վ
	GUIDE_ADJUST_STAFF_INSIDE							= 205007,		// ���ڣ���Ʊ���޸����뷢�г�վƱ��վ

	// �ƣ�磰��������
	GUIDE_ADJUST_SELECT_STATION_MODE                    =  206001, 	    // ���ڣ���ѡ��������ϵͳ��Ӫģʽ
	GUIDE_ADJUST_STATION_MODE_MISMATCH                  =  206002,      // ���죩ѡ���ϵͳ��Ӫģʽ���ܽ������
	// ��ֵ
	// �ƣ�磰��������
	GUIDE_CHARGE_PUT_CARD                               = 301001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������
	GUIDE_CHARGE_SVTICKET                               = 302001, 	    // ���ڣ��������ֵ������ȷ�ϼ�
	GUIDE_CHARGE_PRINT                                  = 302002, 	    // ���ڣ�����ƾ֤���������տ���󰴴�ӡ��
	GUIDE_CHARGE_INPUT_ERROR                            = 302003, 	    // ���죩����������д�����˶Ժ���������
	GUIDE_CHARGE_ECT_MONEY_NOT_BASE_AMOUNT              = 302004, 	    // ���죩��ֵ���Ƿ����������������ĳ�ֵ���
	GUIDE_CHARGE_ECT_MONEY_OVER_MAX                     = 302005, 	    // ���죩��ֵ��������޶��˶Ժ���������
	GUIDE_CHARGE_ECT_BALANCE_OVER_MAX                   = 302006, 	    // ���죩��ֵ��������������������˶Ժ���������
	GUIDE_CHARGE_ECT_AUTHORITY_MONEY_NOT_ENOUGH         = 302007, 	    // ���죩��Ȩ���㣬��˶Ժ�������������ǩ��ǩ��
	GUIDE_CHARGE_NOT_SAME_CARD_PASSENGER                = 302008,       // ���죩��Ƭ��ֵ����ʧ�ܣ���ʹ��ͬһ�ſ�
	GUIDE_CHARGE_CHARGE_ABNORMAL_PASSENGER              = 302009,       // ���죩���ɼ�¼תΪ����
	// �ƣ�磰��������
	GUIDE_CHARGE_WAIT                                   = 303001, 	    // ���ڣ����ڽ��г�ֵ�������Ժ򡭡�
	GUIDE_CHARGE_FINISH                                 = 303002, 	    // ���ڣ���ֵ������������
	GUIDE_CHARGE_ERROR                                  = 303003, 	    // ���죩�أأأأأأ����ܽ��г�ֵ����
	GUIDE_CHARGE_NOT                                    = 303004,    	// ���죩�ó�Ʊ���ܽ��г�ֵ����
	GUIDE_CHARGE_CARD_NOT_FOUND                         = 303005, 	    // ���ڣ�û�п��Դ���ĳ�Ʊ����ֵ�������
	GUIDE_TIME_LIMIT_CHARGE_NOT                         = 303006, 	    // ���죩�ѳ���һ��ͨ��Ȩ��ֹʱ�䣬���ܽ��г�ֵ����
	GUIDE_CHARGE_WRITE_PURSE_FINISH	                    = 303007, 	    // ���ڣ�д��ʱ�ƿ���Ǯ��д��ɹ�����ֵ������������
	GUIDE_CHARGE_BLACK_CARD							= 303008,					//���죩����������������
	// �滻
	// �ƣ�磰��������
	GUIDE_EXCHANGE_PUT_CARD                             = 501001,       // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������
	GUIDE_EXCHANGE_CONFIRM                              = 502001,       // ���ڣ���ȷ�ϳ�Ʊ��Ϣ����ȷ�ϼ�ǰ�������߾ɳ�Ʊ
	GUIDE_EXCHANGE_BACKUP                               = 502002,       // ���죩�ϴ��滻����û�н��������������밴ȷ�ϼ���ȡ�������밴���ؼ� 
	// �ƣ�磰��������
	GUIDE_EXCHANGE_PUT_NEW_CARD                         = 503001,       // ���ڣ��뽫�³�Ʊ�������ⲿIC��д���ĸ�Ӧ����
	GUIDE_EXCHANGE_PUT_OTHER_CARD                       = 503002,       // ���죩�أأأأأأ���������ó�Ʊ���ⲿIC��д���ĸ�Ӧ����
	GUIDE_EXCHANGING                                    = 503003,       // ���ڣ����ڽ����滻�������Ժ򡭡�
	// �ƣ�磰��������
	GUIDE_EXCHANGE_FINISH                               = 504001,       // ���ڣ��滻������������
	GUIDE_EXCHANGE_NOT                                  = 504003,       // ���죩�ó�Ʊ���ܽ����滻����
	GUIDE_EXCHANGE_ERROR                                = 504002,       // ���죩�أأأأأأ����ܽ����滻����
	GUIDE_EXCHANGE_CARD_NOREAD                          = 504004,       // ���ڣ�û�п��Դ���ĳ�Ʊ���滻�������

	// �˿�
	// �ƣ�磰��������
	GUIDE_REFUND_PUT_CARD                               = 601001,       // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������
	GUIDE_REFUND_INPUT_SERIAL_NUMBER                    = 602001, 	    // ���ڣ������복Ʊ���
	GUIDE_REFUND_CONNECTING                             = 602002,       // ���ڣ���λͨ�Ŵ����У����Ժ򡭡�
	GUIDE_REFUND_INPUT_ERROR                            = 602003,       // ���죩����������д�����˶Ժ���������
	// �ƣ�磰��������
	GUIDE_REFUND_SELECT_MTD_CONFIRM                     = 603001,       // ���ڣ�ȷ��������밴ȷ�ϼ�
	GUIDE_REFUND_APPLING_ECT							= 603002,       // ���ڣ����ڽ���һ��ͨ�����˿����봦��,���Ժ򡭡�
	// �ƣ�磰��������
	GUIDE_REFUNDING                                     = 604001,       // ���ڣ����ڽ����˿�����Ժ򡭡�
	GUIDE_REFUND_FINISH									= 604002,       // ���ڣ��˿����������
	GUIDE_REFUND_ERROR                                  = 604004,       // ���죩�أأأأأأ����ܽ����˿��
	GUIDE_REFUND_NOT                                    = 604005,       // ���죩�ó�Ʊ���ܽ����˿��
	GUIDE_REFUND_CARD_NOT_FOUND                         = 604006,       // ���ڣ�û�п��Դ���ĳ�Ʊ���˿�����
	GUIDE_REFUND_WRITE_PURSE_FINISH						= 604007,       // ���ڣ�д��ʱ�ƿ���Ǯ��д��ɹ����˿����������
	GUIDE_REFUND_CPU_NOT                                = 604008,       // ���죩�ó�Ʊ���ܽ��м�ʱ�˿��
	GUIDE_REFUNDING_APPLY                               = 604009,       // ���ڣ����ڽ����˿����룬���Ժ򡭡�
	GUIDE_REFUND_APPLY_NOT								= 604010,       // ���죩�ó�Ʊ���ܽ����˿�����
	GUIDE_REFUND_APPLY_ERROR                            = 604011,       // ���죩�˿����뷢�����󣬲��ܽ����˿��
	GUIDE_REFUNDING_QUERY                               = 604012,       // ���ڣ����ڽ����˿��ѯ�����Ժ򡭡�
	GUIDE_REFUND_QUERY_NOT								= 604013,       // ���죩�ó�Ʊ���ܽ����˿��ѯ
	GUIDE_REFUND_QUERY_ERROR                            = 604014,       // ���죩�˿��ѯ�������󣬲��ܽ����˿��

	// �ƣ�磰��������
	GUIDE_REFUND_INPUT_CENTER_SERIAL_NUMBER             = 605001, 	    // ���ڣ�������������ˮ��
	GUIDE_REFUND_CONNECTING_ECT							= 605002,		// ���ڣ����ڽ��в�Ʊ�������Ժ򡭡�
	GUIDE_REFUND_INPUT_ERROR_ECT                        = 605003,       // ���죩����������д�����˶Ժ���������

	// ����
	// �ƣ�磰��������
	GUIDE_REFRESH_PUT_CARD                              = 701001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������
	GUIDE_REFRESH_CONFIRM_PRESS_OKBTN                   = 702001,    	// ���ڣ���Ʊ��Ϣȷ������󣬰�ȷ�ϼ����м���
	GUIDE_REFRESH_INPUT_ERROR                           = 702002, 	    // ���죩����������д�����˶Ժ���������
	GUIDE_REFRESH_ENTER_DATE                            = 702003, 	    // ���ڣ������복Ʊ������Ч��
	// �ƣ�磰��������
	GUIDE_REFRESH_WAIT                                  = 703001, 	    // ���ڣ����ڽ��м�������Ժ򡭡�
	GUIDE_REFRESH_FINISH                                = 703002,    	// ���ڣ��������������
	GUIDE_REFRESH_ERROR                                 = 703003, 	    // ���죩�أأأأأأ����ܽ��м����
	GUIDE_REFRESH_NOT                                   = 703004, 	    // ���죩�ó�Ʊ���ܽ��м����
	GUIDE_REFRESH_IN_PERIOD_OF_VALIDITY                 = 703005, 	    // ���죩��Ʊ������Ч���ڣ�������м����
	GUIDE_REFRESH_CARD_NOT_FOUND                        = 703006,       // ���ڣ�û�п��Դ���ĳ�Ʊ����������

	// ����
	// �ƣ�磰��������	
	GUIDE_ANALYZE_PUT_CARD                              = 801001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_ANALYZE_SELET_OPERATE                         = 802001,    	// ���ڣ���ȷ�ϳ�Ʊ��Ϣ
	GUIDE_ANALYZE_ERROR                                 = 802002, 	    // ���죩�أأأأأأ����ܽ��з�������
	GUIDE_ANALYZE_CARD_NOT_FOUND                        = 802003,       // ���ڣ�û�п��Դ���ĳ�Ʊ�������������

	// �ƣ�磰��������	
	GUIDE_ANALYZE_PRINT_HISTORY                         = 803001, 	    // ���ڣ���ȷ��������ʾ��Ϣ
	// �ƣ�磰��������	
	GUIDE_ANALYZE_COMMUNICATING                         = 804001, 	    // ���ڣ���λͨ�Ŵ����У����Ժ򡭡�
	GUIDE_ANALYZE_PRINT                                 = 804002,    	// ���ڣ���ȷ��������ʾ��Ϣ
	GUIDE_ANALYZE_SC_NOTFOUND                           = 804003,       // ���ڣ��ó�ƱΪ��Ч��Ʊ���޷���ѯ������Ϣ
	GUIDE_ANALYZE_SC_FAILED                             = 804004,       // ���죩��λͨ�Ŵ���ʧ�ܣ��޷���ѯ������Ϣ
	GUIDE_ANALYZE_BLACKLIST_CARD_BLOCKED                = 804005,       // ���죩������Ʊ����������


	// ����ҵ��
	// �ƣ�磰��������	
	GUIDE_OTHERS_CHOOSE_SERVICE                         = 901001,    	// ���ڣ���ѡ��ҵ������
	GUIDE_OTHERS_FAIL                                   = 901002,    	// ���죩�أأأ��쳣���밴ȷ�ϼ�����ά����Ա��ϵ

	// ��ʧ
	// �ƣ�磰��������	
	GUIDE_REPORTLOSS_INPUT_SERIAL_NUMBER                = 1001001, 	    // ���ڣ������복Ʊ���
	GUIDE_REPORTLOSS_INPUT_ERROR                        = 1001002,   	// ���죩����������д�����˶Ժ���������
	// �ƣ�磰��������	
	GUIDE_REPORTLOSS_CONFIRM                            = 1002001, 	    // ���ڣ���Ʊ��Ϣȷ��������밴ȷ�ϼ���ʧ
	// �ƣ�磰��������	
	GUIDE_REPORTLOSS_FINISH                             = 1003001, 	    // ���ڣ���ʧ������������

	// ����
	// �ƣ�磰��������	
	GUIDE_CANCEL_PUT_CARD                               = 1101001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_CANCEL_CONFIRM                                = 1102001, 	    // ���ڣ���Ʊ��Ϣȷ��������밴ȷ�ϼ�����
	GUIDE_CANCEL_ERROR                                  = 1102002, 	    // ���죩�أأأأأأ����ܽ��е�������
	GUIDE_CANCEL_NOT                                    = 1102003, 	    // ���죩�ó�Ʊ���ܽ��е�������
	GUIDE_CANCEL_CARD_NOT_FOUND                         = 1102004,      // ���ڣ�û�п��Դ���ĳ�Ʊ�������������
	GUIDE_CANCEL_NOT_PRE_SELL									= 1102005,	//���죩�ó�Ʊ����Ԥ��Ʊ
	// ����
	// �ƣ�磰��������	
	GUIDE_REVERSE_SELECT_SORT_AND_QUOMODO               = 1201001, 	    // ���ڣ���ѡ��ҵ������ʹ���ʽ
	// �ƣ�磰��������	
	GUIDE_REVERSE_PUT_CARD                              = 1202001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_REVERSE_CONFIRM                               = 1203001, 	    // ���ڣ���Ʊ��Ϣȷ��������밴ȷ�ϼ����л��˴���
	// �ƣ�磰��������	
	GUIDE_REVERSE_FINISH                                = 1204001, 	    // ���ڣ����˴�����������
	GUIDE_REVERSE_ERROR                                 = 1204002, 	    // ���죩�أأأأأأ����ܽ��л��˴���
	GUIDE_REVERSE_NOT                                   = 1204003, 	    // ���죩�ó�Ʊ���ܽ��л��˴���

	// ����
	// �ƣ�磰��������	
	GUIDE_BLOCK_CHOOSE_SORT                             = 1301001,   	// ���ڣ���ѡ��ҵ������
	// �ƣ�磰��������	
	GUIDE_BLOCK_PUT_CARD                                = 1302001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_BLOCK_SELECT_REASON                           = 1303001,  	// ���ڣ���ѡ��������ԭ��
	GUIDE_BLOCK_SELECT_PRODUCT_AND_SEASON               = 1303002,      // ���ڣ���ѡ��Ҫ�����Ĳ�Ʒ��ԭ��
	// �ƣ�磰��������	
	GUIDE_BLOCK_FINISH                                  = 1304001, 	    // ���ڣ�����������������
	GUIDE_BLOCK_ERROR                                   = 1304002, 	    // ���죩�أأأأأأ����ܽ�����������
	GUIDE_BLOCK_CARD_NOT_FOUND                          = 1304003,      // ���ڣ�û�п��Դ���ĳ�Ʊ�������������

	// �˿�
	// �ƣ�磰��������	
	GUIDE_SURRENDER_PUT_CARD                            = 1401001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_SURRENDER_CONFIRM                             = 1402002, 	    // ���ڣ���Ʊ��Ϣȷ��������밴ȷ�ϼ������˿�����
	// �ƣ�磰��������	
	GUIDE_SURRENDER_FINISH                              = 1403001, 	    // ���ڣ��˿�������������
	GUIDE_SURRENDER_ERROR                               = 1403002,  	// ���죩�أأأأأأ����ܽ����˿�����

	// ��Ʒɾ��
	// �ƣ�磰��������	
	GUIDE_PRODUCT_DELETE_PUT_CARD                       = 1501001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_PRODUCT_DELETE_CONFIRM                        = 1502001, 	    // ���ڣ���Ʊ��Ϣȷ��������밴ȷ�ϼ����в�Ʒɾ������
	// �ƣ�磰��������	
	GUIDE_PRODUCT_DELETE_FINISH                         = 1503001,   	// ���ڣ���Ʒɾ��������������
	GUIDE_PRODUCT_DELETE_ERROR                          = 1503002, 	    // ���죩�أأأأأأ����ܽ��в�Ʒɾ������
	GUIDE_PRODUCT_DELETE_NOT                            = 1503003, 	    // ���죩�ó�Ʊ���ܽ��в�Ʒɾ������

	// TVM�����˿�
	GUIDE_TVMFAILURE_CONFIRM                            = 1601001, 	    // ���ڣ�������Ϣȷ��������밴ȷ�ϼ�
	GUIDE_TVMFAILURE_INPUT_ERROR                        = 1601002, 	    // ���죩����������д�����˶Ժ���������
	GUIDE_TVMFAILURE_PLEASE_INPUT_DEVICE_NO				= 1601003,		// ���죩�������豸���
	GUIDE_TVMFAILURE_PLEASE_INPUT_TICKET_NO				= 1601004,		// ���죩�������˿�ƾ����
	GUIDE_TVMFAILURE_PLEASE_INPUT_MONEY					= 1601005,		// ���죩�������˿���	
	GUIDE_TICKET_MUST_BE_NUMBER							= 1601006,		// ���죩�˿�ƾ���ű���Ϊ10λ����
	GUIDE_DEVICE_MUST_BE_NUMBER							= 1601007,		// ���죩�豸��ű���Ϊ8λ����
	GUIDE_TVMFAILURE_ERROR								= 1601008,		// ���죩���˿�����ظ��˿�
	GUIDE_TVMFAILURE_FINISH                             = 1602001, 	    // ���ڣ�TVM�����˿��ɹ�
	GUIDE_TVM_FAILURE_REFUND_FAIL						= 1602002,		// ���죩TVM�����˿��ʧ��

	// ����ͳ��
	// �ƣ�磰��������	
	GUIDE_AUDIT_PRINT                                   = 1701001, 	    // ���ڣ���ȷ������ͳ����Ϣ
	GUIDE_AUDIT_FINISH                                  = 1701002, 	    // ���ڣ�����ͳ�ƴ�����������

	// ʱ���趨
	// �ƣ�磰��������	
	GUIDE_TIMESET_INPUT_AND_CONFIRM                     = 1801001, 	    // ���ڣ���ֱ�����������ʱ�֣�����ȷ�ϼ�
	GUIDE_TIMESET_INPUT_ERROR                           = 1801002, 	    // ���죩����������д�����˶Ժ���������
	GUIDE_TIMESET_CONFIRM_OR_BACK                       = 1801003, 	    // ���ڣ���Ϣȷ�Ϻ��밴ȷ�ϼ��������޸ģ��밴���ؼ�
	GUIDE_TIMESET_FINISH                                = 1801004, 	    // ���ڣ�ʱ���趨�ɹ�

	// �޸�����
	// �ƣ�磰��������	
	GUIDE_MODIFY_PWD_PUT_CARD                           = 1901001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	// �ƣ�磰��������	
	GUIDE_MODIFY_PWD_CONFIRM                            = 1902001, 	    // ���ڣ������뻭����ʾ�ĸ�����Ϣ��ȷ������󣬰�ȷ�ϼ������޸�
	GUIDE_MODIFY_PWD_READ_ERROR                         = 1902002, 	    // ���죩�ò���Աû��ȡ����Ȩ����˶Ժ�����������
	GUIDE_MODIFY_PWD_INPUT_ERROR                        = 1902003, 	    // ���죩����������д�����˶Ժ���������
	GUIDE_MODIFY_PWD_OLD_PASSWORD_ERROR                 = 1902004, 	    // ���죩���������������˶Ժ���������
	GUIDE_MODIFY_PWD_NEW_PASSWORD_ERROR                 = 1902005, 	    // ���죩�������������벻һ�£���˶Ժ���������
	GUIDE_MODIFY_PWD_PASSWORD_SAME                      = 1902006, 	    // ���죩���������������ͬ����˶Ժ���������
	GUIDE_MODIFY_PWD_PASSWORD_OVER_TIMES                = 1902007, 	    // ���죩��������������������Ѿ������涨����
	GUIDE_MODIFY_PWD_FINISH                             = 1902008, 	    // ���ڣ��޸����봦����������
	//GUIDE_MODIFY_PWD_NOT                                = 1902009, 	    // ���죩�ó�Ʊ���ܽ����޸����봦��

	// Ʊ�۱�
	// �ƣ�磰��������	
	GUIDE_FARE_TABLE_PRINT                              = 2001001, 	    // ���ڣ���ȷ��Ʊ�۱���Ϣ�������ӡ���밴��ӡ��
	GUIDE_FARE_TABLE_FINISH                             = 2001002, 	    // ���ڣ�Ʊ�۱���Ϣȷ�ϴ�����������

	// ģʽ�л�
	// �ƣ�磰��������	
	GUIDE_OPERATE_SWITCH_SELECT                          = 2101001,   	// ���ڣ���ѡ��ҵ��ģʽ���࣬����ȷ�ϼ�����ҵ��ģʽ�л�
	GUIDE_OPERATE_SWITCH_CONFIRM                         = 2101002, 	// ���ڣ���Ϣȷ�Ϻ��밴ȷ�ϼ��������޸ģ��밴���ؼ�
	GUIDE_OPERATE_SWITCH_FINISH                          = 2101003, 	// ���ڣ�ģʽ�л�������������

	// Ʊ�����
	// �ƣ�磰��������	
	GUIDE_TICKETBOX_CHANGE_START                        = 2201001, 	    // ���ڣ��밴������ʼ����ȷ�ϼ���׼������Ʊ��
	GUIDE_TICKETBOX_CHANGE_A_ERROR                      = 2201002, 	    // ���죩Ʊ��A��Ϣ�쳣
	GUIDE_TICKETBOX_CHANGE_B_ERROR                      = 2201003, 	    // ���죩Ʊ��B��Ϣ�쳣
	// �ƣ�磰��������	
	GUIDE_TICKETBOX_CHANGE_FINISH                       = 2202001, 	    // ���ڣ������Ʊ�䣬�����������밴������������ȷ�ϼ�
	// �ƣ�磰��������	
	GUIDE_TICKETBOX_BUSIMESS_FINISH                     = 2203001, 	    // ���ڣ�Ʊ�����������������
	GUIDE_TICKETBOX_BUSIMESS_A_ERROR                    = 2203002, 	    // ���죩Ʊ��A��Ϣ�쳣
	GUIDE_TICKETBOX_BUSIMESS_B_ERROR                    = 2203003, 	    // ���죩Ʊ��B��Ϣ�쳣
	GUIDE_TICKETBOX_CHANGE_A_B_ERROR                    = 2203004, 	    // ���죩Ʊ��A��B��Ϣ�쳣

	// �ƣ�磰��������	
	GUIDE_OTHERS_REST                                   = 2301001, 	    // ���ڣ���Ϣ��

	// �ƣ�磰��������	
	GUIDE_OTHERS_LOGOUT                                 = 2401001, 	    // ���ڣ����ڵǳ������Ժ򡭡�

	// ǰ����Ʊ��ӡ
	GUIDE_REPRINT_CONFIRM								= 2501001, 	    // ���ڣ���ȷ��ǰ�ص�¼��Ϣ�������ӡ���밴��ӡ��
	GUIDE_REPRINT_FINISH								= 2501002, 	    // ���ڣ�ǰ����Ʊ��ӡ������������

	// ��ֵ��ʧ����
	GUIDE_CHARGE_LOST_CONFIRM							= 2601001, 	    // ���ڣ���ȷ�϶�ʧ������Ϣ��ѡ���輯�Ƶĵ��ĺ�ȷ�ϼ�

	// ��Ӫҵ����֧/��������
	GUIDE_NON_BUISNESS_SELECT							= 2701001,		// ���ڣ���ѡ��������������ȷ�ϼ�
	GUIDE_NON_BUISNESS_SUCCEED							= 2701002,		// ���ڣ���������ҵ����ɹ�
	GUIDE_NON_BUISNESS_FAIL								= 2701003,		// ���죩��������ҵ����ʧ��
	GUIDE_NON_BUISNESS_INPUT_NULL						= 2701004,		// ���죩��������֧���
	GUIDE_NON_BUISNESS_INPUT_ERROR						= 2701005,		// ���죩����������д�����˶Ժ���������

	// ����������
	GUIDE_APPLY_SELECT									= 2801001,		// ���ڣ���������������Ϣ��ȷ�ϼ�
	GUIDE_APPLY_SUCCEED									= 2801002,		// ���ڣ�����������ҵ����ɹ�
	GUIDE_APPLY_FAIL									= 2801003,		// ���죩����������״̬��ѯʧ��
	GUIDE_APPLY_CHECKING								= 2801004,		// ���ڣ����ڲ�ѯ�����Ժ󡭡�
	GUIDE_APPLY_INPUT_ERROR					        	= 2801005,		// ���죩����������д�����˶Ժ���������
	GUIDE_APPLY_UPDATA_SUCCEED							= 2801006,		// ���ڣ��������ֿ�����Ϣ�޸Ĵ�����������
	GUIDE_APPLY_CHECK_SUCCEED							= 2801007,		// ���ڣ�����������״̬��ѯ�ɹ�
	GUIDE_APPLY_CARDINFO_ERROR							= 2801008,		// ���죩�ó�Ʊ���ܽ��м�����ҵ��


	//��ʧ���
	GUIDE_SUSPEND_AND_RESUME_SELECT						= 2901001,		// ���ڣ�������ֿ�����Ϣ��ȷ�ϼ�
	GUIDE_SUSPEND_SUCCESS								= 2901002,		// ���ڣ���ʧҵ����ɹ�
	GUIDE_SUSPEND_FAIL									= 2901003,		// ���죩��ʧҵ����ʧ��
	GUIDE_RESUME_SUCCESS								= 2901004,		// ���ڣ����ҵ����ɹ�
	GUIDE_RESUME_FAIL									= 2901005,		// ���죩���ҵ����ʧ��
	//����
	GUIDE_DEFER_PUT_CARD                              = 3001001, 	    // ���ڣ��뽫��Ʊ�������ⲿIC��д���ĸ�Ӧ����
	GUIDE_DEFER_CONFIRM_PRESS_OKBTN                   = 3002001,    	// ���ڣ���Ʊ��Ϣȷ������󣬰�ȷ�ϼ����м���
	GUIDE_DEFER_WAIT                                  = 3002002, 	    // ���ڣ����ڽ������ڴ������Ժ򡭡�
	GUIDE_DEFER_DAYS_ERROR				= 3002003,		// ���죩����ʱ�䲻��ȷ
	GUIDE_DEFER_CASH_ERROR				= 3002004,		// ���죩�Ѹ�����ȷ
	GUIDE_DEFER_FINISH                                = 3003001,    	// ���ڣ����ڴ�����������
	GUIDE_DEFER_ERROR                                 = 3003002, 	    // ���죩�أأأأأأ����ܽ������ڴ���
	GUIDE_DEFER_NOT                                   = 3003003, 	    // ���죩�ó�Ʊ���ܽ������ڴ���

}OPERATION_MSG;


//
//// �������
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
/////////////////////////////////////////////////BUTTON��Ϣ�ı�����///////////////////////////////////////

static const TCHAR* const WM_BTN_PRINT			= _T("print");							///< ��ӡ��ť��Ϣ
static const TCHAR* const WM_BTN_OK				= _T("ok");								///< ȷ�ϰ�ť��Ϣ
static const TCHAR* const WM_BTN_BACK			= _T("back");							///< ���ذ�ť��Ϣ
static const TCHAR* const WM_BTN_RESET       = _T("reset");							///< ���ð�ť��Ϣ
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


