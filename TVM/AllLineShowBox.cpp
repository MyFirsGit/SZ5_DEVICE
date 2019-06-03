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
@brief      ���캯��

@param      (i)CString title        ����
@param      (i)CString message      ��Ϣ����(������Ϣ��"|"�ָ�)
@param      (i)BTN_TYPE btnType     ��ť����
@param      (i)UINT format          ��ʽ��DT_CENTER|DT_VCENTER|DT_SINGLELINE��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAllLineShowBox::CAllLineShowBox(UINT ID, CService* pService,CRect rect,bool needFunctionArea)
	:CReceptionDlg(ID, pService, rect, needFunctionArea)
{
	//ȡ����·��
	CString strAppPath;
	GetAppPath(strAppPath);

	CString imagePath = strAppPath + _T(".\\res\\TVM_all_line_bkg.png");
	m_Image.Load(imagePath);
	
	m_btnClose = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
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
	// ���ڴ�С1155 X 910
	int nMaxPageBtns = 6*5;//(910-250)/(60+40)		(1155-60)/(160+50)=1095/210
	int nPageMaxRow = 6;
	int nPageMaxCol = 5;
	int nLineBtnMargeX = 50;	// ��ťˮƽ���
	int nLineBtnMargeY = 40;	// ��ť��ֱ���
	int nLineBtnWidth = 160;	// ��ť���
	int nLineBtnHeight = 60;	// ��ť�߶�
	int nLineBtnStartX = 60;	// ��ť��ʼX
	int nLineBtnStartY = 250-70;	// ��ť��ʼY
	// ��ʾ��·��ť
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
				buttonInfo.text.Format(_T("%d����"), vec_LineId[nCurIndex]);		
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
	// �رհ�ť
	{
		UIINFO buttonUI;
		buttonUI.m_Location = CPoint(955,30);
		buttonUI.m_pParentWnd = this;
		buttonUI.m_Size = CSize(nLineBtnWidth, nLineBtnHeight);
		BTN_INFO buttonInfo;
		buttonInfo.btnMsg.Format(_T("%d"), 255);	
		if(language==LANGUAGE_ID_CHINESE){
			buttonInfo.text.Format(_T("�ر�"));		
		}
		else{
			buttonInfo.text.Format(_T("Close"));	
		}
		buttonInfo.type = BUTTON_TYPE_CLOSE;
		buttonInfo.btnID = 0;
		m_btnClose = new CXButton(buttonUI,buttonInfo);
		m_btnClose->Create();
	}
	// �������ڴ�С��λ��
	SetWindowPos(&CWnd::wndTopMost, REC_LINE_MAP_RECT.left, REC_LINE_MAP_RECT.top+5, REC_LINE_MAP_RECT.Width(), REC_LINE_MAP_RECT.Height(), SWP_SHOWWINDOW);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ٺ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAllLineShowBox::OnDestroy()
{
	__super::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAllLineShowBox::OnDraw(CDC* pDC)
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
    // ���߿�
    DrawRoundRect(pTmpDc, roundRect, LINE_BORDER, BLUE, WHITE);
	// ͼƬ
	m_Image.Draw(pTmpDc->GetSafeHdc(), roundRect);
	
	// ����������ͼ�ο�������ͼDC
	pDC->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// ��ͼ����
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// ��ͼ����
	
	
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
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
            TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      ��ʾ��Ϣ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
INT_PTR CAllLineShowBox::Show()
{
	m_bShowing = true;
	return DoModal();
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ��������ڣ�

@param      (i)int nResult  ��Ϣ�����ʱ�ķ���ֵ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAllLineShowBox::End(int nResult)
{
	if(m_bShowing){
		EndDialog(nResult);
	}
	m_bShowing = false;
}