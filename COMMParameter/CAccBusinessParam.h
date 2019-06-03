#pragma once
#include "CAccParamMaster.h"

// CAccBusinessParam的唯一接口
#define theACC_BUSINESS CAccBusinessParam::GetInstance()        

/**
 @brief   ACC运营参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccBusinessParam :public CAccParamMaster
{
public: 
    static CAccBusinessParam&  GetInstance()
    {
        return theInstance;
    }

protected:
	
private:
	
	//SECTION_INFO m_section[1];	//分段信息

    // 私有成员变量

    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAccBusinessParam(void);
    ~CAccBusinessParam();
	CAccBusinessParam(const CAccBusinessParam&);
	CAccBusinessParam& operator=(const CAccBusinessParam&);

    static  CAccBusinessParam theInstance;
};
