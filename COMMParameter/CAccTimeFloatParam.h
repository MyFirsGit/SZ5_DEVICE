#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTimeFloatParam��Ψһ�ӿ�
#define theACC_TIMEFLOAT CAccTimeFloatParam::GetInstance()        

/**
@brief   ACCʱ�両���������
*/

class PARAMETERCOMMAND_DECLSPEC CAccTimeFloatParam :public CAccParamMaster
{
public: 

	static CAccTimeFloatParam&  GetInstance()
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

	CAccTimeFloatParam(void);
	~CAccTimeFloatParam();
	CAccTimeFloatParam(const CAccTimeFloatParam&);
	CAccTimeFloatParam& operator=(const CAccTimeFloatParam&);

	static  CAccTimeFloatParam theInstance;
};
