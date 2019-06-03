#pragma once
#include "CAccParamMaster.h"

// CAccOverTimeAdjuestParam��Ψһ�ӿ�
#define theACC_OVERTIMEADJUEST CAccOverTimeAdjuestParam::GetInstance()        

/**
@brief   ACC��ʱ�������ò�����
*/

class PARAMETERCOMMAND_DECLSPEC CAccOverTimeAdjuestParam :public CAccParamMaster
{
public: 

	static CAccOverTimeAdjuestParam&  GetInstance()
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

	CAccOverTimeAdjuestParam(void);
	~CAccOverTimeAdjuestParam();
	CAccOverTimeAdjuestParam(const CAccOverTimeAdjuestParam&);
	CAccOverTimeAdjuestParam& operator=(const CAccOverTimeAdjuestParam&);

	static  CAccOverTimeAdjuestParam theInstance;
};
