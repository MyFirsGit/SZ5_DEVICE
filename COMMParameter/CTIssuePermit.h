#pragma once
#include "CParameterCommandExp.h"

#define theISSUE_PERMIT CCTIssuePermit::GetInstance()

/**
@brief �������Master
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

	// ˽�г�Ա����
	vector<unsigned short>  m_IssueStationList;   

	static CCTIssuePermit theInstance;
	CCTIssuePermit(void);
	~CCTIssuePermit();
	CCTIssuePermit(const CCTIssuePermit&);
	CCTIssuePermit& operator=(const CCTIssuePermit&);
};