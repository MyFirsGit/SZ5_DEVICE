#pragma once
#include "BaseDlg.h"
#include "StandbySvc.h"
#include "TVM.h"
#include "PictureEx.h"

/**
@brief      待机信息画面
*/

class CStandbyDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CStandbyDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStandbyDlg(CService* pService);
	~CStandbyDlg();

	enum { IDD = IDD_20901_SYSTEM_OUT_DLG };						// 对话框数据

	enum _timer
	{
		PLAY_FONT_TIMER,		// 播放文字内容定时器
		PLAY_PICTURE_TIMER		// 播放图片内容定时器
	};

	enum _playtype
	{
		PLAY_TYPE_FONT = 1,		// 播放广告中文字内容
		PLAY_TYPE_PICTURE		// 播放广告中图片内容
	}playtype;

	enum _FontPlayMode
	{
		RTOL_ROLL = 1,			// 由右到左播放
		DTOU_ROLL,				// 由下到上播放
		LTOR_ROLL,				// 由左到右播放
		UTOD_ROLL				// 由上到下播放
	}FontPlayMode;
	
	typedef struct _fontStruct
	{	 
		CString SerialNo;		// 文字播放顺序
		CString Font;			// 字体
		CString Size;			// 大小
		CString Color;			// 颜色
		CString TimeLength;		// 播放时间
		CString Content;		// 播放内容
		CString DisplayMode;	// 播放方式
		CString Speed;			// 播放速度
		_fontStruct()
		{
			SerialNo = _T("");
			Font = _T("");
			Size = _T("");
			Color = _T("");
			TimeLength = _T("");
			Content = _T("");
			DisplayMode = _T("");
			Speed = _T("");
		}
	}FontStruct;

	typedef struct _pictureStruct
	{
		CString SerialNo;				// 图片广告对象播放顺序
		CString TimeLength;				// 图片广告播放时间
		CString FilePathName;			// 图片位置
		_pictureStruct()
		{
			SerialNo = _T("");
			TimeLength = _T("");
			FilePathName = _T("");
		}
	}PictureStruct;

	typedef struct _adStruct
	{
		FontStruct  fontstruct;							// 文字广告
		PictureStruct picturestruct;					// 图片广告
	}AdStruct;

	BOOL IsTimeUp(IN DWORD dwTimer,IN DWORD dwBaseTime);																	//  判断时间处理执行是否超时

	void PlayAdFont(CDC* pDC,CString Font,CString size,CString Color,CString content,CString PlayMode,CString speed);		// 按不同播放方式播放广告中的文字内容
	void PlayAdPicture(CString FilePathName);	

	void GetAdvertisementInfo();																							// 获取广告信息
	static bool sortfontbyserialno(CParseXML::FontStruct fontstruct1,CParseXML::FontStruct fontstruct2);					// 字体按SerialNo排序
	static bool sortpicturebyserialno(CParseXML::PictureStruct picturestruct1,CParseXML::PictureStruct picturestruct2);		// 图片按SerialNo排序
	static bool sortadbyserialno(CParseXML::AdStruct ad_object1,CParseXML::AdStruct ad_object2);							// 广告按SerialNo排序
protected:
	void UpdateUI();
private:
	int font_right;										// 广告中文字对象由右到左播放right坐标
	int font_left;										// 广告中文字对象由左到右播放left坐标			
	int font_bottom;									// 广告中文字对象由下到上播放bottom坐标	
	int font_top;										// 广告中文字对象由上到下播放top坐标
	CDC m_memDC;
	CBitmap m_menBit;

	CImage image;
	CPictureEx pictureex;
	virtual void OnDraw(CDC* pDC);                                  // 画面绘制函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);			// 屏幕点击退出广告播放
	void OnTimer(UINT nIDEvent);
private:
	AdStruct adstruct;
	vector<CParseXML::AdStruct>AdvertisementInfo;
	int Adsz;
	int AdNowNumber;
	int FontNowNumber;
	int PictureNowNumber;
	int FirstTime;
	DWORD PlayFont_OriSeconds;							// 广告中文字播放基本时间
	DWORD PlayPicture_OriSeconds;						// 广告中图片播放基本时间
	bool m_bIsPlayFont;									// 广告是否播放文字？
	bool m_bIsPlayPic;									// 广告是否播放图片？
private:
	int PlayType;										// 广告中文字、图片、视频内容切换标志	
	bool m_bIsShowUp;									// 廣告是否已經顯示？
	bool m_bIsRenderGif;								// GIF图片是否播放？
};

