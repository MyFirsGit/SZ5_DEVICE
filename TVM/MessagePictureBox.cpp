#include "stdafx.h"
#include "MessagePictureBox.h"
#include "UIDefinition.h"
#include "AppSession.h"
#include "MessageIDDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

delegate<void()> CMessagePictureBox::OnShowing;

IMPLEMENT_DYNAMIC(CMessagePictureBox, CBaseDlg)

BEGIN_MESSAGE_MAP(CMessagePictureBox, CBaseDlg)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_XBUTTON, XButtonClick)
    //ON_WM_ERASEBKGND()
    //ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


CMessagePictureBox* CMessagePictureBox::s_curMsgBox = NULL;    // 当前消息框

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
CMessagePictureBox::CMessagePictureBox(CString title, CString message, CString imageLoc, BTN_TYPE btnType, UINT format,int showSeconds,BTN_TYPE focusBtnType)
    : CBaseDlg(CMessagePictureBox::IDD, theAPP_SESSION.GetChildViewRec(), MESSAGE_PICTURE_BOX_FRAME_RECT, SKYBLUE,NULL,false)
{
	//取程序路径
	CString strAppPath;
	GetAppPath(strAppPath);
	if (imageLoc.GetBuffer() == NULL || imageLoc.GetLength() ==0)
	{
		imagePath = _T("");
	}
	else
		imagePath = strAppPath + imageLoc;

    m_btnOK = NULL;
    m_btnCancel = NULL;

	// BUG 2322： IT_1507_TVM_UI_所有弹出框标题应去掉（包括等待刷卡，刷卡错误提示，第二次确认刷卡，降级提示）
	// 应此bug需求，将对话框标题设为控制
    m_title = _T("")/*title*/;
	m_showSeconds = showSeconds;
    // 解析消息内容(去掉左边的颜色提示内容)
    COLORREF color;
    theAPP_SESSION.ParseGuideMsg(message, m_message, color);
    //m_message = message;
    m_btnType = btnType;
    m_format = format;
	if(btnType == BTN_OK){
		m_focusBtnType = BTN_OK;
	}
	else if(btnType == BTN_CANCEL){
		m_focusBtnType = BTN_CANCEL;
	}
	else if(btnType == BTN_OKCANCEL && focusBtnType == BTN_OKCANCEL){
		m_focusBtnType = BTN_OK;
	}
	else{
		m_focusBtnType = focusBtnType;
	}
	hasSetFocus = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMessagePictureBox::~CMessagePictureBox()
{
    delete m_btnOK;
    m_btnOK = NULL;

    delete m_btnCancel;
    m_btnCancel = NULL;
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
int CMessagePictureBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBaseDlg::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // 计算按钮位置
    int x1 = 0;
    int x2 = 0;
    int y = BUTTON_TOP_MARGIN;

    if (m_btnType == BTN_OKCANCEL){
        x1 = (MSG_BOX_WIDTH - BUTTONS_DISTANCE)/2 - DEFAULT_SIZE_MENU_BUTTON.cx;
        x2 = (MSG_BOX_WIDTH + BUTTONS_DISTANCE)/2;
    }
    else{
        x1 = (MSG_BOX_WIDTH-DEFAULT_SIZE_MENU_BUTTON.cx)/2;
        x2 = x1;
    }

    CString btnName = _T("");
	
    // 创建确认按钮
    if (m_btnType == BTN_OK || m_btnType == BTN_OKCANCEL){
        btnName.Format(_T("%i"), BTN_OK);
		UIINFO buttonUI;
		buttonUI.m_Location = CPoint(x1,y);
		buttonUI.m_pParentWnd = this;
		buttonUI.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"),IDOK);
		buttonInfo.text = _tl(STR_OK);
		buttonInfo.type = BUTTON_TYPE_OK;
		buttonInfo.btnID = 0;
        m_btnOK =new CXButton(buttonUI,buttonInfo);
		m_btnOK->Create();
    }

    // 创建取消按钮
    if (m_btnType == BTN_CANCEL || m_btnType == BTN_OKCANCEL) {
        btnName.Format(_T("%i"), BTN_CANCEL);
		UIINFO buttonUI;
		buttonUI.m_Location = CPoint(x2,y);
		buttonUI.m_pParentWnd = this;
		buttonUI.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"),IDCANCEL);
		buttonInfo.text = _tl(STR_CANCEL);
		buttonInfo.type = BUTTON_TYPE_NOSELECT;//对应修改图片底色为蓝色
		buttonInfo.btnID = 1;
        m_btnCancel = new CXButton(buttonUI,buttonInfo);
		m_btnCancel->Create();
    }

	if(m_showSeconds!=INFINITE){
		this->SetTimer(DEFAULT_SHOW_TIMER,1*1000,NULL);
	}
    s_curMsgBox = this;    // 当前消息框
    return 0;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      画面销毁函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMessagePictureBox::OnDestroy()
{
	if(m_showSeconds!=INFINITE){
		this->KillTimer(DEFAULT_SHOW_TIMER);
	}
	__super::OnDestroy();
    s_curMsgBox = NULL;    // 当前消息框
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMessagePictureBox::OnTimer(UINT_PTR timerID)
{
	if(timerID == DEFAULT_SHOW_TIMER){
		m_showSeconds--;
		if(m_showSeconds==0){
			INPUT input[2];
			::ZeroMemory(input, sizeof(input));     
			input[0].type = input[1].type = INPUT_KEYBOARD;
			input[0].ki.wVk  = input[1].ki.wVk = VK_RETURN;  
			input[1].ki.dwFlags = KEYEVENTF_KEYUP;  // THIS IS IMPORTANT
			::SendInput(1, input, sizeof(INPUT));   
			::SendInput(1, input+1, sizeof(INPUT));  
		}
		else if(m_showSeconds<0){
			End(IDABORT);
		}
		else{
			CRect titleRect(200, 10, MSG_BOX_WIDTH, 40);
			//CRect titleRect(740, 16, 740+25, 16+25);
			this->InvalidateRect(titleRect, FALSE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMessagePictureBox::OnDraw(CDC* pDC)
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
	DWORD titleSize;
	CString titleFont;

	DWORD textSize;
	CString textFont;

	/*titleSize = theGUI_INFO.GetRecMessageBoxTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	titleFont = theGUI_INFO.GetRecMessageBoxTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());

	textSize = theGUI_INFO.GetRecMessageBoxFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	textFont = theGUI_INFO.GetRecMessageBoxFontName(theAPP_SESSION.GetCurrentClientLanguageType());*/

    // 画边框
    DrawRoundRect(pTmpDc, roundRect, LINE_BORDER, BLUE, WHITE);

    // 画标题框
    DrawRoundRect(pTmpDc, CRect(0, 0, MSG_BOX_WIDTH, 50), LINE_BORDER, BLUE, BLUE);

    // 写标题
    DrawText(pTmpDc, 20, 10, m_title, GUEST_MSGBOX_TITLE_FONT_NAME/*titleFont*/,GUEST_MSGBOX_TITLE_FONT_SIZE /*titleSize*/, GUEST_MSGBOX_TITLE_FONT_COLOR/*WHITE*/);

	if(m_showSeconds!=INFINITE)
	{
		CString strShowSecond = _T("");
		strShowSecond.Format(_T("%d"),m_showSeconds);
		
		DrawText(pTmpDc,MSG_BOX_WIDTH - 48 - 5*strShowSecond.GetLength(), 10, strShowSecond, GUEST_MSGBOX_TITLE_FONT_NAME/*titleFont*/, GUEST_MSGBOX_TITLE_FONT_SIZE/*titleSize*/, WHITE);
	}
	if(imagePath.GetBuffer() == NULL || imagePath.GetLength() == 0)// 如果未指定图片的路径
	{
		int i = 0;
		CString resToken = m_message;
		CString messageLine = _T("");
		// liuwensheng  修改消息框中的文字内容布局，让文字上下居中
		//算消息内容行数
		int	nLineCount = (m_message.GetLength() % MAX_LINE_LENGTH_ONLY_TXT) > 0 ? ((m_message.GetLength() / MAX_LINE_LENGTH_ONLY_TXT) + 1 ) : (m_message.GetLength() / MAX_LINE_LENGTH_ONLY_TXT);
		int nFirstLineLoc = (BUTTON_TOP_MARGIN - nLineCount*TEXT_HEIGHT) / 2;			// 首行y轴位置
		
		while (resToken != _T("")) {
			messageLine = resToken.Left(MAX_LINE_LENGTH_ONLY_TXT);
			int lineLengh = MAX_LINE_LENGTH_ONLY_TXT;									// 提示信息每行长度
			int index = GetAactualCharacterWidth(messageLine);
			while (index > MAX_LINE_LENGTH_ONLY_TXT){
				messageLine = messageLine.Left(--lineLengh);
				index = GetAactualCharacterWidth(messageLine);
			}
			resToken = resToken.Mid(lineLengh);
			DrawText(pTmpDc, CRect(5, nFirstLineLoc + i * TEXT_HEIGHT, 
				MSG_BOX_WIDTH - 5, nFirstLineLoc + (i + 1) * TEXT_HEIGHT), 
				messageLine, GUEST_MSGBOX_TXT_FONT_NAME, GUEST_MSGBOX_TXT_FONT_SIZE, FW_NORMAL, GUEST_MSGBOX_TXT_FONT_COLOR, m_format);
			i++;
		}
	}else
	{
		// 画图片
		m_Image.Destroy();
		m_Image.Load(imagePath);
		//CDC *dc = this->GetDC();
		//HDC hdc = dc->GetSafeHdc();
		m_Image.Draw(pTmpDc->GetSafeHdc(), PIC_LEFT_MARGIN, PIC_TOP_MARGIN, PIC_WIDTH, PIC_HEIGHT);
		//this->ReleaseDC(dc);

		// 写消息内容
		int i = 0;
		int nCount = GetMessageLineCount();
		CString resToken = m_message;
		CString messageLine = _T("");
		// liuwensheng  修改消息框中的文字内容布局，让文字上下居中
		//算消息内容行数
		int	nLineCount = (m_message.GetLength() % MAX_LINE_LENGTH_EN) > 0 ? ((m_message.GetLength() / MAX_LINE_LENGTH_EN) + 1 ) : (m_message.GetLength() / MAX_LINE_LENGTH_EN);
		int nFirstLineLoc = (BUTTON_TOP_MARGIN - nLineCount*TEXT_HEIGHT) / 2;			// 首行y轴位置
		if ( nCount> 1 )
		{
			CString smsg = m_message;
			int nPos = 0;
			CStringArray sAr;
			// 分行解析字符串
			while( TRUE)
			{
				nPos = resToken.Find(_T("|"), 0);
				if (nPos > 0)
				{
					sAr.Add(resToken.Left(nPos -1 ));
					resToken = resToken.Mid(nPos + 1);
				}else
				{
					sAr.Add(resToken);
					break;
				}
			}
			// 绘制文字
			for ( int m = 0; m<nCount; m++ )
			{
				DrawText(pTmpDc, CRect(TEXT_LEFT_MARGIN - 30, nFirstLineLoc + m * TEXT_HEIGHT, 
					MSG_BOX_WIDTH - TEXT_RIGHT_MARGIN + 20, nFirstLineLoc + (m+ 1) * TEXT_HEIGHT), 
					sAr.GetAt(m), GUEST_MSGBOX_TXT_FONT_NAME, GUEST_MSGBOX_TXT_FONT_SIZE, FW_NORMAL, GUEST_MSGBOX_TXT_FONT_COLOR, m_format);
			}
		}else
		{
			while (resToken != _T("")) {
				messageLine = resToken.Left(MAX_LINE_LENGTH_EN);
				int lineLengh = MAX_LINE_LENGTH_EN;    // 提示信息每行长度
				int index = GetAactualCharacterWidth(messageLine);
				while (index > MAX_LINE_LENGTH_EN){
					messageLine = messageLine.Left(--lineLengh);
					index = GetAactualCharacterWidth(messageLine);
				}
				resToken = resToken.Mid(lineLengh);
				DrawText(pTmpDc, CRect(TEXT_LEFT_MARGIN - 30, nFirstLineLoc/*TEXT_TOP_MARGIN*/ + i * TEXT_HEIGHT, 
					MSG_BOX_WIDTH - TEXT_RIGHT_MARGIN + 20, nFirstLineLoc/*TEXT_TOP_MARGIN*/ + (i + 1) * TEXT_HEIGHT), 
					messageLine, GUEST_MSGBOX_TXT_FONT_NAME/*textFont*/, GUEST_MSGBOX_TXT_FONT_SIZE/*textSize*/, FW_NORMAL, GUEST_MSGBOX_TXT_FONT_COLOR/*BLACK*/, m_format);
				i++;
			}
		}
	}
	
	// 将缓冲区的图形拷贝到绘图DC
	pDC->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// 绘图清理
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// 绘图结束
	
	
	if(!hasSetFocus){
		hasSetFocus = true;
		if(NULL != m_btnOK){
			m_btnOK->SetSelect(false);
		}
		if(NULL != m_btnCancel){
			m_btnCancel->SetSelect(false);
		}
		if(m_focusBtnType == BTN_CANCEL){
			m_btnCancel->SetSelect(true)/*SetFocus()*/;
			m_btnCancel->SetFocus();
		}
		if(m_focusBtnType == BTN_OK){
			m_btnOK->SetSelect(true)/*SetFocus()*/;
			m_btnOK->SetFocus();
		}
		if(m_btnCancel!=NULL){
			m_btnCancel->Invalidate();
		}
		if(m_btnOK!=NULL){
			m_btnOK->Invalidate();
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
LRESULT CMessagePictureBox::XButtonClick(WPARAM wParam, LPARAM lParam)
{
    CString sMsg = (LPCTSTR)lParam;

    this->EndDialog(_ttoi(sMsg));

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示消息框

@param      (i)CString title        标题
@param      (i)CString message      消息内容(多行消息用"|"分割)
@param      (i)BTN_TYPE btnType     按钮种类
@param      (i)UINT format          格式（DT_CENTER|DT_VCENTER|DT_SINGLELINE）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
INT_PTR CMessagePictureBox::Show(CString title, CString message, CString imageLoc, BTN_TYPE btnType, UINT format,int showSeconds,BTN_TYPE focusBtnType)
{
    // 若有消息框已显示，则先关闭
    if (Exist()){
        End(IDABORT);
    }
    CMessagePictureBox msgBox(title, message, imageLoc, btnType, format,showSeconds,focusBtnType);
	OnShowing.Invoke();
    INT_PTR retValue = msgBox.DoModal();
	return retValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否存在消息框

@param      无

@retval     bool    true:存在; false:不存在

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CMessagePictureBox::Exist()
{
    if (s_curMsgBox == NULL){
        return false;
    }
    else{
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      结束消息框（如果存在）

@param      (i)int nResult  消息框结束时的返回值

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CMessagePictureBox::End(int nResult)
{
    if (s_curMsgBox != NULL) {
        s_curMsgBox->EndDialog(nResult);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取提示信息行数

@param      无

@retval     int 提示信息行数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CMessagePictureBox::GetMessageLineCount()
{
    int lineCount = 0;
    int curPos = 0;

    do {
        lineCount++;
        curPos = m_message.Find(_T("|"), curPos + 1);
    }
    while (curPos != -1);

    return lineCount;
}


afx_msg BOOL CMessagePictureBox::OnEraseBkgnd(CDC* pDC)
{
	CBrush bkBrush(BLUE);      // 定义背景色刷子
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &bkBrush); // 用背景色画一个矩形


	//// 画标题框
	//DrawRoundRect(pDC, CRect(0, 0, MSG_BOX_WIDTH, 50), LINE_BORDER, BLUE, BLUE);

	return TRUE;
}


HBRUSH CMessagePictureBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  在此更改 DC 的任何属性
	//if (nCtlColor == CTLCOLOR_STATIC) {
	//	pDC->SetBkMode(TRANSPARENT);
	//	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	//}
	HBRUSH	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}