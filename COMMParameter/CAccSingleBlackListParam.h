#pragma once
#include "CAccParamMaster.h"

// CAccSingleBlackListParam��Ψһ�ӿ�
#define theACC_SINGLEBLACKLIST CAccSingleBlackListParam::GetInstance()        

/**
@brief   ACC��������������������
*/

class PARAMETERCOMMAND_DECLSPEC CAccSingleBlackListParam :public CAccParamMaster
{
public: 

	static CAccSingleBlackListParam&  GetInstance()
	{
		return theInstance;
	}

protected:
	// ˽�г�Ա����

private:

	//SECTION_INFO m_section[6];	//�ֶ���Ϣ

	// ˽�г�Ա����
	virtual void ParseBody(LPBYTE);
	virtual void ParseSectionInfo(LPBYTE);
	virtual bool CheckBodyInfo(LPBYTE);
	virtual void GetParamFileName();				 // ��ȡ�����ļ���

	CAccSingleBlackListParam(void);
	~CAccSingleBlackListParam();
	CAccSingleBlackListParam(const CAccSingleBlackListParam&);
	CAccSingleBlackListParam& operator=(const CAccSingleBlackListParam&);

	static  CAccSingleBlackListParam theInstance;
};
