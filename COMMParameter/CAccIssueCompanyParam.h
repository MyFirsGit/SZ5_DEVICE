#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccIssueCompanyParam的唯一接口
#define theACC_ISSUECOMPANY CAccIssueCompanyParam::GetInstance()        

/**
@brief   ACC票卡发行商参数类
*/

class PARAMETERCOMMAND_DECLSPEC CAccIssueCompanyParam :public CAccParamMaster
{
public: 

	static CAccIssueCompanyParam&  GetInstance()
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

	CAccIssueCompanyParam(void);
	~CAccIssueCompanyParam();
	CAccIssueCompanyParam(const CAccIssueCompanyParam&);
	CAccIssueCompanyParam& operator=(const CAccIssueCompanyParam&);

	static  CAccIssueCompanyParam theInstance;
};
