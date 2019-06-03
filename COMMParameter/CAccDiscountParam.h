#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccDiscountParam的唯一接口
#define theACC_DISCOUNT CAccDiscountParam::GetInstance()        

/**
@brief   ACC折扣率参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccDiscountParam :public CAccParamMaster
{
public: 

	static CAccDiscountParam&  GetInstance()
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

	CAccDiscountParam(void);
	~CAccDiscountParam();
	CAccDiscountParam(const CAccDiscountParam&);
	CAccDiscountParam& operator=(const CAccDiscountParam&);

	static  CAccDiscountParam theInstance;
};
