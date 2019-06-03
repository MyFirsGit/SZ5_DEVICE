#pragma once
#include "Func.h"
#include "Panel.h"
#include "bag.h"
#include "UIDefinition.h"
#include "gedit.h"
#include "PictureEx.h"

static const PADDINGS GSTATIC_PADDING = PADDINGS(5,5,5,5); //Բ������
// ////////////////////////////////////////////////////////////////////////////////////////////////////
// GStatic��ʽ����
// �˿Ϳ���Ϣ����
static const int GSTATIC_STYLE_1_WIDTH			= 1920-160-460/*950*/;					// �ؼ����
static const int GSTATIC_STYLE_1_BORDER			= 2;					// �߿��ϸ
static const CPoint GSTATIC_STYLE_1_ROUND		= CPoint(20,20);		// Բ�ǻ���
static const MARGINS GSTATIC_STYLE_1_MARGIN		= {5,5,5,5};			// �ؼ���߾�
static const PADDINGS GSTATIC_STYLE_1_PADDING	= PADDINGS(10,10,5,5);	// �ؼ��ڱ߾�
static const int GSTATIC_STYLE_1_LABEL_HEIGHT	= 28;					// �ı������и߶�
static const int GSTATIC_STYLE_1_TITLE_HEIGHT	= GSTATIC_STYLE_1_LABEL_HEIGHT * 1.8;	// �ı�������߶�
static const int LABEL_1_LEFT_WITH  =      600;    // �ı����������
static const int LABEL_1_RIGHT_WITH =      600;    // �ı������Ҳ���
static const int LABEL_1_MID_WITH  =       20;     // �ı����������

// �˿�խ��Ϣ����
static const int GSTATIC_STYLE_3_WIDTH			= 680;					// �ؼ����
static const int GSTATIC_STYLE_3_BORDER			= 2;					// �߿��ϸ
static const CPoint GSTATIC_STYLE_3_ROUND		= CPoint(20,20);		// Բ�ǻ���
static const MARGINS GSTATIC_STYLE_3_MARGIN		= {5,5,5,5};			// �ؼ���߾�
static const PADDINGS GSTATIC_STYLE_3_PADDING	= PADDINGS(10,10,5,5);	// �ؼ��ڱ߾�
static const int GSTATIC_STYLE_3_LABEL_HEIGHT	= 37;					// �ı������и߶�
static const int GSTATIC_STYLE_3_LABEL_PADDING	= 53;					// �ı�����������
static const int GSTATIC_STYLE_3_TITLE_HEIGHT	= GSTATIC_STYLE_1_LABEL_HEIGHT * 1.2;	// �ı�������߶�
static const int LABEL_3_LEFT_WITH  =      240;    // �ı����������
static const int LABEL_3_RIGHT_WITH =      240;    // �ı������Ҳ���
static const int LABEL_3_MID_WITH  =       10;     // �ı����������
static const int LABEL_3_MAX_WIDTH =	   20;	   // �ı����������

// ά������
static const int GSTATIC_STYLE_2_WIDTH			= CXSCREEN;				// �ؼ����
static const int GSTATIC_STYLE_2_BORDER			= 1;					// �߿��ϸ
static const CPoint GSTATIC_STYLE_2_ROUND		= CPoint(12,12);		// Բ�ǻ���
static const MARGINS GSTATIC_STYLE_2_MARGIN		= {2,0,0,8};			// �ؼ���߾�
static const PADDINGS GSTATIC_STYLE_2_PADDING	= PADDINGS(4,5,0,0);	// �ؼ��ڱ߾�
static const int GSTATIC_STYLE_2_LABEL_HEIGHT	= 34 /** HRESOLUTION*/;		// �ı��и߶�
static const int GSTATIC_STYLE_2_TITLE_HEIGHT	= 0;					// �����и߶�
static const int LABEL_2_LEFT_WITH  =      190;    // �ı����������
static const int LABEL_2_RIGHT_WITH =      190;    // �ı������Ҳ���
static const int LABEL_2_MID_WITH  =       10;     // �ı����������

// ����ҵ��˿ͽ���
static const int GSTATIC_STYLE_4_WIDTH			= 950;					// �ؼ����
static const int GSTATIC_STYLE_4_BORDER			= 2;					// �߿��ϸ
static const CPoint GSTATIC_STYLE_4_ROUND		= CPoint(20,20);		// Բ�ǻ���
static const MARGINS GSTATIC_STYLE_4_MARGIN		= {5,5,5,5};			// �ؼ���߾�
static const PADDINGS GSTATIC_STYLE_4_PADDING	= PADDINGS(10,10,5,5);	// �ؼ��ڱ߾�
static const int GSTATIC_STYLE_4_LABEL_HEIGHT	= 37;					// �ı������и߶�
static const int GSTATIC_STYLE_4_TITLE_HEIGHT	= GSTATIC_STYLE_4_LABEL_HEIGHT * 1.2;	// �ı�������߶�
static const int LABEL_4_LEFT_WITH  =      400;    // �ı����������
static const int LABEL_4_RIGHT_WITH =      400;    // �ı������Ҳ���
static const int LABEL_4_MID_WITH  =       10;     // �ı����������

// ����ҵ��˿ͽ���
static const int GSTATIC_STYLE_5_WIDTH			= line_map_size.cx;		// �ؼ����
static const int GSTATIC_STYLE_5_BORDER			= 2;					// �߿��ϸ
static const CPoint GSTATIC_STYLE_5_ROUND		= CPoint(20,20);		// Բ�ǻ���
static const MARGINS GSTATIC_STYLE_5_MARGIN		= {5,5,5,5};			// �ؼ���߾�
static const PADDINGS GSTATIC_STYLE_5_PADDING	= PADDINGS(10,10,5,5);	// �ؼ��ڱ߾�
static const int GSTATIC_STYLE_5_LABEL_HEIGHT	= 28;					// �ı������и߶�
static const int GSTATIC_STYLE_5_TITLE_HEIGHT	= GSTATIC_STYLE_1_LABEL_HEIGHT * 1.8;	// �ı�������߶�
static const int LABEL_5_LEFT_WITH  =      250;    // �ı����������
static const int LABEL_5_RIGHT_WITH =      300;    // �ı������Ҳ���
static const int LABEL_5_MID_WITH  =       20;     // �ı����������

#define MAX_ROW              15
#define MAX_COL              7

#define NOTIFY_DLG_ID 2017


enum static_style{
	PASSAGE_WIDE_STYLE = 1,
	OPERATOR_STATIC_STYLE = 2,
	PASSAGE_NARROW_STYLE = 3,
	SIGLE_ELEMENT_STYLE = 4,
	ANYLE_INFO_STYLE = 5,
	ISSUE_INFO_STYLE = 6,		// �����С����·��ͼ��Сλ��һ��
	IMAGE_CENTER_STYLE = 7,		// �����С����·��ͼ��Сλ��һ�£�������λ����ʾͼƬ
	CHARGE_INFO_STYLE	= 8,	// ��ֵ
};

typedef struct label_group
{
    CString nameLeft;               // �ı�������
    CString nameRight;              // �ı�������
	label_group()
	{
		nameLeft=_T("");
		nameRight=_T("");
	}
} LABEL_GROUP, *PLABEL_GROUP;

typedef struct label_group_three
{
    CString nameLeft;               // �ı�������
    CString nameCenter;             // �ı�������
    CString nameRight;              // �ı�������
        label_group_three()
        {
            nameLeft = "";
            nameCenter = "";
            nameRight = "";
        }
} LABEL_GROUP_THREE, *PLABEL_GROUP_THREE;

typedef struct label_group_four
{
    CString name1;                  // �ı�����1
    CString name2;                  // �ı�����2
    CString name3;                  // �ı�����3
    CString name4;                  // �ı�����4
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
    CString name1;                  // �ı�����1
    CString name2;                  // �ı�����2
    CString name3;                  // �ı�����3
    CString name4;                  // �ı�����4
    CString name5;                  // �ı�����5
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
    CString name1;                  // �ı�����1
    CString name2;                  // �ı�����2
    CString name3;                  // �ı�����3
    CString name4;                  // �ı�����4
    CString name5;                  // �ı�����5
    CString name6;                  // �ı�����6
    bool isForECT;                  // �Ƿ�һ��ͨ��ϸ��־ʹ��

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
	CString name1;                  // �ı�����1
	CString name2;                  // �ı�����2
	CString name3;                  // �ı�����3
	CString name4;                  // �ı�����4
	CString name5;                  // �ı�����5
	CString name6;                  // �ı�����6
	CString name7;                  // �ı�����7

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
    int cnt;                        // �༭������
    CRect* editRect;                // �༭����������
    bool isLeftAlign;               // �Ƿ������
        edit_group()
        {   
            cnt = 0;
            editRect = NULL;
            isLeftAlign = true;
        }
} EDIT_GROUP, *PEDIT_GROUP;

typedef struct gstatic_title_info
{
    CString titleName;              // ��������
    COLORREF color;                 // ������ɫ
        gstatic_title_info()
        {
            titleName = "";
            color = 0;
        }
} GSTATIC_TITLE_INFO, *PGSTATIC_TITLE_INFO;

typedef struct gstatic_detail_info
{
    int row;                        // ����
    COLORREF color;                 // ������ɫ
    PLABEL_GROUP labelGroup;        // �ı���
    EDIT_GROUP editGroup;           // �༭����
        gstatic_detail_info()
        {
            row = 0;
            color = 0;
        }

} GSTATIC_DETAIL_INFO, *PGSTATIC_DETAIL_INFO;

typedef struct gstatic_info
{
    CPoint point;                   // ������ʼ��
	CSize size;
    GSTATIC_TITLE_INFO titleInfo;   // ������Ϣ
    GSTATIC_DETAIL_INFO detailInfo; // ��ϸ��Ϣ
	CString szContent;				// ���е�����ʾ����
	int stytle;                     // �����ʽ
	int type;						// ��ʾ��������0����Ʊ��1����ֵ;2:ά��
	gstatic_info(){
		stytle = OPERATOR_STATIC_STYLE;
		size = CSize(0,0);
		szContent = _T("");
	}
} GSTATIC_INFO, *PGSTATIC_INFO;

//
//@brief   ����Բ�Ǿ��μ����е��ı���ͱ༭�򣬲�ִ����ز��� 

class CGStatic : public CStatic
{
	DECLARE_DYNAMIC(CGStatic)
	DECLARE_MESSAGE_MAP()

public:
	CGStatic(CWnd* pParentWnd, CRect rect);
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, bool isUsedForAuditSvc = false,CImage* pImge = NULL);  // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_THREE);              // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FOUR);               // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FIVE);               // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SIX);                // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SEVEN);              // ���캯��
	virtual ~CGStatic();                                                        // ��������

	void SetContent(CString);										// ������ʾ����
	void SetStyle();
	void SetTitleText(CString);                                     // ���ñ��������
	void SetLabelText(int, int, CString);                           // �����ı������ʾ����
	void SetLabelColor(int, int, COLORREF);                         // �����ı������ֵ���ɫ
	void SetLableAlign(int row, int col, DWORD align);				// �����ı������ֵĶ��뷽ʽ
	void SetLableAlign(int col, DWORD align);						// �����ı������ֵĶ��뷽ʽ��һ�У�
	void SetLableWidth(int row, int col, int width);				// �����ı������ֵĿ��
	void SetLableWidth(int col, int width);							// �����ı������ֵĿ�ȣ�һ�У�
	CGEdit* GetEdit(int);                                           // ȡ�ñ༭��ĵ�ַ
	COLORREF m_Title_Font_Color;									// ����������ɫ

protected:
	virtual void PreSubclassWindow();

private:
	PGSTATIC_INFO m_gstaticInfo;                                    // �����е�����
	PLABEL_GROUP_THREE m_labelGroupThree;                           // 3������
	PLABEL_GROUP_FOUR m_labelGroupFour;                             // 4������ 
	PLABEL_GROUP_FIVE m_labelGroupFive;                             // 5������
	PLABEL_GROUP_SIX  m_labelGroupSix;                              // 6������
	PLABEL_GROUP_SEVEN m_labelGroupSeven;                           // 7������
	CGEdit* m_editArray[64];                                        // �༭������
	int m_labelRow;                                                 // �ı�������

	CFont m_fontR;
	CFont m_font;                                                  // ��������
	CFont m_titlefont;                                             // ������������
	int m_col;                                                      // ����
	COLORREF m_colorArray[MAX_ROW][MAX_COL];                        // ���е���ɫ
	DWORD m_alignArray[MAX_ROW][MAX_COL];							// ���еĶ��뷽ʽ
	int m_widthArray[MAX_ROW][MAX_COL];								// ���еĿ��

	bool m_isUsedForAuditSvc;                                       // �Ƿ�Ϊ����ͳ��ҵ��ר��

	afx_msg void OnPaint();                                         // ��������
	afx_msg BOOL OnEraseBkgnd(CDC*);									// ��������
	afx_msg int OnCreate(LPCREATESTRUCT);                           // ����仯ʱˢ�»���
	CRect GetDrawRect( CPoint point, int rows );
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	int m_width;			// �ؼ����
	int m_border;			// �߿��С
	CPoint m_round;			// Բ�ǻ���
	MARGINS m_margin;		// ��߾�
	PADDINGS m_padding;		// �ڱ߾�
	int m_lable_height;		// �ı��и߶�
	int m_title_height;		// �����и߶�
	int m_lable_left_with;    // �ı����������
	int m_lable_right_with;    // �ı������Ҳ���
	int m_lable_mid_with;     // �ı����������
	CString m_szContent;
	// ��ά����ʾ���
	void OnTimer(UINT nIDEvent);
	CPictureEx m_picex;// ��̬ͼƬ
	CImage* m_pErweimaImg;
	CImage* m_pPaymentTipsImg;
	CImage* m_pUseTipImg;
	CImage* m_pNoticeImgBk;
	CImage* m_pBkImg;
};