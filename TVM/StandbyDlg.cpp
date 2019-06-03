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
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
CStandbyDlg::CStandbyDlg(CService* pService)
	: CBaseDlg(CStandbyDlg::IDD,theAPP_SESSION.GetChildViewRec(), WELCOME_FRAME_RECT, BLACK, pService, false)
{
	font_right = 1920;				// 播放文字初始的right坐标
	font_left = 0;					// 播放文字初始的left坐标
	font_bottom = 1080;				// 播放文字初始的bottom坐标
	font_top = 0;					// 播放文字初始的top坐标
	//ServiceSwitch = 1;
	Adsz = 0;						// 广告中广告对象初始数量
	AdNowNumber = 0;				// 播放对象标号
	FontNowNumber = 0;				// 广告对象中文字对象标号
	PictureNowNumber = 0;			// 广告对象中图片对象标号
	FirstTime = 1;					// 广告对象首次播放
	m_bIsPlayFont = false;			// 广告中是否播放文字
	m_bIsPlayPic  = false;			// 广告中是否播放图片

	PlayFont_OriSeconds = 0;		// 广告中文字播放基本时间
	PlayPicture_OriSeconds = 0;		// 广告中图片播放基本时间
	// 获取XML中内容
	GetAdvertisementInfo();

	// 根据广告SerialNo播放文字、图片
	// 字体SerialNo < 图片SerialNo,首先播放文字内容
	vector<CParseXML::AdStruct>::iterator it = AdvertisementInfo.begin();
	vector<CParseXML::AdStruct>::iterator first = AdvertisementInfo.begin();
	while(it != AdvertisementInfo.end()){
		if(it->advertisementobject.SerialNo < first->advertisementobject.SerialNo){// 整体广告播放优先级
			first = it;
		}
		if(!it->adfontstruct.FontPlayNumber.IsEmpty()){// 是否有文字广告？
			m_bIsPlayFont = true;
		}
		if(!it->adpicturestruct.SerialNo.IsEmpty()){// 是否有图片广告？
			m_bIsPlayPic = true;
		}
		it++;
	}

	// 根据优先级别，先播放图片还是文字
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
@brief      析构函数

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
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int     0:创建成功    -1:创建失败

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
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{		
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	// 隐藏“欢迎光临”画面
	theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->ShowWindow(SW_HIDE);
	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_HIDE);
	
	AdNowNumber = 0;				// 播放对象标号
	FontNowNumber = 0;				// 广告对象中文字对象标号
	PictureNowNumber = 0;			// 广告对象中图片对象标号
	FirstTime = 1;					// 广告对象首次播放
	m_bIsShowUp = false;			// 廣告是否顯示

	// 播放广告中文字内容定时器
	m_bIsPlayFont ? SetTimer(PLAY_FONT_TIMER,50,NULL) : NULL;

	// 播放广告中图片内容定时器
	m_bIsPlayPic ?  SetTimer(PLAY_PICTURE_TIMER,50,NULL) : NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务数据模型发生变化时的更新界面   

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
@brief      时间控制

@param      UINT nIDEvent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	// 播放广告中的文字内容
	case PLAY_FONT_TIMER:
		// 广告播放方式为文字播放
		if(PLAY_TYPE_FONT == PlayType)
		{
			this->Invalidate();
		}
		break;
	// 播放广告中的图片内容
	case PLAY_PICTURE_TIMER:
		// 广告播放方式为图片播放
		if(PLAY_TYPE_PICTURE == PlayType)
		{	
			// 播放图片开始计时
			if(1 == FirstTime)
			{
				adstruct.picturestruct.TimeLength = AdvertisementInfo[AdNowNumber].adpicturestruct.pictureinfo[PictureNowNumber].TimeLength;				// 图片切换时间
				adstruct.picturestruct.FilePathName = AdvertisementInfo[AdNowNumber].adpicturestruct.pictureinfo[PictureNowNumber].FilePathName;			// 图片位置
				// 基本时间
				PlayPicture_OriSeconds = GetTickCount();
				// 播放图片
				PlayAdPicture(adstruct.picturestruct.FilePathName);
				m_bIsShowUp = true;
				FirstTime ++;
			}
			// 切换时间
			if(IsTimeUp(_ttoi(adstruct.picturestruct.TimeLength)*1000,PlayPicture_OriSeconds))
			{
				FirstTime = 1;
				PictureNowNumber++;
				// 一组图片播放完成
				if(PictureNowNumber >= AdvertisementInfo[AdNowNumber].adpicturestruct.pictureinfo.size())
				{
					// 图片在文字播放后
					if(AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber < AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo)
					{
						AdNowNumber++;
						// 广告对象播放完，从头开始重新播放
						if(AdNowNumber >= AdvertisementInfo.size())
						{
							AdNowNumber = 0;
						}
						// 下一组文字播放SerialNo > 图片文字播放SerialNo
						if((AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber > AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo) || AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber.IsEmpty())
						{
							PlayType = PLAY_TYPE_PICTURE;	// 播放广告中的图片内容
						}
						// 下一组文字播放SerialNo < 图片文字播放SerialNo
						else
						{
							PlayType = PLAY_TYPE_FONT;		// 播放广告中的文字内容
						}
					}
					// 文字在图片播放后
					else if(!AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber.IsEmpty())// 无文字内容
					{
						PlayType = PLAY_TYPE_FONT;			// 播放广告中的文字内容
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
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::OnDraw(CDC* pDC)
{
	// 广告播放方式为文字播放
	if(PLAY_TYPE_FONT == PlayType)
	{
		// 播放GIF的窗口存在，进行销毁
		if(IsWindow(pictureex.GetSafeHwnd()) && m_bIsRenderGif)
		{
			pictureex.pauseRender();
		}
		adstruct.fontstruct.Font = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Font;					// 广告中文字对象的字体
		adstruct.fontstruct.Size = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Size;					// 广告中文字对象的大小
		adstruct.fontstruct.Color = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Color;					// 广告中文字对象的颜色
		adstruct.fontstruct.TimeLength = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].TimeLength;		// 广告中文字对象的播放时长
		adstruct.fontstruct.Content = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Content;				// 广告中文字对象的内容
		adstruct.fontstruct.DisplayMode = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].DisplayMode;		// 广告中文字对象的播放方式
		adstruct.fontstruct.Speed = AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo[FontNowNumber].Speed;					// 广告中文字对象的播放速度
		PlayAdFont(pDC,adstruct.fontstruct.Font,adstruct.fontstruct.Size,adstruct.fontstruct.Color,adstruct.fontstruct.Content,adstruct.fontstruct.DisplayMode,adstruct.fontstruct.Speed);
		// 首次播放
		if(1 == FirstTime)
		{
			PlayFont_OriSeconds = GetTickCount();		// 文字播放开始计时
			FirstTime ++;
		}
		
		if(IsTimeUp(_ttoi(adstruct.fontstruct.TimeLength)*1000,PlayFont_OriSeconds))
		{
			FirstTime = 1;
			FontNowNumber++;		// 一个广告对象中的下一个文字对象
			// 下一个广告对象中的文字对象
			if(FontNowNumber >= AdvertisementInfo[AdNowNumber].adfontstruct.fontinfo.size())
			{
				
				font_right = 1280;				// 播放文字初始的right坐标
				font_left = 0;					// 播放文字初始的left坐标
				font_bottom = 1024;				// 播放文字初始的bottom坐标
				font_top = 0;					// 播放文字初始的top坐标


				// 文字播放在图片播放后
				if(AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber > AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo)
				{
					AdNowNumber++;
					// 广告对象播放完，从头开始重新播放
					if(AdNowNumber >= AdvertisementInfo.size())
					{
						AdNowNumber = 0;
					}
					// // 下一组文字播放SerialNo < 图片文字播放SerialNo
					if(AdvertisementInfo[AdNowNumber].adfontstruct.FontPlayNumber < AdvertisementInfo[AdNowNumber].adpicturestruct.SerialNo)
					{
						PlayType = PLAY_TYPE_FONT;		// 播放广告中的文字内容
					}
					// 下一组文字播放SerialNo < 图片文字播放SerialNo
					else
					{
						PlayType = PLAY_TYPE_PICTURE;	// 播放广告中的图片内容
					}
				}
				// 图片播放在文字播放后
				else
				{
					PlayType = PLAY_TYPE_PICTURE;		// 播放广告中的图片内容
				}
				
				FontNowNumber = 0;
				
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief      重画Dialog背景

 @param     (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CStandbyDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      按不同播放方式播放广告中的文字内容

 @param     (i) CDC* pDC   设备上下文

 @param     (iii) Font     字体

 @param     (iv)  size     字体大小

 @param     (v)   Color    字体颜色

 @param     (vi)  content   广告中的文字内容

 @param     (ii) PlayMode  播放方式

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::PlayAdFont(CDC* pDC,CString Font,CString size,CString Color,CString content,CString PlayMode,CString speed)
{
	CSize sz(0,0);		// 字体信息
	CStringArray fontcontentarray; 
	// 用二级缓存来减少刷新时导致的文字闪烁
	
	m_memDC.CreateCompatibleDC(NULL);					// 创建一个与内存表面相关联的DC            
	m_menBit.CreateCompatibleBitmap(pDC,1920,1080);		// 创建与DC相关的位图
	m_memDC.SelectObject(&m_menBit);

	m_memDC.FillSolidRect(0,0,1920,1080,RGB(0,255,0));	// 填充屏幕的背景色
	
	CFont font;
	font.CreatePointFont(_ttoi(size),Font);				// 创建大小为size，字体为Font的字体
	CFont* pOldFont = m_memDC.SelectObject(&font);
	
	COLORREF rgb = _ttoi(Color);
	m_memDC.SetTextColor(rgb);							// 设置字体颜色
	
	// 判断是否有换行
	int Index = content.Find(_T("|"));
	// 根据不同的播放方式播放文字
	switch(_ttoi(PlayMode))
	{
	// 由右到左播放
	case RTOL_ROLL:
		sz = m_memDC.GetTextExtent(content);
		// 未找到换行符'|'
		if(-1 == Index)
		{
		}
		// 找到换行符'|'
		else
		{
			content.Replace(_T("|"),_T(" "));		// '|'用空格代替
		}
		m_memDC.TextOut(font_right,(1080-sz.cy)/2,content);   
		if(font_right -= _ttoi(speed),font_right <= -sz.cx)
		{
			font_right = 1920;
		}
		break;

	// 由下到上播放
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
		// 找到换行符'|'
		else
		{
			int find_index = content.ReverseFind('|');			// 反向查找'|'
			fontcontentarray.Add(content.Mid(find_index+1));	// 取找到的第一个'|'放CStringArray中
			// 查找所有'|'，取字符串放CStringArray中
			while(find_index>0)
			{
				content=content.Mid(0,find_index);
				find_index=content.ReverseFind('|');
				fontcontentarray.Add(content.Mid(find_index+1));
			}
			int size = fontcontentarray.GetSize();
			// 格式化输出
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

		// 由左到右播放
	case LTOR_ROLL:
		sz = m_memDC.GetTextExtent(content);
		// 未找到换行符'|'
		if(-1 == Index)
		{
		}
		// 找到换行符'|'
		else
		{
			content.Replace(_T("|"),_T(" "));		// '|'用空格代替
		}
		m_memDC.TextOut(font_left,(1080-sz.cy)/2,content);   
		if(font_left += _ttoi(speed),font_left >= 1280+sz.cx)
		{
			font_left = 0;
		}
		break;

		// 由上到下播放
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
		// 找到换行符'|'
		else
		{
			int find_index = content.ReverseFind('|');			// 反向查找'|'
			fontcontentarray.Add(content.Mid(find_index+1));	// 取找到的第一个'|'放CStringArray中
			// 查找所有'|'，取字符串放CStringArray中
			while(find_index>0)
			{
				content=content.Mid(0,find_index);
				find_index=content.ReverseFind('|');
				fontcontentarray.Add(content.Mid(find_index+1));
			}
			int size = fontcontentarray.GetSize();
			// 格式化输出
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

	// 默认静止
	default:
		if(-1 == Index)
		{
			sz = m_memDC.GetTextExtent(content);
			m_memDC.TextOut((1920-sz.cx)/2,(1080-sz.cy)/2,content);
		}
		// 找到换行符'|'
		else
		{
			int find_index = content.ReverseFind('|');			// 反向查找'|'
			fontcontentarray.Add(content.Mid(find_index+1));	// 取找到的第一个'|'放CStringArray中
			// 查找所有'|'，取字符串放CStringArray中
			while(find_index>0)
			{
				content=content.Mid(0,find_index);
				find_index=content.ReverseFind('|');
				fontcontentarray.Add(content.Mid(find_index+1));
			}
			int size = fontcontentarray.GetSize();
			// 格式化输出
			for(int index = 0; index < size;index++)
			{
				sz = m_memDC.GetTextExtent(fontcontentarray.GetAt(index));
				m_memDC.TextOut((1920-sz.cx)/2,(800-sz.cy*index*2-20)/2,fontcontentarray.GetAt(index));
			}
		}
		break;

	}
	//把内存绘图拷贝到屏幕
	pDC->BitBlt(0,0,1920,1080,&m_memDC,0,0,SRCCOPY);

	//清理释放
	m_memDC.SelectObject(pOldFont);
	ReleaseDC(pDC);
	m_memDC.DeleteDC();
	m_menBit.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      按不同格式播放广告中的图片内容

 @param     (i) CDC* pDC               设备上下文

 @param     (i) CString FilePathName   图片资源位置

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStandbyDlg::PlayAdPicture(CString FilePathName)
{
	if(this->IsWindowVisible()){
		int Index = FilePathName.Find(_T("gif"));
		// FilePathName中找到"gif"
		if(-1 != Index)
		{
			CRect rect(0,0,1920,1080);	
			pictureex.loadPic(FilePathName);
			pictureex.MoveWindow(rect);
			pictureex.resumeRender();
			m_bIsRenderGif = true;
			//pictureex.renderPicture(FilePathName,this->m_hWnd,rect);
			//BOOL IsCreate = pictureex.Create(_T(""),WS_CHILD|WS_VISIBLE,rect,this,21234);  
			//// 创建窗口返回TRUE
			//if(IsCreate)
			//{
			//	pictureex.Load(FilePathName); 
			//	BOOL IsGif = pictureex.IsGIF();
			//	// 不是GIF
			//	if(!IsGif)
			//	{
			//		return;
			//	}
			//	// 是GIF，进行绘图
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
		// 不是GIF
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
@brief     字体按SerialNo排序

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
@brief     图片按SerialNo排序

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
@brief     广告按SerialNo排序

@param     (i)CParseXML::Advertisement_Object ad_object1   广告对象1

@param     (ii)CParseXML::Advertisement_Object ad_object1  广告对象2

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
@brief     获取广告信息 

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
	// 广告顺序播放
	sort(AdvertisementInfo.begin(),AdvertisementInfo.end(),sortadbyserialno);
	// 广告中文字内容顺序播放
	for(vector<CParseXML::AdStruct>::iterator adinfo = AdvertisementInfo.begin();adinfo != AdvertisementInfo.end(); adinfo++)
	{
		sort(adinfo->adfontstruct.fontinfo.begin(),adinfo->adfontstruct.fontinfo.end(),sortfontbyserialno);						// 字体按SerialNo排序
		sort(adinfo->adpicturestruct.pictureinfo.begin(),adinfo->adpicturestruct.pictureinfo.end(),sortpicturebyserialno);		// 图片按SerialNo排序
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断时间处理执行是否超时

@param      (i)DWORD dwTimer		超时时间间隔
@param      (i)DWORD dwBaseTime		基本时间

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
		dwCurTime = GetTickCount();		// 当前时间
		dwUpTime = dwBaseTime + dwTimer;// 结束时间
		if (dwUpTime >= dwBaseTime){
			if (dwCurTime >= dwBaseTime){
				// 超时（当前时间 > 结束时间）
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
@brief      屏幕点击退出广告播放

@param      (i)CPoint point		鼠标位置

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CStandbyDlg::OnNcHitTest(CPoint point)
{
	if(this->IsWindowVisible()){// 只有当前界面已经显示时才响应此函数
		if(REST == theSERVICE_MGR.GetState() && (point.x >= 0 && point.x <= 1280) && ( point.y >= 0 && point.y <= 1024) && m_bIsShowUp){
			KillTimer(PLAY_FONT_TIMER);	  // 停止計時器
			KillTimer(PLAY_PICTURE_TIMER);// 停止計時器
			if(m_bIsRenderGif){
				pictureex.pauseRender();// 停止动态图播放
				m_bIsRenderGif = false;
			}
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
			m_bIsShowUp = false;
		}
	}	
	return TRUE;
}