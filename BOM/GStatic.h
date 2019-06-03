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
    CString nameLeft;               // �ı�������
    CString nameRight;              // �ı�������
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
            name1 = "";
            name2 = "";
            name3 = "";
            name4 = "";
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
    GSTATIC_TITLE_INFO titleInfo;   // ������Ϣ
    GSTATIC_DETAIL_INFO detailInfo; // ��ϸ��Ϣ

} GSTATIC_INFO, *PGSTATIC_INFO;




//
//@brief   ����Բ�Ǿ��μ����е��ı���ͱ༭�򣬲�ִ����ز��� 

class CGStatic : public CStatic
{
	DECLARE_DYNAMIC(CGStatic)
	DECLARE_MESSAGE_MAP()

public:
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, bool isUsedForAuditSvc = false);  // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_THREE);              // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FOUR);               // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_FIVE);               // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SIX);                // ���캯��
	CGStatic(CWnd* pParentWnd, PGSTATIC_INFO, PLABEL_GROUP_SEVEN);              // ���캯��
	virtual ~CGStatic();                                                        // ��������

	void SetLabelText(int, int, CString);                           // �����ı������ʾ����
	CGEdit* GetEdit(int);                                           // ȡ�ñ༭��ĵ�ַ
	void SetLabelColor(int, int, COLORREF);                         // �����ı������ֵ���ɫ
	void SetTitleText(CString);                                     // ���ñ��������

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

	CFont* m_font;                                                  // ��������
	int m_col;                                                      // ����
	COLORREF m_colorArray[MAX_ROW][MAX_COL];                        // ���е���ɫ

	bool m_isUsedForAuditSvc;                                       // �Ƿ�Ϊ����ͳ��ҵ��ר��

	afx_msg void OnPaint();                                         // ��������
	afx_msg int OnCreate(LPCREATESTRUCT);                           // ����仯ʱˢ�»���
	CRect GetDrawRect( CPoint point, int rows );
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};