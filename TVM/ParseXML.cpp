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
@brief      ���캯��

@param      ��
 
@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CParseXML::CParseXML()
{
    
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CParseXML::~CParseXML()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ݽ������ȡ���ָ��

@param      TiXmlElement* pRootEle  ��Ԫ��

@param      CString strNodeName �����

@param      TiXmlElement* &destNode ���ָ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CParseXML::GetNodePointerByName(TiXmlElement* pRootEle,CString strNodeName,TiXmlElement* &destNode)
{
	// ������ڸ���������˳�
	if(strNodeName == pRootEle->Value())
	{
		destNode = pRootEle;
		return true;
	}

	TiXmlElement* pEle = pRootEle;
	for(pEle = pRootEle->FirstChildElement(); pEle; pEle = pEle->NextSiblingElement())
	{
		// �ݹ鴦���ӽ�㣬��ȡ���ָ��
		if(GetNodePointerByName(pEle,strNodeName,destNode))
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ȡXML�ļ�

@param      CString XMLFileName		�ļ���

@param      CString strNodeName		�����

@param      CString strText			����

@retval     ��

@exception  ��
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

	TiXmlElement* RootElement = doc->RootElement();		// ��Ԫ��
	if(NULL == RootElement)
	{
		return false;
	}
	TiXmlElement* pEle = NULL;

	// �ҵ�ֵΪstrNodeName�Ľ��
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
@brief     ��ȡXML�ļ�

@param      CString XMLFileName		�ļ���

@param      CString strNodeName		�����

@param      AdStruct& adinfo		�����Ϣ

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CParseXML::QueryElementValue(CString XMLFileName,CString strNodeName,vector<AdStruct>& AdvertisementInfo)
{
	AdStruct adstruct;							// �����Ϣ
	TiXmlElement* Advertisement_Object = NULL;	

	TiXmlAttribute* Ad_Font_Attribute = NULL;	// ��������
	FontStruct fontstruct;						// ������Ϣ�ṹ��
	TiXmlElement* Text_Object = NULL;			// �������-Text_Object

	TiXmlAttribute *Ad_Picture_Attribute = NULL; // ͼƬ����
	PictureStruct picturestruct;				 // ͼƬ��Ϣ�ṹ��
	TiXmlElement* Picture_Object = NULL;		 // ͼƬ����-Picture_Object

	TiXmlAttribute *Ad_Video_Attribute = NULL;	 // ��Ƶ����
	VideoStruct videostruct;					 // ��Ƶ��Ϣ�ṹ��
	TiXmlElement* Video_Object = NULL;			 // ��Ƶ����-Video_Object

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
	// �����
	TiXmlElement* pRootEle = doc->RootElement();
	if(NULL == pRootEle)
	{
		return false;
	}
	TiXmlElement* pEle = NULL;
	// ������
	CString Ad_Object = _T("Advertisement_Object");
	GetNodePointerByName(pRootEle,Ad_Object,pEle);
	for(;pEle != NULL;pEle = pEle->NextSiblingElement())
	{
		TiXmlAttribute *Ad_Attribute = pEle->FirstAttribute();

		adstruct.advertisementobject.AdvertisementName = Ad_Attribute->Value();		// �����
		Ad_Attribute = Ad_Attribute->Next();

		adstruct.advertisementobject.SerialNo = Ad_Attribute->Value();				// ��沥��˳��
		Ad_Attribute = Ad_Attribute->Next();

		adstruct.advertisementobject.ValidateTime = Ad_Attribute->Value();			// ���ʧЧʱ��
		Ad_Attribute = Ad_Attribute->Next();

		_DATE_TIME currentTime = ComGetCurTime();									// ϵͳ��ǰʱ��
		CString currentTimeToint = currentTime.ToString(_T("%04d%02d%02d%02d%02d%02d"));
		// ���ʧЧ
		if(adstruct.advertisementobject.ValidateTime < currentTimeToint)
		{
			continue;
		}
		// ���δʧЧ
		else
		{
			adstruct.advertisementobject.ResourcePath = Ad_Attribute->Value();		// ���λ��
			Ad_Attribute = Ad_Attribute->Next();
			TiXmlElement* Ad_element = pEle->FirstChildElement();
			CString ObjectName = _T("");
			ObjectName = Ad_element->Value();
			// ���Ƿ�
			if(_T("Advertisement_Text") != ObjectName && _T("Advertisement_Picture") != ObjectName && _T("Advertisement_Video") != ObjectName)
			{
				continue;
			}
			// ���Ϸ�
			else
			{
				// ���ΪAdvertisement_Text--���ֹ��
				if(_T("Advertisement_Text") == ObjectName)
				{
					Ad_Font_Attribute = Ad_element->FirstAttribute();
					adstruct.adfontstruct.FontPlayNumber = Ad_Font_Attribute->Value();	// ���ֹ�������֡�ͼƬ����Ƶ����в��ŵ�˳��
					//Ad_Font_Attribute = Ad_Font_Attribute->Next();

					//adstruct.adfontstruct.PlaybackMode = Ad_Font_Attribute->Value();	// ���ֹ��Ĳ��ŷ�ʽ
					for(Text_Object = Ad_element->FirstChildElement();Text_Object != NULL;Text_Object = Text_Object->NextSiblingElement())
					{
						Ad_Font_Attribute = Text_Object->FirstAttribute();
						fontstruct.SerialNo = Ad_Font_Attribute->Value();				// ���ֹ�������ֶ���Ĳ���˳��
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Font = ComUtf8ToUtf16(const_cast<char*>(Ad_Font_Attribute->Value()));					// ���ֹ�������ֶ��������
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Size = Ad_Font_Attribute->Value();					// ���ֹ�������ֶ���������Ĵ�С
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Color = Ad_Font_Attribute->Value();					// ���ֹ�������ֶ������������ɫ
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.TimeLength = Ad_Font_Attribute->Value();				// ���ֹ�������ֶ��󲥷�ʱ��
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Content = ComUtf8ToUtf16(const_cast<char*>(Ad_Font_Attribute->Value()));					// ���ֹ�������ֶ�������
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.DisplayMode = Ad_Font_Attribute->Value();			// ���ֹ�������ֶ��󲥷ŷ�ʽ
						Ad_Font_Attribute = Ad_Font_Attribute->Next();

						fontstruct.Speed = Ad_Font_Attribute->Value();					// ���ֹ�������ֶ��󲥷��ٶ�
						adstruct.adfontstruct.fontinfo.push_back(fontstruct);
					}
					// ���ֶ������һ������
					if(Ad_element = Ad_element->NextSiblingElement(),Ad_element != NULL)
					{
						// ͼƬ���
						if(ObjectName = Ad_element->Value(),_T("Advertisement_Picture") == ObjectName)
						{
							Ad_Picture_Attribute = Ad_element->FirstAttribute();
							adstruct.adpicturestruct.SerialNo = Ad_Picture_Attribute->Value();		// ͼƬ��沥��˳��

							for(Picture_Object = Ad_element->FirstChildElement();Picture_Object != NULL;Picture_Object = Picture_Object->NextSiblingElement())
							{
								Ad_Picture_Attribute = Picture_Object->FirstAttribute();
								picturestruct.SerialNo = Ad_Picture_Attribute->Value();				// ͼƬ�����ͼƬ���󲥷�˳��
								Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

								picturestruct.TimeLength = Ad_Picture_Attribute->Value();			// ͼƬ�����ͼƬ�����л�ʱ��
								Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

								picturestruct.FilePathName = Ad_Picture_Attribute->Value();			// ͼƬ�����ͼƬ����λ��

								adstruct.adpicturestruct.pictureinfo.push_back(picturestruct);
							}

							// ��Ƶ���������
							if(Ad_element = Ad_element->NextSiblingElement(),Ad_element != NULL)
							{
								// ��Ƶ���
								if(ObjectName = Ad_element->Value(),_T("Advertisement_Video") == ObjectName)
								{
									Ad_Video_Attribute = Ad_element->FirstAttribute();
									adstruct.advideostruct.SerialNo = Ad_Video_Attribute->Value();				// ��Ƶ��沥��˳��
									for(Video_Object = Ad_element->FirstChildElement();Video_Object != NULL;Video_Object = Video_Object->NextSiblingElement())
									{
										Ad_Video_Attribute = Video_Object->FirstAttribute();					
										videostruct.SerialNo = Ad_Video_Attribute->Value();						// ��Ƶ�����󲥷�˳��
										Ad_Video_Attribute = Ad_Video_Attribute->Next();			

										videostruct.FilePathName = Ad_Video_Attribute->Value();					// ��Ƶ���λ��
										adstruct.advideostruct.videoinfo.push_back(videostruct);
									}

								}

							}

						}
					}

				}
				// ���ΪAdvertisement_Picture--ͼƬ���,�������ֹ��
				else if(_T("Advertisement_Picture") == ObjectName)
				{
					Ad_Picture_Attribute = Ad_element->FirstAttribute();
					adstruct.adpicturestruct.SerialNo = Ad_Picture_Attribute->Value();		// ͼƬ��沥��˳��

					for(Picture_Object = Ad_element->FirstChildElement();Picture_Object != NULL;Picture_Object = Picture_Object->NextSiblingElement())
					{
						Ad_Picture_Attribute = Picture_Object->FirstAttribute();
						picturestruct.SerialNo = Ad_Picture_Attribute->Value();				// ͼƬ�����ͼƬ���󲥷�˳��
						Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

						picturestruct.TimeLength = Ad_Picture_Attribute->Value();			// ͼƬ�����ͼƬ�����л�ʱ��
						Ad_Picture_Attribute = Ad_Picture_Attribute->Next();

						picturestruct.FilePathName = Ad_Picture_Attribute->Value();			// ͼƬ�����ͼƬ����λ��

						adstruct.adpicturestruct.pictureinfo.push_back(picturestruct);
					}
					// ��Ƶ���������
					if(Ad_element = Ad_element->NextSiblingElement(),Ad_element != NULL)
					{
						// ��Ƶ���
						if(ObjectName = Ad_element->Value(),_T("Advertisement_Video") == ObjectName)
						{
							Ad_Video_Attribute = Ad_element->FirstAttribute();
							adstruct.advideostruct.SerialNo = Ad_Video_Attribute->Value();				// ��Ƶ��沥��˳��
							for(Video_Object = Ad_element->FirstChildElement();Video_Object != NULL;Video_Object = Video_Object->NextSiblingElement())
							{
								Ad_Video_Attribute = Video_Object->FirstAttribute();					
								videostruct.SerialNo = Ad_Video_Attribute->Value();						// ��Ƶ�����󲥷�˳��
								Ad_Video_Attribute = Ad_Video_Attribute->Next();			

								videostruct.FilePathName = Ad_Video_Attribute->Value();					// ��Ƶ���λ��
								adstruct.advideostruct.videoinfo.push_back(videostruct);
							}

						}

					}
				}
				// ���ΪAdvertisement_Video--��Ƶ��棬�������֡�ͼƬ���
				else if(_T("Advertisement_Video") == ObjectName)
				{
					Ad_Video_Attribute = Ad_element->FirstAttribute();
					adstruct.advideostruct.SerialNo = Ad_Video_Attribute->Value();				// ��Ƶ��沥��˳��
					for(Video_Object = Ad_element->FirstChildElement();Video_Object != NULL;Video_Object = Video_Object->NextSiblingElement())
					{
						Ad_Video_Attribute = Video_Object->FirstAttribute();					
						videostruct.SerialNo = Ad_Video_Attribute->Value();						// ��Ƶ�����󲥷�˳��
						Ad_Video_Attribute = Ad_Video_Attribute->Next();			

						videostruct.FilePathName = Ad_Video_Attribute->Value();					// ��Ƶ���λ��
						adstruct.advideostruct.videoinfo.push_back(videostruct);
					}
				}
			}

		}
		// ������
		AdvertisementInfo.push_back(adstruct);
		adstruct.adfontstruct.fontinfo.clear();
		adstruct.adpicturestruct.pictureinfo.clear();
		adstruct.advideostruct.videoinfo.clear();
	}
	
	
	return true;
}