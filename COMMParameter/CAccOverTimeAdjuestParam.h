#pragma once
#include "CAccParamMaster.h"

// CAccOverTimeAdjuestParam的唯一接口
#define theACC_OVERTIMEADJUEST CAccOverTimeAdjuestParam::GetInstance()        

/**
@brief   ACC超时补交费用参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccOverTimeAdjuestParam :public CAccParamMaster
{
public: 

	static CAccOverTimeAdjuestParam&  GetInstance()
	{
		return theInstance;
	}

protected:
	// 私有成员变量

private:

	//SECTION_INFO m_section[6];	//分段信息

	// 私有成员函数
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccOverTimeAdjuestParam(void);
	~CAccOverTimeAdjuestParam();
	CAccOverTimeAdjuestParam(const CAccOverTimeAdjuestParam&);
	CAccOverTimeAdjuestParam& operator=(const CAccOverTimeAdjuestParam&);

	static  CAccOverTimeAdjuestParam theInstance;
};
