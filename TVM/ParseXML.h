#include "tinystr.h" 
#include "tinyxml.h"
#include <map>

#define thePARSE_XML	CParseXML::GetInstance()			///< CParseXML的唯一接口

/**
@brief 解析XML文件
*/

class CParseXML
{
public:
	typedef struct _Advertisement_Object
	{
		CString AdvertisementName;	// 广告名
		CString SerialNo;			// 广告播放顺序号
		CString ValidateTime;		// 失效时间
		CString ResourcePath;		// 文件位置
		_Advertisement_Object()
		{
			AdvertisementName = _T("");
			SerialNo = _T("");
			ValidateTime = _T("");
			ResourcePath = _T("");
		}
	}Advertisement_Object;

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

	typedef struct _AdfontStruct
	{	
		CString FontPlayNumber;			// 文字播放顺序号
		vector<FontStruct> fontinfo;	// 字体信息
		_AdfontStruct()
		{
			FontPlayNumber = _T("");
		}
	}AdFontStruct;

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

	typedef struct _AdpictureStruct
	{
		CString SerialNo;		// 图片广告播放顺序
		vector<PictureStruct> pictureinfo;
		_AdpictureStruct()
		{
			SerialNo = _T("");
		}
	}AdPictureStruct;

	typedef struct _videoStruct
	{
		CString	SerialNo;				// 视频广告对象播放顺序
		CString FilePathName;			// 视频位置
		_videoStruct()
		{
			SerialNo = _T("");
			FilePathName = _T("");
		}
	}VideoStruct;

	typedef struct _AdvideoStruct
	{
		CString	SerialNo;				// 视频广告播放顺序
		vector<VideoStruct> videoinfo;
		_AdvideoStruct()
		{
			SerialNo = _T("");
		}
	}AdvideoStruct;

	typedef struct _adStruct
	{
		Advertisement_Object advertisementobject;			// 广告属性
		AdFontStruct adfontstruct;							// 文字广告
		AdPictureStruct adpicturestruct;					// 图片广告
		AdvideoStruct advideostruct;						// 视频广告
	}AdStruct;

	vector<AdStruct>AdvertisementInfo;						// 广告信息
public:
	bool GetNodePointerByName(TiXmlElement* pRootEle/*根元素*/,CString strNodeName/*结点名*/,TiXmlElement* &destNode/*结点指针*/);		// 根据结点名获取结点指针
	bool QueryNodeText(CString XMLFileName,CString strNodeName,CString &strText);						// 读取XML文件
	bool QueryElementValue(CString XMLFileName,CString strNodeName,vector<AdStruct>& AdvertisementInfo);		// 读取XML文件

	static CParseXML &GetInstance()
	{
		return theInstance;
	}
private:
	AdStruct adstruct;
	static CParseXML theInstance;
	
	// 构造函数私有
	CParseXML();
	~CParseXML();
	CParseXML(const CParseXML&);
	CParseXML &operator=(const CParseXML&);
};