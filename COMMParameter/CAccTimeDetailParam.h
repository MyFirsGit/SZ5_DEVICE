#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccTimeDetailParam��Ψһ�ӿ�
#define theACC_TIMEDETAIL CAccTimeDetailParam::GetInstance()        

/**
@brief   ACCʱ��������ϸ������
*/

class PARAMETERCOMMAND_DECLSPEC CAccTimeDetailParam :public CAccParamMaster
{
public: 

	static CAccTimeDetailParam&  GetInstance()
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

	CAccTimeDetailParam(void);
	~CAccTimeDetailParam();
	CAccTimeDetailParam(const CAccTimeDetailParam&);
	CAccTimeDetailParam& operator=(const CAccTimeDetailParam&);

	static  CAccTimeDetailParam theInstance;
};
