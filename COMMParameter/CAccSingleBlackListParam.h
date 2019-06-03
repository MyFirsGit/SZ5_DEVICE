#pragma once
#include "CAccParamMaster.h"

// CAccSingleBlackListParam的唯一接口
#define theACC_SINGLEBLACKLIST CAccSingleBlackListParam::GetInstance()        

/**
@brief   ACC单条黑名单参数参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccSingleBlackListParam :public CAccParamMaster
{
public: 

	static CAccSingleBlackListParam&  GetInstance()
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

	CAccSingleBlackListParam(void);
	~CAccSingleBlackListParam();
	CAccSingleBlackListParam(const CAccSingleBlackListParam&);
	CAccSingleBlackListParam& operator=(const CAccSingleBlackListParam&);

	static  CAccSingleBlackListParam theInstance;
};
