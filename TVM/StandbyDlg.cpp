#include "stdafx.h"
#include "StandbyDlg.h"
#include "atlimage.h"
#include "MainFrmRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStandbyDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CStandbyDlg, CBaseDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

static CRect TEXT_RECT_CN = CRect(10*REC_RATIOWIDTH,350*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,550*REC_RATIOHEIGHT);
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CStandbyDlg::CStandbyDlg(CService* pService)
	: CBaseDlg(CStandbyDlg::IDD,theAPP_SESSION.GetChildViewRec(), WELCOME_FRAME_RECT, BLACK, pService, false)
{
	font_right = 1920;				// �������ֳ�ʼ��right����
	font_left = 0;					// �������ֳ�ʼ��left����
	font_bottom = 1080;				// �������ֳ�ʼ��bottom����
	font_top = 0;					// �������ֳ�ʼ��top����
	//ServiceSwitch = 1;
	Adsz = 0;						// ����й������ʼ����
	AdNowNumber = 0;				// ���Ŷ�����
	FontNowNumber = 0;				// �����������ֶ�����
	PictureNowNumber = 0;			// ��������ͼƬ������
	FirstTime = 1;					// �������״β���
	m_bIsPlayFont = false;			// ������Ƿ񲥷�����
	m_bIsPlayPic  = false;			// ������Ƿ񲥷�ͼƬ

	PlayFont_OriSeconds = 0;		// ��������ֲ��Ż���ʱ��
	PlayPicture_OriSeconds = 0;		// �����ͼƬ���Ż���ʱ��
	// ��ȡXML������
	GetAdvertisementInfo();

	// ���ݹ��SerialNo�������֡�ͼƬ
	// ����SerialNo < ͼƬSerialNo,���Ȳ�����������
	vector<CParseXML::AdStruct>::iterator it = AdvertisementInfo.begin();
	vector<CParseXML::AdStruct>::iterator first = AdvertisementInfo.begin();
	while(it != AdvertisementInfo.end()){
		if(it->advertisementobject.SerialNo < first->advertisementobject.SerialNo){// �����沥�����ȼ�
			first = it;
		}
		if(!it->adfontstruct.FontPlayNumber.IsEmpty()){// �Ƿ������ֹ�棿
			m_bIsPlayFont = true;
		}
		if(!it->adpicturestruct.SerialNo.IsEmpty()){// �Ƿ���ͼƬ��棿
			m_bIsPlayPic = true;
		}
		it++;
	}

	// �������ȼ����Ȳ���ͼƬ��������
	if(m_bIsPlayPic && m_bIsPlayFont){
		if(first->adfontstruct.FontPlayNumber < first->adpicturestruct.SerialNo){
			PlayType = PLAY_TYPE_FONT;
		}
		else{
			PlayType = PLAY_TYPE_PICTURE;
		}
	}
	else if(m_bIsPlayFont && !m_bIsPlayPic){
		PlayType = PLAY_TYPE_FONT;
	}
	else if(m_bIsPlayPic && ! m_bIsPlayFont){
		PlayType = PLAY_TYPE_PICTURE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CStandbyDlg::~CStandbyDlg()
{
	
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:�����ɹ�    -1:����ʧ��

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
int CStandbyDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	//theSERVICE_MGR.SetState(REST);
	
	pictureex.createPicWnd(this->m_hWnd);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	// ���ء���ӭ���١�����
	theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->ShowWindow(SW_HIDE);
	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_HIDE);
	
	AdNowNumber = 0;				// ���Ŷ�����
	FontNowNumber = 0;				// �����������ֶ�����
	PictureNowNumber = 0;			// ��������ͼƬ������
	FirstTime = 1;					// �������״β���
	m_bIsShowUp = false;			// �V���Ƿ��@ʾ

	// ���Ź�����������ݶ�ʱ��
	m_bIsPlayFont ? SetTimer(PLAY_FONT_TIMER,50,NULL) : NULL;

	// ���Ź����ͼƬ���ݶ�ʱ��
	m_bIsPlayPic ?  SetTimer(PLAY_PICTURE_TIMER,50,NULL) : NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ģ�ͷ����仯ʱ�ĸ��½���   

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::UpdateUI()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʱ�����

@param      UINT nIDEvent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	// ���Ź���е���������
	case PLAY_FONT_TIMER:
		// ��沥�ŷ�ʽΪ���ֲ���
		if(PLAY_TYPE_FONT == PlayType)
		{
			this->Invalidate();
		}
		break;
	// ���Ź���е�ͼƬ����
	case PLAY_PICTURE_TIMER:
		// ��沥�ŷ�ʽΪͼƬ����
		if(PLAY_TYPE_PICTURE == PlayType)
		{	
			// ����ͼƬ��ʼ��ʱ
			if(1 == FirstTime)
			{
				adstruct.picturestruct.TimeLength = AdvertisementInfo[AdNowNumber].adpicturestruct.pictureinfo[PictureNowNumber].TimeLength;				// ͼƬ�л�ʱ��
				adstruct.picturestruct.FilePathName = AdvertisementInfo[AdNowNumber].adpicturestruct.pictureinfo[PictureNowNumber].FilePathName;			// ͼƬλ��
				// ����ʱ��
				PlayPicture_OriSeconds = GetTickCount();
				// ����ͼƬ
				PlayAdPicture(adstruct.picturestruct.FilePathName);
				m_bIsShowUp = true;
				FirstTime ++;
			}
			// �л�ʱ��
			if(IsTimeUp(_ttoi(adstruct.picturestruct.TimeLength)*1000,PlayPicture_OriSeconds))
			{
				FirstTime = 1;
				PictureNowNumber++;
				// һ��ͼƬ�������
				if(PictureNowNumber >= AdvertisementInfo[AdNowNumber].adpicturestruct.pictureinfo.size())
				{
					// ͼƬ�����ֲ��ź�
					if(AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber < AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo)
					{
						AdNowNumber++;
						// �����󲥷��꣬��ͷ��ʼ���²���
						if(AdNowNumber >= AdvertisementInfo.size())
						{
							AdNowNumber = 0;
						}
						// ��һ�����ֲ���SerialNo > ͼƬ���ֲ���SerialNo
						if((AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber > AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo) || AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber.IsEmpty())
						{
							PlayType = PLAY_TYPE_PICTURE;	// ���Ź���е�ͼƬ����
						}
						// ��һ�����ֲ���SerialNo < ͼƬ���ֲ���SerialNo
						else
						{
							PlayType = PLAY_TYPE_FONT;		// ���Ź���е���������
						}
					}
					// ������ͼƬ���ź�
					else if(!AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber.IsEmpty())// ����������
					{
						PlayType = PLAY_TYPE_FONT;			// ���Ź���е���������
					}
					PictureNowNumber = 0;
				}
			}
		}
		break;
	default:
		break;
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
void CStandbyDlg::OnDraw(CDC* pDC)
{
	// ��沥�ŷ�ʽΪ���ֲ���
	if(PLAY_TYPE_FONT == PlayType)
	{
		// ����GIF�Ĵ��ڴ��ڣ���������
		if(IsWindow(pictureex.GetSafeHwnd()) && m_bIsRenderGif)
		{
			pictureex.pauseRender();
		}
		adstruct.fontstruct.Font = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Font;					// ��������ֶ��������
		adstruct.fontstruct.Size = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Size;					// ��������ֶ���Ĵ�С
		adstruct.fontstruct.Color = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Color;					// ��������ֶ������ɫ
		adstruct.fontstruct.TimeLength = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].TimeLength;		// ��������ֶ���Ĳ���ʱ��
		adstruct.fontstruct.Content = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Content;				// ��������ֶ��������
		adstruct.fontstruct.DisplayMode = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].DisplayMode;		// ��������ֶ���Ĳ��ŷ�ʽ
		adstruct.fontstruct.Speed = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Speed;					// ��������ֶ���Ĳ����ٶ�
		PlayAdFont(pDC,adstruct.fontstruct.Font,adstruct.fontstruct.Size,adstruct.fontstruct.Color,adstruct.fontstruct.Content,adstruct.fontstruct.DisplayMode,adstruct.fontstruct.Speed);
		// �״β���
		if(1 == FirstTime)
		{
			PlayFont_OriSeconds = GetTickCount();		// ���ֲ��ſ�ʼ��ʱ
			FirstTime ++;
		}
		
		if(IsTimeUp(_ttoi(adstruct.fontstruct.TimeLength)*1000,PlayFont_OriSeconds))
		{
			FirstTime = 1;
			FontNowNumber++;		// һ���������е���һ�����ֶ���
			// ��һ���������е����ֶ���
			if(FontNowNumber >= AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo.size())
			{
				
				font_right = 1280;				// �������ֳ�ʼ��right����
				font_left = 0;					// �������ֳ�ʼ��left����
				font_bottom = 1024;				// �������ֳ�ʼ��bottom����
				font_top = 0;					// �������ֳ�ʼ��top����


				// ���ֲ�����ͼƬ���ź�
				if(AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber > AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo)
				{
					AdNowNumber++;
					// �����󲥷��꣬��ͷ��ʼ���²���
					if(AdNowNumber >= AdvertisementInfo.size())
					{
						AdNowNumber = 0;
					}
					// // ��һ�����ֲ���SerialNo < ͼƬ���ֲ���SerialNo
					if(AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber < AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo)
					{
						PlayType = PLAY_TYPE_FONT;		// ���Ź���е���������
					}
					// ��һ�����ֲ���SerialNo < ͼƬ���ֲ���SerialNo
					else
					{
						PlayType = PLAY_TYPE_PICTURE;	// ���Ź���е�ͼƬ����
					}
				}
				// ͼƬ���������ֲ��ź�
				else
				{
					PlayType = PLAY_TYPE_PICTURE;		// ���Ź���е�ͼƬ����
				}
				
				FontNowNumber = 0;
				
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief      �ػ�Dialog����

 @param     (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CStandbyDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ����ͬ���ŷ�ʽ���Ź���е���������

 @param     (i) CDC* pDC   �豸������

 @param     (iii) Font     ����

 @param     (iv)  size     �����С

 @param     (v)   Color    ������ɫ

 @param     (vi)  content   ����е���������

 @param     (ii) PlayMode  ���ŷ�ʽ

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::PlayAdFont(CDC* pDC,CString Font,CString size,CString Color,CString content,CString PlayMode,CString speed)
{
	CSize sz(0,0);		// ������Ϣ
	CStringArray fontcontentarray; 
	// �ö�������������ˢ��ʱ���µ�������˸
	
	m_memDC.CreateCompatibleDC(NULL);					// ����һ�����ڴ�����������DC            
	m_menBit.CreateCompatibleBitmap(pDC,1920,1080);		// ������DC��ص�λͼ
	m_memDC.SelectObject(&m_menBit);

	m_memDC.FillSolidRect(0,0,1920,1080,RGB(0,255,0));	// �����Ļ�ı���ɫ
	
	CFont font;
	font.CreatePointFont(_ttoi(size),Font);				// ������СΪsize������ΪFont������
	CFont* pOldFont = m_memDC.SelectObject(&font);
	
	COLORREF rgb = _ttoi(Color);
	m_memDC.SetTextColor(rgb);							// ����������ɫ
	
	// �ж��Ƿ��л���
	int Index = content.Find(_T("|"));
	// ���ݲ�ͬ�Ĳ��ŷ�ʽ��������
	switch(_ttoi(PlayMode))
	{
	// ���ҵ��󲥷�
	case RTOL_ROLL:
		sz = m_memDC.GetTextExtent(content);
		// δ�ҵ����з�'|'
		if(-1 == Index)
		{
		}
		// �ҵ����з�'|'
		else
		{
			content.Replace(_T("|"),_T(" "));		// '|'�ÿո����
		}
		m_memDC.TextOut(font_right,(1080-sz.cy)/2,content);   
		if(font_right -= _ttoi(speed),font_right <= -sz.cx)
		{
			font_right = 1920;
		}
		break;

	// ���µ��ϲ���
	case DTOU_ROLL:
		if(-1 == Index)
		{
			sz = m_memDC.GetTextExtent(content);
			m_memDC.TextOut((1920-sz.cx)/2,font_bottom,content);
			if(font_bottom -= _ttoi(speed),font_bottom <= 0)
			{
				font_bottom = 1080;
			}
		}
		// �ҵ����з�'|'
		else
		{
			int find_index = content.ReverseFind('|');			// �������'|'
			fontcontentarray.Add(content.Mid(find_index+1));	// ȡ�ҵ��ĵ�һ��'|'��CStringArray��
			// ��������'|'��ȡ�ַ�����CStringArray��
			while(find_index>0)
			{
				content=content.Mid(0,find_index);
				find_index=content.ReverseFind('|');
				fontcontentarray.Add(content.Mid(find_index+1));
			}
			int size = fontcontentarray.GetSize();
			// ��ʽ�����
			for(int index = 0; index < size;index++)
			{
				sz = m_memDC.GetTextExtent(fontcontentarray.GetAt(index));
				m_memDC.TextOut((1920-sz.cx)/2,font_bottom-sz.cy*index-20,fontcontentarray.GetAt(index));
			}
			if(font_bottom -= _ttoi(speed),font_bottom <= 0)
			{
				font_bottom = 1080;
			}
		}
		break;

		// �����Ҳ���
	case LTOR_ROLL:
		sz = m_memDC.GetTextExtent(content);
		// δ�ҵ����з�'|'
		if(-1 == Index)
		{
		}
		// �ҵ����з�'|'
		else
		{
			content.Replace(_T("|"),_T(" "));		// '|'�ÿո����
		}
		m_memDC.TextOut(font_left,(1080-sz.cy)/2,content);   
		if(font_left += _ttoi(speed),font_left >= 1280+sz.cx)
		{
			font_left = 0;
		}
		break;

		// ���ϵ��²���
	case UTOD_ROLL:
		if(-1 == Index)
		{
			sz = m_memDC.GetTextExtent(content);
			m_memDC.TextOut((1920-sz.cx)/2,font_top,content);
			if(font_top += _ttoi(speed),font_top >= 1080)
			{
				font_top = 0;
			}
		}
		// �ҵ����з�'|'
		else
		{
			int find_index = content.ReverseFind('|');			// �������'|'
			fontcontentarray.Add(content.Mid(find_index+1));	// ȡ�ҵ��ĵ�һ��'|'��CStringArray��
			// ��������'|'��ȡ�ַ�����CStringArray��
			while(find_index>0)
			{
				content=content.Mid(0,find_index);
				find_index=content.ReverseFind('|');
				fontcontentarray.Add(content.Mid(find_index+1));
			}
			int size = fontcontentarray.GetSize();
			// ��ʽ�����
			for(int index = 0; index < size;index++)
			{
				sz = m_memDC.GetTextExtent(fontcontentarray.GetAt(index));
				m_memDC.TextOut((1920-sz.cx)/2,font_top+sz.cy*index+20,fontcontentarray.GetAt(index));
			}
			if(font_top += _ttoi(speed),font_top >= 1080)
			{
				font_top = 0;
			}
		}
		break;

	// Ĭ�Ͼ�ֹ
	default:
		if(-1 == Index)
		{
			sz = m_memDC.GetTextExtent(content);
			m_memDC.TextOut((1920-sz.cx)/2,(1080-sz.cy)/2,content);
		}
		// �ҵ����з�'|'
		else
		{
			int find_index = content.ReverseFind('|');			// �������'|'
			fontcontentarray.Add(content.Mid(find_index+1));	// ȡ�ҵ��ĵ�һ��'|'��CStringArray��
			// ��������'|'��ȡ�ַ�����CStringArray��
			while(find_index>0)
			{
				content=content.Mid(0,find_index);
				find_index=content.ReverseFind('|');
				fontcontentarray.Add(content.Mid(find_index+1));
			}
			int size = fontcontentarray.GetSize();
			// ��ʽ�����
			for(int index = 0; index < size;index++)
			{
				sz = m_memDC.GetTextExtent(fontcontentarray.GetAt(index));
				m_memDC.TextOut((1920-sz.cx)/2,(800-sz.cy*index*2-20)/2,fontcontentarray.GetAt(index));
			}
		}
		break;

	}
	//���ڴ��ͼ��������Ļ
	pDC->BitBlt(0,0,1920,1080,&m_memDC,0,0,SRCCOPY);

	//�����ͷ�
	m_memDC.SelectObject(pOldFont);
	ReleaseDC(pDC);
	m_memDC.DeleteDC();
	m_menBit.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ����ͬ��ʽ���Ź���е�ͼƬ����

 @param     (i) CDC* pDC               �豸������

 @param     (i) CString FilePathName   ͼƬ��Դλ��

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::PlayAdPicture(CString FilePathName)
{
	if(this->IsWindowVisible()){
		int Index = FilePathName.Find(_T("gif"));
		// FilePathName���ҵ�"gif"
		if(-1 != Index)
		{
			CRect rect(0,0,1920,1080);	
			pictureex.loadPic(FilePathName);
			pictureex.MoveWindow(rect);
			pictureex.resumeRender();
			m_bIsRenderGif = true;
			//pictureex.renderPicture(FilePathName,this->m_hWnd,rect);
			//BOOL IsCreate = pictureex.Create(_T(""),WS_CHILD|WS_VISIBLE,rect,this,21234);  
			//// �������ڷ���TRUE
			//if(IsCreate)
			//{
			//	pictureex.Load(FilePathName); 
			//	BOOL IsGif = pictureex.IsGIF();
			//	// ����GIF
			//	if(!IsGif)
			//	{
			//		return;
			//	}
			//	// ��GIF�����л�ͼ
			//	else
			//	{
			//		if(IsWindow(pictureex.GetSafeHwnd()))
			//		{
			//			pictureex.SetPaintRect(&rect);  
			//			BOOL IsDraw = pictureex.Draw(); 
			//		}	
			//	}	
			//}
		}
		// ����GIF
		else
		{
			if(IsWindow(pictureex.GetSafeHwnd()) && m_bIsRenderGif)
			{
				pictureex.pauseRender();
				m_bIsRenderGif = false;
				//pictureex.stopRender();
				//pictureex.Stop();
				//pictureex.DestroyWindow();
			}
			CDC *pDC = GetDC();
			image.Load(FilePathName);
			image.Draw(pDC->m_hDC,0,0,1920,1080);
			if(!image.IsNull())image.Destroy();
			ReleaseDC(pDC);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���尴SerialNo����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CStandbyDlg::sortfontbyserialno(CParseXML::FontStruct fontstruct1,CParseXML::FontStruct fontstruct2)
{
	return (fontstruct1.SerialNo < fontstruct2.SerialNo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ͼƬ��SerialNo����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CStandbyDlg::sortpicturebyserialno(CParseXML::PictureStruct picturestruct1,CParseXML::PictureStruct picturestruct2)
{
	return (picturestruct1.SerialNo < picturestruct2.SerialNo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ��水SerialNo����

@param     (i)CParseXML::Advertisement_Object ad_object1   ������1

@param     (ii)CParseXML::Advertisement_Object ad_object1  ������2

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CStandbyDlg::sortadbyserialno(CParseXML::AdStruct ad_object1,CParseXML::AdStruct ad_object2)
{
	return (ad_object1.advertisementobject.SerialNo < ad_object2.advertisementobject.SerialNo);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     ��ȡ�����Ϣ 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::GetAdvertisementInfo()
{
	CString str = _T("");
	CString XMLPath = _T(".\\res\\ADV\\AdvertisementDescription.xml");
	//CString XMLPath = _T("F:\\NEW_TFS\\Device\\TVM\\res\\AdvertisementDescription.xml");
	CString Node = _T("Advertisement_Text");
	thePARSE_XML.QueryElementValue(XMLPath,Node,AdvertisementInfo);
	Adsz = AdvertisementInfo.size();
	// ���˳�򲥷�
	sort(AdvertisementInfo.begin(),AdvertisementInfo.end(),sortadbyserialno);
	// �������������˳�򲥷�
	for(vector<CParseXML::AdStruct>::iterator adinfo = AdvertisementInfo.begin();adinfo != AdvertisementInfo.end(); adinfo++)
	{
		sort(adinfo->adfontstruct.fontinfo.begin(),adinfo->adfontstruct.fontinfo.end(),sortfontbyserialno);						// ���尴SerialNo����
		sort(adinfo->adpicturestruct.pictureinfo.begin(),adinfo->adpicturestruct.pictureinfo.end(),sortpicturebyserialno);		// ͼƬ��SerialNo����
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�ʱ�䴦��ִ���Ƿ�ʱ

@param      (i)DWORD dwTimer		��ʱʱ����
@param      (i)DWORD dwBaseTime		����ʱ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL CStandbyDlg::IsTimeUp(IN DWORD dwTimer,IN DWORD dwBaseTime)
{
	BOOL bRet = FALSE;
	DWORD dwCurTime;
	DWORD dwUpTime;
	//            dwTime
	//    |-------------------|
	// dwBaseTime          dwUpTime
	//    |----------|--------|-----
	//           dwCurTime
	// Has timer
	if (dwTimer != 0){
		dwCurTime = GetTickCount();		// ��ǰʱ��
		dwUpTime = dwBaseTime + dwTimer;// ����ʱ��
		if (dwUpTime >= dwBaseTime){
			if (dwCurTime >= dwBaseTime){
				// ��ʱ����ǰʱ�� > ����ʱ�䣩
				if (dwCurTime >= dwUpTime){
					bRet = TRUE;
				}
			}
			else{
				bRet = TRUE;
			}
		}
		else{
			if (dwCurTime < dwBaseTime){
				if (dwCurTime >= dwUpTime){
					bRet = TRUE;
				}
			}
		}
	}
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ļ����˳���沥��

@param      (i)CPoint point		���λ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStandbyDlg::OnNcHitTest(CPoint point)
{
	if(this->IsWindowVisible()){// ֻ�е�ǰ�����Ѿ���ʾʱ����Ӧ�˺���
		if(REST == theSERVICE_MGR.GetState() && (point.x >= 0 && point.x <= 1280) && ( point.y >= 0 && point.y <= 1024) && m_bIsShowUp){
			KillTimer(PLAY_FONT_TIMER);	  // ֹͣӋ�r��
			KillTimer(PLAY_PICTURE_TIMER);// ֹͣӋ�r��
			if(m_bIsRenderGif){
				pictureex.pauseRender();// ֹͣ��̬ͼ����
				m_bIsRenderGif = false;
			}
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
			m_bIsShowUp = false;
		}
	}	
	return TRUE;
}