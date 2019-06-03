#include "StdAfx.h"
#include "ParseStationMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CParseStationMapXML CParseStationMapXML::theInstance;


static void transparentPng(const TCHAR* tcpath,CImage* pImage)
{
	FILE *pfile = NULL;
	errno_t err;
	err = _tfopen_s(&pfile,tcpath, _T("r"));
	if(err==0)
	{
		byte PNGMARK[8] = { 0x89, 0x50, 0x4E, 0x47};
		byte pngHead[8];
		size_t readCount = fread(pngHead, sizeof(byte), 4, pfile);
		fclose(pfile);
		pfile = NULL;
		if (readCount == 4)
		{
			if (memcmp(pngHead, PNGMARK, 4)==0)
			{
				for (int i = 0; i < pImage->GetWidth(); i++)
				{
					for (int j = 0; j < pImage->GetHeight(); j++)
					{
						unsigned char* pucColor = reinterpret_cast<unsigned char *>(pImage->GetPixelAddress(i, j));
						pucColor[0] = pucColor[0] * pucColor[3] / 255;
						pucColor[1] = pucColor[1] * pucColor[3] / 255;
						pucColor[2] = pucColor[2] * pucColor[3] / 255;
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无
 
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParseStationMapXML::CParseStationMapXML()
{
    m_bInit = ParseXMLFile();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CParseStationMapXML::~CParseStationMapXML()
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
bool CParseStationMapXML::GetNodePointerByName(TiXmlElement* pRootEle,CString strNodeName,TiXmlElement* &destNode)
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

@param     void

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseStationMapXML::ParseXMLFile()
{
	CString appPath;
	GetAppPath(appPath);
	CString sDir = appPath + TVM_GUI_LAYOUT;
	CString sFilePath = sDir + _T("StationMap.xml");
	USES_CONVERSION;
	const char* destFilePath = T2A(sFilePath);

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

	CString Object_name;
	// 版本信息
	Object_name = _T("Version");
	GetNodePointerByName(pRootEle,Object_name,pEle);

	TiXmlAttribute *Ad_Attribute = pEle->FirstAttribute();

	VersionId = Ad_Attribute->Value();		// 版本ID
	Ad_Attribute = Ad_Attribute->Next();

	VersionDate = Ad_Attribute->Value();	// 生成时间
	Ad_Attribute = Ad_Attribute->Next();

	VersionType = Ad_Attribute->Value();	// 类型
	Ad_Attribute = Ad_Attribute->Next();

	//VersionValidate = Ad_Attribute->Value();// 生效时间
	//Ad_Attribute = Ad_Attribute->Next();

	//ValidateType = Ad_Attribute->Value();	// 生效类型
	//Ad_Attribute = Ad_Attribute->Next();

	InValidateTime = Ad_Attribute->Value();	// 失效时间
	Ad_Attribute = Ad_Attribute->Next();

	VersionSystem = Ad_Attribute->Value();	// 版本系统
	Ad_Attribute = Ad_Attribute->Next();

	VersionOper = Ad_Attribute->Value();	// 版本Oper
	Ad_Attribute = Ad_Attribute->Next();

	FormatVersion = Ad_Attribute->Value();	// formatVersion
	Ad_Attribute = Ad_Attribute->Next();


	StationMapInfo.clear();
	// 线路站点信息
	Object_name = _T("ResolutionInfo");
	GetNodePointerByName(pRootEle,Object_name,pEle);
	for(;pEle != NULL;pEle = pEle->NextSiblingElement())
	{
		ResolutionInfo_Object* pResolutionInfo = new ResolutionInfo_Object;
		TiXmlAttribute *Ad_Attribute = pEle->FirstAttribute();

		pResolutionInfo->ResolutionId = Ad_Attribute->Value();	// 分辨率ID
		Ad_Attribute = Ad_Attribute->Next();

		pResolutionInfo->Content = Ad_Attribute->Value();			// 分辨率大小
		Ad_Attribute = Ad_Attribute->Next();

		if (pResolutionInfo->ResolutionId!=_T("05"))// 选择特定分辨率的界面信息
		{
			continue;
		}

		sDir += _T("05\\");
		TiXmlElement* Ad_element = NULL;
		for(Ad_element = pEle->FirstChildElement();Ad_element != NULL;Ad_element = Ad_element->NextSiblingElement())
		{
			CString NodeName = _T("");
			NodeName = Ad_element->Value();

			// xml内容异常
			if(_T("GlobalInfo") != NodeName && _T("LineInfo") != NodeName)
			{
				return false;
			}

			// 总图信息
			if (_T("GlobalInfo") == NodeName)
			{
				Ad_Attribute = Ad_element->FirstAttribute();
				// 总图图片
				pResolutionInfo->GlobalInfo.ImageName = Ad_Attribute->Value();
				Ad_Attribute = Ad_Attribute->Next();
				pResolutionInfo->GlobalInfo.pImage = new CImage;
				HRESULT hr = pResolutionInfo->GlobalInfo.pImage->Load(sDir + pResolutionInfo->GlobalInfo.ImageName);
				if(SUCCEEDED(hr)){
					transparentPng(sDir + pResolutionInfo->GlobalInfo.ImageName, pResolutionInfo->GlobalInfo.pImage);
				}
				else{
					delete pResolutionInfo->GlobalInfo.pImage;
					pResolutionInfo->GlobalInfo.pImage = NULL;
				}
				// 中文总图
				CString strImgNameCN = pResolutionInfo->GlobalInfo.ImageName;
				int pos = strImgNameCN.ReverseFind('.');
				if (pos>0){
					strImgNameCN = strImgNameCN.Left(pos) + _T("_CN") + strImgNameCN.Right(4);
				}
				pResolutionInfo->GlobalInfo.ImageName_CN = sDir + strImgNameCN;	
				pResolutionInfo->GlobalInfo.pImage_CN = new CImage;
				hr = pResolutionInfo->GlobalInfo.pImage_CN->Load(pResolutionInfo->GlobalInfo.ImageName_CN);
				if(SUCCEEDED(hr)){
					transparentPng(sDir + pResolutionInfo->GlobalInfo.ImageName_CN, pResolutionInfo->GlobalInfo.pImage_CN);
				}
				else{
					delete pResolutionInfo->GlobalInfo.pImage_CN;
					pResolutionInfo->GlobalInfo.pImage_CN = NULL;
				}
				// 英文总图
				CString strImgNameEN = pResolutionInfo->GlobalInfo.ImageName;
				pos = strImgNameEN.ReverseFind('.');
				if (pos>0){
					strImgNameEN = strImgNameEN.Left(pos) + _T("_EN") + strImgNameEN.Right(4);
				}
				pResolutionInfo->GlobalInfo.ImageName_EN = sDir + strImgNameEN;	
				pResolutionInfo->GlobalInfo.pImage_EN = new CImage;
				hr = pResolutionInfo->GlobalInfo.pImage_EN->Load(pResolutionInfo->GlobalInfo.ImageName_EN);
				if(SUCCEEDED(hr)){
					transparentPng(sDir + pResolutionInfo->GlobalInfo.ImageName_EN, pResolutionInfo->GlobalInfo.pImage_EN);
				}
				else{
					delete pResolutionInfo->GlobalInfo.pImage_EN;
					pResolutionInfo->GlobalInfo.pImage_EN = NULL;
				}


				pResolutionInfo->GlobalInfo.StationCount = Ad_Attribute->Value();	// 站点数量
				Ad_Attribute = Ad_Attribute->Next();

				pResolutionInfo->GlobalInfo.vec_StationsInfo.clear();
				StationInfoStruct stationInfo;
				TiXmlElement* station_Object = NULL;
				for(station_Object = Ad_element->FirstChildElement();station_Object != NULL;station_Object = station_Object->NextSiblingElement())
				{
					Ad_Attribute = station_Object->FirstAttribute();
					stationInfo.StationId = Ad_Attribute->Value();				// 站点ID
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.IsUse = Ad_Attribute->Value();					// 是否启用
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.Pos_x = Ad_Attribute->Value();					// 左上角x坐标
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.Pos_y = Ad_Attribute->Value();					// 左上角y坐标
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.Width = Ad_Attribute->Value();					// 宽度
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.Height = Ad_Attribute->Value();					// 高度
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.Rx = Ad_Attribute->Value();						// 圆角矩形x
					Ad_Attribute = Ad_Attribute->Next();

					stationInfo.Ry = Ad_Attribute->Value();						// 圆角矩形y
					Ad_Attribute = Ad_Attribute->Next();

					pResolutionInfo->GlobalInfo.vec_StationsInfo.push_back(stationInfo);
				}
			}

			// 线路信息
			if (_T("LineInfo") == NodeName)
			{
				pResolutionInfo->LinesInfo.map_LinesInfo.clear();
				SigleLineInfo sigleLineInfo;
				TiXmlElement* map_Object = NULL;
				for(map_Object = Ad_element->FirstChildElement();map_Object != NULL;map_Object = map_Object->NextSiblingElement())
				{
					CString nodeName;
					nodeName = map_Object->Value();
					if (nodeName!=_T("MapInfo"))
					{
						return false;
					}

					Ad_Attribute = map_Object->FirstAttribute();
					sigleLineInfo.LineId = Ad_Attribute->Value();				// 线路ID
					Ad_Attribute = Ad_Attribute->Next();

					sigleLineInfo.ImageName = Ad_Attribute->Value();
					Ad_Attribute = Ad_Attribute->Next();
					sigleLineInfo.pImage = new CImage;
					HRESULT hr = sigleLineInfo.pImage->Load(sDir + sigleLineInfo.ImageName);
					if(SUCCEEDED(hr)){
						transparentPng(sDir + sigleLineInfo.ImageName, sigleLineInfo.pImage);
					}
					else{
						delete sigleLineInfo.pImage;
						sigleLineInfo.pImage = NULL;
					}
					// 中文总图
					CString strImgNameCN = sigleLineInfo.ImageName;
					int pos = strImgNameCN.ReverseFind('.');
					if (pos>0){
						strImgNameCN = strImgNameCN.Left(pos) + _T("_CN") + strImgNameCN.Right(4);
					}
					sigleLineInfo.ImageName_CN = sDir + strImgNameCN;	
					sigleLineInfo.pImage_CN = new CImage;
					hr = sigleLineInfo.pImage_CN->Load(sigleLineInfo.ImageName_CN);
					if(SUCCEEDED(hr)){
						transparentPng(sDir + sigleLineInfo.ImageName_CN, sigleLineInfo.pImage_CN);
					}
					else{
						delete sigleLineInfo.pImage_CN;
						sigleLineInfo.pImage_CN = NULL;
					}
					// 英文总图
					CString strImgNameEN = sigleLineInfo.ImageName;
					pos = strImgNameEN.ReverseFind('.');
					if (pos>0){
						strImgNameEN = strImgNameEN.Left(pos) + _T("_EN") + strImgNameEN.Right(4);
					}
					sigleLineInfo.ImageName_EN = sDir + strImgNameEN;	
					sigleLineInfo.pImage_EN = new CImage;
					hr = sigleLineInfo.pImage_EN->Load(sigleLineInfo.ImageName_EN);
					if(SUCCEEDED(hr)){
						transparentPng(sDir + sigleLineInfo.ImageName_EN, sigleLineInfo.pImage_EN);
					}
					else{
						delete sigleLineInfo.pImage_EN;
						sigleLineInfo.pImage_EN = NULL;
					}

					sigleLineInfo.IsUse = Ad_Attribute->Value();				// 是否启用
					Ad_Attribute = Ad_Attribute->Next();

					sigleLineInfo.StationCount = Ad_Attribute->Value();			// 站点数量
					Ad_Attribute = Ad_Attribute->Next();

					sigleLineInfo.vec_StationsInfo.clear();
					StationInfoStruct stationInfo;
					TiXmlElement* station_Object = NULL;
					for(station_Object = map_Object->FirstChildElement();station_Object != NULL;station_Object = station_Object->NextSiblingElement())
					{
						Ad_Attribute = station_Object->FirstAttribute();
						stationInfo.StationId = Ad_Attribute->Value();				// 站点ID
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.IsUse = Ad_Attribute->Value();					// 是否启用
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.Pos_x = Ad_Attribute->Value();					// 左上角x坐标
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.Pos_y = Ad_Attribute->Value();					// 左上角y坐标
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.Width = Ad_Attribute->Value();					// 宽度
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.Height = Ad_Attribute->Value();					// 高度
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.Rx = Ad_Attribute->Value();						// 圆角矩形x
						Ad_Attribute = Ad_Attribute->Next();

						stationInfo.Ry = Ad_Attribute->Value();						// 圆角矩形y
						Ad_Attribute = Ad_Attribute->Next();

						sigleLineInfo.vec_StationsInfo.push_back(stationInfo);
					}
					pResolutionInfo->LinesInfo.map_LinesInfo[sigleLineInfo.LineId] = sigleLineInfo;
				}
			}
		}
		StationMapInfo.push_back(pResolutionInfo);
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     获取总图图片名称

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParseStationMapXML::GetGlobalImageName()
{
	if(!m_bInit || StationMapInfo.size()==0){
		return _T("");
	}

	return StationMapInfo[0]->GlobalInfo.ImageName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     获取线路图片名称

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CParseStationMapXML::GetLineImageName(int LineId)
{
	CString img;
	if(!m_bInit || StationMapInfo.size()==0){
		return _T("");
	}

	CString strLineId;
	strLineId.Format(_T("%d"), LineId);
	map<CString,SigleLineInfo> &LinesInfo = StationMapInfo[0]->LinesInfo.map_LinesInfo;
	if(LinesInfo.find(strLineId)!=LinesInfo.end()){
		img = LinesInfo[strLineId].ImageName;
	}

	return img;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     获取总图信息

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseStationMapXML::GetGlobalStationInfo(GlobalInfoStruct& lineInfo)
{
	if(!m_bInit || StationMapInfo.size()==0){
		return false;
	}

	lineInfo = StationMapInfo[0]->GlobalInfo;

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     获取线路信息

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseStationMapXML::GetLineStationInfo(int LineId, SigleLineInfo& lineInfo)
{
	bool bFind = false;
	if(!m_bInit || StationMapInfo.size()==0){
		return false;
	}

	CString strLineId;
	strLineId.Format(_T("%02d"), LineId);
	map<CString,SigleLineInfo> &LinesInfo = StationMapInfo[0]->LinesInfo.map_LinesInfo;
	if(LinesInfo.find(strLineId)!=LinesInfo.end()){
		lineInfo = LinesInfo[strLineId];
		bFind = true;
	}

	return bFind;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     根据坐标查找匹配的站点

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseStationMapXML::FindMatchStation(int LineId, const CPoint& pos, StationInfoStruct stationInfo)
{
	if(!m_bInit || StationMapInfo.size()==0){
		return false;
	}

	CString strLineId;
	strLineId.Format(_T("%d"), LineId);


	map<CString,SigleLineInfo> &LinesInfo = StationMapInfo[0]->LinesInfo.map_LinesInfo;
	if(LinesInfo.find(strLineId)!=LinesInfo.end()){
		vector<StationInfoStruct>& vec_StationsInfo = LinesInfo[strLineId].vec_StationsInfo;
		vector<StationInfoStruct>::iterator it = vec_StationsInfo.begin();
		vector<StationInfoStruct>::iterator ie = vec_StationsInfo.end();
		for(;it!=ie;it++){
			CRect rect(_ttoi(it->Pos_x), _ttoi(it->Pos_y), _ttoi(it->Width), _ttoi(it->Height));
			if (PtInRect(&rect, pos)){
				stationInfo = *it;
				return true;
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     获取可用的线路ID

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseStationMapXML::GetLineIds(vector<int>& vec_lineId)
{
	if(!m_bInit || StationMapInfo.size()==0){
		return false;
	}

	map<CString,SigleLineInfo> &LinesInfo = StationMapInfo[0]->LinesInfo.map_LinesInfo;
	map<CString,SigleLineInfo>::iterator it = LinesInfo.begin();
	while(it!=LinesInfo.end()){
		//if(it->second.IsUse = _T("1")){
			vec_lineId.push_back(_ttoi(it->second.LineId));
		//}
		it++;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     获取指定的站点信息

@param     void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CParseStationMapXML::GetOneStationInfo(int StationId, StationInfoStruct stationInfo)
{
	if(!m_bInit || StationMapInfo.size()==0){
		return false;
	}

	CString strLineId;
	strLineId.Format(_T("%d"), StationId/100);


	map<CString,SigleLineInfo> &LinesInfo = StationMapInfo[0]->LinesInfo.map_LinesInfo;
	if(LinesInfo.find(strLineId)!=LinesInfo.end()){
		vector<StationInfoStruct>& vec_StationsInfo = LinesInfo[strLineId].vec_StationsInfo;
		vector<StationInfoStruct>::iterator it = vec_StationsInfo.begin();
		vector<StationInfoStruct>::iterator ie = vec_StationsInfo.end();
		for(;it!=ie;it++){
			if (strLineId==it->StationId){
				stationInfo = *it;
				return true;
			}
		}
	}

	return false;
}