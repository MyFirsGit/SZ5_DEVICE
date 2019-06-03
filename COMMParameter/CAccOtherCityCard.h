#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccOtherCityCard的唯一接口
#define theACC_OTHERCITYCARD CAccOtherCityCard::GetInstance()        

/**
@brief   ACC异地卡/城市代码对照参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccOtherCityCard :public CAccParamMaster
{
public: 

	static CAccOtherCityCard&  GetInstance()
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

	CAccOtherCityCard(void);
	~CAccOtherCityCard();
	CAccOtherCityCard(const CAccOtherCityCard&);
	CAccOtherCityCard& operator=(const CAccOtherCityCard&);

	static  CAccOtherCityCard theInstance;
};
