#pragma once
#include "CAfcParamMaster.h"
#include "CParameterCommandExp.h"

// CAfcOperatorCodeTableParam的唯一接口
#define theAFC_OPERATOR_CODE CAfcOperatorCodeTableParam::GetInstance()        

/**
 @brief   运营商代码映射表参数
*/

class PARAMETERCOMMAND_DECLSPEC CAfcOperatorCodeTableParam :public CAfcParamMaster
{
public: 

	// 共有接口
    //void GetOperationTimeParam(OPRATION_TIME&);		// 运营商代码映射表
	
    static CAfcOperatorCodeTableParam&  GetInstance()
    {
        return theInstance;
    }

private:

	//SECTION_INFO m_section;	//分段信息

    // 私有成员变量
    //OPRATION_TIME	m_operation_param;			// 运营时间参数
	
    // 私有成员函数
    virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // 获取参数文件名

	CAfcOperatorCodeTableParam(void);
    ~CAfcOperatorCodeTableParam();
	CAfcOperatorCodeTableParam(const CAfcOperatorCodeTableParam&);
	CAfcOperatorCodeTableParam& operator=(const CAfcOperatorCodeTableParam&);

    static  CAfcOperatorCodeTableParam theInstance;
};
