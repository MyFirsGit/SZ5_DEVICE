// CLogoDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "TVM.h"
#include "LogoDLG.h"
#include "servicemgr.h"
#include "servicedefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CLogoDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CLogoDlg, CBaseDlg)
END_MESSAGE_MAP()

static CRect  LOGO_TEXT_RECT = CRect(105,20,267,70);
const static TCHAR* const LOGO_TEXT = _T("XX地铁");
static CRect  LOGO_ENGLISH_TEXT_RECT = CRect(105,70,267,90);
const static TCHAR* const LOGO_ENGLISH_TEXT = _T("XX Metro");
static CRect VERSION_RECT = CRect(25,0,267,20);

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLogoDlg::CLogoDlg(CWnd* pParent)
: CBaseDlg(CLogoDlg::IDD, pParent, LOGO_FRAME_RECT,OPERATOR_HEAD_BK_COLOR), m_hLogoBmp(NULL)
{
    m_hLogoBmp = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, _T(".\\res\\logo.bmp"),IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	theVERSION_INI.VersionNoChanged.AddHandler(this,&CLogoDlg::OnVersionNoChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLogoDlg::~CLogoDlg()
{
	theVERSION_INI.VersionNoChanged.RemoveHandler(this,&CLogoDlg::OnVersionNoChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLogoDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	if (m_hLogoBmp != NULL) {
		// 获取Logo窗口大小
		CRect rect;
		this->GetWindowRect(&rect);
		ScreenToClient(&rect);          // 转换成相对于当前OperationDlg的位置

		// 画图
		CDC srcDC;
		srcDC.CreateCompatibleDC(pDC);

		CBitmap* pLogoBmp = CBitmap::FromHandle(m_hLogoBmp);
		CBitmap* pOldBmp = srcDC.SelectObject(pLogoBmp);
		
		// LOGO
		pDC->BitBlt(LOGO_RECT.left, LOGO_RECT.top, LOGO_RECT.Width(), LOGO_RECT.Height(), &srcDC, 0, 0, SRCCOPY);	
		srcDC.SelectObject(pOldBmp);
	}
	CString strVersion = _T("");
	strVersion.Format(_T("TVM:V%d RW:V%d "), theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CHARGE_RW_APP));
	DrawText(pDC,VERSION_RECT,strVersion,LOGO_ENGLISH_TEXT,VERSION_FONT_SIZE,FW_NORMAL,BLACKGRAY,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      版本号发生改变时响应函数

@param      

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLogoDlg::OnVersionNoChanged(CVersionInfo::VER_TYPE verType, CVersionInfo::PROG_TYPE progType, DWORD versionNo)
{
	this->Invalidate();
}