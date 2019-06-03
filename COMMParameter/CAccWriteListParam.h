#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccWriteListParam的唯一接口
#define theACC_WRITELIST CAccWriteListParam::GetInstance()        

/**
@brief   ACC白名单参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccWriteListParam :public CAccParamMaster
{
public: 

	static CAccWriteListParam&  GetInstance()
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

	CAccWriteListParam(void);
	~CAccWriteListParam();
	CAccWriteListParam(const CAccWriteListParam&);
	CAccWriteListParam& operator=(const CAccWriteListParam&);

	static  CAccWriteListParam theInstance;
};
