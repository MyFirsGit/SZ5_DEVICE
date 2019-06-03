#include "stdafx.h"
#include "AllLineShowBox.h"
#include "UIDefinition.h"
#include "AppSession.h"
#include "MessageIDDefinition.h"
#include "ParseStationMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CAllLineShowBox, CBaseDlg)

BEGIN_MESSAGE_MAP(CAllLineShowBox, CBaseDlg)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_XBUTTON, XButtonClick)
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CString title        标题
@param      (i)CString message      消息内容(多行消息用"|"分割)
@param      (i)BTN_TYPE btnType     按钮种类
@param      (i)UINT format          格式（DT_CENTER|DT_VCENTER|DT_SINGLELINE）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAllLineShowBox::CAllLineShowBox(UINT ID, CService* pService,CRect rect,bool needFunctionArea)
	:CReceptionDlg(ID, pService, rect, needFunctionArea)
{
	//取程序路径
	CString strAppPath;
	GetAppPath(strAppPath);

	CString imagePath = strAppPath + _T(".\\res\\TVM_all_line_bkg.png");
	m_Image.Load(imagePath);
	
	m_btnClose = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAllLineShowBox::~CAllLineShowBox()
{
	if(m_btnClose){
		delete m_btnClose;
		m_btnClose = NULL;
	}
	
	for (int i=0;i<m_vecLineBtn.size();i++){
		delete m_vecLineBtn[i];
	}
	m_vecLineBtn.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CAllLineShowBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBaseDlg::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    return 0;
}

BOOL CAllLineShowBox::OnInitDialog()
{
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();
	// 窗口大小1155 X 910
	int nMaxPageBtns = 6*5;//(910-250)/(60+40)		(1155-60)/(160+50)=1095/210
	int nPageMaxRow = 6;
	int nPageMaxCol = 5;
	int nLineBtnMargeX = 50;	// 按钮水平间距
	int nLineBtnMargeY = 40;	// 按钮垂直间距
	int nLineBtnWidth = 160;	// 按钮宽度
	int nLineBtnHeight = 60;	// 按钮高度
	int nLineBtnStartX = 60;	// 按钮起始X
	int nLineBtnStartY = 250-70;	// 按钮起始Y
	// 显示线路按钮
	vector<int> vec_LineId;
	theSTATION_MAP_XML.GetLineIds(vec_LineId);
	int nLineCount = vec_LineId.size();
	int nCurPosX = nLineBtnStartX;
	int nCurPosY = nLineBtnStartY;
	for (int i=0;i<nPageMaxRow;i++){
		nCurPosX = nLineBtnStartX;
		for(int j=0;j<nPageMaxCol;j++){
			int nCurIndex = i*nPageMaxCol+j;
			if(nCurIndex >= nLineCount){
				break;
			}
			UIINFO buttonUI;
			buttonUI.m_Location = CPoint(nCurPosX,nCurPosY);
			buttonUI.m_pParentWnd = this;
			buttonUI.m_Size = CSize(nLineBtnWidth, nLineBtnHeight);
			BTN_INFO buttonInfo;
			buttonInfo.btnMsg.Format(_T("%d"), vec_LineId[nCurIndex]);	
			if(language==LANGUAGE_ID_CHINESE){
				buttonInfo.text.Format(_T("%d号线"), vec_LineId[nCurIndex]);		
			}
			else{
				buttonInfo.text.Format(_T("Line %d"), vec_LineId[nCurIndex]);	
			}
			buttonInfo.type = BUTTON_TYPE_BLUE;
			buttonInfo.btnID = 0;
			CXButton* pBtn = new CXButton(buttonUI,buttonInfo);
			pBtn->Create();
			m_vecLineBtn.push_back(pBtn);
			nCurPosX += nLineBtnWidth + nLineBtnMargeX;
		}
		nCurPosY += nLineBtnHeight + nLineBtnMargeY;
	}
	// 关闭按钮
	{
		UIINFO buttonUI;
		buttonUI.m_Location = CPoint(955,30);
		buttonUI.m_pParentWnd = this;
		buttonUI.m_Size = CSize(nLineBtnWidth, nLineBtnHeight);
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"), 255);	
		if(language==LANGUAGE_ID_CHINESE){
			buttonInfo.text.Format(_T("关闭"));		
		}
		else{
			buttonInfo.text.Format(_T("Close"));	
		}
		buttonInfo.type = BUTTON_TYPE_CLOSE;
		buttonInfo.btnID = 0;
		m_btnClose = new CXButton(buttonUI,buttonInfo);
		m_btnClose->Create();
	}
	// 调整窗口大小及位置
	SetWindowPos(&CWnd::wndTopMost, REC_LINE_MAP_RECT.left, REC_LINE_MAP_RECT.top+5, REC_LINE_MAP_RECT.Width(), REC_LINE_MAP_RECT.Height(), SWP_SHOWWINDOW);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面销毁函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllLineShowBox::OnDestroy()
{
	__super::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllLineShowBox::OnDraw(CDC* pDC)
{
	CRect roundRect;
	GetClientRect(roundRect);
	// 创建内存DC
	CDC MemDc;
	MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
	// 创建内存位图
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC,roundRect.Width(),roundRect.Height());
	// 内存位图与绘图DC关联
	MemDc.SelectObject(MemBitmap);
	// 设置背景色
	MemDc.FillSolidRect(0,0,roundRect.Width(),roundRect.Height(),RGB(255,255,255));
	// 使用内存DC绘图
	CDC* pTmpDc = &MemDc;

	// 绘图中。。。
    // 画边框
    DrawRoundRect(pTmpDc, roundRect, LINE_BORDER, BLUE, WHITE);
	// 图片
	m_Image.Draw(pTmpDc->GetSafeHdc(), roundRect);
	
	// 将缓冲区的图形拷贝到绘图DC
	pDC->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// 绘图清理
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// 绘图结束
	
	
	if(!hasSetFocus){
		hasSetFocus = true;
		if(NULL != m_btnClose){
			m_btnClose->SetSelect(false);
		}
		if(m_btnClose!=NULL){
			m_btnClose->Invalidate();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
            TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CAllLineShowBox::XButtonClick(WPARAM wParam, LPARAM lParam)
{
    CString sMsg = (LPCTSTR)lParam;
	m_bShowing = false;
    this->EndDialog(_ttoi(sMsg));

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示消息框

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
INT_PTR CAllLineShowBox::Show()
{
	m_bShowing = true;
	return DoModal();
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      结束消息框（如果存在）

@param      (i)int nResult  消息框结束时的返回值

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAllLineShowBox::End(int nResult)
{
	if(m_bShowing){
		EndDialog(nResult);
	}
	m_bShowing = false;
}