#ifndef _CGUIPARM_H
#define _CGUIPARM_H

/** 
version 1.0.0
author mxw
time stamp 20140704 09.31

time stamp 20140714 17.31	修正 getNext** 系列函数中错误,应该都是getNextLine

time stamp 20140716 10.00	修正 UE_CSV_RESULT::UEUNKNOWN和其他的文件中UE_UNKNOWN冲突的问题
*/
#include <list>
#include <map>
#include <vector>
#define UD_MAX_LINEWORD	1024	//定义每行最多1024个字符
#define UD_MEM_ALIGNMEMENT 16
typedef enum _UE_CSV_RESULT
{
	UE_SUCCESS=0,	//操作成功
	UE_ERRPATH=1,	//指定路径的文件不存在
	UE_FAILOPEN,	//打开文件失败
	UE_OUTMEM,		//堆溢出
	UE_FAILE,		//操作失败
	UE_EMPTYLIST,	//空链表
	UE_LISTEND,		//链表尾部
	UE_UNKNOWNCSVRET//未知错误
}UE_CSV_RESULT;

class CCsvReader
{
public:
	CCsvReader();
	~CCsvReader();
public:
	//inner class
	class CLineData
	{
	public:
		std::map<int ,const TCHAR*> m_mapLine;	
	private:
		std::map<int ,const TCHAR* >::iterator m_iter;
	public:
		CLineData();
		~CLineData();
	public:
		TCHAR* getFirstSplit();
		TCHAR* getNextSplit();	
		TCHAR* operator[](size_t index);
		size_t getMapSize();
	};
private:
	std::list<CLineData*>			m_listLines;
	std::list<CLineData*>::iterator	m_iter;
public:
	UE_CSV_RESULT	loadCsvFile(const TCHAR* tcpath,const TCHAR* tcsplit);
	CLineData*		getFirstLine();
	CLineData*		getNextLine();
	size_t			getLineCount();
private:
	UE_CSV_RESULT	splitLine(const TCHAR* tcLineData,const TCHAR* tcsplit);
};

typedef struct _US_LAYOUT_DATA
{
	typedef enum _UE_LAYOUT_INDEX
	{
		UE_PAGECODE=0,		//页编码
		UE_PAGEMARK,		//页标识
		UE_CHBASEMAPCODE,	//中文底图编号
		UE_ENBASEMAPCODE,	//英文底图编号
		UE_PUBASEMAPCODE,	//葡文底图编号
		UE_TOPLEFTX,		//左上角X坐标
		UE_TOPLEFTY,		//左上角Y坐标
		UE_PICWIDTH,		//图片宽度
		UE_PICHEIGHT,		//图片高度
		UE_ISHOMEPAGE,		//是否是否是首页
		UE_ENABLEAMRK,		//启用标志
		UE_ELEMENTENABLEMARK//元素启用标志
	}UE_LAYOUT_INDEX;

	TCHAR* pPageCode;		//页编码
	TCHAR* pPageMark;		//页标识
	TCHAR* pChBaseMapCode;	//中文底图编号
	TCHAR* pEnBaseMapCode;	//英文底图编号
	TCHAR* pPuBaseMapCode;	//葡文底图编号
	TCHAR* PTopLeftX;		//左上角X坐标
	TCHAR* PTopLeftY;		//左上角Y坐标
	TCHAR* pPicWidth;		//图片宽度
	TCHAR* pPicHeight;		//图片高度
	TCHAR* pIsHomePage;		//是否首页
	TCHAR* PEnableMark;		//启用标志
	TCHAR* pElementEnableMark;//元素启用标志

	_US_LAYOUT_DATA()
	{
		pPageCode=NULL;
		pPageMark=NULL;
		pChBaseMapCode=NULL;
		pEnBaseMapCode=NULL;
		pPuBaseMapCode=NULL;
		PTopLeftX=NULL;
		PTopLeftY=NULL;
		pPicWidth=NULL;
		pPicHeight=NULL;
		pIsHomePage=NULL;
		PEnableMark=NULL;
		pElementEnableMark=NULL;
	}
	~_US_LAYOUT_DATA()
	{
	}
}US_LAYOUT_DATA;

typedef struct _US_PIC_DATA
{
	TCHAR* pPicCode;
	TCHAR* pPicName;
	typedef enum _UE_PIC_INDEX
	{
		UE_PICCODE=0,
		UE_PICNAME
	}UE_PIC_INDEX;

	_US_PIC_DATA()
	{
		pPicCode=NULL;
		pPicName=NULL;
	}
	~_US_PIC_DATA()
	{
	}
}US_PIC_DATA;

typedef struct _US_BUTTON_DATA
{
	TCHAR* pBtnCode;			//按钮编号
	TCHAR* pBtnPage;			//所属页编号
	TCHAR* pBtnMark;			//按钮标识
	TCHAR* pBtnValue;			//按钮值
	TCHAR* pBtnJumpPage;		//跳转页编码
	TCHAR* pBtnXPos;			//x坐标
	TCHAR* pBtnYPos;			//Y坐标
	TCHAR* pBtnWidth;			//按钮宽度
	TCHAR* pBtnHeight;			//按钮高度
	TCHAR* pBtnNormamPicCode;	//正常图片编号
	TCHAR* pBtnPressedPicCode;	//按下是图片编号
	TCHAR* pBtnHoverPicCode;	//选择图片编号
	TCHAR* pBtnDisablePicCode;	//无效图片编号
	TCHAR* pBtnIsEnable;		//是否可用标志
	TCHAR* pBtnIsShowMark;		//是否显示标志
	TCHAR* pBtnIsHotStation;	//是否是热站点

	TCHAR* pChtxtPos;			//文字位置[中]
	TCHAR* pChcontent;			//内容[中]
	TCHAR* pChfontName;			//字体名称[中]
	TCHAR* pChfontSize;			//字体大小[中]
	TCHAR* pChfontColor;		//颜色[中]

	TCHAR* pEntxtPos;			//文字位置[英]
	TCHAR* pEncontent;			//内容[英]
	TCHAR* pEnfontName;			//字体名称[英]
	TCHAR* pEnfontSize;			//字体大小[英]
	TCHAR* pEnfontColor;		//颜色[英]

	TCHAR* pPutxtPos;			//文字位置[葡]
	TCHAR* pPucontent;			//内容[葡]
	TCHAR* pPufontName;			//字体名称[葡]
	TCHAR* pPufontSize;			//字体大小
	TCHAR* pPufontColor;		//颜色[葡]

	typedef enum _UE_BTN_INDEX
	{
		UE_BTNCODE=0,
		UE_BTNPAGE,
		UE_BTNMARK,
		UE_BTNVALUE,
		UE_BTNJUMPPAGE,
		UE_BTNXPOS,
		UE_BTNYPOS,
		UE_BTNWIDTH,
		UE_BTNHEIGHT,
		UE_BTNNORMALPICCODE,
		UE_BTNPRESSEDPICCODE,
		UE_BTNHOVERPICCODE,
		UE_BTNDISABLEPICCODE,

		UE_BTNCHTXTPOS,
		UE_BTNCHTXTCONTENT,
		UE_BTNCHFONTNAME,
		UE_BTNCHFONTSIZE,
		UE_BTNCHFONTCOLOR,

		UE_BTNENTXTPOS,
		UE_BTNENTXTCONTENT,
		UE_BTNENFONTNAME,
		UE_BTNENFONTSIZE,
		UE_BTNENFONTCOLOR,

		UE_BTNPUTXTPOS,
		UE_BTNPUTXTCONTENT,
		UE_BTNPUFONTNAME,
		UE_BTNPUFONTSIZE,
		UE_BTNPUFONTCOLOR,

		UE_BTNISENABLE,
		UE_BTNISSHOWMARK,
		UE_BTNISHOTSTATION,
	}UE_BTN_INDEX;

	_US_BUTTON_DATA()
	{
		pBtnCode=NULL;
		pBtnPage=NULL;
		pBtnMark=NULL;
		pBtnValue=NULL;
		pBtnJumpPage=NULL;
		pBtnXPos=NULL;
		pBtnYPos=NULL;
		pBtnWidth=NULL;
		pBtnHeight=NULL;
		pBtnNormamPicCode=NULL;
		pBtnPressedPicCode=NULL;
		pBtnHoverPicCode=NULL;
		pBtnDisablePicCode=NULL;
		pBtnIsEnable=NULL;
		pBtnIsShowMark=NULL;
		pBtnIsHotStation=NULL;

		pChtxtPos=NULL;
		pChcontent=NULL;
		pChfontName=NULL;
		pChfontSize=NULL;
		pChfontColor=NULL;

		pEntxtPos=NULL;
		pEncontent=NULL;
		pEnfontName=NULL;
		pEnfontSize=NULL;
		pEnfontColor=NULL;

		pPutxtPos=NULL;
		pPucontent=NULL;
		pPufontName=NULL;
		pPufontSize=NULL;
		pPufontColor=NULL;
	}
}US_BUTTON_DATA;

typedef struct _US_SPECIALBTNDATAS
{
	US_BUTTON_DATA*	m_pDataPtr;
	int				m_idataCount;
	_US_SPECIALBTNDATAS()
	{
		m_pDataPtr=NULL;
		m_idataCount=0;
	}
	~_US_SPECIALBTNDATAS()
	{
		if(NULL!=m_pDataPtr)
		{
			_aligned_free(m_pDataPtr);
			m_idataCount=0;
		}
	}
}US_SPECIALBTNDATAS;

typedef struct _US_SPECIALPICDATAS
{
	US_PIC_DATA*	m_pDataPtr;
	int				m_idataCount;
	_US_SPECIALPICDATAS()
	{
		m_pDataPtr=NULL;
		m_idataCount=0;
	}
	~_US_SPECIALPICDATAS()
	{
		if(NULL!=m_pDataPtr)
		{
			_aligned_free(m_pDataPtr);
			m_idataCount=0;
		}
	}
}US_SPECIALPICDATAS;

typedef struct _US_SPECIALLAYOUTDATAS
{
	US_LAYOUT_DATA* m_pDataPtr;
	int				idataCount;
	_US_SPECIALLAYOUTDATAS()
	{
		m_pDataPtr=NULL;
		idataCount=0;
	}
	~_US_SPECIALLAYOUTDATAS()
	{
		if(NULL!=m_pDataPtr)
		{
			_aligned_free(m_pDataPtr);
			m_pDataPtr=NULL;
		}
	}
}US_SPECIALLAYOUTDATAS;

typedef struct _area_btn_info{
	DWORD		number;						//按钮编号
	CPoint		stationBtnPoint;			//按鈕座標
	CSize		stationBtnSize;				//按鈕大小
	//CString		stationNormalBitmap;		//按鈕正常圖片
	//CString		stationOnBitmap;			//按鈕懸浮圖片
	//CString		stationDownBitmap;			//按鈕按下圖片
	//CString		stationDisableBitmap;		//按鈕不可用圖片
	CImage*		pImageNormal;				//正常图片 //图片对象指针 生命周期和guiparm对象相等
	CImage*		pImageHover;				//悬浮图片
	CImage*		pImagePressed;				//按下图片
	CImage*		pImageDisabled;				//不可用图片
	bool		isEnable;					//按鈕是否可用標誌
	bool		isVisiable;					//按鈕是否顯示標誌
	bool		isHotStation;				//是否是熱點站
	DWORD		btnID;						// 按钮值	第四列
	DWORD		nextPage;					//跳转页编码
	BYTE		textLocationType;			//文字位置
	CString		text;						//文本
	CString		fontname;					//字体名称
	int			fontsize;					//字体大小
	COLORREF	textcolor;					//字体颜色
	POINT		textoffset;					//文本相对按钮坐标的偏移
	_area_btn_info()
	{
		number = 0;

		stationBtnPoint.x=0;
		stationBtnPoint.y=0;
		
		stationBtnSize.cx=0;
		stationBtnSize.cy=0;

		/*stationNormalBitmap=(_T(""));
		stationOnBitmap=(_T(""));
		stationDownBitmap=_T("");
		stationDisableBitmap=_T("");*/
		pImageNormal=NULL;				//正常图片 //图片对象指针 生命周期和guiparm对象相等
		pImageHover=NULL;				//悬浮图片
		pImagePressed=NULL;				//按下图片
		pImageDisabled=NULL;				//不可用图片
		isEnable=false;
		isVisiable=false;
		isHotStation=false;
		btnID=0;   // 按钮值
		nextPage=0;   //跳转页编码
		textLocationType=0;
		text = _T("");
		fontname = _T("宋体");
		fontsize = 15;
		textcolor = WHITE;
	}
}AREA_BTN_INFO;

typedef struct _area_layout{
	DWORD layOutId;				//頁編碼
	CPoint startpoint;			//左上角
	CSize areaSize;				//圖片大小 寬度 高度
	CImage* pImageBackground;
	//CString areaBackImage;		//底圖 (路徑)
	bool isEnable;				//啟用標誌
	_area_layout()
	{
		layOutId=0;
		startpoint.x=0;
		startpoint.y=0;

		areaSize.cx=0;
		areaSize.cy=0;

		pImageBackground=NULL;
		//areaBackImage=_T("");
		isEnable=false;
	}
}AREA_LAYOUT;

class CImageMgr
{
public:
	CImageMgr();
	~CImageMgr();
private:
	std::map<UINT,CImage*> m_mapImages;
public:
	void	insertImage(TCHAR* tcpiccode, const TCHAR* pcpath);
	CImage* getImage(TCHAR* tcpiccode);
};

class CGuiParm
{
public:
	CGuiParm();
	~CGuiParm();
public:
	//讀入LayOut配置文件
	void initializeLayOut(const TCHAR* tcpath,const TCHAR* tcsplit = _T("$"));
	//讀入Button配置文件
	void initializeButton(const TCHAR* tcpath,const TCHAR* tcsplit = _T("$"));
	//讀入Pic配置文件
	void initializePic(const TCHAR* tcpath,const TCHAR* tcsplit = _T("$"));

	//获取layout相关内容
	//獲取第一條 layout 信息
	US_LAYOUT_DATA*			getFirstLayOutData();
	//獲取下一條 layout 信息
	US_LAYOUT_DATA*			getNextLayOutData();
	//獲取指定 layout 信息
	US_SPECIALLAYOUTDATAS*	getSpecialLayoutDatas(US_LAYOUT_DATA::UE_LAYOUT_INDEX ueIndex,const TCHAR* tcSpecialValue);
	//获取pic相关内容
	US_PIC_DATA*			getFirstPicData();
	US_PIC_DATA*			getNextPicData();
	US_SPECIALPICDATAS*		getSpecialPicDatas(US_PIC_DATA::UE_PIC_INDEX ueIndex,const TCHAR* tcSpecialValue);
	//获取button相关内容
	US_BUTTON_DATA*			getFirstBtnData();
	US_BUTTON_DATA*			getNextBtnData();
	US_SPECIALBTNDATAS*		getSpecialBtnDatas(US_BUTTON_DATA::UE_BTN_INDEX ueIndex,const TCHAR* tcSpecialValue);
//ex
	long getLayOutPageCodes(vector<DWORD>& pageCodes);
	long getPicData(vector<US_PIC_DATA>& picsInfo);
	//獲取 id  語言 獲取button 信息
	long getAreaBtnInfo(DWORD areaID,LANGUAGETYPE_T languageType,vector<AREA_BTN_INFO>& btnInfos);
//ex
	//獲取 第一條 layout 信息
	long					getFirstAreaLaylout(LANGUAGETYPE_T languageType,AREA_LAYOUT& firstLayOut);
	//根據指定 id 語言 獲取 layout 信息
	long					getAreaLaylout(DWORD areaLayoutID,LANGUAGETYPE_T languageType,AREA_LAYOUT& layOutData);

	// 获取Home Page界面
	long GetHomePageLayout(LANGUAGETYPE_T languageType,AREA_LAYOUT& homeLayout);
private:
	void m_getAreaInfo(US_LAYOUT_DATA* playOutData,LANGUAGETYPE_T languageType,AREA_LAYOUT& firstLayOut);
private:
	//各列数据的结构体
	US_LAYOUT_DATA			m_layoutData;
	US_PIC_DATA				m_picData;
	US_BUTTON_DATA			m_btnData;

	CCsvReader				m_layOutCsv;	//储存layout文件的对象
	CCsvReader				m_buttonCsv;	//存储button文件的对象
	CCsvReader				m_picCsv;		//储存pic文件的对象

	UE_CSV_RESULT			m_ueLayOutRet;	//标记初始化是否成功
	UE_CSV_RESULT			m_ueButtonRet;	//标记初始化是否成功
	UE_CSV_RESULT			m_uePicRet;		//标记初始化是否成功
	//根据指定列的指定内容查找的结果集
	US_SPECIALLAYOUTDATAS	m_usSpecialLayoutDatas;
	US_SPECIALPICDATAS		m_usSpecialPicDatas;
	US_SPECIALBTNDATAS		m_usSpecialBtnDatas;

	CImageMgr				m_imageMgr;
};

#endif