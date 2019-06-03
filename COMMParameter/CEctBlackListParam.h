#pragma once
#include "CEctParamMaster.h"

// CEctBlackListParam的唯一接口
#define theECT_BLACKLIST CEctBlackListParam::GetInstance()        

/**
@brief   ECT系统参数类
*/

class PARAMETERCOMMAND_DECLSPEC CEctBlackListParam :public CEctParamMaster
{
public: 

	static CEctBlackListParam&  GetInstance()
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

	CEctBlackListParam(void);
	~CEctBlackListParam();
	CEctBlackListParam(const CEctBlackListParam&);
	CEctBlackListParam& operator=(const CEctBlackListParam&);

	static  CEctBlackListParam theInstance;
};
