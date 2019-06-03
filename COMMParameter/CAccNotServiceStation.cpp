#include "StdAfx.h"
#include "CAccNotServiceStation.h"
#include "LogException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAccNotServiceStation CAccNotServiceStation::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccNotServiceStation::CAccNotServiceStation():CAccParamMaster()
{
	m_vec_NotServiceStationList_param.clear();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�����ļ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAccNotServiceStation::GetParamFileName()
{
	GetParamFileNameSub(ACC_PARAM_ID_0608);		// ��ȡ�����ļ���
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAccNotServiceStation::~CAccNotServiceStation(void)
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
void CAccNotServiceStation::ParseBody(LPBYTE lpData)
{
	try{    
		// ƫ��������
		m_vec_NotServiceStationList_param.clear();
		lpData += m_section[0].startPosition;
		if (m_section[0].recordCount != 0) { 			
			WORD notServiceList_len;
			memcpy(&notServiceList_len,lpData,2);
			lpData += 2;      
			for(int i = 0;i<notServiceList_len;i++){
				NO_SERV_STATION_CODE tmpserviceFeeInfo;
				//ͣ�˳�վ��
				tmpserviceFeeInfo.noServStationCode = BCD2int((char*)lpData, 2);
				lpData += 2;		
				m_vec_NotServiceStationList_param.push_back(tmpserviceFeeInfo);
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
@brief      �����ֶ���Ϣ

@param      (i)LPBYTE lpData       �ļ�����

@retval     none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CAccNotServiceStation::ParseSectionInfo(LPBYTE lpData)
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
bool CAccNotServiceStation::CheckBodyInfo(LPBYTE lpData)
{   
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ͣ�˳�վ�����

@param      none

@retval     vector<FARE_TYPE_LIST>   ͣ�˳�վ�������������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CAccNotServiceStation::NO_SERV_STATION_CODE>&  CAccNotServiceStation::GetNotServiceStationListParam()
{
	return m_vec_NotServiceStationList_param;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ���ͣ�˳�վ

@param      (in)WORD StationCode     ��վ����

@retval     bool  true:��ͣ�˳�վ     false:����ͣ�˳�վ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool  CAccNotServiceStation::IsNotServiceStationParam(WORD StationCode)
{
	vector<CAccNotServiceStation::NO_SERV_STATION_CODE>::iterator  it;

	for (it=m_vec_NotServiceStationList_param.begin(); it!=m_vec_NotServiceStationList_param.end(); it++){
		if(it->noServStationCode == StationCode){
			return true;
		}
	}
	return false;
}



