#include "tinystr.h" 
#include "tinyxml.h"
#include "Windows.h"

#define theSTATION_MAP_XML	CParseStationMapXML::GetInstance()			///< CParseStationMapXML��Ψһ�ӿ�

/**
@brief ����XML�ļ�
*/

class CParseStationMapXML
{
public:
	// վ����Ϣ
	typedef struct _stationInfoStruct
	{	
		CString StationId;							// վ��ID
		CString IsUse;								// �Ƿ�����
		CString Pos_x;								// ���Ͻ�x����
		CString Pos_y;								// ���Ͻ�y����
		CString Width;								// ���
		CString Height;								// �߶�
		CString Rx;									// Բ�Ǿ���x
		CString Ry;									// Բ�Ǿ���y

		_stationInfoStruct()
		{
			StationId		= _T("");
			IsUse			= _T("");
			Pos_x			= _T("");
			Pos_y			= _T("");
			Width			= _T("");
			Height			= _T("");
			Rx				= _T("");
			Ry				= _T("");
		}
	}StationInfoStruct;

	// ��ͼ��Ϣ
	typedef struct _globalInfoStruct
	{	 
		CString ImageName;							// ��ͼ����
		CImage* pImage;								// ��ͼָ��
		CString ImageName_CN;						// ������ͼ����
		CImage* pImage_CN;							// ������ͼָ��
		CString ImageName_EN;						// Ӣ����ͼ����
		CImage* pImage_EN;							// Ӣ����ͼָ��
		CString StationCount;						// վ������
		vector<StationInfoStruct> vec_StationsInfo;	// վ����Ϣ

		_globalInfoStruct()
		{
			ImageName		= _T("");
			StationCount	= _T("");
			vec_StationsInfo.clear();
		}
	}GlobalInfoStruct;

	// ��·��Ϣ
	struct SigleLineInfo{
		CString LineId;							// ��·ID
		CString ImageName;						// ��·ͼ����
		CImage* pImage;							// ��·ͼָ��
		CString ImageName_CN;					// ������·ͼ����
		CImage* pImage_CN;						// ������·ͼָ��
		CString ImageName_EN;					// Ӣ����·ͼ����
		CImage* pImage_EN;						// Ӣ����·ͼָ��
		CString IsUse;							// �Ƿ�����
		CString StationCount;					// վ������
		vector<StationInfoStruct> vec_StationsInfo;	// վ����Ϣ

		SigleLineInfo(){
			LineId		= _T("");
			ImageName	= _T("");
			IsUse		= _T("");
			StationCount= _T("");
			vec_StationsInfo.clear();
		}
	};

	typedef struct _linesInfoStruct
	{	 
		map<CString,SigleLineInfo> map_LinesInfo;		// ��·��Ϣ

		_linesInfoStruct()
		{
			map_LinesInfo.clear();
		}
	}LinesInfoStruct;

	// ĳ�ֱַ��ʵĽ�����Ϣ
	typedef struct _resolutionInfo_Object
	{
		CString ResolutionId;						// �ֱ���ID
		CString Content;							// �ֱ��ʴ�С
		GlobalInfoStruct GlobalInfo;				// ��ͼ��Ϣ
		LinesInfoStruct LinesInfo;					// ��·��Ϣ

		_resolutionInfo_Object()
		{
			ResolutionId	= _T("");
			Content			= _T("");
		}
	}ResolutionInfo_Object;

public:
	CString GetGlobalImageName();					// ��ȡ��ͼͼƬ����
	CString GetLineImageName(int LineId);			// ��ȡ��·ͼƬ����
	bool GetGlobalStationInfo(GlobalInfoStruct& lineInfo);		// ��ȡ��ͼ��Ϣ
	bool GetLineStationInfo(int LineId, SigleLineInfo& lineInfo);		// ��ȡ��·��Ϣ
	bool FindMatchStation(int LineId, const CPoint& pos, StationInfoStruct stationInfo);	// �����������ƥ���վ��
	bool GetLineIds(vector<int>& vec_lineId);	// ��ȡ��·ID

	bool GetOneStationInfo(int StationId, StationInfoStruct stationInfo);		// ��ȡָ����վ����Ϣ

	static CParseStationMapXML &GetInstance()
	{
		return theInstance;
	}

public:
	bool m_bInit;
	CString VersionId;				// �汾ID
	CString VersionDate;			// ����ʱ��
	CString VersionType;			// ����
	CString VersionValidate;		// ��Чʱ��
	CString ValidateType;			// ��Ч����
	CString InValidateTime;			// ʧЧʱ��
	CString VersionSystem;			// �汾ϵͳ
	CString VersionOper;			// �汾Oper
	CString FormatVersion;			// ...
	vector<ResolutionInfo_Object*> StationMapInfo;		// ��·վ����Ϣ

	static CParseStationMapXML theInstance;

	// ���캯��˽��
	CParseStationMapXML();
	~CParseStationMapXML();
	CParseStationMapXML(const CParseStationMapXML&);
	CParseStationMapXML &operator=(const CParseStationMapXML&);

	bool GetNodePointerByName(TiXmlElement* pRootEle/*��Ԫ��*/,CString strNodeName/*�����*/,TiXmlElement* &destNode/*���ָ��*/);		// ���ݽ������ȡ���ָ��
	bool ParseXMLFile();
};