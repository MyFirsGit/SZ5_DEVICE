#include "StdAfx.h"
#include "CAccLineInfoParam.h"
#include "LogException.h"
#include "BomInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccLineInfoParam CAccLineInfoParam::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccLineInfoParam::CAccLineInfoParam():CAccParamMaster()
{
	m_vec_lineInfoList_param.clear();
	m_vec_stationInfoList_param.clear();
	m_vec_exchangeInfoList_param.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取参数文件名

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAccLineInfoParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0301);		// 获取参数文件名
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAccLineInfoParam::~CAccLineInfoParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出当前文件的内容

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccLineInfoParam::ParseBody(LPBYTE lpData)
{
	try{		
		// 车站信息
		m_vec_stationInfoList_param.clear();
		m_map_lineInfoList_param.clear();
		m_vec_lineInfoList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			 
			// ACC定义的车站信息循环体数量
			DWORD stationInfoList_len = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));;
			lpData += 4;
			for(int i = 0;i<stationInfoList_len;i++){
				STATION_INFO  tmpStationInfo;
				// 车站代码
				tmpStationInfo.stationNum = ComMakeWORD(*lpData, *(lpData + 1));
				lpData += 2;
				// 车站中文名称
				char lang_name_cn[33] = {0};
				memcpy(lang_name_cn, lpData, 32);
				tmpStationInfo.station_Chinese_Name = lang_name_cn;
				tmpStationInfo.station_Chinese_Name.Trim();				
				lpData += 32;  
				// 车站英文名称
				char lang_name_en[33] = {0};
				memset(lang_name_en,0,32);
				memcpy(lang_name_en, lpData, 32);
				tmpStationInfo.station_Eng_Name = lang_name_en;
				tmpStationInfo.station_Eng_Name.Trim();
				lpData +=32;
				//计费站点编码
				tmpStationInfo.fareStationNum =ComMakeWORD(*lpData, *(lpData + 1));
				lpData += 2;
				//是否换乘站
				tmpStationInfo.isExchangeStation = BCD2int((char*)lpData, 1);
				lpData ++;
				m_vec_stationInfoList_param.push_back(tmpStationInfo);
			}
		}
		SetLinInfo();
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读出分段信息

@param      (i)LPBYTE lpData       文件内容

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccLineInfoParam::ParseSectionInfo(LPBYTE lpData)
{
	try{
		if(m_packageHead.sectionCount != 0){			
			for(int i =0;i<m_packageHead.sectionCount;i++){
				m_section[i].startPosition = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
				m_section[i].recordCount = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));
				lpData+=4;
			}
		}
	}
	// 异常捕获
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查电文结构调整部分内容正确性

@param      (i)LPBYTE           电文信息

@retval     bool   true:正确  false：不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAccLineInfoParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      线路信息参数

@param      none

@retval     vector<LINE_INFO_LIST>   线路信息参数向量数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::LINE_INFO>& CAccLineInfoParam::GetLinInfo()				
{
	return m_vec_lineInfoList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      线路信息参数

@param      none

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAccLineInfoParam::SetLinInfo()				
{
	vector<CAccLineInfoParam::STATION_INFO>::iterator  it;

	//for (it= m_vec_stationInfoList_param.begin(); it!=m_vec_stationInfoList_param.end(); it++){
	//	if (m_map_lineInfoList_param.find(HIBYTE(it->stationNum)) == m_map_lineInfoList_param.end()){
	//		LINE_INFO lineInfo;
	//		lineInfo.lineNum = HIBYTE(it->stationNum);
	//		lineInfo.line_Chinese_Name = theTVM_INFO.GetLineName(lineInfo.lineNum);
	//		lineInfo.line_Eng_Name = theBOM_INFO.GetLineNameEn(lineInfo.lineNum);
	//		lineInfo.Active_flag = 1;
	//		m_map_lineInfoList_param.insert(map<int ,CAccLineInfoParam::LINE_INFO>::value_type(HIBYTE(it->stationNum),lineInfo));
	//		m_vec_lineInfoList_param.push_back(lineInfo);
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     车站信息参数

@param      none

@retval     vector<STATION_INFO_LIST>   车站信息参数向量数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::STATION_INFO>& CAccLineInfoParam::GetStationInfo()			
{
	return m_vec_stationInfoList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     获得指定线路车站信息

@param      (i)DWORD inLineNum

@retval     vector<STATION_INFO_LIST>   指定线路的车站信息向量数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::STATION_INFO>& CAccLineInfoParam::GetStationInfo(DWORD inLineNum) 
{
	m_vec_stationInfoListOfLine_param.clear();
	vector<CAccLineInfoParam::STATION_INFO>::const_iterator  it;
	for (it= m_vec_stationInfoList_param.begin(); it!=m_vec_stationInfoList_param.end(); it++){
		if(HIBYTE(it->stationNum) == inLineNum){
			m_vec_stationInfoListOfLine_param.push_back(*it);
		}

	}
	return m_vec_stationInfoListOfLine_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     换乘站信息参数

@param      none

@retval     vector<CHANGE_STATION_INFOLIST>   换乘站信息参数向量数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::EXCHANGE_STATION_INFO>&	CAccLineInfoParam::GetExchangeInfo()			
{
	return m_vec_exchangeInfoList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得指定车站信息
 
@param      (i)DWORD inStationNum  车站代码
@param      (o)CAccLineInfoParam::STATION_INFO_LIST  车站信息

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CAccLineInfoParam::GetStationInfobyCode(WORD inStationNum,CAccLineInfoParam::STATION_INFO& stationinfo)
{
	vector<CAccLineInfoParam::STATION_INFO>::iterator  it;

	for (it= m_vec_stationInfoList_param.begin(); it!=m_vec_stationInfoList_param.end(); it++){
		if(it->stationNum == inStationNum){
			stationinfo = *it;
			return true;
		}
	}
	return false;	
}
CString CAccLineInfoParam::GetLocationName(WORD inStationNum, LANGUAGETYPE_T lan)
{
	vector<CAccLineInfoParam::STATION_INFO>::iterator  it;

	for (it= m_vec_stationInfoList_param.begin(); it!=m_vec_stationInfoList_param.end(); it++){
		if(it->stationNum == inStationNum){
			if (lan == CHINESE)
				return it->station_Chinese_Name;
			else
				return it->station_Eng_Name;
		}
	}
	return _T("");	

}
