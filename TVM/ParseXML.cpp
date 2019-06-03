#include "StdAfx.h"
#include "ParseXML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CParseXML CParseXML::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无
 
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParseXML::CParseXML()
{
    
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParseXML::~CParseXML()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     根据结点名获取结点指针

@param      TiXmlElement* pRootEle  根元素

@param      CString strNodeName 结点名

@param      TiXmlElement* &destNode 结点指针

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseXML::GetNodePointerByName(TiXmlElement* pRootEle,CString strNodeName,TiXmlElement* &destNode)
{
	// 如果等于根结点名则退出
	if(strNodeName == pRootEle->Value())
	{
		destNode = pRootEle;
		return true;
	}

	TiXmlElement* pEle = pRootEle;
	for(pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
	{
		// 递归处理子结点，获取结点指针
		if(GetNodePointerByName(pEle,strNodeName,destNode))
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     读取XML文件

@param      CString XMLFileName		文件名

@param      CString strNodeName		结点名

@param      CString strText			文字

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseXML::QueryNodeText(CString XMLFileName,CString strNodeName,CString &strText)
{
	const wchar_t* wFilePath = (LPCTSTR)XMLFileName;
	char buf[200] = {0};
	wcstombs(buf,wFilePath,wcslen(wFilePath));
	const char* destFilePath = buf;
	TiXmlDocument* doc = new TiXmlDocument(destFilePath);
	bool loadOK = doc->LoadFile();

	TiXmlElement* RootElement = doc->RootElement();		// 根元素
	if(NULL == RootElement)
	{
		return false;
	}
	TiXmlElement* pEle = NULL;

	// 找到值为strNodeName的结点
	GetNodePointerByName(RootElement,strNodeName,pEle);
	if(NULL != pEle)
	{
		strText = pEle->GetText();
		return true;
	}
	else
	{
		return false;
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     读取XML文件

@param      CString XMLFileName		文件名

@param      CString strNodeName		结点名

@param      AdStruct& adinfo		广告信息

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseXML::QueryElementValue(CString XMLFileName,CString strNodeName,vector<AdStruct>& AdvertisementInfo)
{
	AdStruct adstruct;							// 广告信息
	TiXmlElement* Advertisement_Object = NULL;	

	TiXmlAttribute* Ad_Font_Attribute = NULL;	// 字体属性
	FontStruct fontstruct;						// 字体信息结构体
	TiXmlElement* Text_Object = NULL;			// 字体对象-Text_Object

	TiXmlAttribute *Ad_Picture_Attribute = NULL; // 图片属性
	PictureStruct picturestruct;				 // 图片信息结构体
	TiXmlElement* Picture_Object = NULL;		 // 图片对象-Picture_Object

	TiXmlAttribute *Ad_Video_Attribute = NULL;	 // 视频属性
	VideoStruct videostruct;					 // 视频信息结构体
	TiXmlElement* Video_Object = NULL;			 // 视频对象-Video_Object

	const wchar_t* wFilePath = (LPCTSTR)XMLFileName;
	char buf[200] = {0};
	wcstombs(buf,wFilePath,wcslen(wFilePath));
	const char* destFilePath = buf;

	TiXmlDocument* doc = new TiXmlDocument;
	if(NULL == doc)
	{
		return false;
	}
	bool loadad = doc->LoadFile(destFilePath,TIXML_ENCODING_UTF8);
	// 根结点
	TiXmlElement* pRootEle = doc->RootElement();
	if(NULL == pRootEle)
	{
		return false;
	}
	TiXmlElement* pEle = NULL;
	// 广告对象
	CString Ad_Object = _T("Advertisement_Object");
	GetNodePointerByName(pRootEle,Ad_Object,pEle);
	for(;pEle != NULL;pEle = pEle->NextSiblingElement())
	{
		TiXmlAttribute *Ad_Attribute = pEle->FirstAttribute();

		adstruct.advertisementobject.AdvertisementName = Ad_Attribute->Value();		// 广告名
		Ad_Attribute = Ad_Attribute->Next();

		adstruct.advertisementobject.SerialNo = Ad_Attribute->Value();				// 广告播放顺序
		Ad_Attribute = Ad_Attribute->Next();

		adstruct.advertisementobject.ValidateTime = Ad_Attribute->Value();			// 广告失效时间
		Ad_Attribute = Ad_Attribute->Next();

		_DATE_TIME currentTime = ComGetCurTime();									// 系统当前时间
		CString currentTimeToint = currentTime.ToString(_T("%04d%02d%02d%02d%02d%02d"));
		// 广告失效
		if(adstruct.advertisementobject.ValidateTime < currentTimeToint)
		{
			continue;
		}
		// 广告未失效
		else
		{
			adstruct.advertisementobject.ResourcePath = Ad_Attribute->Value();		// 广告位置
			Ad_Attribute = Ad_Attribute->Next();
			TiXmlElement* Ad_element = pEle->FirstChildElement();
			CString ObjectName = _T("");
			ObjectName = Ad_element->Value();
			// 广告非法
			if(_T("Advertisement_Text") != ObjectName && _T("Advertisement_Picture") != ObjectName && _T("Advertisement_Video") != ObjectName)
			{
				continue;
			}
			// 广告合法
			else
			{
				// 结点为Advertisement_Text--文字广告
				if(_T("Advertisement_Text") == ObjectName)
				{
					Ad_Font_Attribute = Ad_element->FirstAttribute();
					adstruct.adfontstruct.FontPlayNumber = Ad_Font_Attribute->Value();	// 文字广告在文字、图片、视频广告中播放的顺序
					//Ad_Font_Attribute = Ad_Font_Attribute->Next();

					//adstruct.adfontstruct.PlaybackMode = Ad_Font_Attribute->Value();	// 文字广告的播放方式
					for(Text_Object = Ad_element->FirstChildElement();Text_Object != NULL;Text_Object = Text_Object->NextSiblingElement())
					{
						Ad_Font_Attribute = Text_Object->FirstAttribute();
						fontstruct.SerialNo = Ad_Font_Attribute->Value();				// 文字广告中文字对象的播放顺序
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Font = ComUtf8ToUtf16(const_cast<char*>(Ad_Font_Attribute->Value()));					// 文字广告中文字对象的字体
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Size = Ad_Font_Attribute->Value();					// 文字广告中文字对象中字体的大小
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Color = Ad_Font_Attribute->Value();					// 文字广告中文字对象中字体的颜色
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.TimeLength = Ad_Font_Attribute->Value();				// 文字广告中文字对象播放时长
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Content = ComUtf8ToUtf16(const_cast<char*>(Ad_Font_Attribute->Value()));					// 文字广告中文字对象内容
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.DisplayMode = Ad_Font_Attribute->Value();			// 文字广告中文字对象播放方式
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Speed = Ad_Font_Attribute->Value();					// 文字广告中文字对象播放速度
						adstruct.adfontstruct.fontinfo.push_back(fontstruct);
					}
					// 文字对象的下一个对象
					if(Ad_element = Ad_element->NextSiblingElement(),Ad_element != NULL)
					{
						// 图片广告
						if(ObjectName = Ad_element->Value(),_T("Advertisement_Picture") == ObjectName)
						{
							Ad_Picture_Attribute = Ad_element->FirstAttribute();
							adstruct.adpicturestruct.SerialNo = Ad_Picture_Attribute->Value();		// 图片广告播放顺序

							for(Picture_Object = Ad_element->FirstChildElement();Picture_Object != NULL;Picture_Object = Picture_Object->NextSiblingElement())
							{
								Ad_Picture_Attribute = Picture_Object->FirstAttribute();
								picturestruct.SerialNo = Ad_Picture_Attribute->Value();				// 图片广告中图片对象播放顺序
								Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

								picturestruct.TimeLength = Ad_Picture_Attribute->Value();			// 图片广告中图片对象切换时长
								Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

								picturestruct.FilePathName = Ad_Picture_Attribute->Value();			// 图片广告中图片对象位置

								adstruct.adpicturestruct.pictureinfo.push_back(picturestruct);
							}

							// 视频广告对象存在
							if(Ad_element = Ad_element->NextSiblingElement(),Ad_element != NULL)
							{
								// 视频广告
								if(ObjectName = Ad_element->Value(),_T("Advertisement_Video") == ObjectName)
								{
									Ad_Video_Attribute = Ad_element->FirstAttribute();
									adstruct.advideostruct.SerialNo = Ad_Video_Attribute->Value();				// 视频广告播放顺序
									for(Video_Object = Ad_element->FirstChildElement();Video_Object != NULL;Video_Object = Video_Object->NextSiblingElement())
									{
										Ad_Video_Attribute = Video_Object->FirstAttribute();					
										videostruct.SerialNo = Ad_Video_Attribute->Value();						// 视频广告对象播放顺序
										Ad_Video_Attribute = Ad_Video_Attribute->Next();			

										videostruct.FilePathName = Ad_Video_Attribute->Value();					// 视频广告位置
										adstruct.advideostruct.videoinfo.push_back(videostruct);
									}

								}

							}

						}
					}

				}
				// 结点为Advertisement_Picture--图片广告,不含文字广告
				else if(_T("Advertisement_Picture") == ObjectName)
				{
					Ad_Picture_Attribute = Ad_element->FirstAttribute();
					adstruct.adpicturestruct.SerialNo = Ad_Picture_Attribute->Value();		// 图片广告播放顺序

					for(Picture_Object = Ad_element->FirstChildElement();Picture_Object != NULL;Picture_Object = Picture_Object->NextSiblingElement())
					{
						Ad_Picture_Attribute = Picture_Object->FirstAttribute();
						picturestruct.SerialNo = Ad_Picture_Attribute->Value();				// 图片广告中图片对象播放顺序
						Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

						picturestruct.TimeLength = Ad_Picture_Attribute->Value();			// 图片广告中图片对象切换时长
						Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

						picturestruct.FilePathName = Ad_Picture_Attribute->Value();			// 图片广告中图片对象位置

						adstruct.adpicturestruct.pictureinfo.push_back(picturestruct);
					}
					// 视频广告对象存在
					if(Ad_element = Ad_element->NextSiblingElement(),Ad_element != NULL)
					{
						// 视频广告
						if(ObjectName = Ad_element->Value(),_T("Advertisement_Video") == ObjectName)
						{
							Ad_Video_Attribute = Ad_element->FirstAttribute();
							adstruct.advideostruct.SerialNo = Ad_Video_Attribute->Value();				// 视频广告播放顺序
							for(Video_Object = Ad_element->FirstChildElement();Video_Object != NULL;Video_Object = Video_Object->NextSiblingElement())
							{
								Ad_Video_Attribute = Video_Object->FirstAttribute();					
								videostruct.SerialNo = Ad_Video_Attribute->Value();						// 视频广告对象播放顺序
								Ad_Video_Attribute = Ad_Video_Attribute->Next();			

								videostruct.FilePathName = Ad_Video_Attribute->Value();					// 视频广告位置
								adstruct.advideostruct.videoinfo.push_back(videostruct);
							}

						}

					}
				}
				// 结点为Advertisement_Video--视频广告，不含文字、图片广告
				else if(_T("Advertisement_Video") == ObjectName)
				{
					Ad_Video_Attribute = Ad_element->FirstAttribute();
					adstruct.advideostruct.SerialNo = Ad_Video_Attribute->Value();				// 视频广告播放顺序
					for(Video_Object = Ad_element->FirstChildElement();Video_Object != NULL;Video_Object = Video_Object->NextSiblingElement())
					{
						Ad_Video_Attribute = Video_Object->FirstAttribute();					
						videostruct.SerialNo = Ad_Video_Attribute->Value();						// 视频广告对象播放顺序
						Ad_Video_Attribute = Ad_Video_Attribute->Next();			

						videostruct.FilePathName = Ad_Video_Attribute->Value();					// 视频广告位置
						adstruct.advideostruct.videoinfo.push_back(videostruct);
					}
				}
			}

		}
		// 广告个体
		AdvertisementInfo.push_back(adstruct);
		adstruct.adfontstruct.fontinfo.clear();
		adstruct.adpicturestruct.pictureinfo.clear();
		adstruct.advideostruct.videoinfo.clear();
	}
	
	
	return true;
}