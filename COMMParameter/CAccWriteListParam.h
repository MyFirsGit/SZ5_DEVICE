#pragma once
#include "CAccParamMaster.h"
#include "CParameterCommandExp.h"

// CAccWriteListParam��Ψһ�ӿ�
#define theACC_WRITELIST CAccWriteListParam::GetInstance()        

/**
@brief   ACC������������
*/

class PARAMETERCOMMAND_DECLSPEC CAccWriteListParam :public CAccParamMaster
{
public: 

	static CAccWriteListParam&  GetInstance()
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

	CAccWriteListParam(void);
	~CAccWriteListParam();
	CAccWriteListParam(const CAccWriteListParam&);
	CAccWriteListParam& operator=(const CAccWriteListParam&);

	static  CAccWriteListParam theInstance;
};
