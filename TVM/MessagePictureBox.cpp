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


CMessagePictureBox* CMessagePictureBox::s_curMsgBox = NULL;    // ��ǰ��Ϣ��

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CString title        ����
@param      (i)CString message      ��Ϣ����(������Ϣ��"|"�ָ�)
@param      (i)BTN_TYPE btnType     ��ť����
@param      (i)UINT format          ��ʽ��DT_CENTER|DT_VCENTER|DT_SINGLELINE��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMessagePictureBox::CMessagePictureBox(CString title, CString message, CString imageLoc, BTN_TYPE btnType, UINT format,int showSeconds,BTN_TYPE focusBtnType)
    : CBaseDlg(CMessagePictureBox::IDD, theAPP_SESSION.GetChildViewRec(), MESSAGE_PICTURE_BOX_FRAME_RECT, SKYBLUE,NULL,false)
{
	//ȡ����·��
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

	// BUG 2322�� IT_1507_TVM_UI_���е��������Ӧȥ���������ȴ�ˢ����ˢ��������ʾ���ڶ���ȷ��ˢ����������ʾ��
	// Ӧ��bug���󣬽��Ի��������Ϊ����
    m_title = _T("")/*title*/;
	m_showSeconds = showSeconds;
    // ������Ϣ����(ȥ����ߵ���ɫ��ʾ����)
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CMessagePictureBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBaseDlg::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // ���㰴ťλ��
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
	
    // ����ȷ�ϰ�ť
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

    // ����ȡ����ť
    if (m_btnType == BTN_CANCEL || m_btnType == BTN_OKCANCEL) {
        btnName.Format(_T("%i"), BTN_CANCEL);
		UIINFO buttonUI;
		buttonUI.m_Location = CPoint(x2,y);
		buttonUI.m_pParentWnd = this;
		buttonUI.m_Size = DEFAULT_SIZE_MENU_BUTTON;
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"),IDCANCEL);
		buttonInfo.text = _tl(STR_CANCEL);
		buttonInfo.type = BUTTON_TYPE_NOSELECT;//��Ӧ�޸�ͼƬ��ɫΪ��ɫ
		buttonInfo.btnID = 1;
        m_btnCancel = new CXButton(buttonUI,buttonInfo);
		m_btnCancel->Create();
    }

	if(m_showSeconds!=INFINITE){
		this->SetTimer(DEFAULT_SHOW_TIMER,1*1000,NULL);
	}
    s_curMsgBox = this;    // ��ǰ��Ϣ��
    return 0;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ٺ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMessagePictureBox::OnDestroy()
{
	if(m_showSeconds!=INFINITE){
		this->KillTimer(DEFAULT_SHOW_TIMER);
	}
	__super::OnDestroy();
    s_curMsgBox = NULL;    // ��ǰ��Ϣ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      

@param      ��

@retval     ��

@exception  ��
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
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CMessagePictureBox::OnDraw(CDC* pDC)
{
	CRect roundRect;
	GetClientRect(roundRect);
	// �����ڴ�DC
	CDC MemDc;
	MemDc.CreateCompatibleDC(NULL);// �ڴ���׼��ͼ��
	// �����ڴ�λͼ
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC,roundRect.Width(),roundRect.Height());
	// �ڴ�λͼ���ͼDC����
	MemDc.SelectObject(MemBitmap);
	// ���ñ���ɫ
	MemDc.FillSolidRect(0,0,roundRect.Width(),roundRect.Height(),RGB(255,255,255));
	// ʹ���ڴ�DC��ͼ
	CDC* pTmpDc = &MemDc;

	// ��ͼ�С�����
	DWORD titleSize;
	CString titleFont;

	DWORD textSize;
	CString textFont;

	/*titleSize = theGUI_INFO.GetRecMessageBoxTitleFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	titleFont = theGUI_INFO.GetRecMessageBoxTitleFontName(theAPP_SESSION.GetCurrentClientLanguageType());

	textSize = theGUI_INFO.GetRecMessageBoxFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
	textFont = theGUI_INFO.GetRecMessageBoxFontName(theAPP_SESSION.GetCurrentClientLanguageType());*/

    // ���߿�
    DrawRoundRect(pTmpDc, roundRect, LINE_BORDER, BLUE, WHITE);

    // �������
    DrawRoundRect(pTmpDc, CRect(0, 0, MSG_BOX_WIDTH, 50), LINE_BORDER, BLUE, BLUE);

    // д����
    DrawText(pTmpDc, 20, 10, m_title, GUEST_MSGBOX_TITLE_FONT_NAME/*titleFont*/,GUEST_MSGBOX_TITLE_FONT_SIZE /*titleSize*/, GUEST_MSGBOX_TITLE_FONT_COLOR/*WHITE*/);

	if(m_showSeconds!=INFINITE)
	{
		CString strShowSecond = _T("");
		strShowSecond.Format(_T("%d"),m_showSeconds);
		
		DrawText(pTmpDc,MSG_BOX_WIDTH - 48 - 5*strShowSecond.GetLength(), 10, strShowSecond, GUEST_MSGBOX_TITLE_FONT_NAME/*titleFont*/, GUEST_MSGBOX_TITLE_FONT_SIZE/*titleSize*/, WHITE);
	}
	if(imagePath.GetBuffer() == NULL || imagePath.GetLength() == 0)// ���δָ��ͼƬ��·��
	{
		int i = 0;
		CString resToken = m_message;
		CString messageLine = _T("");
		// liuwensheng  �޸���Ϣ���е��������ݲ��֣����������¾���
		//����Ϣ��������
		int	nLineCount = (m_message.GetLength() % MAX_LINE_LENGTH_ONLY_TXT) > 0 ? ((m_message.GetLength() / MAX_LINE_LENGTH_ONLY_TXT) + 1 ) : (m_message.GetLength() / MAX_LINE_LENGTH_ONLY_TXT);
		int nFirstLineLoc = (BUTTON_TOP_MARGIN - nLineCount*TEXT_HEIGHT) / 2;			// ����y��λ��
		
		while (resToken != _T("")) {
			messageLine = resToken.Left(MAX_LINE_LENGTH_ONLY_TXT);
			int lineLengh = MAX_LINE_LENGTH_ONLY_TXT;									// ��ʾ��Ϣÿ�г���
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
		// ��ͼƬ
		m_Image.Destroy();
		m_Image.Load(imagePath);
		//CDC *dc = this->GetDC();
		//HDC hdc = dc->GetSafeHdc();
		m_Image.Draw(pTmpDc->GetSafeHdc(), PIC_LEFT_MARGIN, PIC_TOP_MARGIN, PIC_WIDTH, PIC_HEIGHT);
		//this->ReleaseDC(dc);

		// д��Ϣ����
		int i = 0;
		int nCount = GetMessageLineCount();
		CString resToken = m_message;
		CString messageLine = _T("");
		// liuwensheng  �޸���Ϣ���е��������ݲ��֣����������¾���
		//����Ϣ��������
		int	nLineCount = (m_message.GetLength() % MAX_LINE_LENGTH_EN) > 0 ? ((m_message.GetLength() / MAX_LINE_LENGTH_EN) + 1 ) : (m_message.GetLength() / MAX_LINE_LENGTH_EN);
		int nFirstLineLoc = (BUTTON_TOP_MARGIN - nLineCount*TEXT_HEIGHT) / 2;			// ����y��λ��
		if ( nCount> 1 )
		{
			CString smsg = m_message;
			int nPos = 0;
			CStringArray sAr;
			// ���н����ַ���
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
			// ��������
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
				int lineLengh = MAX_LINE_LENGTH_EN;    // ��ʾ��Ϣÿ�г���
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
	
	// ����������ͼ�ο�������ͼDC
	pDC->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// ��ͼ����
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// ��ͼ����
	
	
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
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
            TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ��ʾ��Ϣ��

@param      (i)CString title        ����
@param      (i)CString message      ��Ϣ����(������Ϣ��"|"�ָ�)
@param      (i)BTN_TYPE btnType     ��ť����
@param      (i)UINT format          ��ʽ��DT_CENTER|DT_VCENTER|DT_SINGLELINE��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
INT_PTR CMessagePictureBox::Show(CString title, CString message, CString imageLoc, BTN_TYPE btnType, UINT format,int showSeconds,BTN_TYPE focusBtnType)
{
    // ������Ϣ������ʾ�����ȹر�
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
@brief      �Ƿ������Ϣ��

@param      ��

@retval     bool    true:����; false:������

@exception  ��
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
@brief      ������Ϣ��������ڣ�

@param      (i)int nResult  ��Ϣ�����ʱ�ķ���ֵ

@retval     ��

@exception  ��
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
@brief      ��ȡ��ʾ��Ϣ����

@param      ��

@retval     int ��ʾ��Ϣ����

@exception  ��
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
	CBrush bkBrush(BLUE);      // ���屳��ɫˢ��
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &bkBrush); // �ñ���ɫ��һ������


	//// �������
	//DrawRoundRect(pDC, CRect(0, 0, MSG_BOX_WIDTH, 50), LINE_BORDER, BLUE, BLUE);

	return TRUE;
}


HBRUSH CMessagePictureBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//// TODO:  �ڴ˸��� DC ���κ�����
	//if (nCtlColor == CTLCOLOR_STATIC) {
	//	pDC->SetBkMode(TRANSPARENT);
	//	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	//}
	HBRUSH	hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}