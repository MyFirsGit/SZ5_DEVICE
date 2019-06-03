#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTimeFloatParam的唯一接口
#define theACC_TIMEFLOAT CAccTimeFloatParam::GetInstance()        

/**
@brief   ACC时间浮动表参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccTimeFloatParam :public CAccParamMaster
{
public: 

	static CAccTimeFloatParam&  GetInstance()
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

	CAccTimeFloatParam(void);
	~CAccTimeFloatParam();
	CAccTimeFloatParam(const CAccTimeFloatParam&);
	CAccTimeFloatParam& operator=(const CAccTimeFloatParam&);

	static  CAccTimeFloatParam theInstance;
};
