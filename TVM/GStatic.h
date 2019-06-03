#pragma once
#include "Func.h"
#include "Panel.h"
#include "bag.h"
#include "UIDefinition.h"
#include "gedit.h"
#include "PictureEx.h"

static const PADDINGS GSTATIC_PADDING = PADDINGS(5,5,5,5); //圆角区域
// ////////////////////////////////////////////////////////////////////////////////////////////////////
// GStatic样式定义
// 乘客宽信息界面
static const int GSTATIC_STYLE_1_WIDTH			= 1920-160-460/*950*/;					// 控件宽度
static const int GSTATIC_STYLE_1_BORDER			= 2;					// 边框粗细
static const CPoint GSTATIC_STYLE_1_ROUND		= CPoint(20,20);		// 圆角弧度
static const MARGINS GSTATIC_STYLE_1_MARGIN		= {5,5,5,5};			// 控件外边距
static const PADDINGS GSTATIC_STYLE_1_PADDING	= PADDINGS(10,10,5,5);	// 控件内边距
static const int GSTATIC_STYLE_1_LABEL_HEIGHT	= 28;					// 文本区域单行高度
static const int GSTATIC_STYLE_1_TITLE_HEIGHT	= GSTATIC_STYLE_1_LABEL_HEIGHT * 1.8;	// 文本区标题高度
static const int LABEL_1_LEFT_WITH  =      600;    // 文本区域左侧宽度
static const int LABEL_1_RIGHT_WITH =      600;    // 文本区域右侧宽度
static const int LABEL_1_MID_WITH  =       20;     // 文本区域间隔宽度

// 乘客窄信息界面
static const int GSTATIC_STYLE_3_WIDTH			= 680;					// 控件宽度
static const int GSTATIC_STYLE_3_BORDER			= 2;					// 边框粗细
static const CPoint GSTATIC_STYLE_3_ROUND		= CPoint(20,20);		// 圆角弧度
static const MARGINS GSTATIC_STYLE_3_MARGIN		= {5,5,5,5};			// 控件外边距
static const PADDINGS GSTATIC_STYLE_3_PADDING	= PADDINGS(10,10,5,5);	// 控件内边距
static const int GSTATIC_STYLE_3_LABEL_HEIGHT	= 37;					// 文本区域单行高度
static const int GSTATIC_STYLE_3_LABEL_PADDING	= 53;					// 文本上下区域间距
static const int GSTATIC_STYLE_3_TITLE_HEIGHT	= GSTATIC_STYLE_1_LABEL_HEIGHT * 1.2;	// 文本区标题高度
static const int LABEL_3_LEFT_WITH  =      240;    // 文本区域左侧宽度
static const int LABEL_3_RIGHT_WITH =      240;    // 文本区域右侧宽度
static const int LABEL_3_MID_WITH  =       10;     // 文本区域间隔宽度
static const int LABEL_3_MAX_WIDTH =	   20;	   // 文本区域间隔宽度

// 维护界面
static const int GSTATIC_STYLE_2_WIDTH			= CXSCREEN;				// 控件宽度
static const int GSTATIC_STYLE_2_BORDER			= 1;					// 边框粗细
static const CPoint GSTATIC_STYLE_2_ROUND		= CPoint(12,12);		// 圆角弧度
static const MARGINS GSTATIC_STYLE_2_MARGIN		= {2,0,0,8};			// 控件外边距
static const PADDINGS GSTATIC_STYLE_2_PADDING	= PADDINGS(4,5,0,0);	// 控件内边距
static const int GSTATIC_STYLE_2_LABEL_HEIGHT	= 34 /** HRESOLUTION*/;		// 文本行高度
static const int GSTATIC_STYLE_2_TITLE_HEIGHT	= 0;					// 标题行高度
static const int LABEL_2_LEFT_WITH  =      190;    // 文本区域左侧宽度
static const int LABEL_2_RIGHT_WITH =      190;    // 文本区域右侧宽度
static const int LABEL_2_MID_WITH  =       10;     // 文本区域间隔宽度

// 分析业务乘客界面
static const int GSTATIC_STYLE_4_WIDTH			= 950;					// 控件宽度
static const int GSTATIC_STYLE_4_BORDER			= 2;					// 边框粗细
static const CPoint GSTATIC_STYLE_4_ROUND		= CPoint(20,20);		// 圆角弧度
static const MARGINS GSTATIC_STYLE_4_MARGIN		= {5,5,5,5};			// 控件外边距
static const PADDINGS GSTATIC_STYLE_4_PADDING	= PADDINGS(10,10,5,5);	// 控件内边距
static const int GSTATIC_STYLE_4_LABEL_HEIGHT	= 37;					// 文本区域单行高度
static const int GSTATIC_STYLE_4_TITLE_HEIGHT	= GSTATIC_STYLE_4_LABEL_HEIGHT * 1.2;	// 文本区标题高度
static const int LABEL_4_LEFT_WITH  =      400;    // 文本区域左侧宽度
static const int LABEL_4_RIGHT_WITH =      400;    // 文本区域右侧宽度
static const int LABEL_4_MID_WITH  =       10;     // 文本区域间隔宽度

// 发售业务乘客界面
static const int GSTATIC_STYLE_5_WIDTH			= line_map_size.cx;		// 控件宽度
static const int GSTATIC_STYLE_5_BORDER			= 2;					// 边框粗细
static const CPoint GSTATIC_STYLE_5_ROUND		= CPoint(20,20);		// 圆角弧度
static const MARGINS GSTATIC_STYLE_5_MARGIN		= {5,5,5,5};			// 控件外边距
static const PADDINGS GSTATIC_STYLE_5_PADDING	= PADDINGS(10,10,5,5);	// 控件内边距
static const int GSTATIC_STYLE_5_LABEL_HEIGHT	= 28;					// 文本区域单行高度
static const int GSTATIC_STYLE_5_TITLE_HEIGHT	= GSTATIC_STYLE_1_LABEL_HEIGHT * 1.8;	// 文本区标题高度
static const int LABEL_5_LEFT_WITH  =      250;    // 文本区域左侧宽度
static const int LABEL_5_RIGHT_WITH =      300;    // 文本区域右侧宽度
static const int LABEL_5_MID_WITH  =       20;     // 文本区域间隔宽度

#define MAX_ROW              15
#define MAX_COL              7

#define NOTIFY_DLG_ID 2017


enum static_style{
	PASSAGE_WIDE_STYLE = 1,
	OPERATOR_STATIC_STYLE = 2,
	PASSAGE_NARROW_STYLE = 3,
	SIGLE_ELEMENT_STYLE = 4,
	ANYLE_INFO_STYLE = 5,
	ISSUE_INFO_STYLE = 6,		// 区域大小和线路总图大小位置一致
	IMAGE_CENTER_STYLE = 7,		// 区域大小和线路总图大小位置一致，且中心位置显示图片
	CHARGE_INFO_STYLE	= 8,	// 充值
};

typedef struct label_group
{
    CString nameLeft;               // 文本内容左
    CString nameRight;              // 文本内容右
	label_group()
	{
		nameLeft=_T("");
		nameRight=_T("");
	}
} LABEL_GROUP, *PLABEL_GROUP;

typedef struct label_group_three
{
    CString nameLeft;               // 文本内容左
    CString nameCenter;             // 文本内容中
    CString nameRight;              // 文本内容右
        label_group_three()
        {
            nameLeft = "";
            nameCenter = "";
            nameRight = "";
        }
} LABEL_GROUP_THREE, *PLABEL_GROUP_THREE;

typedef struct label_group_four
{
    CString name1;                  // 文本内容1
    CString name2;                  // 文本内容2
    CString name3;                  // 文本内容3
    CString name4;                  // 文本内容4
        label_group_four()
        {
            name1 = _T("");
            name2 = _T("");
            name3 = _T("");
            name4 = _T("");
        }
} LABEL_GROUP_FOUR, *PLABEL_GROUP_FOUR;

typedef struct label_group_five
{
    CString name1;                  // 文本内容1
    CString name2;                  // 文本内容2
    CString name3;                  // 文本内容3
    CString name4;                  // 文本内容4
    CString name5;                  // 文本内容5
        label_group_five()
        {
            name1 = "";
            name2 = "";
            name3 = "";
            name4 = "";
            name5 = "";
        }
} LABEL_GROUP_FIVE, *PLABEL_GROUP_FIVE;

typedef struct label_group_six
{
    CString name1;                  // 文本内容1
    CString name2;                  // 文本内容2
    CString name3;                  // 文本内容3
    CString name4;                  // 文本内容4
    CString name5;                  // 文本内容5
    CString name6;                  // 文本内容6
    bool isForECT;                  // 是否一卡通详细日志使用

    label_group_six()
    {
        name1 = "";
        name2 = "";
        name3 = "";
        name4 = "";
        name5 = "";
        name6 = "";
        isForECT = false;
    }
} LABEL_GROUP_SIX, *PLABEL_GROUP_SIX;

typedef struct label_group_seven
{
	CString name1;                  // 文本内容1
	CString name2;                  // 文本内容2
	CString name3;                  // 文本内容3
	CString name4;                  // 文本内容4
	CString name5;                  // 文本内容5
	CString name6;                  // 文本内容6
	CString name7;                  // 文本内容7

	label_group_seven()
	{
		name1 = "";
		name2 = "";
		name3 = "";
		name4 = "";
		name5 = "";
		name6 = "";
		name7 = "";
	}
} LABEL_GROUP_SEVEN, *PLABEL_GROUP_SEVEN;

typedef struct edit_group
{
    int cnt;                        // 编辑框数量
    CRect* editRect;                // 编辑框坐标数组
    bool isLeftAlign;               // 是否左对齐
        edit_group()
        {   
            cnt = 0;
            editRect = NULL;
            isLeftAlign = true;
        }
} EDIT_GROUP, *PEDIT_GROUP;

typedef struct gstatic_title_info
{
    CString titleName;              // 标题文字
    COLORREF color;                 // 标题颜色
        gstatic_title_info()
        {
            titleName = "";
            color = 0;
        }
} GSTATIC_TITLE_INFO, *PGSTATIC_TITLE_INFO;

typedef struct gstatic_detail_info
{
    int row;                        // 行数
    COLORREF color;                 // 标题颜色
    PLABEL_GROUP labelGroup;        // 文本组
    EDIT_GROUP editGroup;           // 编辑框组
        gstatic_detail_info()
        {
            row = 0;
            color = 0;
        }

} GSTATIC_DETAIL_INFO, *PGSTATIC_DETAIL_INFO;

typedef struct gstatic_info
{
    CPoint point;                   // 区域起始点
	CSize size;
    GSTATIC_TITLE_INFO titleInfo;   // 标题信息
    GSTATIC_DETAIL_INFO detailInfo; // 详细信息
	CString szContent;				// 单行单列显示内容
	int stytle;                     // 区域樣式
	int type;						// 显示区域类型0：售票；1：充值;2:维护
	gstatic_info(){
		stytle = OPERATOR_STATIC_STYLE;
		size = CSize(0,0);
		szContent = _T("");
	}
} GSTATIC_INFO, *PGSTATIC_INFO;

//
//@brief   建立圆角矩形及其中的文本框和编辑框，并执行相关操作 

class CGStatic : public CStatic
{
	DECLARE_DYNAMIC(CGStatic)
	DECLARE_MESSAGE_MAP()

public:
	CGStatic(CWnd* pParentWnd, CRect rect);
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, bool isUsedForAuditSvc = false,CImage* pImge = NULL);  // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_THREE);              // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FOUR);               // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FIVE);               // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SIX);                // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SEVEN);              // 构造函数
	virtual ~CGStatic();                                                        // 析构函数

	void SetContent(CString);										// 设置显示内容
	void SetStyle();
	void SetTitleText(CString);                                     // 设置标题的文字
	void SetLabelText(int, int, CString);                           // 设置文本框的显示文字
	void SetLabelColor(int, int, COLORREF);                         // 设置文本框文字的颜色
	void SetLableAlign(int row, int col, DWORD align);				// 设置文本框文字的对齐方式
	void SetLableAlign(int col, DWORD align);						// 设置文本框文字的对齐方式（一列）
	void SetLableWidth(int row, int col, int width);				// 设置文本框文字的宽度
	void SetLableWidth(int col, int width);							// 设置文本框文字的宽度（一列）
	CGEdit* GetEdit(int);                                           // 取得编辑框的地址
	COLORREF m_Title_Font_Color;									// 标题字体颜色

protected:
	virtual void PreSubclassWindow();

private:
	PGSTATIC_INFO m_gstaticInfo;                                    // 区域中的数据
	PLABEL_GROUP_THREE m_labelGroupThree;                           // 3列数据
	PLABEL_GROUP_FOUR m_labelGroupFour;                             // 4列数据 
	PLABEL_GROUP_FIVE m_labelGroupFive;                             // 5列数据
	PLABEL_GROUP_SIX  m_labelGroupSix;                              // 6列数据
	PLABEL_GROUP_SEVEN m_labelGroupSeven;                           // 7列数据
	CGEdit* m_editArray[64];                                        // 编辑框数组
	int m_labelRow;                                                 // 文本框行数

	CFont m_fontR;
	CFont m_font;                                                  // 字体名称
	CFont m_titlefont;                                             // 标题字体名称
	int m_col;                                                      // 列数
	COLORREF m_colorArray[MAX_ROW][MAX_COL];                        // 行列的颜色
	DWORD m_alignArray[MAX_ROW][MAX_COL];							// 行列的对齐方式
	int m_widthArray[MAX_ROW][MAX_COL];								// 行列的宽度

	bool m_isUsedForAuditSvc;                                       // 是否为销售统计业务专用

	afx_msg void OnPaint();                                         // 建立画面
	afx_msg BOOL OnEraseBkgnd(CDC*);									// 背景擦除
	afx_msg int OnCreate(LPCREATESTRUCT);                           // 画面变化时刷新画面
	CRect GetDrawRect( CPoint point, int rows );
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	int m_width;			// 控件宽度
	int m_border;			// 边框大小
	CPoint m_round;			// 圆角弧度
	MARGINS m_margin;		// 外边距
	PADDINGS m_padding;		// 内边距
	int m_lable_height;		// 文本行高度
	int m_title_height;		// 标题行高度
	int m_lable_left_with;    // 文本区域左侧宽度
	int m_lable_right_with;    // 文本区域右侧宽度
	int m_lable_mid_with;     // 文本区域间隔宽度
	CString m_szContent;
	// 二维码显示相关
	void OnTimer(UINT nIDEvent);
	CPictureEx m_picex;// 动态图片
	CImage* m_pErweimaImg;
	CImage* m_pPaymentTipsImg;
	CImage* m_pUseTipImg;
	CImage* m_pNoticeImgBk;
	CImage* m_pBkImg;
};