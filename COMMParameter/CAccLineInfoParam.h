#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccLineInfoParam的唯一接口
#define theACC_LOCATION CAccLineInfoParam::GetInstance()        
#define theACC_LINE theACC_LOCATION
/**
@brief   ACC线路信息参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccLineInfoParam :public CAccParamMaster
{
public:
	
	// 线路信息记录体
	typedef struct _line_Info {
		WORD   lineNum;							//线路代码		
		CString line_Chinese_Name;				//线路中文名称
		CString line_Eng_Name;				    //线路英文名称
		DWORD   Active_flag;                    //启用标志		

		_line_Info()
		{
			lineNum = 0;
			line_Chinese_Name = "";
			line_Eng_Name = "";
			Active_flag = 1;
		};
	}LINE_INFO;	

	// 车站信息记录体
	typedef struct _station_Info {
		WORD stationNum;						//车站代码		
		CString station_Chinese_Name;	        //车站中文名称
		CString station_Eng_Name;				//车站英文名称
		WORD fareStationNum;					//计费站点编码		
		BYTE Active_flag;                       //启用标志
		BYTE isExchangeStation;					//是否换乘站

		_station_Info()
		{

			stationNum = 0;
			station_Chinese_Name = "";
			station_Eng_Name = "";
			fareStationNum = 0;
			Active_flag = 1;
			isExchangeStation = 0;
		};
	}STATION_INFO;	


	
	// 换乘站信息记录体
	typedef struct _exchange_Station_Info {
		WORD  stationNum;						//车站代码		
		WORD  exchangeStationNum;				//换乘车站代码
		_exchange_Station_Info()
		{
			stationNum = 0;
			exchangeStationNum = 0;
		}
	}EXCHANGE_STATION_INFO;	

	// 共有接口
	const vector<LINE_INFO>&					GetLinInfo();					// 线路信息
	const vector<STATION_INFO>&					GetStationInfo();				// 获得全路网车站信息
	const vector<STATION_INFO>&					GetStationInfo(DWORD inLineNum);// 获得指定线路车站信息
	const vector<EXCHANGE_STATION_INFO>&	    GetExchangeInfo();				// 换乘站信息
	bool  GetStationInfobyCode(WORD inStationNum,STATION_INFO& stationinfo);	// 获得指定车站信息
	CString GetLocationName(WORD inStationNum, LANGUAGETYPE_T);						// 获得指定位置编码的车站名称

	static CAccLineInfoParam&  GetInstance()
	{
		return theInstance;
	}

protected:

	map<int,LINE_INFO>             m_map_lineInfoList_param;    		// 线路信息参数
	vector<LINE_INFO>              m_vec_lineInfoList_param;    		// 线路信息参数
	vector<STATION_INFO>	    m_vec_stationInfoList_param;			// 车站信息参数	
	vector<STATION_INFO>		m_vec_stationInfoListOfLine_param;		// 指定线路车站信息
	vector<EXCHANGE_STATION_INFO> m_vec_exchangeInfoList_param;			// 换乘站信息参数

	void					SetLinInfo();					// 线路信息

private:

	SECTION_INFO m_section[3];                  	//分段信息

	// 私有成员变量
	

	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccLineInfoParam(void);
	~CAccLineInfoParam();
	CAccLineInfoParam(const CAccLineInfoParam&);
	CAccLineInfoParam& operator=(const CAccLineInfoParam&);

	static  CAccLineInfoParam theInstance;
};


