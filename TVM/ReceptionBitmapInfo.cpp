// ReceptionStaticInfo.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "TVM.h"
#include "ReceptionBitmapInfo.h"
#include "UIDefinition.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CReceptionBitmapInfo

IMPLEMENT_DYNAMIC(CReceptionGuideInfo, CStatic)

BEGIN_MESSAGE_MAP(CReceptionGuideInfo, CStatic)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
  @brief      ���캯��

  @param      (i)CWnd*          pParentWnd      ������ָ��
  @param      (i)PMSG_INFO     kindInfo        ��������          

  @retval     ��     

  @exception  ��
*/
//////////////////////////////////////////////////////////////////////////

CReceptionGuideInfo::CReceptionGuideInfo(CWnd* pParentWnd, PIMAGE_GROUP_INFO msgInfo,UINT type)
{
    CStatic::CStatic();
    m_msgInfo = msgInfo;
	m_type = type;
	if(m_msgInfo->tipsCount>4)
	{
		m_uiTipsCount=4;
	}else
	{
		m_uiTipsCount=m_msgInfo->tipsCount;
	}
	//��������
	CRect rect = REC_GUIDE_RECT;
	if (m_type == 1)
	{
		rect = CRect(CPoint(REC_GUIDE_RECT.left+40,REC_GUIDE_RECT.top+50), REC_GUIDE_RECT.Size() );
	}
    if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, rect, pParentWnd, 0)) {
        throw "�����ı���ʧ��";
    }

	CRect rectBmp[4];
	CRect rectTip[4];
	switch(m_uiTipsCount)
	{
	case 1:
		m_rectBmp[0]=REC_GROUP1_BMP1;
		m_rectTip[0]=REC_GROUP1_TIP1;
		break;
	case 2:
		m_rectBmp[0]=REC_GROUP2_BMP1;
		m_rectBmp[1]=REC_GROUP2_BMP2;
		m_rectTip[1]=REC_GROUP2_TIP2;
		break;
	case 3:
		m_rectBmp[0]=REC_GROUP3_BMP1;
		m_rectBmp[1]=REC_GROUP3_BMP2;
		m_rectBmp[2]=REC_GROUP3_BMP3;
		m_rectTip[0]=REC_GROUP3_TIP1;
		m_rectTip[1]=REC_GROUP3_TIP2;
		m_rectTip[2]=REC_GROUP3_TIP3;
		break;
	case 4:
		m_rectBmp[0]=REC_GROUP4_BMP1;
		m_rectBmp[1]=REC_GROUP4_BMP2;
		m_rectBmp[2]=REC_GROUP4_BMP3;
		m_rectBmp[3]=REC_GROUP4_BMP4;
		m_rectTip[0]=REC_GROUP4_TIP1;
		m_rectTip[1]=REC_GROUP4_TIP2;
		m_rectTip[2]=REC_GROUP4_TIP3;
		m_rectTip[3]=REC_GROUP4_TIP4;
		break;
	default:
		break;
	}
}


CReceptionGuideInfo::CReceptionGuideInfo()
{
	m_uiTipsCount=0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��       

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionGuideInfo::~CReceptionGuideInfo()
{
}

BOOL CReceptionGuideInfo::createWnd(CWnd* pParentWnd)
{
	BOOL bRet=FALSE;
	//��������
	CRect rect = REC_GUIDE_RECT;
	if (m_type == 1)
	{
		rect = CRect(CPoint(REC_GUIDE_RECT.left+40,REC_GUIDE_RECT.top+50), REC_GUIDE_RECT.Size() );
	}

    if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, rect, pParentWnd, 0)) 
	{
        throw "�����ı���ʧ��";
    }else{
		bRet=TRUE;

		for(int iloop=0;iloop<4;iloop++)
		{
			m_gif[iloop].createPicWnd(this->m_hWnd);
		}
	}
	return bRet;
}

void CReceptionGuideInfo::renderGuideInfo()
{
	if(IsWindow(this->m_hWnd))
	{
		for(int iloop=0;iloop<m_uiTipsCount;iloop++)
		{
			m_gif[iloop].resumeRender();
		}
		this->InvalidateRect(NULL,TRUE);
		this->UpdateWindow();
	}
}

void CReceptionGuideInfo::setGuideInfo(PIMAGE_GROUP_INFO& msgInfo)
{
    m_msgInfo = msgInfo;
	if(m_msgInfo->tipsCount>4)
	{
		m_uiTipsCount=4;
	}else
	{
		m_uiTipsCount=m_msgInfo->tipsCount;
	}

	CRect rectBmp[4];
	CRect rectTip[4];
	switch(m_uiTipsCount)
	{
	case 1:
		m_rectBmp[0]=REC_GROUP1_BMP1;
		m_rectTip[0]=REC_GROUP1_TIP1;
		break;
	case 2:
		m_rectBmp[0]=REC_GROUP2_BMP1;
		m_rectBmp[1]=REC_GROUP2_BMP2;
		m_rectTip[1]=REC_GROUP2_TIP2;
		break;
	case 3:
		m_rectBmp[0]=REC_GROUP3_BMP1;
		m_rectBmp[1]=REC_GROUP3_BMP2;
		m_rectBmp[2]=REC_GROUP3_BMP3;
		m_rectTip[0]=REC_GROUP3_TIP1;
		m_rectTip[1]=REC_GROUP3_TIP2;
		m_rectTip[2]=REC_GROUP3_TIP3;
		break;
	case 4:
		m_rectBmp[0]=REC_GROUP4_BMP1;
		m_rectBmp[1]=REC_GROUP4_BMP2;
		m_rectBmp[2]=REC_GROUP4_BMP3;
		m_rectBmp[3]=REC_GROUP4_BMP4;
		m_rectTip[0]=REC_GROUP4_TIP1;
		m_rectTip[1]=REC_GROUP4_TIP2;
		m_rectTip[2]=REC_GROUP4_TIP3;
		m_rectTip[3]=REC_GROUP4_TIP4;
		break;
	default:
		break;
	}

	CString apppath;
	GetAppPath(apppath);
	for(int iloop=0;iloop<m_uiTipsCount;iloop++)
	{
		CRect* rcBmp=m_rectBmp+iloop;
		RECT rcpic;
		rcpic.left=rcBmp->left;
		rcpic.top=rcBmp->top;
		rcpic.right=rcBmp->right;
		rcpic.bottom=rcBmp->bottom;
		CString imagePath = apppath + m_msgInfo->detailList[iloop].strfileName;
		if(PathFileExists(imagePath))
		{
			m_gif[iloop].loadPic(imagePath);
			m_gif[iloop].MoveWindow(rcpic.left,rcpic.top,rcpic.right-rcpic.left,rcpic.bottom-rcpic.top,FALSE);
			m_gif[iloop].ShowWindow(TRUE);
			m_gif[iloop].resumeRender();
		}
	}

	for(int iloop=m_uiTipsCount;iloop<4;iloop++)
	{
		if(IsWindow(m_gif[iloop].m_hWnd))
		{
			m_gif[iloop].ShowWindow(FALSE);
		}
	}

	/*for(int iloop=m_uiTipsCount;iloop<4;iloop++)
	{
		if(IsWindow(m_gif[iloop].m_hWnd))
		{
			m_gif[iloop].ShowWindow(FALSE);
		}
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰtip����

@param      ��       

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CReceptionGuideInfo::getTipCount()
{
	return m_uiTipsCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ����ָ����gifͼƬ

@param      uiindex ָ��gif ����� ��0��ʼ����       

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionGuideInfo::stopGif(UINT uiindex)
{
	if(IsWindow(m_gif[uiindex].GetSafeHwnd()))
	{
		//m_gif[uiindex].stopRender();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��Ϊ��ǰ��ʱ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionGuideInfo::PreSubclassWindow()
{
    //ModifyStyle(0, BS_OWNERDRAW);
    CStatic::PreSubclassWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��Ϊ��ǰ��ʱ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CReceptionGuideInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ͸������

@param      CImage* image

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionGuideInfo::Convert(CImage* image)
{
	for(int i = 0; i < image->GetWidth(); i++){  
		for(int j = 0; j < image->GetHeight(); j++){  
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(image->GetPixelAddress(i , j));  
			pucColor[0] = pucColor[0] * pucColor[3] / 255;  
			pucColor[1] = pucColor[1] * pucColor[3] / 255;  
			pucColor[2] = pucColor[2] * pucColor[3] / 255;  
		}  
	}  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����仯ʱˢ�»���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionGuideInfo::OnPaint()
{
    CPaintDC dc(this); 
    CRect rect;
    GetClientRect(&rect);
	//���ƶԻ������
	//CPen pen(PS_SOLID, 1, BLACK);
	//CBrush brush(GRAY);
	//CBrush* pOldBrush = dc.SelectObject(&brush);
	//CPen* pOldPen = dc.SelectObject(&pen);
	CRect roundRect1 = rect;
	roundRect1.DeflateRect(5,5,5,5);
	dc.RoundRect(roundRect1,ROUND_POINT);
	//dc.SelectObject(pOldPen); 
	//dc.SelectObject(pOldBrush); 

	//��TVM_GUIInfo.ini �ļ�������������ı���ɫ
	CRgn *rgn = new CRgn;
	CRect rcRgn=roundRect1;
	rcRgn.left+=/*5+*/1;
	rcRgn.top+=/*5+*/1;
	rcRgn.right-=/*5+*/1;
	rcRgn.bottom-=/*5+*/1;
	rgn->CreateRoundRectRgn(rcRgn.left,rcRgn.top,rcRgn.right,rcRgn.bottom,5,5);
	CBrush *pColorBrush = new CBrush(RECEPTION_GUIDE_BK_COLOR);
	dc.FillRgn(rgn, pColorBrush);

	//�����ı���ʾ��ʽ
	dc.SetBkMode(TRANSPARENT);
		CFont tipfont;
		/*DWORD textSize = theGUI_INFO.GetRecBaseInfoFontSize(theAPP_SESSION.GetCurrentClientLanguageType());
		CString textfont = theGUI_INFO.GetRecBaseInfoFontName(theAPP_SESSION.GetCurrentClientLanguageType());*/

		tipfont.CreateFont(TVM_PAS_GUIDE_FONT_SIZE/*textSize*/, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS, TVM_PAS_GUIDE_FONT_NAME/*textfont*/);
		CFont *poldFont = dc.SelectObject(&tipfont);

	//ԭ����ͼƬ�ĵط�
		//������ʾ�ı�
	dc.SetTextColor(TVM_PAS_GUIDE_FONT_COLOR/*m_msgInfo->detailList[iloop].color*/);
	for(int iloop=0;iloop<m_uiTipsCount;iloop++)
	{
		if (!m_msgInfo->detailList[iloop].bitInfo.IsEmpty())
		{
			CString szContent = m_msgInfo->detailList[iloop].bitInfo;
			int nIndex = szContent.Find(_T("^"));
			CRect rect = m_rectTip[iloop];
			CString szCon1 = _T(""),szCon2 = _T("");
			if (nIndex > 0)
			{
				szCon1 = szContent.Left(nIndex);
				szCon2 = szContent.Mid(nIndex+1);
			}
			else
			{
				szCon1 = szContent;
			}
			dc.DrawText(szCon1,rect, DT_CENTER|DT_VCENTER|DT_WORDBREAK );
			if (!szCon2.IsEmpty())
			{
				rect = CRect(CPoint(rect.left,rect.bottom+5),rect.Size());
				dc.DrawText(szCon2,rect, DT_CENTER|DT_VCENTER|DT_WORDBREAK );

			}
		}
	}

	//for(int iloop=0;iloop<m_uiTipsCount;iloop++)
	//{
	//	if (!m_msgInfo->detailList[iloop].bitInfo.IsEmpty())
	//	{
	//		int nOffset = 0;
	//		CString szContent = m_msgInfo->detailList[iloop].bitInfo;
	//		int nIndex = szContent.Find(_T("^"));
	//		while(nIndex > -1)
	//		{
	//			CString szTmp = szContent.Mid(nOffset,nIndex);
	//			//����^�ж��Ƿ�����ʾ
	//			dc.DrawText(m_msgInfo->detailList[iloop].bitInfo,m_rectTip[iloop], DT_CENTER|DT_VCENTER|DT_WORDBREAK );

	//		}
	//	}
	//}
	for(int iloop=0;iloop<m_uiTipsCount;iloop++)
	{
		m_gif[iloop].resumeRender();
	}

	dc.SelectObject(poldFont);
	// �ڴ�й¶zhengxianle
	if(rgn != NULL){
		delete rgn;
		rgn = NULL;
	}
	if(pColorBrush != NULL){
		delete pColorBrush;
		pColorBrush = NULL;
	}
}

