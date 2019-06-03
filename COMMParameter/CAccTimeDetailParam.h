#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTimeDetailParam的唯一接口
#define theACC_TIMEDETAIL CAccTimeDetailParam::GetInstance()        

/**
@brief   ACC时间类型明细参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccTimeDetailParam :public CAccParamMaster
{
public: 

	static CAccTimeDetailParam&  GetInstance()
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

	CAccTimeDetailParam(void);
	~CAccTimeDetailParam();
	CAccTimeDetailParam(const CAccTimeDetailParam&);
	CAccTimeDetailParam& operator=(const CAccTimeDetailParam&);

	static  CAccTimeDetailParam theInstance;
};
