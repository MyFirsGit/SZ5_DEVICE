#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTimeTypeParam��Ψһ�ӿ�
#define theACC_TIMETYPE CAccTimeTypeParam::GetInstance()        

/**
@brief   ACCʱ�����Ͳ�����
*/

class PARAMETERCOMMAND_DECLSPEC CAccTimeTypeParam :public CAccParamMaster
{
public: 

	static CAccTimeTypeParam&  GetInstance()
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

	CAccTimeTypeParam(void);
	~CAccTimeTypeParam();
	CAccTimeTypeParam(const CAccTimeTypeParam&);
	CAccTimeTypeParam& operator=(const CAccTimeTypeParam&);

	static  CAccTimeTypeParam theInstance;
};
