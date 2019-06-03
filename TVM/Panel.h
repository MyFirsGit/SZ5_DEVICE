#pragma once
#include "Func.h"
#include "bag.h"
#include "UIDefinition.h"
#include <shlwapi.h>
#include <atlimage.h>


typedef struct _UIInfo
{
	CRect m_Area;					//按钮区域
	CPoint m_Location;				//位置
	CSize	m_Size;					//大小
	DOCKSTYLE m_DockStyle;			//对齐方式
	PADDINGS m_Paddings;			//内补白
	MARGINS m_Margins;				//外补白
	int		m_BorderWidth;			//边框大小
	COLORREF m_BorderColor;			//边框颜色
	COLORREF m_BackColor;			//背景色
	COLORREF m_ForeColor;			//前景色
	CImage*		m_pBackgroundImage;
	//CBitmap *m_pBackgroundImage;		//背景图片
	CString m_strBackgroundImage;		//背景图片
	int m_BKMode;					//背景模式
	CWnd* m_pParentWnd;				//父窗体
	BOOL m_Enabled;					//可用
	BOOL m_Visible;					//可见
	LOGFONT m_Font;					//字体
	int m_nParentType;				//扩展字段，用于标识类型进行区分父窗口。默认0，现在仅用1区分总图窗口。

	_UIInfo()
	{
		m_Area = CRect(0,0,0,0);
		m_Location = CPoint(0,0);
		m_Size = DEFAULT_SIZE_STATIC;
		m_DockStyle = DOCKSTYLE::NONE;
		m_Paddings = DEFAULT_PADDINGS;
		m_Margins = DEFAULT_MARGINS;
		m_BorderWidth = 0;
		m_BorderColor = BLACK;
		m_BackColor = DEFAULT_COLOR_STATIC;
		//m_pBackgroundImage = NULL;
		m_pBackgroundImage=NULL;
		m_ForeColor = BLACK;
		m_BKMode = TRANSPARENT;
		m_pParentWnd = NULL;
		m_Enabled = TRUE;
		m_Visible = TRUE;
		m_Font = DEFAULT_FONT;
		m_strBackgroundImage = _T("");
		m_nParentType = 0;
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
		m_Font.lfWidth == another.m_Font.lfWidth&&
		//m_strBackgroundImage == another.m_strBackgroundImage;
		m_pBackgroundImage==another.m_pBackgroundImage&&
		m_nParentType == another.m_nParentType;
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

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);   // 在窗口显示按钮
	
	virtual BOOL Create(Func<void (CPanel*)> * creating);

	virtual BOOL Create();

	virtual CRect GetFrameRect();

	virtual CRect GetContentRect();

	CBaseDlg* GetParentDlg();

	LRESULT CallWndProc(UINT message, WPARAM wParam, LPARAM lParam);

	CBag* GetBag();
	BOOL DrawTransprentDlg(HWND hWnd,BYTE transprt);

	void ConvertPng(CImage* image);
protected:
		
	

	void CalculateFrameRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint point);

	UIINFO m_UIInfo;
	
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CPoint m_ActualLocation;
	CSize m_ActualSize;

	bool calculatedFrameRect;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);                        // 背景色能否删除
private:
	CBag* m_pBag;
};