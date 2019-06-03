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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAccLineInfoParam::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0301);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccLineInfoParam::~CAccLineInfoParam(void)
{	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ǰ�ļ�������

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccLineInfoParam::ParseBody(LPBYTE lpData)
{
	try{		
		// ��վ��Ϣ
		m_vec_stationInfoList_param.clear();
		m_map_lineInfoList_param.clear();
		m_vec_lineInfoList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			 
			// ACC����ĳ�վ��Ϣѭ��������
			DWORD stationInfoList_len = mltol(ComMakeLong(*lpData, *(lpData + 1), *(lpData + 2), *(lpData + 3)));;
			lpData += 4;
			for(int i = 0;i<stationInfoList_len;i++){
				STATION_INFO  tmpStationInfo;
				// ��վ����
				tmpStationInfo.stationNum = ComMakeWORD(*lpData, *(lpData + 1));
				lpData += 2;
				// ��վ��������
				char lang_name_cn[33] = {0};
				memcpy(lang_name_cn, lpData, 32);
				tmpStationInfo.station_Chinese_Name = lang_name_cn;
				tmpStationInfo.station_Chinese_Name.Trim();				
				lpData += 32;  
				// ��վӢ������
				char lang_name_en[33] = {0};
				memset(lang_name_en,0,32);
				memcpy(lang_name_en, lpData, 32);
				tmpStationInfo.station_Eng_Name = lang_name_en;
				tmpStationInfo.station_Eng_Name.Trim();
				lpData +=32;
				//�Ʒ�վ�����
				tmpStationInfo.fareStationNum =ComMakeWORD(*lpData, *(lpData + 1));
				lpData += 2;
				//�Ƿ񻻳�վ
				tmpStationInfo.isExchangeStation = BCD2int((char*)lpData, 1);
				lpData ++;
				m_vec_stationInfoList_param.push_back(tmpStationInfo);
			}
		}
		SetLinInfo();
	}
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ֶ���Ϣ

@param      (i)LPBYTE lpData       �ļ�����

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
	// �쳣����
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CLogException(CLogException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ľṹ��������������ȷ��

@param      (i)LPBYTE           ������Ϣ

@retval     bool   true:��ȷ  false������ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAccLineInfoParam::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��·��Ϣ����

@param      none

@retval     vector<LINE_INFO_LIST>   ��·��Ϣ������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::LINE_INFO>& CAccLineInfoParam::GetLinInfo()				
{
	return m_vec_lineInfoList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��·��Ϣ����

@param      none

@retval     ��

@exception  ��
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
@brief     ��վ��Ϣ����

@param      none

@retval     vector<STATION_INFO_LIST>   ��վ��Ϣ������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::STATION_INFO>& CAccLineInfoParam::GetStationInfo()			
{
	return m_vec_stationInfoList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ָ����·��վ��Ϣ

@param      (i)DWORD inLineNum

@retval     vector<STATION_INFO_LIST>   ָ����·�ĳ�վ��Ϣ��������

@exception  ��
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
@brief     ����վ��Ϣ����

@param      none

@retval     vector<CHANGE_STATION_INFOLIST>   ����վ��Ϣ������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccLineInfoParam::EXCHANGE_STATION_INFO>&	CAccLineInfoParam::GetExchangeInfo()			
{
	return m_vec_exchangeInfoList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ָ����վ��Ϣ
 
@param      (i)DWORD inStationNum  ��վ����
@param      (o)CAccLineInfoParam::STATION_INFO_LIST  ��վ��Ϣ

@retval     bool

@exception  ��
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
