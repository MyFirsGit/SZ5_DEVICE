#include "tinystr.h" 
#include "tinyxml.h"
#include "Windows.h"

#define theSTATION_MAP_XML	CParseStationMapXML::GetInstance()			///< CParseStationMapXML的唯一接口

/**
@brief 解析XML文件
*/

class CParseStationMapXML
{
public:
	// 站点信息
	typedef struct _stationInfoStruct
	{	
		CString StationId;							// 站点ID
		CString IsUse;								// 是否启用
		CString Pos_x;								// 左上角x坐标
		CString Pos_y;								// 左上角y坐标
		CString Width;								// 宽度
		CString Height;								// 高度
		CString Rx;									// 圆角矩形x
		CString Ry;									// 圆角矩形y

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

	// 总图信息
	typedef struct _globalInfoStruct
	{	 
		CString ImageName;							// 总图名称
		CImage* pImage;								// 总图指针
		CString ImageName_CN;						// 中文总图名称
		CImage* pImage_CN;							// 中文总图指针
		CString ImageName_EN;						// 英文总图名称
		CImage* pImage_EN;							// 英文总图指针
		CString StationCount;						// 站点数量
		vector<StationInfoStruct> vec_StationsInfo;	// 站点信息

		_globalInfoStruct()
		{
			ImageName		= _T("");
			StationCount	= _T("");
			vec_StationsInfo.clear();
		}
	}GlobalInfoStruct;

	// 线路信息
	struct SigleLineInfo{
		CString LineId;							// 线路ID
		CString ImageName;						// 线路图名称
		CImage* pImage;							// 线路图指针
		CString ImageName_CN;					// 中文线路图名称
		CImage* pImage_CN;						// 中文线路图指针
		CString ImageName_EN;					// 英文线路图名称
		CImage* pImage_EN;						// 英文线路图指针
		CString IsUse;							// 是否启用
		CString StationCount;					// 站点数量
		vector<StationInfoStruct> vec_StationsInfo;	// 站点信息

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
		map<CString,SigleLineInfo> map_LinesInfo;		// 线路信息

		_linesInfoStruct()
		{
			map_LinesInfo.clear();
		}
	}LinesInfoStruct;

	// 某种分辨率的界面信息
	typedef struct _resolutionInfo_Object
	{
		CString ResolutionId;						// 分辨率ID
		CString Content;							// 分辨率大小
		GlobalInfoStruct GlobalInfo;				// 总图信息
		LinesInfoStruct LinesInfo;					// 线路信息

		_resolutionInfo_Object()
		{
			ResolutionId	= _T("");
			Content			= _T("");
		}
	}ResolutionInfo_Object;

public:
	CString GetGlobalImageName();					// 获取总图图片名称
	CString GetLineImageName(int LineId);			// 获取线路图片名称
	bool GetGlobalStationInfo(GlobalInfoStruct& lineInfo);		// 获取总图信息
	bool GetLineStationInfo(int LineId, SigleLineInfo& lineInfo);		// 获取线路信息
	bool FindMatchStation(int LineId, const CPoint& pos, StationInfoStruct stationInfo);	// 根据坐标查找匹配的站点
	bool GetLineIds(vector<int>& vec_lineId);	// 获取线路ID

	bool GetOneStationInfo(int StationId, StationInfoStruct stationInfo);		// 获取指定的站点信息

	static CParseStationMapXML &GetInstance()
	{
		return theInstance;
	}

public:
	bool m_bInit;
	CString VersionId;				// 版本ID
	CString VersionDate;			// 生成时间
	CString VersionType;			// 类型
	CString VersionValidate;		// 生效时间
	CString ValidateType;			// 生效类型
	CString InValidateTime;			// 失效时间
	CString VersionSystem;			// 版本系统
	CString VersionOper;			// 版本Oper
	CString FormatVersion;			// ...
	vector<ResolutionInfo_Object*> StationMapInfo;		// 线路站点信息

	static CParseStationMapXML theInstance;

	// 构造函数私有
	CParseStationMapXML();
	~CParseStationMapXML();
	CParseStationMapXML(const CParseStationMapXML&);
	CParseStationMapXML &operator=(const CParseStationMapXML&);

	bool GetNodePointerByName(TiXmlElement* pRootEle/*根元素*/,CString strNodeName/*结点名*/,TiXmlElement* &destNode/*结点指针*/);		// 根据结点名获取结点指针
	bool ParseXMLFile();
};