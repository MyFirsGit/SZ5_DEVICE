#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccSectionParam的唯一接口
#define theACC_SECTION CAccSectionParam::GetInstance()        

/**
 @brief   ACC基础票价表参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccSectionParam :public CAccParamMaster
{
public: 

    static CAccSectionParam&  GetInstance()
    {
        return theInstance;
    }

protected:

private:

//	SECTION_INFO m_section[4];	//分段信息

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名


	CAccSectionParam(void);
    ~CAccSectionParam();
	CAccSectionParam(const CAccSectionParam&);
	CAccSectionParam& operator=(const CAccSectionParam&);

    static  CAccSectionParam theInstance;
};
