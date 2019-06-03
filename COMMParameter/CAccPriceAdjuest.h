#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccPriceAdjuest的唯一接口
#define theACC_PRICEADJUEST CAccPriceAdjuest::GetInstance()        

/**
@brief   ACC票价调整策略表参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccPriceAdjuest :public CAccParamMaster
{
public:

	static CAccPriceAdjuest&  GetInstance()
	{
		return theInstance;
	}

protected:

private:

	//SECTION_INFO m_section[3];                  	//分段信息

	// 私有成员变量


	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccPriceAdjuest(void);
	~CAccPriceAdjuest();
	CAccPriceAdjuest(const CAccPriceAdjuest&);
	CAccPriceAdjuest& operator=(const CAccPriceAdjuest&);

	static  CAccPriceAdjuest theInstance;
};


