#pragma once
#include "Func.h"
#include "Panel.h"
#include "bag.h"
#include "UIDefinition.h"
#include "gedit.h"

static const MARGINS GSTATIC_MARGINS = {5,5,5,5};
static const PADDINGS GSTATIC_PADDING = PADDINGS(5,5,5,5);
static const int GSTATIC_LABEL_HEIGHT = 37;
static const int GSTATIC_TITLE_HEIGHT = GSTATIC_LABEL_HEIGHT * 1.2;
static const int GSTATIC_WIDTH =       GUIDE_FRAME_RECT.Width();
static const int LABEL_RIGHT_X  =      163*CXSCREENRATIO;


#define MAX_ROW              15
#define MAX_COL              7



typedef struct label_group
{
    CString nameLeft;               // 文本内容左
    CString nameRight;              // 文本内容右
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
            name1 = "";
            name2 = "";
            name3 = "";
            name4 = "";
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
    GSTATIC_TITLE_INFO titleInfo;   // 标题信息
    GSTATIC_DETAIL_INFO detailInfo; // 详细信息

} GSTATIC_INFO, *PGSTATIC_INFO;




//
//@brief   建立圆角矩形及其中的文本框和编辑框，并执行相关操作 

class CGStatic : public CStatic
{
	DECLARE_DYNAMIC(CGStatic)
	DECLARE_MESSAGE_MAP()

public:
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, bool isUsedForAuditSvc = false);  // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_THREE);              // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FOUR);               // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FIVE);               // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SIX);                // 构造函数
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SEVEN);              // 构造函数
	virtual ~CGStatic();                                                        // 析构函数

	void SetLabelText(int, int, CString);                           // 设置文本框的显示文字
	CGEdit* GetEdit(int);                                           // 取得编辑框的地址
	void SetLabelColor(int, int, COLORREF);                         // 设置文本框文字的颜色
	void SetTitleText(CString);                                     // 设置标题的文字

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

	CFont* m_font;                                                  // 字体名称
	int m_col;                                                      // 列数
	COLORREF m_colorArray[MAX_ROW][MAX_COL];                        // 行列的颜色

	bool m_isUsedForAuditSvc;                                       // 是否为销售统计业务专用

	afx_msg void OnPaint();                                         // 建立画面
	afx_msg int OnCreate(LPCREATESTRUCT);                           // 画面变化时刷新画面
	CRect GetDrawRect( CPoint point, int rows );
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};