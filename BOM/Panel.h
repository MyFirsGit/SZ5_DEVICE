#pragma once
#include "Func.h"
#include "bag.h"
#include "UIDefinition.h"
#include <shlwapi.h>


typedef struct _UIInfo
{
	CPoint m_Location;				//λ��
	CSize	m_Size;					//��С
	DOCKSTYLE m_DockStyle;			//���뷽ʽ
	PADDINGS m_Paddings;			//�ڲ���
	MARGINS m_Margins;				//�ⲹ��
	int		m_BorderWidth;			//�߿��С
	COLORREF m_BorderColor;			//�߿���ɫ
	COLORREF m_BackColor;			//����ɫ
	COLORREF m_ForeColor;			//ǰ��ɫ
	CBitmap *m_pBackgroundImage;		//����ͼƬ
	int m_BKMode;					//����ģʽ
	CWnd* m_pParentWnd;				//������
	BOOL m_Enabled;					//����
	BOOL m_Visible;					//�ɼ�
	LOGFONT m_Font;					//����

	_UIInfo()
	{
		m_Location = CPoint(0,0);
		m_Size = DEFAULT_SIZE_STATIC;
		m_DockStyle = DOCKSTYLE::NONE;
		m_Paddings = DEFAULT_PADDINGS;
		m_Margins = DEFAULT_MARGINS;
		m_BorderWidth = 0;
		m_BorderColor = BLACK;
		m_BackColor = DEFAULT_COLOR_STATIC;
		m_pBackgroundImage = NULL;
		m_ForeColor = BLACK;
		m_BKMode = TRANSPARENT;
		m_pParentWnd = NULL;
		m_Enabled = TRUE;
		m_Visible = TRUE;
		m_Font = DEFAULT_FONT;
	}

	bool operator == (const _UIInfo& another) const
	{
		return m_Location == another.m_Location &&
		m_Size == another.m_Size &&
		m_DockStyle == another.m_DockStyle &&
		m_Paddings == another.m_Paddings &&
		m_Margins.cxLeftWidth == another.m_Margins.cxLeftWidth &&
		m_Margins.cxRightWidth == another.m_Margins.cxRightWidth &&
		m_Margins.cyTopHeight == another.m_Margins.cyTopHeight &&
		m_Margins.cyBottomHeight == another.m_Margins.cyBottomHeight &&
		m_BorderWidth == another.m_BorderWidth &&
		m_BackColor == another.m_BackColor &&
		m_pBackgroundImage == another.m_pBackgroundImage &&
		m_ForeColor == another.m_ForeColor &&
		m_BKMode == another.m_BKMode &&
		m_pParentWnd == another.m_pParentWnd &&
		m_Enabled == another.m_Enabled &&
		m_Visible == another.m_Visible &&
		m_Font.lfCharSet == another.m_Font.lfCharSet &&
		m_Font.lfClipPrecision == another.m_Font.lfClipPrecision &&
		m_Font.lfEscapement == another.m_Font.lfEscapement &&
		StrCmp(m_Font.lfFaceName,another.m_Font.lfFaceName) == 0 &&
		//m_Font.lfFaceName == another.m_Font.lfFaceName &&
		m_Font.lfHeight == another.m_Font.lfHeight &&
		m_Font.lfItalic == another.m_Font.lfItalic &&
		m_Font.lfOrientation == another.m_Font.lfOrientation &&
		m_Font.lfOutPrecision == another.m_Font.lfOutPrecision &&
		m_Font.lfPitchAndFamily == another.m_Font.lfPitchAndFamily &&
		m_Font.lfQuality == another.m_Font.lfQuality &&
		m_Font.lfStrikeOut == another.m_Font.lfStrikeOut &&
		m_Font.lfUnderline == another.m_Font.lfUnderline &&
		m_Font.lfWeight == another.m_Font.lfWeight &&
		m_Font.lfWidth == another.m_Font.lfWidth;
	}
	
} UIINFO,*PUIINFO;

class CBaseDlg;
class CBag;
class CPanel:public CStatic
{

	DECLARE_DYNAMIC(CPanel)
	DECLARE_MESSAGE_MAP()
public:

	CPanel(UIINFO& uIInfo);

	virtual ~CPanel();

	UIINFO GetUIInfo();

	virtual void SetUIInfo(UIINFO& uIInfo);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);   // �ڴ�����ʾ��ť
	
	virtual BOOL Create(Func<void (CPanel*)> * creating);

	virtual BOOL Create();

	virtual CRect GetFrameRect();

	virtual CRect GetContentRect();

	CBaseDlg* GetParentDlg();

	LRESULT CallWndProc(UINT message, WPARAM wParam, LPARAM lParam);

	CBag* GetBag();

protected:
		
	

	void CalculateFrameRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint point);

	UIINFO m_UIInfo;
	
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CPoint m_ActualLocation;
	CSize m_ActualSize;

	bool calculatedFrameRect;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);                        // ����ɫ�ܷ�ɾ��
private:
	CBag* m_pBag;
};