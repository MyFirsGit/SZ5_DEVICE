#include "stdafx.h"
#include "MessageBox.h"
#include "UIDefinition.h"
#include "AppSession.h"
#include "MessageIDDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

delegate<void()> CMessageBox::OnShowing;

IMPLEMENT_DYNAMIC(CMessageBox, CBaseDlg)

BEGIN_MESSAGE_MAP(CMessageBox, CBaseDlg)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_MESSAGE(WM_XBUTTON, XButtonClick)
	ON_WM_TIMER()
END_MESSAGE_MAP()


CMessageBox* CMessageBox::s_curMsgBox = NULL;    // ��ǰ��Ϣ��

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
CMessageBox::CMessageBox(CString title, CString message, BTN_TYPE btnType, UINT format,int showSeconds,BTN_TYPE focusBtnType)
    : CBaseDlg(CMessageBox::IDD, theAPP_SESSION.GetChildViewRec(), MESSAGE_BOX_FRAME_RECT, SKYBLUE,NULL,false)
{
    m_btnOK = NULL;
    m_btnCancel = NULL;

    m_title = title;
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
CMessageBox::~CMessageBox()
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
int CMessageBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CBaseDlg::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // ���㰴ťλ��
    int x1 = 0;
    int x2 = 0;
    int y = 0;

    switch (GetMessageLineCount()) {
    case 0:
        y = 135;
        break;

    case 4:
        y = 175;
        break;

    default:
        y = 155;
        break;
    }

    if (m_btnType == BTN_OKCANCEL) {
        x1 = 70;
        x2 = 245;
    }
    else {
        x1 = 156;
        x2 = x1;
    }

    CString btnName = _T("");
	
    // ����ȷ�ϰ�ť
    if (m_btnType == BTN_OK || m_btnType == BTN_OKCANCEL) {
        btnName.Format(_T("%i"), BTN_OK);
		UIINFO buttonUI;
		buttonUI.m_Location = CPoint(x1,y);
		buttonUI.m_pParentWnd = this;
		buttonUI.m_Size = CSize(80,46);
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"),IDOK);
		buttonInfo.text = _tl(STR_OK);
		buttonInfo.type = BUTTON_TYPE_NOSELECT;
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
		buttonUI.m_Size = CSize(80,46);
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"),IDCANCEL);
		buttonInfo.text = _tl(STR_CANCEL);
		buttonInfo.type = BUTTON_TYPE_NOSELECT;
		buttonInfo.btnID = 1;
        m_btnCancel = new CXButton(buttonUI,buttonInfo);
		m_btnCancel->Create();
    }

	if(m_showSeconds!=INFINITE)
	{
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
void CMessageBox::OnDestroy()
{
	if(m_showSeconds!=INFINITE)
	{
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
void CMessageBox::OnTimer(UINT_PTR timerID)
{
	if(timerID == DEFAULT_SHOW_TIMER)
	{
		m_showSeconds--;
		if(m_showSeconds==0)
		{
			INPUT input[2];
			::ZeroMemory(input, sizeof(input));     
			input[0].type = input[1].type = INPUT_KEYBOARD;
			input[0].ki.wVk  = input[1].ki.wVk = VK_RETURN;  
			input[1].ki.dwFlags = KEYEVENTF_KEYUP;  // THIS IS IMPORTANT
			::SendInput(1, input, sizeof(INPUT));   
			::SendInput(1, input+1, sizeof(INPUT));  
		}
		else if(m_showSeconds<0)
		{
			End(IDABORT);
		}
		else
		{
			CRect titleRect(200, 10, MSG_BOX_WIDTH, 40);
			this->InvalidateRect(titleRect);
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
void CMessageBox::OnDraw(CDC* pDC)
{
    // ���߿�
    CRect roundRect;
    GetClientRect(roundRect);
    DrawRoundRect(pDC, roundRect, LINE_BORDER, BLUE, WHITE);

    // �������
    DrawRoundRect(pDC, CRect(0, 0, MSG_BOX_WIDTH, 40), LINE_BORDER, BLUE, BLUE);

    // д����
    DrawText(pDC, 20, 10, m_title, GUEST_MSGBOX_TITLE_FONT_NAME/*TITLE_FONT_NAME*/, GUEST_MSGBOX_TITLE_FONT_SIZE/*TITLE_FONT_SIZE*/, GUEST_MSGBOX_TITLE_FONT_COLOR/*WHITE*/);
	if(m_showSeconds!=INFINITE){
		CString strShowSecond = _T("");
		strShowSecond.Format(_T("%d"),m_showSeconds);
		
		DrawText(pDC,MSG_BOX_WIDTH - 30 - 5*strShowSecond.GetLength(),10,strShowSecond,GUEST_MSGBOX_TITLE_FONT_NAME/*TITLE_FONT_NAME*/,GUEST_MSGBOX_TITLE_FONT_SIZE/*TITLE_FONT_SIZE*/,GUEST_MSGBOX_TITLE_FONT_COLOR/*WHITE*/);
	}
 	
	// д��Ϣ����
	int i = 0;

	CString resToken = m_message;
	CString messageLine = _T("");

	while (resToken != _T("")) {
		messageLine = resToken.Left(CMessageBox::MAX_LINE_LENGTH_EN);
		int lineLengh = CMessageBox::MAX_LINE_LENGTH_EN;    // ��ʾ��Ϣÿ�г���
		int index = GetAactualCharacterWidth(messageLine);
		while (index > CMessageBox::MAX_LINE_LENGTH_EN){
			messageLine = messageLine.Left(--lineLengh);
			index = GetAactualCharacterWidth(messageLine);
		}
		resToken = resToken.Mid(lineLengh);
		DrawText(pDC, CRect(TEXT_LEFT_MARGIN, TEXT_TOP_MARGIN + i * TEXT_HEIGHT, 
			MSG_BOX_WIDTH - TEXT_LEFT_MARGIN, TEXT_TOP_MARGIN + (i + 1) * TEXT_HEIGHT), 
			messageLine, GUEST_MSGBOX_TXT_FONT_NAME/*NORMAL_FONT_NAME*/, GUEST_MSGBOX_TXT_FONT_SIZE/*NORMAL_FONT_SIZE*/,FW_NORMAL,GUEST_MSGBOX_TXT_FONT_COLOR/*BLACK*/, m_format);
		i++;
	}

	if(!hasSetFocus){
		hasSetFocus = true;
		if(m_focusBtnType == BTN_CANCEL){
			m_btnCancel->SetFocus();
		}
		if(m_focusBtnType == BTN_OK){
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
LRESULT CMessageBox::XButtonClick(WPARAM wParam, LPARAM lParam)
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
INT_PTR CMessageBox::Show(CString title, CString message, BTN_TYPE btnType, UINT format,int showSeconds,BTN_TYPE focusBtnType)
{
    // ������Ϣ������ʾ�����ȹر�
    if (Exist()) {
        End(IDABORT);
    }
    CMessageBox msgBox(title, message, btnType, format,showSeconds,focusBtnType);
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
bool CMessageBox::Exist()
{
    if (s_curMsgBox == NULL) {
        return false;
    }
    else {
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
void CMessageBox::End(int nResult)
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
int CMessageBox::GetMessageLineCount()
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
