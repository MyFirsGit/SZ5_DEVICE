#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccNotServiceStation的唯一接口
#define theACC_NOTSERVSTATION CAccNotServiceStation::GetInstance()        

/**
@brief   停运车站表参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccNotServiceStation :public CAccParamMaster
{
public: 

	//停运车站表记录体
	typedef struct _no_Serv_Station_Code {
		WORD	noServStationCode;				// 停运车站编码
		_no_Serv_Station_Code(){				
			noServStationCode =0;					    		   	
		}
	}NO_SERV_STATION_CODE;



	// 共有接口
	const vector<NO_SERV_STATION_CODE>&  GetNotServiceStationListParam();										                // 获取所有停运车站编码
	bool  IsNotServiceStationParam(WORD StationCode);																            // 判定是否是停运车站
	static CAccNotServiceStation&  GetInstance()
	{
		return theInstance;
	}

protected:

	vector<NO_SERV_STATION_CODE>  m_vec_NotServiceStationList_param;			// 所有车票参数
private:

	SECTION_INFO m_section[1];	//分段信息

	// 私有成员变量


	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccNotServiceStation(void);
	~CAccNotServiceStation();
	CAccNotServiceStation(const CAccNotServiceStation&);
	CAccNotServiceStation& operator=(const CAccNotServiceStation&);

	static  CAccNotServiceStation theInstance;
};

