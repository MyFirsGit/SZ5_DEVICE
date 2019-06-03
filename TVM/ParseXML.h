#include "tinystr.h" 
#include "tinyxml.h"
#include <map>

#define thePARSE_XML	CParseXML::GetInstance()			///< CParseXML��Ψһ�ӿ�

/**
@brief ����XML�ļ�
*/

class CParseXML
{
public:
	typedef struct _Advertisement_Object
	{
		CString AdvertisementName;	// �����
		CString SerialNo;			// ��沥��˳���
		CString ValidateTime;		// ʧЧʱ��
		CString ResourcePath;		// �ļ�λ��
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
		CString SerialNo;		// ���ֲ���˳��
		CString Font;			// ����
		CString Size;			// ��С
		CString Color;			// ��ɫ
		CString TimeLength;		// ����ʱ��
		CString Content;		// ��������
		CString DisplayMode;	// ���ŷ�ʽ
		CString Speed;			// �����ٶ�
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
		CString FontPlayNumber;			// ���ֲ���˳���
		vector<FontStruct> fontinfo;	// ������Ϣ
		_AdfontStruct()
		{
			FontPlayNumber = _T("");
		}
	}AdFontStruct;

	typedef struct _pictureStruct
	{
		CString SerialNo;				// ͼƬ�����󲥷�˳��
		CString TimeLength;				// ͼƬ��沥��ʱ��
		CString FilePathName;			// ͼƬλ��
		_pictureStruct()
		{
			SerialNo = _T("");
			TimeLength = _T("");
			FilePathName = _T("");
		}
	}PictureStruct;

	typedef struct _AdpictureStruct
	{
		CString SerialNo;		// ͼƬ��沥��˳��
		vector<PictureStruct> pictureinfo;
		_AdpictureStruct()
		{
			SerialNo = _T("");
		}
	}AdPictureStruct;

	typedef struct _videoStruct
	{
		CString	SerialNo;				// ��Ƶ�����󲥷�˳��
		CString FilePathName;			// ��Ƶλ��
		_videoStruct()
		{
			SerialNo = _T("");
			FilePathName = _T("");
		}
	}VideoStruct;

	typedef struct _AdvideoStruct
	{
		CString	SerialNo;				// ��Ƶ��沥��˳��
		vector<VideoStruct> videoinfo;
		_AdvideoStruct()
		{
			SerialNo = _T("");
		}
	}AdvideoStruct;

	typedef struct _adStruct
	{
		Advertisement_Object advertisementobject;			// �������
		AdFontStruct adfontstruct;							// ���ֹ��
		AdPictureStruct adpicturestruct;					// ͼƬ���
		AdvideoStruct advideostruct;						// ��Ƶ���
	}AdStruct;

	vector<AdStruct>AdvertisementInfo;						// �����Ϣ
public:
	bool GetNodePointerByName(TiXmlElement* pRootEle/*��Ԫ��*/,CString strNodeName/*�����*/,TiXmlElement* &destNode/*���ָ��*/);		// ���ݽ������ȡ���ָ��
	bool QueryNodeText(CString XMLFileName,CString strNodeName,CString &strText);						// ��ȡXML�ļ�
	bool QueryElementValue(CString XMLFileName,CString strNodeName,vector<AdStruct>& AdvertisementInfo);		// ��ȡXML�ļ�

	static CParseXML &GetInstance()
	{
		return theInstance;
	}
private:
	AdStruct adstruct;
	static CParseXML theInstance;
	
	// ���캯��˽��
	CParseXML();
	~CParseXML();
	CParseXML(const CParseXML&);
	CParseXML &operator=(const CParseXML&);
};