#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTollStationParam的唯一接口
#define theACC_TOLLSTATION CAccTollStationParam::GetInstance()        

/**
 @brief   ACC计费站点参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccTollStationParam :public CAccParamMaster
{
public: 

    static CAccTollStationParam&  GetInstance()
    {
        return theInstance;
    }

protected:

private:

	//SECTION_INFO m_section[2];	//分段信息

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccTollStationParam(void);
    ~CAccTollStationParam();
	CAccTollStationParam(const CAccTollStationParam&);
	CAccTollStationParam& operator=(const CAccTollStationParam&);

    static  CAccTollStationParam theInstance;
};
