#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccIssueCompanyParam��Ψһ�ӿ�
#define theACC_ISSUECOMPANY CAccIssueCompanyParam::GetInstance()        

/**
@brief   ACCƱ�������̲�����
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

	//SECTION_INFO m_section[2];	//�ֶ���Ϣ

	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccIssueCompanyParam(void);
	~CAccIssueCompanyParam();
	CAccIssueCompanyParam(const CAccIssueCompanyParam&);
	CAccIssueCompanyParam& operator=(const CAccIssueCompanyParam&);

	static  CAccIssueCompanyParam theInstance;
};
