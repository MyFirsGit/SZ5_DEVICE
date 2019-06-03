#pragma once
#include "CParameterCommandExp.h"

#define theISSUE_PERMIT CCTIssuePermit::GetInstance()

/**
@brief 发售许可Master
*/
class PARAMETERCOMMAND_DECLSPEC  CCTIssuePermit
{
public:

	static CCTIssuePermit &GetInstance()
	{
		return theInstance;
	}

	void AddIssueStationPermit(unsigned short);
	void DeleteIssueStation(unsigned short);
	void DeleteAllIssueStation();
	bool IsPermitIssueStation(unsigned short);
private:

	// 私有成员变量
	vector<unsigned short>  m_IssueStationList;   

	static CCTIssuePermit theInstance;
	CCTIssuePermit(void);
	~CCTIssuePermit();
	CCTIssuePermit(const CCTIssuePermit&);
	CCTIssuePermit& operator=(const CCTIssuePermit&);
};