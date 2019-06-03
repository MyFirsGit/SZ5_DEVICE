#pragma once
#include "CAccParamMaster.h"

// CAccSectionBlackListParam的唯一接口
#define theACC_SECTIONBLACKLIST CAccSectionBlackListParam::GetInstance()        

/**
 @brief   ACC区段黑名单参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccSectionBlackListParam :public CAccParamMaster
{
public: 

    static CAccSectionBlackListParam&  GetInstance()
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

	CAccSectionBlackListParam(void);
    ~CAccSectionBlackListParam();
	CAccSectionBlackListParam(const CAccSectionBlackListParam&);
	CAccSectionBlackListParam& operator=(const CAccSectionBlackListParam&);

    static  CAccSectionBlackListParam theInstance;
};
