#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTimeTypeParam的唯一接口
#define theACC_TIMETYPE CAccTimeTypeParam::GetInstance()        

/**
@brief   ACC时间类型参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccTimeTypeParam :public CAccParamMaster
{
public: 

	static CAccTimeTypeParam&  GetInstance()
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

	CAccTimeTypeParam(void);
	~CAccTimeTypeParam();
	CAccTimeTypeParam(const CAccTimeTypeParam&);
	CAccTimeTypeParam& operator=(const CAccTimeTypeParam&);

	static  CAccTimeTypeParam theInstance;
};
